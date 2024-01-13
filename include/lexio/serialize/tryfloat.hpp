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

using float32_t = float;
using float64_t = double;

/**
 * @brief Try to read a little-endian float32_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat32LE(float32_t &out, ReaderRef reader)
{
    uint32_t bits = 0;
    if (!TryReadU32LE(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float32_t>(bits);
    return true;
}

/**
 * @brief Try to read a big-endian float32_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat32BE(float32_t &out, ReaderRef reader)
{
    uint32_t bits = 0;
    if (!TryReadU32BE(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float32_t>(bits);
    return true;
}

/**
 * @brief Try to write a little-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat32LE(WriterRef writer, float32_t value)
{
    const uint32_t bits = Detail::BitCast<uint32_t>(value);
    return TryWriteU32LE(writer, bits);
}

/**
 * @brief Try to write a big-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat32BE(WriterRef writer, float32_t value)
{
    const uint32_t bits = Detail::BitCast<uint32_t>(value);
    return TryWriteU32BE(writer, bits);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian float64_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat64LE(float64_t &out, ReaderRef reader)
{
    uint64_t bits = 0;
    if (!TryReadU64LE(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float64_t>(bits);
    return true;
}

/**
 * @brief Try to read a big-endian float64_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat64BE(float64_t &out, ReaderRef reader)
{
    uint64_t bits = 0;
    if (!TryReadU64BE(bits, reader))
    {
        return false;
    }
    out = Detail::BitCast<float64_t>(bits);
    return true;
}

/**
 * @brief Try to write a little-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat64LE(WriterRef writer, float64_t value)
{
    const uint64_t bits = Detail::BitCast<uint64_t>(value);
    return TryWriteU64LE(writer, bits);
}

/**
 * @brief Try to write a big-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat64BE(WriterRef writer, float64_t value)
{
    const uint64_t bits = Detail::BitCast<uint64_t>(value);
    return TryWriteU64BE(writer, bits);
}

} // namespace LexIO
