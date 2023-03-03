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
// Sample buffer abstraction that wraps STL-like types.
//

#pragma once

#include "lexio_core.h"

namespace LexIO
{

/**
 * @brief A seekable reader/writer that uses a backing type exposing STL
 *        conventions.
 *
 * @details This class is very inconvenient to construct, prefer one of
 *          the other StdBuffer types.
 *
 * @tparam T Type to wrap.
 */
template <typename T>
class StdBufferBase
{
  private:
    T m_buffer;
    size_t m_offset = 0;

  protected:
    T &Buffer() { return m_buffer; }
    size_t &Offset() { return m_offset; }

    void OffsetCheck(const ptrdiff_t offset)
    {
        if (offset < 0)
        {
            throw std::runtime_error("attempted seek to negative position");
        }
    }

  public:
    StdBufferBase() = delete;
    StdBufferBase(const T &buffer) : m_buffer(buffer) {}
    StdBufferBase(T &&buffer) : m_buffer(buffer) {}

    size_t RawRead(SpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_offset + buffer.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(m_buffer.begin() + m_offset, m_buffer.begin() + m_offset + actualLength, buffer.begin());
        m_offset += actualLength;
        return actualLength;
    }

    void Flush() {}

    ConstSpanT Data() const noexcept { return ConstSpanT(m_buffer.data(), m_buffer.size()); }

    size_t RawWrite(ConstSpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_buffer.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(buffer.begin(), buffer.begin() + actualLength, m_buffer.begin() + m_offset);
        m_offset += actualLength;
        return actualLength;
    }

    SpanT Data() noexcept { return SpanT(m_buffer.data(), m_buffer.size()); }

    size_t Seek(const WhenceStart whence)
    {
        OffsetCheck(whence.offset);
        m_offset = static_cast<size_t>(whence.offset);
        return m_offset;
    }

    size_t Seek(const WhenceCurrent whence)
    {
        const ptrdiff_t offset = static_cast<ptrdiff_t>(m_offset) + whence.offset;
        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }

    size_t Seek(const WhenceEnd whence)
    {
        const ptrdiff_t offset = static_cast<ptrdiff_t>(m_buffer.size()) - whence.offset;
        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }
};

/**
 * @brief StdBuffer wrapping a type whose size is permanent after construction.
 */
template <typename T> class FixedStdBuffer : public StdBufferBase<T>
{
  public:
    explicit FixedStdBuffer(const size_t size) : StdBufferBase<T>(T()) { this->Buffer().resize(size); }
};

/**
 * @brief StdBuffer wrapping a type of default-constructed size.
 *
 * @detail Useful for types where size is known at compile-time, like std::array.
 */
template <typename T> class StaticStdBuffer : public StdBufferBase<T>
{
  public:
    explicit StaticStdBuffer() : StdBufferBase<T>(T()) {}
};

/**
 * @brief The standard StdBuffer, where writing off the end of the buffer
 *        resizes the underlying type.
 */
template <typename T> class StdBuffer : public StdBufferBase<T>
{
  public:
    /**
     * @brief Constructs an empty StdBuffer.
     */
    StdBuffer() : StdBufferBase<T>(T()) {}

    /**
     * @brief Constructs a StdBuffer with a pre-allocated size.
     *
     * @param size Size of underlying type.
     */
    explicit StdBuffer(const size_t size) : StdBufferBase<T>(T()) { this->Buffer().resize(size); }

    /**
     * @brief Construct a StdBuffer with data already written into the buffer.
     *
     * @param list Initializer list of data.
     */
    StdBuffer(std::initializer_list<uint8_t> list) : StdBufferBase<T>(T())
    {
        this->Buffer().resize(list.size());
        std::copy(list.begin(), list.end(), this->Buffer().begin());
    }

    size_t RawWrite(ConstSpanT buffer)
    {
        // Writes off the end of the burffer grow the buffer to fit.
        const size_t wantedOffset = this->Offset() + buffer.size();
        this->Buffer().resize(std::max(wantedOffset, this->Buffer().size()));
        std::copy(buffer.begin(), buffer.end(), this->Buffer().begin() + this->Offset());
        this->Offset() += buffer.size();
        return buffer.size();
    }
};

/**
 * @brief A span buffer using the current span type.
 */
using SpanBuffer = StdBuffer<SpanT>;

} // namespace LexIO
