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

    FixedBufWriter(WRITER &&wrapped, const size_t bufSize = 8192)
        : m_wrapped(wrapped), m_buffer(::new uint8_t[bufSize]), m_allocSize(bufSize)
    {
    }

    ~FixedBufWriter()
    {
        LexFlush();
        ::delete[] m_buffer;
    }

    WRITER &&Writer() && { return std::move(m_wrapped); }

    template <typename = std::enable_if_t<IsReaderV<WRITER>>>
    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        return Read<WRITER>(m_wrapped, outDest, count);
    }

    template <typename = std::enable_if_t<IsBufferedReaderV<WRITER>>>
    BufferView LexFillBuffer(const size_t count)
    {
        return LexFillBuffer<WRITER>(m_wrapped, count);
    }

    template <typename = std::enable_if_t<IsBufferedReaderV<WRITER>>>
    void LexConsumeBuffer(const size_t count)
    {
        LexConsumeBuffer<WRITER>(m_wrapped, count);
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

    template <typename = std::enable_if_t<IsSeekableV<WRITER>>>
    size_t LexSeek(const SeekPos pos)
    {
        LexFlush();
        return Seek<WRITER>(m_wrapped, pos);
    }
};

} // namespace LexIO
