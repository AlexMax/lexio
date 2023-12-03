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

#include "./tryfloat.hpp"

namespace LexIO
{

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline float ReadFloatLE(READER &reader)
{
    return Detail::ReadWithExcept<float>(reader, TryReadFloatLE<READER>);
}

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline float ReadFloatBE(READER &reader)
{
    return Detail::ReadWithExcept<float>(reader, TryReadFloatBE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteFloatLE(WRITER &writer, const float value)
{
    Detail::WriteWithExcept<float>(writer, value, TryWriteFloatLE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteFloatBE(WRITER &writer, const float value)
{
    Detail::WriteWithExcept<float>(writer, value, TryWriteFloatBE<WRITER>);
}

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline double ReadDoubleLE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64LE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteDoubleLE(WRITER &writer, const double value)
{
    uint64_t out;
    std::memcpy(&out, &value, sizeof(out));
    WriteU64LE(writer, out);
}

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline double ReadDoubleBE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64BE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteDoubleBE(WRITER &writer, const double value)
{
    uint64_t out;
    std::memcpy(&out, &value, sizeof(out));
    WriteU64BE(writer, out);
}

} // namespace LexIO
