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

#include "./tryint.hpp"

namespace LexIO
{

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadFloatLE(float &out, READER &reader)
{
    uint32_t bits;
    if (!TryReadU32LE<READER>(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float, uint32_t>(bits);
    return true;
}

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadFloatBE(float &out, READER &reader)
{
    uint32_t bits;
    if (!TryReadU32BE<READER>(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float, uint32_t>(bits);
    return true;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteFloatLE(WRITER &writer, const float value)
{
    const uint32_t bits = Detail::BitCast<uint32_t, float>(value);
    return TryWriteU32LE<WRITER>(writer, bits);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteFloatBE(WRITER &writer, const float value)
{
    const uint32_t bits = Detail::BitCast<uint32_t, float>(value);
    return TryWriteU32BE<WRITER>(writer, bits);
}

//******************************************************************************

} // namespace LexIO
