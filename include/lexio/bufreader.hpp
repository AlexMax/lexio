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

template <typename T>
class GenericBufReader
{
    T m_wrapped;
    uint8_t *m_buffer = nullptr;
    size_t m_allocSize = 0;
    size_t m_size = 0;

  public:
    GenericBufReader(T &&wrapped) : m_wrapped(wrapped) {}

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
            uint8_t *buffer = ::new uint8_t[count];
            std::copy(&m_buffer[0], &m_buffer[m_size], buffer);
            ::delete[] m_buffer;
            m_buffer = buffer;
            m_allocSize = count;
        }

        // Read into the buffer.
        const size_t wanted = count - m_size;
        const size_t actual = LexIO::Read<T>(&m_buffer[m_size], wanted, m_wrapped);
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
};

} // namespace LexIO
