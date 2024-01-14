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
 * @file tryvarint.hpp
 * @brief Protobuf-style varint serialization functions that return false on
 *        failure.
 */

#pragma once

#include "./tryint.hpp"

namespace LexIO
{

//******************************************************************************

inline bool TryReadUVarint32(uint32_t &out, ReaderRef reader)
{
    constexpr int MAX_BYTES = 5;
    uint32_t rvo = 0;
    uint8_t b = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            return false;
        }
        else if (!TryReadU8(b, reader))
        {
            return false;
        }

        rvo |= static_cast<uint32_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    out = rvo;
    return true;
}

inline bool TryWriteUVarint32(WriterRef writer, uint32_t value)
{
    uint32_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8(writer, static_cast<uint8_t>(v));
}

constexpr size_t UVarint32Bytes(uint32_t value)
{
    size_t count = 1;
    uint32_t v = value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

inline bool TryReadVarint32(int32_t &out, ReaderRef reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadUVarint32);
}

inline bool TryWriteVarint32(WriterRef writer, int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteUVarint32);
}

constexpr size_t Varint32Bytes(int32_t value)
{
    size_t count = 1;
    uint32_t v = 0 + value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

inline bool TryReadSVarint32(int32_t &out, ReaderRef reader)
{
    uint32_t outVal;
    if (!TryReadUVarint32(outVal, reader))
    {
        return false;
    }
    out = static_cast<int32_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

inline bool TryWriteSVarint32(WriterRef writer, int32_t value)
{
    const uint32_t var = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    return TryWriteUVarint32(writer, var);
}

constexpr size_t SVarint32Bytes(int32_t value)
{
    size_t count = 1;
    uint32_t v = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

inline bool TryReadUVarint64(uint64_t &out, ReaderRef reader)
{
    constexpr int MAX_BYTES = 10;
    uint64_t rvo = 0;
    uint8_t b = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            return false;
        }
        if (!TryReadU8(b, reader))
        {
            return false;
        }

        rvo |= static_cast<uint64_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    out = rvo;
    return true;
}

inline bool TryWriteUVarint64(WriterRef writer, const uint64_t value)
{
    uint64_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8(writer, static_cast<uint8_t>(v));
}

constexpr size_t UVarint64Bytes(const uint64_t value)
{
    size_t count = 1;
    uint64_t v = value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

inline bool TryReadVarint64(int64_t &out, ReaderRef reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadUVarint64);
}

inline bool TryWriteVarint64(WriterRef writer, int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteUVarint64);
}

constexpr size_t Varint64Bytes(int64_t value)
{
    size_t count = 1;
    uint64_t v = 0 + value;
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

//******************************************************************************

inline bool TryReadSVarint64(int64_t &out, ReaderRef reader)
{
    uint64_t outVal;
    if (!TryReadUVarint64(outVal, reader))
    {
        return false;
    }
    out = static_cast<int64_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

inline bool TryWriteSVarint64(WriterRef writer, int64_t value)
{
    const uint64_t var = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    return TryWriteUVarint64(writer, var);
}

constexpr size_t SVarint64Bytes(int64_t value)
{
    size_t count = 1;
    uint64_t v = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    while (v >= 0x80)
    {
        count += 1;
        v >>= 7;
    }
    return count;
}

} // namespace LexIO
