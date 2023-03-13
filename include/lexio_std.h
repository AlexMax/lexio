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

#include <iterator>
#include <vector> // Used by StdBufReader

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

    virtual size_t RawRead(ByteSpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_buffer.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(m_buffer.begin() + m_offset, m_buffer.begin() + m_offset + actualLength, buffer.begin());
        m_offset += actualLength;
        return actualLength;
    }

    virtual void Flush() {}

    virtual size_t RawWrite(ConstByteSpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_buffer.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(buffer.begin(), buffer.begin() + actualLength, m_buffer.begin() + m_offset);
        m_offset += actualLength;
        return actualLength;
    }

    virtual size_t Seek(const WhenceStart whence)
    {
        OffsetCheck(whence.offset);
        m_offset = static_cast<size_t>(whence.offset);
        return m_offset;
    }

    virtual size_t Seek(const WhenceCurrent whence)
    {
        const ptrdiff_t offset = static_cast<ptrdiff_t>(m_offset) + whence.offset;
        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }

    virtual size_t Seek(const WhenceEnd whence)
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
template <typename T>
class FixedStdBuffer : public StdBufferBase<T>
{
  public:
    explicit FixedStdBuffer(const size_t size) : StdBufferBase<T>(T()) { this->Buffer().resize(size); }
};

/**
 * @brief StdBuffer wrapping a type of default-constructed size.
 *
 * @detail Useful for types where size is known at compile-time, like std::array.
 */
template <typename T>
class StaticStdBuffer : public StdBufferBase<T>
{
  public:
    explicit StaticStdBuffer() : StdBufferBase<T>(T()) {}
};

/**
 * @brief The standard StdBuffer, where writing off the end of the buffer
 *        resizes the underlying type.
 */
template <typename T>
class StdBuffer : public StdBufferBase<T>
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

    size_t RawWrite(ConstByteSpanT buffer) override
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
 * @brief A buffer using a std::vector.
 *
 * @detail StdBufReader depends on std::vector, so we provide a type for a
 *         vector buffer.
 *
 * @tparam ALLOC Allocator used by vector.
 */
template <typename ALLOC = std::allocator<uint8_t>>
using VectorBuffer = LexIO::StdBuffer<std::vector<uint8_t, ALLOC>>;

/**
 * @brief A class that wraps a Reader type and gives it basic BufferedReader
 *        support.
 *
 * @detail Assumes buffer abstraction has STL methods.
 */
template <typename READER>
class StdBufReader
{
    READER m_reader;               // Underlying READER class.
    std::vector<uint8_t> m_buffer; // Buffer used for reading.
    size_t m_start = 0;            // Starting offset of buffered data.
    size_t m_end = 0;              // Ending offset of buffered data.

    StdBufReader(READER &&reader, const size_t startSize) : m_reader(std::move(reader)) { m_buffer.resize(startSize); }

  public:
    static constexpr size_t DEFAULT_BUFFER_SIZE = 8192;

    /**
     * @brief Construct a BufReader from the passed reading-capable type.
     *
     * @param reader Reader to wrap - moves into and takes ownership.
     * @param startSize Starting allocation size for the buffer.
     * @return A StdBufReader that wraps READER.
     */
    static StdBufReader FromReader(READER &&reader, const size_t startSize = DEFAULT_BUFFER_SIZE)
    {
        return StdBufReader{std::move(reader), startSize};
    }

    size_t RawRead(ByteSpanT buffer)
    {
        ConstByteSpanT peek = FillBuffer(buffer.size());
        std::copy(peek.begin(), peek.end(), buffer.begin());
        ConsumeBuffer(peek.size());
        return peek.size();
    }

    size_t GetBufferSize() noexcept { return m_buffer.size(); }

    ConstByteSpanT FillBuffer(const size_t size)
    {
        size_t wantedEnd = m_start + size;
        if (wantedEnd < m_end)
        {
            // We have enough data in the buffer, return the entire buffer.
            return ConstByteSpanT(&m_buffer[m_start], &m_buffer[m_end]);
        }
        else if (m_buffer.empty())
        {
            // Buffer is empty, resize it to fit.
            m_buffer.resize(size);
        }
        else if (wantedEnd >= m_buffer.size())
        {
            // Move unconsumed data to the start of the buffer and set new
            // start index to match.
            std::copy(m_buffer.begin() + m_start, m_buffer.begin() + m_end, m_buffer.begin());
            m_start = 0;
            if (size >= m_buffer.size())
            {
                // Not enough room in the data structure to fit incoming data,
                // so we grow it to fit.
                m_buffer.resize(size);
            }
        }

        // We don't have enough data buffered, read to make up the difference
        // and set the new end index appropriately.
        ByteSpanT target(m_buffer.begin() + m_start, m_buffer.begin() + static_cast<ptrdiff_t>(size));
        const size_t actualSize = m_reader.RawRead(target);
        m_end = m_start + actualSize;
        return ConstByteSpanT(m_buffer.begin() + m_start, m_buffer.begin() + m_end);
    }

    void ConsumeBuffer(const size_t size)
    {
        const size_t wantedStart = m_start + size;
        if (wantedStart > m_end)
        {
            throw std::runtime_error("Tried to consume more data than buffered.");
        }
        m_start = wantedStart;
    }
};

} // namespace LexIO
