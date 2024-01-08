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

/**
 * @brief Read string data from the current offset, inserting it into the
 *        passed buffer.
 *
 * @param outDest Pointer to starting byte of output buffer.
 * @param count Size of output buffer in bytes.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadChars(char *outDest, const size_t count, READER &reader)
{
    uint8_t *outDestByte = Detail::BitCast<uint8_t *>(outDest);
    return Read<READER>(outDestByte, count, reader);
}

/**
 * @brief Read string data from the current offset, inserting it into the
 *        passed buffer.
 *
 * @param outArray Output buffer array.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER, size_t N, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadChars(char (&outArray)[N], READER &reader)
{
    uint8_t *outDestByte = Detail::BitCast<uint8_t *>(&outArray[0]);
    return Read<READER>(outDestByte, N, reader);
}

/**
 * @brief Read string data from the current offset, inserting it into the
 *        passed buffer.
 *
 * @param outStart Iterator to start byte of output buffer.
 * @param outEnd Iterator to end byte of output buffer.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadChars(IT outStart, IT outEnd, READER &reader)
{
    if (outStart == outEnd)
    {
        return 0;
    }

    uint8_t *outDestByte = Detail::BitCast<uint8_t *>(std::addressof(*outStart));
    const size_t count = std::distance(outStart, outEnd);
    return Read<READER>(outDestByte, count, reader);
}

/**
 * @brief Write a buffer of string data at the current offset.
 *
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteChars(WRITER &writer, const char *src, const size_t count)
{
    const uint8_t *srcByte = Detail::BitCast<const uint8_t *>(src);
    return Write<WRITER>(writer, srcByte, count);
}

/**
 * @brief Write a buffer of string data at the current offset.
 *
 * @param writer Writer to operate on.
 * @param array Input buffer array.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, size_t N, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteChars(WRITER &writer, const char (&array)[N])
{
    const uint8_t *srcByte = Detail::BitCast<const uint8_t *>(&array[0]);
    return Write<WRITER>(writer, srcByte, N);
}

/**
 * @brief Write a buffer of string data at the current offset.
 *
 * @param writer Writer to operate on.
 * @param start Iterator to start byte of input buffer.
 * @param end Iterator to end byte of input buffer.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteChars(WRITER &writer, IT start, IT end)
{
    if (start == end)
    {
        return 0;
    }

    const uint8_t *src = Detail::BitCast<const uint8_t *>(std::addressof(*start));
    const size_t count = std::distance(start, end);
    return Write<WRITER>(writer, src, count);
}

} // namespace LexIO
