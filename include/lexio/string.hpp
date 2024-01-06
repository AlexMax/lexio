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

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadString(char *outDest, const size_t count, READER &reader)
{
    uint8_t *outDestByte = reinterpret_cast<uint8_t *>(outDest);
    return Read<READER>(outDestByte, count, reader);
}

template <typename READER, size_t N, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadString(char (&outArray)[N], READER &reader)
{
    uint8_t *outDestByte = reinterpret_cast<uint8_t *>(&outArray[0]);
    return Read<READER>(outDestByte, N, reader);
}

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadString(IT outStart, IT outEnd, READER &reader)
{
    uint8_t *outDestByte = reinterpret_cast<uint8_t *>(std::addressof(*outStart));
    const size_t count = std::distance(outStart, outEnd);
    return Read<READER>(outDestByte, count, reader);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteString(WRITER &writer, const char *src, const size_t count)
{
    const uint8_t *srcByte = reinterpret_cast<const uint8_t *>(src);
    return Write<WRITER>(writer, srcByte, count);
}

template <typename WRITER, size_t N, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteString(WRITER &writer, const char (&array)[N])
{
    const uint8_t *srcByte = reinterpret_cast<const uint8_t *>(&array[0]);
    return Write<WRITER>(writer, srcByte, N);
}

template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteString(WRITER &writer, IT start, IT end)
{
    if (start == end)
    {
        return 0;
    }

    const uint8_t *src = reinterpret_cast<const uint8_t *>(std::addressof(*start));
    const size_t count = std::distance(start, end);
    return Write<WRITER>(writer, src, count);
}

} // namespace LexIO
