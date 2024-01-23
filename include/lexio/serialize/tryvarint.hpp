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

/**
 * @brief Try to read a uint32_t from a stream as a varint.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadUVarint32(uint32_t &out, const ReaderRef &reader)
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

/**
 * @brief Try to write a uint32_t to a stream as a varint.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteUVarint32(const WriterRef &writer, uint32_t value)
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

/**
 * @brief Return number of bytes needed to encode an uint32_t as a varint.
 */
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

/**
 * @brief Try to read a int32_t from a stream as a varint.  Negative values
 *        are decoded as large positive integers.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadVarint32(int32_t &out, const ReaderRef &reader)
{
    uint32_t outVal = 0;
    if (!TryReadUVarint32(outVal, reader))
    {
        return false;
    }
    out = static_cast<int32_t>(outVal);
    return true;
}

/**
 * @brief Try to write a int32_t to a stream as a varint.  Negative values
 *        are encoded as large positive integers.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteVarint32(const WriterRef &writer, int32_t value)
{
    return TryWriteUVarint32(writer, static_cast<uint32_t>(value));
}

/**
 * @brief Return number of bytes needed to encode an int32_t as a varint.
 */
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

/**
 * @brief Try to read a int32_t from a stream as a varint.  Negative values
 *        are decoded as small positive integers using zig-zag encoding.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadSVarint32(int32_t &out, const ReaderRef &reader)
{
    uint32_t outVal;
    if (!TryReadUVarint32(outVal, reader))
    {
        return false;
    }
    out = static_cast<int32_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

/**
 * @brief Try to write a int32_t to a stream as a varint.  Negative values
 *        are encoded as small positive integers using zig-zag encoding.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteSVarint32(const WriterRef &writer, int32_t value)
{
    const uint32_t var = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    return TryWriteUVarint32(writer, var);
}

/**
 * @brief Return number of bytes needed to encode an int32_t as a varint using
 *        zig-zag encoding.
 */
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

/**
 * @brief Try to read a uint64_t from a stream as a varint.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadUVarint64(uint64_t &out, const ReaderRef &reader)
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

/**
 * @brief Try to write a uint64_t to a stream as a varint.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteUVarint64(const WriterRef &writer, const uint64_t value)
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

/**
 * @brief Return number of bytes needed to encode an uint64_t as a varint.
 */
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

/**
 * @brief Try to read a int64_t from a stream as a varint.  Negative values
 *        are decoded as large positive integers.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadVarint64(int64_t &out, const ReaderRef &reader)
{
    uint64_t outVal = 0;
    if (!TryReadUVarint64(outVal, reader))
    {
        return false;
    }
    out = static_cast<int64_t>(outVal);
    return true;
}

/**
 * @brief Try to write a int64_t to a stream as a varint.  Negative values
 *        are encoded as large positive integers.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteVarint64(const WriterRef &writer, int64_t value)
{
    return TryWriteUVarint64(writer, static_cast<uint64_t>(value));
}

/**
 * @brief Return number of bytes needed to encode an int64_t as a varint.
 */
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

/**
 * @brief Try to read a int64_t from a stream as a varint.  Negative values
 *        are decoded as small positive integers using zig-zag encoding.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadSVarint64(int64_t &out, const ReaderRef &reader)
{
    uint64_t outVal;
    if (!TryReadUVarint64(outVal, reader))
    {
        return false;
    }
    out = static_cast<int64_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

/**
 * @brief Try to write a int64_t to a stream as a varint.  Negative values
 *        are encoded as small positive integers using zig-zag encoding.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteSVarint64(const WriterRef &writer, int64_t value)
{
    const uint64_t var = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    return TryWriteUVarint64(writer, var);
}

/**
 * @brief Return number of bytes needed to encode an int64_t as a varint using
 *        zig-zag encoding.
 */
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

//******************************************************************************

} // namespace LexIO
