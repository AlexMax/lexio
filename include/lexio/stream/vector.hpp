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

#include "./container.hpp"

#include <vector>

namespace LexIO
{

class VectorStream
{
  public:
    using container_type = std::vector<uint8_t>;

  protected:
    container_type m_container;
    size_t m_offset = 0;

    void OffsetCheck(const ptrdiff_t offset)
    {
        if (offset < 0)
        {
            throw std::runtime_error("attempted seek to negative position");
        }
    }

  public:
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
    const container_type &Container() const { return m_container; }

    /**
     * @brief Replace existing container with passed container.
     */
    void Container(container_type &&container) { m_container = container; }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        const size_t wantedOffset = m_offset + count;
        const size_t destOffset = std::min(wantedOffset, m_container.size());
        const size_t actualLength = destOffset - m_offset;
        std::memcpy(outDest, m_container.data() + m_offset, actualLength);
        m_offset += actualLength;
        return actualLength;
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        // Writes off the end of the burffer grow the buffer to fit.
        const size_t wantedOffset = m_offset + count;
        m_container.resize(std::max(wantedOffset, m_container.size()));
        std::memcpy(m_container.data() + m_offset, src, count);
        m_offset += count;
        return count;
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
            offset = static_cast<ptrdiff_t>(m_container.size()) - pos.offset;
            break;
        }

        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }
};

} // namespace LexIO
