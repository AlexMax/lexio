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

#include "./core.h"

#include "./container.h"

#include <vector>

namespace LexIO
{

/**
 * @brief A stream backed by a std::vector.
 */
using VectorStream = LexIO::ContainerDynamic<std::vector<uint8_t>>;

/**
 * @brief A class that wraps a Reader type and gives it basic BufferedReader
 *        support.
 *
 * @detail Assumes buffer abstraction has STL methods.
 */
template <typename READER>
class VectorBufReader
{
    READER m_reader;                  // Underlying READER class.
    std::vector<uint8_t> m_container; // Buffer used for reading.
    size_t m_start = 0;               // Starting offset of buffered data.
    size_t m_end = 0;                 // Ending offset of buffered data.

    VectorBufReader(READER &&reader, const size_t startSize) : m_reader(std::move(reader))
    {
        m_container.resize(startSize);
    }

  public:
    static constexpr size_t DEFAULT_BUFFER_SIZE = 8192;

    /**
     * @brief Construct a BufReader from the passed reading-capable type.
     *
     * @param reader Reader to wrap - moves into and takes ownership.
     * @param startSize Starting allocation size for the buffer.
     * @return A VectorBufReader that wraps READER.
     */
    static VectorBufReader FromReader(READER &&reader, const size_t startSize = DEFAULT_BUFFER_SIZE)
    {
        return VectorBufReader{std::move(reader), startSize};
    }

    size_t RawRead(ByteSpanT buffer)
    {
        ConstByteSpanT peek = FillBuffer(buffer.size());
        std::copy(peek.begin(), peek.end(), buffer.begin());
        ConsumeBuffer(peek.size());
        return peek.size();
    }

    size_t GetBufferSize() noexcept { return m_container.size(); }

    ConstByteSpanT FillBuffer(const size_t size)
    {
        size_t wantedEnd = m_start + size;
        if (wantedEnd < m_end)
        {
            // We have enough data in the buffer, return the entire buffer.
            return ConstByteSpanT(&m_container[m_start], &m_container[m_end]);
        }
        else if (m_container.empty())
        {
            // Buffer is empty, resize it to fit.
            m_container.resize(size);
        }
        else if (wantedEnd >= m_container.size())
        {
            // Move unconsumed data to the start of the buffer and set new
            // start index to match.
            std::copy(m_container.begin() + m_start, m_container.begin() + m_end, m_container.begin());
            m_start = 0;
            if (size >= m_container.size())
            {
                // Not enough room in the data structure to fit incoming data,
                // so we grow it to fit.
                m_container.resize(size);
            }
        }

        // We don't have enough data buffered, read to make up the difference
        // and set the new end index appropriately.
        ByteSpanT target(m_container.begin() + m_start, m_container.begin() + static_cast<ptrdiff_t>(size));
        const size_t actualSize = m_reader.RawRead(target);
        m_end = m_start + actualSize;
        return ConstByteSpanT(m_container.begin() + m_start, m_container.begin() + m_end);
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
