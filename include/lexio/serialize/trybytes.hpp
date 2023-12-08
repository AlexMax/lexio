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

#include "../core.hpp"

namespace LexIO
{

//******************************************************************************

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadBytes(IT outBegin, IT outEnd, READER &reader)
{
    const size_t length = outEnd - outBegin;
    const size_t count = Read<READER>(&(*outBegin), length, reader);
    if (count != length)
    {
        return false;
    }
    return true;
}

template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteBytes(WRITER &writer, IT begin, IT end)
{
    const size_t length = end - begin;
    const size_t count = Write<WRITER>(writer, &(*begin), length);
    if (count != length)
    {
        return false;
    }
    return true;
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadData(void *outData, const size_t length, READER &reader)
{
    uint8_t *castData = Detail::BitCast<uint8_t *>(outData);

    const size_t count = Read<READER>(castData, length, reader);
    if (count != length)
    {
        return false;
    }
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteData(WRITER &writer, const void *data, const size_t length)
{
    const uint8_t *castData = Detail::BitCast<const uint8_t *>(data);

    const size_t count = Write<WRITER>(writer, castData, length);
    if (count != length)
    {
        return false;
    }
    return true;
}

//******************************************************************************

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadString(IT outBegin, IT outEnd, READER &reader)
{
    uint8_t *castBegin = Detail::BitCast<uint8_t *>(&(*outBegin));
    const size_t length = outEnd - outBegin;

    const size_t count = Read<READER>(castBegin, length, reader);
    if (count != length)
    {
        return false;
    }
    return true;
}

template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteString(WRITER &writer, IT begin, IT end)
{
    const uint8_t *castBegin = Detail::BitCast<const uint8_t *>(&(*begin));
    const size_t length = end - begin;

    const size_t count = Write<WRITER>(writer, castBegin, length);
    if (count != length)
    {
        return false;
    }
    return true;
}

} // namespace LexIO
