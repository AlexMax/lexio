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
// Sample buffer abstraction that uses malloc, realloc, and free under the hood.
//

#ifndef LEXIO_BUFREADER_H
#define LEXIO_BUFREADER_H

#include "./core.h"

namespace LexIO
{

class Buffer
{
  public:
    virtual ~Buffer() = 0;
    virtual uint8_t *Data() = 0;
    virtual size_t Size() const = 0;
    virtual void Resize(const size_t size) = 0;
};

class CBuffer final : public Buffer
{
    uint8_t *m_ptr = nullptr;
    size_t m_size = 0;

  public:
    CBuffer(const CBuffer &o) : m_ptr(static_cast<uint8_t *>(std::malloc(o.m_size))), m_size(o.m_size)
    {
        std::copy(o.m_ptr, o.m_ptr + m_size, m_ptr);
    }
    CBuffer(CBuffer &&o) noexcept : m_ptr(o.m_ptr), m_size(o.m_size)
    {
        o.m_ptr = nullptr;
        o.m_size = 0;
    }
    CBuffer &operator=(const CBuffer &o)
    {
        *this = CBuffer(o);
        return *this;
    }
    CBuffer &operator=(CBuffer &&o) noexcept
    {
        o.m_ptr = nullptr;
        o.m_size = 0;
        return *this;
    }
    ~CBuffer() { std::free(m_ptr); }

    uint8_t *Data() { return m_ptr; }

    size_t Size() const { return m_size; }

    void Resize(const size_t size)
    {
        if (size > m_size)
        {
            uint8_t *newptr = static_cast<uint8_t *>(std::realloc(m_ptr, size));
            if (newptr == nullptr)
            {
                std::terminate();
            }
            m_ptr = newptr;
            m_size = size;
        }
    }
};

template <typename READER, typename BUFFER = CBuffer>
class BufReader
{
    READER m_reader;    // Underlying READER class.
    BUFFER m_buffer;    // Buffer used for reading.
    size_t m_start = 0; // Starting offset of buffered data.
    size_t m_end = 0;   // Ending offset of buffered data.

    BufReader(READER &&reader, const size_t startSize) : m_reader(std::move(reader)) { m_buffer.Resize(startSize); }

  public:
    static constexpr size_t DEFAULT_BUFFER_SIZE = 8192;

    /**
     * @brief Construct a BufReader from the passed reading-capable type.
     *
     * @param reader Reader to wrap - moves into and takes ownership.
     * @param startSize Starting allocation size for the buffer.
     * @return A StdBufReader that wraps READER.
     */
    static BufReader FromReader(READER &&reader, const size_t startSize = DEFAULT_BUFFER_SIZE)
    {
        return BufReader{std::move(reader), startSize};
    }

    size_t RawRead(ByteSpanT buffer)
    {
        ConstByteSpanT peek = FillBuffer(buffer.size());
        std::copy(peek.begin(), peek.end(), buffer.begin());
        ConsumeBuffer(peek.size());
        return peek.size();
    }

    size_t GetBufferSize() noexcept { return m_buffer.Size(); }

    ConstByteSpanT FillBuffer(const size_t size)
    {
        size_t wantedEnd = m_start + size;
        if (wantedEnd < m_end)
        {
            // We have enough data in the buffer, return the entire buffer.
            return ConstByteSpanT(&m_buffer[m_start], &m_buffer[m_end]);
        }

        // Ensure buffer can hold the amount of data we want to return.
        m_buffer.Resize(size);

        if (wantedEnd >= m_buffer.Size())
        {
            // Move unconsumed data to the start of the buffer and set new
            // start index to match.
            std::copy(m_buffer.Data() + m_start, m_buffer.Data() + m_end, m_buffer.Data());
            m_start = 0;
        }

        // We don't have enough data buffered, read to make up the difference
        // and set the new end index appropriately.
        ByteSpanT target(m_buffer.Data() + m_start, m_buffer.Data() + size);
        const size_t actualSize = m_reader.RawRead(target);
        m_end = m_start + actualSize;
        return ConstByteSpanT(m_buffer.Data() + m_start, m_buffer.Data() + m_end);
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

#endif
