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
 * @file lib.hpp
 * Common stream functionality that builds off of basic functions.
 */

#pragma once

#include "./core.hpp"

#include <algorithm>
#include <memory>

namespace LexIO
{

/**
 * @brief Read the entire contents of the stream.  Uses an internal buffer.
 *
 * @param outIt Output iterator to write result into.
 * @param reader Reader to operate on.
 * @return Total number of bytes read.
 */
template <typename OUT_ITER>
inline size_t ReadToEOF(OUT_ITER outIt, UnbufferedReaderRef reader)
{
    constexpr size_t BUFFER_SIZE = 8192;

    // Try and read on the stack first.
    uint8_t shortData[32] = {0};
    size_t count = Read(shortData, reader);
    if (count == 0)
    {
        // Nothing to read.
        return 0;
    }

    // Copy data into the iterator.
    std::copy(&shortData[0], &shortData[0] + count, outIt);
    if (count < 32)
    {
        return count;
    }

    // Allocate a temporary buffer to read into.
    size_t total = count;
    std::unique_ptr<uint8_t[]> data{::new uint8_t[BUFFER_SIZE]};
    for (;;)
    {
        count = Read(data.get(), reader, BUFFER_SIZE);
        if (count == 0)
        {
            // Read all data there was to read.
            return total;
        }

        // Copy data into the iterator.
        std::copy(data.get(), data.get() + count, outIt);
        total += count;
    }
}

/**
 * @brief Read the entire contents of the stream.
 *
 * @param outIt Output iterator to write result into.
 * @param bufReader BufferedReader to operate on.
 * @return Total number of bytes read.
 */
template <typename OUT_ITER>
inline size_t ReadToEOF(OUT_ITER outIt, BufferedReaderRef bufReader)
{
    constexpr size_t BUFFER_SIZE = 8192;

    size_t total = 0;
    for (;;)
    {
        BufferView buf = FillBuffer(bufReader, BUFFER_SIZE);
        if (buf.Size() == 0)
        {
            // Read all data there was to read.
            return total;
        }

        // Copy buffered data into the iterator.
        std::copy(buf.Data(), buf.Data() + buf.Size(), outIt);

        // Consume what we've read.
        ConsumeBuffer(bufReader, buf.Size());
        total += buf.Size();
    }
}

/**
 * @brief Read the entire contents of the stream until we hit a terminating byte
 *        or until EOF is hit.  The output will contain the terminator as the
 *        last character, if seen.
 *
 * @param outIt Output iterator to write result into.
 * @param bufReader BufferedReader to operate on.
 * @param term Byte to stop at.
 * @return Total number of bytes read.
 */
template <typename OUT_ITER>
inline size_t ReadUntil(OUT_ITER outIt, BufferedReaderRef bufReader, const uint8_t term)
{
    constexpr size_t BUFFER_SIZE = 8192;

    size_t size = 0;
    for (;;)
    {
        BufferView buf = FillBuffer(bufReader, BUFFER_SIZE);
        if (buf.Size() == 0)
        {
            // Read all data there was to read.
            return size;
        }

        // Copy the buffered data into the iterator until we hit the passed byte.
        const uint8_t *it = buf.Data();
        for (size_t i = 0; i < buf.Size(); i++)
        {
            if (*it == term)
            {
                // Found the terminator, append it and stop.
                *outIt++ = *it++;
                ConsumeBuffer(bufReader, i + 1);
                size += i + 1;
                return size;
            }
            *outIt++ = *it++;
        }

        // Consume what we've read.
        ConsumeBuffer(bufReader, BUFFER_SIZE);
        size += BUFFER_SIZE;
    }
}

/**
 * @brief Copy the contents of a buffered reader to a writer until EOF is hit
 *        on the reader.
 *
 * @param writer Writer to copy to.
 * @param bufReader Buffered read to read from.
 * @return Number of bytes copied.
 */
inline size_t Copy(WriterRef writer, BufferedReaderRef bufReader)
{
    constexpr size_t BUFFER_SIZE = 8192;

    size_t count = 0;
    for (;;)
    {
        const BufferView buffer = FillBuffer(bufReader, BUFFER_SIZE);
        if (buffer.Size() == 0)
        {
            return count;
        }

        const size_t written = Write(writer, buffer.Data(), buffer.Size());
        ConsumeBuffer(bufReader, written);
        count += written;
    }
}

} // namespace LexIO
