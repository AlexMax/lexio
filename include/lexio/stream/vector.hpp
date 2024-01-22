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
 * @file vector.hpp
 * @brief Stream implementation that uses std::vector under the hood.
 */

#pragma once

#include "../core.hpp"

#include <stdexcept>
#include <vector>

namespace LexIO
{

/**
 * @brief Stream implementation that uses std::vector under the hood.
 */
class VectorStream
{
  public:
    using container_type = std::vector<uint8_t>;

    // Explicit rule-of-five for code coverage.

    ~VectorStream() {}
    VectorStream(const VectorStream &) = default;
    VectorStream(VectorStream &&) noexcept = default;
    VectorStream &operator=(const VectorStream &) = default;
    VectorStream &operator=(VectorStream &&) noexcept = default;

    VectorStream() = default;
    VectorStream(const container_type &buffer) : m_container(buffer) {}
    VectorStream(container_type &&buffer) : m_container(buffer) {}

    /**
     * @brief Construct stream with contents of initializer list.
     */
    VectorStream(std::initializer_list<uint8_t> list) : m_container(container_type{list}) {}

    /**
     * @brief A read-only reference of the wrapped container.
     */
    const container_type &Container() const & { return m_container; }

    /**
     * @brief Obtain the underlying container while moving-from the
     *        VectorStream.
     */
    container_type Container() && { return m_container; }

    /**
     * @brief Replace existing container with copy of passed container.
     */
    void Container(const container_type &container) { m_container = container; }

    /**
     * @brief Replace existing container with passed container.
     */
    void Container(container_type &&container) { m_container = container; }

    size_t LexRead(uint8_t *outDest, size_t count)
    {
        BufferView data = LexFillBuffer(count);
        const size_t actualSize = Detail::Min(count, data.Size());
        std::memcpy(outDest, data.Data(), actualSize);
        LexConsumeBuffer(actualSize);
        return actualSize;
    }

    BufferView LexFillBuffer(size_t count)
    {
        if (m_bufferOffset == m_container.size())
        {
            // EOF, return nullptr to avoid OOB subscript.
            return BufferView{nullptr, 0};
        }

        size_t bufferLength = BufferSize();
        if (count <= bufferLength)
        {
            // Return view to current buffer.
            return BufferView{&m_container[m_bufferOffset], bufferLength};
        }

        // Grow the buffer by reading into it.
        const size_t wantedOffset = m_bufferOffset + count;
        m_offset = Detail::Min(wantedOffset, m_container.size());
        bufferLength = BufferSize();

        // Return view to new buffer.
        return BufferView{&m_container[m_bufferOffset], bufferLength};
    }

    void LexConsumeBuffer(size_t count)
    {
        if (count > BufferSize())
        {
            throw new std::runtime_error("can't consume more bytes than buffer size");
        }

        // Shrink the buffer.
        m_bufferOffset += count;
    }

    size_t LexWrite(const uint8_t *src, size_t count)
    {
        // Writes off the end of the burffer grow the buffer to fit.
        const size_t wantedOffset = m_offset + count;
        m_container.resize(Detail::Max(wantedOffset, m_container.size()));
        std::memcpy(m_container.data() + m_offset, src, count);
        m_offset += count;
        m_bufferOffset = m_offset;
        return count;
    }

    void LexFlush() {}

    size_t LexSeek(const SeekPos &pos)
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
            offset = static_cast<ptrdiff_t>(m_container.size()) - pos.offset;
            break;
        }

        if (offset < 0)
        {
            // Negative offsets are invalid.
            throw std::runtime_error("attempted seek to negative position");
        }

        m_offset = static_cast<size_t>(offset);
        m_bufferOffset = m_offset;
        return m_offset;
    }

  protected:
    container_type m_container;
    size_t m_offset = 0;
    size_t m_bufferOffset = 0;

    size_t BufferSize() const { return m_offset - m_bufferOffset; }
};

} // namespace LexIO
