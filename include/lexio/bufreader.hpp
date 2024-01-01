//
// Copyright 2023 Lexi Mayfield
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#pragma once

#include "./core.hpp"

#include <utility>

namespace LexIO
{

/**
 * @brief Turn any Reader into a BufferedReader, backed by a buffer allocated
 *        with ::new[] and ::delete[].
 *
 * @tparam READER Reader type to wrap
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
class GenericBufReader
{
    READER m_wrapped;
    uint8_t *m_buffer = nullptr;
    size_t m_allocSize = 0;
    size_t m_size = 0;

    /**
     * @brief Calculate size of allocated buffer.
     *
     * @param wantSize Wanted capacity of buffer.
     * @return Calculated capacity of allocation.
     */
    size_t CalcGrowth(const size_t wantSize)
    {
        const size_t oldSize = m_allocSize;
        if (oldSize > SIZE_MAX - (oldSize / 2))
        {
            // Would overflow.
            return SIZE_MAX;
        }

        const size_t nextSize = oldSize + (oldSize / 2);
        if (wantSize <= nextSize)
        {
            // 1.5x growth factor is OK.
            return nextSize;
        }

        // Grow to exact size.
        return wantSize;
    }

  public:
    GenericBufReader() = delete;

    GenericBufReader(const GenericBufReader &other)
        : m_wrapped(other.m_wrapped), m_buffer(::new uint8_t[other.m_allocSize]), m_allocSize(other.m_allocSize),
          m_size(other.m_size)
    {
        std::copy(&other.m_buffer[0], &other.m_buffer[m_size], m_buffer);
    }

    GenericBufReader(GenericBufReader &&other) noexcept
        : m_wrapped(std::move(other.m_wrapped)), m_buffer(std::exchange(other.m_buffer, nullptr)),
          m_allocSize(other.m_allocSize), m_size(other.m_size)
    {
    }

    GenericBufReader(READER &&wrapped) : m_wrapped(wrapped) {}
    ~GenericBufReader() { ::delete[] m_buffer; }

    GenericBufReader &operator=(const GenericBufReader &other)
    {
        if (this == &other)
        {
            return *this;
        }

        GenericBufReader copy{other};
        std::swap(m_wrapped, copy.m_wrapped);
        std::swap(m_buffer, copy.m_buffer);
        std::swap(m_allocSize, copy.m_allocSize);
        std::swap(m_size, copy.m_size);
        return *this;
    }

    GenericBufReader &operator=(GenericBufReader &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ::delete[] m_buffer;
        m_wrapped = std::move(other.m_wrapped);
        m_buffer = std::exchange(other.m_buffer, nullptr);
        m_allocSize = other.m_allocSize;
        m_size = other.m_size;
        return *this;
    }

    /**
     * @brief Return underlying Reader.
     */
    const READER &Reader() const & { return m_wrapped; }

    /**
     * @brief Obtain the underlying wrapped Reader while moving-from the
     *        GenericBufReader.
     */
    READER Reader() && { return m_wrapped; }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        BufferView data = LexFillBuffer(count);
        std::copy(data.first, data.first + data.second, outDest);
        LexConsumeBuffer(data.second);
        return data.second;
    }

    BufferView LexFillBuffer(const size_t count)
    {
        if (count <= m_size)
        {
            // We already have enough data buffered.
            return BufferView{m_buffer, m_size};
        }

        if (count > m_allocSize)
        {
            // Reallocate our buffer with any existing data.
            const size_t newAllocSize = CalcGrowth(count);
            uint8_t *buffer = ::new uint8_t[newAllocSize];
            std::copy(&m_buffer[0], &m_buffer[m_size], buffer);
            ::delete[] m_buffer;
            m_buffer = buffer;
            m_allocSize = newAllocSize;
        }

        // Read into the buffer.
        const size_t wanted = count - m_size;
        const size_t actual = Read<READER>(&m_buffer[m_size], wanted, m_wrapped);
        m_size += actual;
        return BufferView{m_buffer, m_size};
    }

    void LexConsumeBuffer(const size_t count)
    {
        if (count > m_size)
        {
            throw new std::runtime_error("can't consume more bytes than buffer size");
        }
        std::copy(&m_buffer[count], &m_buffer[m_size], &m_buffer[0]);
        m_size -= count;
    }

    template <typename WRITER = READER, typename = std::enable_if_t<IsWriterV<WRITER>>>
    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        return Write<READER>(m_wrapped, src, count);
    }

    template <typename WRITER = READER, typename = std::enable_if_t<IsWriterV<WRITER>>>
    void LexFlush()
    {
        Flush<READER>(m_wrapped);
    }

    template <typename SEEKABLE = READER, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
    size_t LexSeek(const SeekPos pos)
    {
        return Seek<READER>(m_wrapped, pos);
    }
};

} // namespace LexIO
