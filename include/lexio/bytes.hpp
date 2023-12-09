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

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadBytes(IT outStart, IT outEnd, READER &reader)
{
    const size_t count = std::distance(outStart, outEnd);
    return Read<READER>(&(*outStart), count, reader);
}

template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteBytes(WRITER &writer, IT start, IT end)
{
    const size_t count = std::distance(start, end);
    return Write<WRITER>(writer, &(*start), count);
}

template <typename READER, size_t N, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadBytes(uint8_t (&outArray)[N], READER &reader)
{
    return Read<READER>(&outArray[0], N, reader);
}

template <typename WRITER, size_t N, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t WriteBytes(WRITER &writer, uint8_t (&array)[N])
{
    return Write<WRITER>(writer, &array[0], N);
}

} // namespace LexIO
