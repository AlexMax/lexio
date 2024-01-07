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

//
// Stream and BufReader that uses std::vector under the hood.
//

#pragma once

#include "../core.hpp"

namespace LexIO
{

class ViewStream
{
  public:
    // Explicit rule-of-five for code coverage.

    ~ViewStream() {}
    ViewStream(const ViewStream &) = default;
    ViewStream(ViewStream &&) noexcept = default;
    ViewStream &operator=(const ViewStream &) = default;
    ViewStream &operator=(ViewStream &&) noexcept = default;

    ViewStream() = default;

    ViewStream(uint8_t *start, uint8_t *end) : m_start(start), m_end(end) {}

    template <size_t N>
    ViewStream(uint8_t (&array)[N]) : m_start(&array[0]), m_end(&array[N])
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        BufferView data = LexFillBuffer(count);
        std::memcpy(outDest, data.first, data.second);
        LexConsumeBuffer(data.second);
        return data.second;
    }

    BufferView LexFillBuffer(const size_t count)
    {
        if (m_offset == Size())
        {
            // EOF, return nullptr to avoid OOB subscript.
            return BufferView{nullptr, 0};
        }
        if (count <= m_bufferLength)
        {
            // Return view to current buffer.
            BufferView{&m_start[m_offset], m_bufferLength};
        }

        // Grow the buffer, if possible.
        const size_t wantedOffset = m_offset + count;
        const size_t destOffset = std::min(wantedOffset, Size());
        m_bufferLength = destOffset - m_offset;

        // Return view to new buffer.
        return BufferView{&m_start[m_offset], m_bufferLength};
    }

    void LexConsumeBuffer(const size_t count)
    {
        if (count > m_bufferLength)
        {
            throw new std::runtime_error("can't consume more bytes than buffer size");
        }

        // Advance the offset and shrink the buffer.
        m_offset += count;
        m_bufferLength -= count;
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        const size_t wantedOffset = m_offset + count;
        const size_t destOffset = std::min(wantedOffset, Size());
        const size_t actualLength = destOffset - m_offset;
        std::memcpy(m_start + m_offset, src, actualLength);
        m_offset += actualLength;
        return actualLength;
    }

    void LexFlush() {}

    size_t LexSeek(const SeekPos pos)
    {
        ptrdiff_t offset = 0;
        switch (pos.whence)
        {
        case LexIO::Whence::start:
            offset = pos.offset;
            break;
        case LexIO::Whence::current:
            offset = static_cast<ptrdiff_t>(m_offset) + pos.offset;
            break;
        case LexIO::Whence::end:
            offset = static_cast<ptrdiff_t>(Size()) - pos.offset;
            break;
        }

        if (offset < 0)
        {
            // Negative offsets are invalid.
            throw std::runtime_error("attempted seek to negative position");
        }

        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }

  protected:
    uint8_t *m_start = nullptr;
    uint8_t *m_end = nullptr;
    size_t m_offset = 0;
    size_t m_bufferLength = 0;

    size_t Size() const { return size_t(std::distance(m_start, m_end)); }
};

class ConstViewStream
{
  public:
    // Explicit rule-of-five for code coverage.

    ~ConstViewStream() {}
    ConstViewStream(const ConstViewStream &) = default;
    ConstViewStream(ConstViewStream &&) noexcept = default;
    ConstViewStream &operator=(const ConstViewStream &) = default;
    ConstViewStream &operator=(ConstViewStream &&) noexcept = default;

    ConstViewStream() = default;

    ConstViewStream(const uint8_t *start, const uint8_t *end) : m_start(start), m_end(end) {}

    template <size_t N>
    ConstViewStream(const uint8_t (&array)[N]) : m_start(&array[0]), m_end(&array[N])
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        const size_t wantedOffset = m_offset + count;
        const size_t destOffset = std::min(wantedOffset, Size());
        const size_t actualLength = destOffset - m_offset;
        std::memcpy(outDest, m_start + m_offset, actualLength);
        m_offset += actualLength;
        return actualLength;
    }

    size_t LexSeek(const SeekPos pos)
    {
        ptrdiff_t offset = 0;
        switch (pos.whence)
        {
        case LexIO::Whence::start:
            offset = pos.offset;
            break;
        case LexIO::Whence::current:
            offset = static_cast<ptrdiff_t>(m_offset) + pos.offset;
            break;
        case LexIO::Whence::end:
            offset = static_cast<ptrdiff_t>(Size()) - pos.offset;
            break;
        }

        if (offset < 0)
        {
            // Negative offsets are invalid.
            throw std::runtime_error("attempted seek to negative position");
        }

        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }

  protected:
    const uint8_t *m_start = nullptr;
    const uint8_t *m_end = nullptr;
    size_t m_offset = 0;
    size_t m_bufferLength = 0;

    size_t Size() const { return size_t(std::distance(m_start, m_end)); }
};

} // namespace LexIO
