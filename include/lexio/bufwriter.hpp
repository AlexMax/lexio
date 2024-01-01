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

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
class FixedBufWriter
{
    WRITER m_wrapped;
    uint8_t *m_buffer = nullptr;
    size_t m_allocSize = 0;
    size_t m_size = 0;

  protected:
    void FlushBuffer()
    {
        size_t offset = 0;
        size_t countLeft = m_size;
        const size_t totalSize = m_size;
        while (offset < totalSize)
        {
            const size_t written = Write<WRITER>(m_wrapped, m_buffer + offset, countLeft);
            offset += written;
            countLeft -= written;
        }

        m_size = 0;
    }

  public:
    FixedBufWriter() = delete;

    FixedBufWriter(const FixedBufWriter &other)
        : m_wrapped(other.m_wrapped), m_buffer(::new uint8_t[other.m_allocSize]), m_allocSize(other.m_allocSize),
          m_size(other.m_size)
    {
        std::copy(&other.m_buffer[0], &other.m_buffer[m_size], m_buffer);
    }

    FixedBufWriter(FixedBufWriter &&other) noexcept
        : m_wrapped(std::move(other.m_wrapped)), m_buffer(std::exchange(other.m_buffer, nullptr)),
          m_allocSize(other.m_allocSize), m_size(other.m_size)
    {
    }

    FixedBufWriter(WRITER &&wrapped, const size_t bufSize = 8192)
        : m_wrapped(wrapped), m_buffer(::new uint8_t[bufSize]), m_allocSize(bufSize)
    {
    }

    ~FixedBufWriter()
    {
        if (m_buffer)
        {
            LexFlush();
            ::delete[] m_buffer;
        }
    }

    FixedBufWriter &operator=(const FixedBufWriter &other)
    {
        if (this == &other)
        {
            return *this;
        }

        FixedBufWriter copy{other};
        std::swap(m_wrapped, copy.m_wrapped);
        std::swap(m_buffer, copy.m_buffer);
        std::swap(m_allocSize, copy.m_allocSize);
        std::swap(m_size, copy.m_size);
        return *this;
    }

    FixedBufWriter &operator=(FixedBufWriter &&other) noexcept
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
     * @brief Return underlying Writer.
     */
    const WRITER &Writer() const & { return m_wrapped; }

    /**
     * @brief Obtain the underlying wrapped Writer while moving-from the
     *        FixedBufWriter.
     */
    WRITER Writer() && { return m_wrapped; }

    template <typename READER = WRITER, typename = std::enable_if_t<IsReaderV<READER>>>
    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        return Read<WRITER>(m_wrapped, outDest, count);
    }

    template <typename BUFFERED_READER = WRITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
    BufferView LexFillBuffer(const size_t count)
    {
        return FillBuffer<WRITER>(m_wrapped, count);
    }

    template <typename BUFFERED_READER = WRITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
    void LexConsumeBuffer(const size_t count)
    {
        ConsumeBuffer<WRITER>(m_wrapped, count);
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        const size_t wantSize = m_size + count;
        if (wantSize < m_allocSize)
        {
            // Fast path, just append to the buffer.
            std::copy(src, src + count, &m_buffer[m_size]);
            m_size = wantSize;
            return count;
        }

        // Flush the current contents of the buffer.
        LexFlush();

        if (count < m_allocSize)
        {
            // Room in the buffer, write to it.
            std::copy(src, src + count, &m_buffer[0]);
            m_size = count;
            return count;
        }

        // Write is too large for buffer, pass through.
        return Write<WRITER>(m_wrapped, src, count);
    }

    void LexFlush()
    {
        FlushBuffer();
        Flush<WRITER>(m_wrapped);
    }

    template <typename SEEKABLE = WRITER, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
    size_t LexSeek(const SeekPos pos)
    {
        LexFlush();
        return Seek<WRITER>(m_wrapped, pos);
    }
};

} // namespace LexIO
