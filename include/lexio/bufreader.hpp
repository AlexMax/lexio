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

/**
 * @file bufreader.hpp
 * Contains implementations of buffered reader functionality.
 */

#pragma once

#include "./core.hpp"

#include <stdexcept>

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
    READER m_reader;
    uint8_t *m_buffer = nullptr;
    size_t m_allocSize = 0;
    size_t m_size = 0;

    /**
     * @brief Calculate size of allocated buffer.
     *
     * @param wantSize Wanted capacity of buffer.
     * @return Calculated capacity of allocation.
     */
    size_t CalcGrowth(size_t wantSize)
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
    /**
     * @brief Default constructor.
     */
    GenericBufReader() = default;

    /**
     * @brief Copy constructor.
     */
    GenericBufReader(const GenericBufReader &other)
        : m_reader(other.m_reader), m_buffer(::new uint8_t[other.m_allocSize]), m_allocSize(other.m_allocSize),
          m_size(other.m_size)
    {
        std::memcpy(m_buffer, &other.m_buffer[0], m_size);
    }

    /**
     * @brief Move constructor.
     */
    GenericBufReader(GenericBufReader &&other) noexcept
        : m_reader(std::move(other.m_reader)), m_buffer(std::exchange(other.m_buffer, nullptr)),
          m_allocSize(other.m_allocSize), m_size(other.m_size)
    {
    }

    /**
     * @brief Constructor from existing Reader.
     *
     * @param reader Reader to wrap with a buffer.
     */
    GenericBufReader(READER &&reader) : m_reader(std::move(reader)) {}

    /**
     * @brief Destructor.
     */
    ~GenericBufReader() { ::delete[] m_buffer; }

    /**
     * @brief Copy assignment operator.
     */
    GenericBufReader &operator=(const GenericBufReader &other)
    {
        if (this == &other)
        {
            return *this;
        }

        GenericBufReader copy{other};
        std::swap(m_reader, copy.m_reader);
        std::swap(m_buffer, copy.m_buffer);
        std::swap(m_allocSize, copy.m_allocSize);
        std::swap(m_size, copy.m_size);
        return *this;
    }

    /**
     * @brief Move assignment operator.
     */
    GenericBufReader &operator=(GenericBufReader &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ::delete[] m_buffer;
        m_reader = std::move(other.m_reader);
        m_buffer = std::exchange(other.m_buffer, nullptr);
        m_allocSize = other.m_allocSize;
        m_size = other.m_size;
        return *this;
    }

    /**
     * @brief Return underlying Reader.
     */
    const READER &Reader() const & { return m_reader; }

    /**
     * @brief Obtain the underlying reader Reader while moving-from the
     *        GenericBufReader.
     */
    READER Reader() && { return m_reader; }

    size_t LexRead(uint8_t *outDest, size_t count)
    {
        BufferView data = LexFillBuffer(count);
        std::memcpy(outDest, data.Data(), data.Size());
        LexConsumeBuffer(data.Size());
        return data.Size();
    }

    BufferView LexFillBuffer(size_t count)
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
            std::memcpy(buffer, &m_buffer[0], m_size);
            ::delete[] m_buffer;
            m_buffer = buffer;
            m_allocSize = newAllocSize;
        }

        // Read into the buffer.
        const size_t wanted = count - m_size;
        const size_t actual = Read(&m_buffer[m_size], m_reader, wanted);
        m_size += actual;
        return BufferView{m_buffer, m_size};
    }

    void LexConsumeBuffer(size_t count)
    {
        if (count > m_size)
        {
            throw new std::runtime_error("can't consume more bytes than buffer size");
        }
        std::memcpy(&m_buffer[0], &m_buffer[count], m_size);
        m_size -= count;
    }

    template <typename WRITER = READER, typename = std::enable_if_t<IsWriterV<WRITER>>>
    size_t LexWrite(const uint8_t *src, size_t count)
    {
        m_size = 0; // Invalidate buffer.
        return Write(m_reader, src, count);
    }

    template <typename WRITER = READER, typename = std::enable_if_t<IsWriterV<WRITER>>>
    void LexFlush()
    {
        Flush(m_reader);
    }

    template <typename SEEKABLE = READER, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
    size_t LexSeek(SeekPos pos)
    {
        m_size = 0; // Invalidate buffer.
        return Seek(m_reader, pos);
    }
};

} // namespace LexIO
