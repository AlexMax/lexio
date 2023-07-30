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

//
// Serialization functions.
//

#ifndef LEXIO_SERIALIZE_H
#define LEXIO_SERIALIZE_H

#include "./core.h"

#include <cstring>

namespace LexIO
{

namespace Detail
{

template <typename T>
inline constexpr T FromByte(const uint8_t ch, const int sh)
{
    return T(ch) << sh;
}

template <typename T>
inline constexpr uint8_t ToByte(const T val, const int sh, const T mask = 0xff)
{
    return static_cast<uint8_t>((val & (mask << sh)) >> sh);
}

template <typename TYPE, typename READER, typename TRY_READ>
inline constexpr TYPE ReadWithExcept(READER &reader, TRY_READ &tryRead)
{
    TYPE rvo;
    if (!tryRead(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

template <typename TYPE, typename WRITER, typename TRY_WRITE>
inline constexpr void WriteWithExcept(WRITER &writer, const TYPE &value, TRY_WRITE &tryWrite)
{
    if (!tryWrite(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

template <typename TYPE, typename READER, typename TRY_READ>
inline constexpr bool ReadSigned(TYPE &out, READER &reader, TRY_READ &tryRead)
{
    using UNSIGNED_TYPE = std::make_unsigned_t<TYPE>;
    UNSIGNED_TYPE outVal;
    if (!tryRead(outVal, reader))
    {
        return false;
    }
    out = static_cast<TYPE>(outVal);
    return true;
}

template <typename TYPE, typename WRITER, typename TRY_WRITE>
inline constexpr bool WriteSigned(WRITER &writer, const TYPE &value, TRY_WRITE &tryWrite)
{
    return tryWrite(writer, static_cast<TYPE>(value));
}

template <typename TYPE, typename READER, typename READ>
inline constexpr TYPE ReadSignedWithExcept(READER &reader, READ &read)
{
    return static_cast<TYPE>(read(reader));
}

template <typename TYPE, typename WRITER, typename WRITE>
inline constexpr void WriteSignedWithExcept(WRITER &writer, const TYPE &value, WRITE &write)
{
    using UNSIGNED_TYPE = std::make_unsigned_t<TYPE>;
    write(writer, static_cast<UNSIGNED_TYPE>(value));
}

} // namespace Detail

//******************************************************************************

template <typename READER>
inline bool TryReadU8(uint8_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint8_t))
    {
        return false;
    }
    out = buf[0];
    return true;
}

template <typename WRITER>
inline bool TryWriteU8(WRITER &writer, const uint8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint8_t);
}

template <typename READER>
inline uint8_t ReadU8(READER &reader)
{
    return Detail::ReadWithExcept<uint8_t>(reader, TryReadU8<READER>);
}

template <typename WRITER>
inline void WriteU8(WRITER &writer, const uint8_t value)
{
    Detail::WriteWithExcept<uint8_t>(writer, value, TryWriteU8<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryRead8(int8_t &out, READER &reader)
{
    return Detail::ReadSigned<int8_t>(out, reader, TryReadU8<READER>);
}

template <typename WRITER>
inline bool TryWrite8(WRITER &writer, const int8_t value)
{
    return Detail::WriteSigned<int8_t>(writer, value, TryWriteU8<WRITER>);
}

template <typename READER>
inline int8_t Read8(READER &reader)
{
    return Detail::ReadSignedWithExcept<int8_t>(reader, ReadU8<READER>);
}

template <typename WRITER>
inline void Write8(WRITER &writer, const int8_t value)
{
    Detail::WriteSignedWithExcept<int8_t>(writer, WriteU8<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryReadU16LE(uint16_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint16_t))
    {
        return false;
    }
    out = Detail::FromByte<uint16_t>(buf[0], 0) | Detail::FromByte<uint16_t>(buf[1], 8);
    return true;
}

template <typename READER>
inline uint16_t TryReadU16BE(uint16_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint16_t))
    {
        return false;
    }
    out = Detail::FromByte<uint16_t>(buf[0], 8) | Detail::FromByte<uint16_t>(buf[1], 0);
    return true;
}

template <typename WRITER>
inline bool TryWriteU16LE(WRITER &writer, const uint16_t value)
{
    const uint8_t buf[sizeof(uint16_t)] = {
        Detail::ToByte(value, 0),
        Detail::ToByte(value, 8),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint16_t);
}

template <typename WRITER>
inline bool TryWriteU16BE(WRITER &writer, const uint16_t value)
{
    const uint8_t buf[sizeof(uint16_t)] = {
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 0),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint16_t);
}

template <typename READER>
inline uint16_t ReadU16LE(READER &reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16LE<READER>);
}

template <typename READER>
inline uint16_t ReadU16BE(READER &reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16BE<READER>);
}

template <typename WRITER>
inline void WriteU16LE(WRITER &writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16LE<WRITER>);
}

template <typename WRITER>
inline void WriteU16BE(WRITER &writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryRead16LE(int16_t &out, READER &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16LE<READER>);
}

template <typename READER>
inline bool TryRead16BE(int16_t &out, READER &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16BE<READER>);
}

template <typename WRITER>
inline bool TryWrite16LE(WRITER &writer, const int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16LE<WRITER>);
}

template <typename WRITER>
inline bool TryWrite16BE(WRITER &writer, const int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16BE<WRITER>);
}

template <typename READER>
inline int16_t Read16LE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int16_t>(reader, ReadU16LE<READER>);
}

template <typename READER>
inline int16_t Read16BE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int16_t>(reader, ReadU16BE<READER>);
}

template <typename WRITER>
inline void Write16LE(WRITER &writer, const int16_t value)
{
    Detail::WriteSignedWithExcept<uint16_t>(writer, value, TryWriteU16LE<WRITER>);
}

template <typename WRITER>
inline void Write16BE(WRITER &writer, const int16_t value)
{
    Detail::WriteSignedWithExcept<uint16_t>(writer, value, TryWriteU16BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryReadU32LE(uint32_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint32_t))
    {
        return false;
    }
    out = Detail::FromByte<uint32_t>(buf[0], 0) | Detail::FromByte<uint32_t>(buf[1], 8) |
          Detail::FromByte<uint32_t>(buf[2], 16) | Detail::FromByte<uint32_t>(buf[3], 24);
    return true;
}

template <typename READER>
inline bool TryReadU32BE(uint32_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint32_t))
    {
        return false;
    }
    out = Detail::FromByte<uint32_t>(buf[0], 24) | Detail::FromByte<uint32_t>(buf[1], 16) |
          Detail::FromByte<uint32_t>(buf[2], 8) | Detail::FromByte<uint32_t>(buf[3], 0);
    return true;
}

template <typename WRITER>
inline bool TryWriteU32LE(WRITER &writer, const uint32_t value)
{
    const uint8_t buf[sizeof(uint32_t)] = {
        Detail::ToByte(value, 0),
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 16),
        Detail::ToByte(value, 24),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint32_t);
}

template <typename WRITER>
inline bool TryWriteU32BE(WRITER &writer, const uint32_t value)
{
    const uint8_t buf[sizeof(uint32_t)] = {
        Detail::ToByte(value, 24),
        Detail::ToByte(value, 16),
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 0),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint32_t);
}

template <typename READER>
inline uint32_t ReadU32LE(READER &reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32LE<READER>);
}

template <typename READER>
inline uint32_t ReadU32BE(READER &reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32BE<READER>);
}

template <typename WRITER>
inline void WriteU32LE(WRITER &writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32LE<WRITER>);
}

template <typename WRITER>
inline void WriteU32BE(WRITER &writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryRead32LE(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32LE<READER>);
}

template <typename READER>
inline bool TryRead32BE(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32BE<READER>);
}

template <typename WRITER>
inline bool TryWrite32LE(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32LE<WRITER>);
}

template <typename WRITER>
inline bool TryWrite32BE(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32BE<WRITER>);
}

template <typename READER>
inline int32_t Read32LE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int32_t>(reader, ReadU32LE<READER>);
}

template <typename READER>
inline int32_t Read32BE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int32_t>(reader, ReadU32BE<READER>);
}

template <typename WRITER>
inline void Write32LE(WRITER &writer, const int32_t value)
{
    Detail::WriteSignedWithExcept<int32_t>(writer, value, TryWriteU32LE<WRITER>);
}

template <typename WRITER>
inline void Write32BE(WRITER &writer, const int32_t value)
{
    Detail::WriteSignedWithExcept<int32_t>(writer, value, TryWriteU32BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline float ReadFloatLE(READER &reader)
{
    uint32_t bits = ReadU32LE(reader);
    return Detail::BitCast<float>(bits);
}

template <typename READER>
inline float ReadFloatBE(READER &reader)
{
    uint32_t bits = ReadU32BE(reader);
    return Detail::BitCast<float>(bits);
}

template <typename WRITER>
inline void WriteFloatLE(WRITER &writer, const float value)
{
    WriteU32LE(writer, Detail::BitCast<uint32_t>(value));
}

template <typename WRITER>
inline void WriteFloatBE(WRITER &writer, const float value)
{
    WriteU32BE(writer, Detail::BitCast<uint32_t>(value));
}

//******************************************************************************

template <typename READER>
inline bool TryReadU64LE(uint64_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint64_t))
    {
        return false;
    }
    out = Detail::FromByte<uint64_t>(buf[0], 0) | Detail::FromByte<uint64_t>(buf[1], 8) |
          Detail::FromByte<uint64_t>(buf[2], 16) | Detail::FromByte<uint64_t>(buf[3], 24) |
          Detail::FromByte<uint64_t>(buf[4], 32) | Detail::FromByte<uint64_t>(buf[5], 40) |
          Detail::FromByte<uint64_t>(buf[6], 48) | Detail::FromByte<uint64_t>(buf[7], 56);
    return true;
}

template <typename READER>
inline bool TryReadU64BE(uint64_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(ByteSpanT(buf, sizeof(buf)), reader);
    if (count != sizeof(uint64_t))
    {
        return false;
    }
    out = Detail::FromByte<uint64_t>(buf[0], 56) | Detail::FromByte<uint64_t>(buf[1], 48) |
          Detail::FromByte<uint64_t>(buf[2], 40) | Detail::FromByte<uint64_t>(buf[3], 32) |
          Detail::FromByte<uint64_t>(buf[4], 24) | Detail::FromByte<uint64_t>(buf[5], 16) |
          Detail::FromByte<uint64_t>(buf[6], 8) | Detail::FromByte<uint64_t>(buf[7], 0);
    return true;
}

template <typename WRITER>
inline bool TryWriteU64LE(WRITER &writer, const uint64_t value)
{
    const uint8_t buf[sizeof(uint64_t)] = {
        Detail::ToByte(value, 0),  Detail::ToByte(value, 8),  Detail::ToByte(value, 16), Detail::ToByte(value, 24),
        Detail::ToByte(value, 32), Detail::ToByte(value, 40), Detail::ToByte(value, 48), Detail::ToByte(value, 56),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint64_t);
}

template <typename WRITER>
inline bool TryWriteU64BE(WRITER &writer, const uint64_t value)
{
    const uint8_t buf[sizeof(uint64_t)] = {
        Detail::ToByte(value, 56), Detail::ToByte(value, 48), Detail::ToByte(value, 40), Detail::ToByte(value, 32),
        Detail::ToByte(value, 24), Detail::ToByte(value, 16), Detail::ToByte(value, 8),  Detail::ToByte(value, 0),
    };
    const size_t count = Write(writer, ConstByteSpanT(buf, sizeof(buf)));
    return count == sizeof(uint64_t);
}

template <typename READER>
inline uint64_t ReadU64LE(READER &reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64LE<READER>);
}

template <typename READER>
inline uint64_t ReadU64BE(READER &reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64BE<READER>);
}

template <typename WRITER>
inline void WriteU64LE(WRITER &writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64LE<WRITER>);
}

template <typename WRITER>
inline void WriteU64BE(WRITER &writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryRead64LE(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64LE<READER>);
}

template <typename READER>
inline bool TryRead64BE(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64BE<READER>);
}

template <typename WRITER>
inline bool TryWrite64LE(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64LE<WRITER>);
}

template <typename WRITER>
inline bool TryWrite64BE(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64BE<WRITER>);
}

template <typename READER>
inline int64_t Read64LE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int64_t>(reader, ReadU64LE<READER>);
}

template <typename READER>
inline int64_t Read64BE(READER &reader)
{
    return Detail::ReadSignedWithExcept<int64_t>(reader, ReadU64BE<READER>);
}

template <typename WRITER>
inline void Write64LE(WRITER &writer, const int64_t value)
{
    Detail::WriteSignedWithExcept<int64_t>(writer, value, TryWriteU64LE<WRITER>);
}

template <typename WRITER>
inline void Write64BE(WRITER &writer, const int64_t value)
{
    Detail::WriteSignedWithExcept<int64_t>(writer, value, TryWriteU64BE<WRITER>);
}

//******************************************************************************

template <typename READER>
inline double ReadDoubleLE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64LE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER>
inline void WriteDoubleLE(WRITER &writer, const double value)
{
    uint64_t out;
    std::memcpy(&out, &value, sizeof(out));
    WriteU64LE(writer, out);
}

template <typename READER>
inline double ReadDoubleBE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64BE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER>
inline void WriteDoubleBE(WRITER &writer, const double value)
{
    uint64_t out;
    std::memcpy(&out, &value, sizeof(out));
    WriteU64BE(writer, out);
}

//******************************************************************************

/**
 * @brief Read a protobuf-style Varint.
 *
 * @detail This variable-length integer encoding uses the least significant
 *         7 bits of each byte for the numeric payload, and the msb is a
 *         continuation flag.
 *
 * @param reader Reader to operate on.
 * @return An unsigned 32-bit integer from the Reader.
 * @throws std::runtime_error if there are too many varint bytes for a 64-bit integer.
 */
template <typename READER>
inline uint32_t ReadUVarint32(READER &reader)
{
    constexpr int MAX_BYTES = 5;
    uint32_t rvo = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            throw std::runtime_error("too many bytes for 32-bit integer");
        }
        const uint8_t b = ReadU8(reader);
        rvo |= static_cast<uint32_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    return rvo;
}

/**
 * @brief Write a protobuf-style Varint.
 *
 * @detail This variable-length integer encoding uses the least significant
 *         7 bits of each byte for the numeric payload, and the msb is a
 *         continuation flag.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteUVarint32(WRITER &writer, const uint32_t value)
{
    uint32_t v = value;
    while (v >= 0x80)
    {
        WriteU8(writer, static_cast<uint8_t>(v | 0x80));
        v >>= 7;
    }
    WriteU8(writer, static_cast<uint8_t>(v));
}

/**
 * @brief Read a protobuf-style Varint.
 *
 * @detail This variable-length integer encoding uses the least significant
 *         7 bits of each byte for the numeric payload, and the msb is a
 *         continuation flag.
 *
 * @param reader Reader to operate on.
 * @return An unsigned 64-bit integer from the Reader.
 * @throws std::runtime_error if there are too many varint bytes for a 64-bit integer.
 */
template <typename READER>
inline uint64_t ReadUVarint64(READER &reader)
{
    constexpr int MAX_BYTES = 10;
    uint64_t rvo = 0;

    for (int count = 0;; count++)
    {
        if (count == MAX_BYTES)
        {
            throw std::runtime_error("too many bytes for 64-bit integer");
        }
        const uint8_t b = ReadU8(reader);
        rvo |= static_cast<uint64_t>(b & 0x7F) << (7 * count);

        if ((b & 0x80) == 0)
        {
            break;
        }
    }

    return rvo;
}

/**
 * @brief Write a protobuf-style Varint.
 *
 * @detail This variable-length integer encoding uses the least significant
 *         7 bits of each byte for the numeric payload, and the msb is a
 *         continuation flag.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteUVarint64(WRITER &writer, const uint64_t value)
{
    uint64_t v = value;
    while (v >= 0x80)
    {
        WriteU8(writer, static_cast<uint8_t>(v | 0x80));
        v >>= 7;
    }
    WriteU8(writer, static_cast<uint8_t>(v));
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
template <typename READER>
inline int32_t ReadVarint32(READER &reader)
{
    return static_cast<int32_t>(ReadUVarint32(reader));
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteVarint32(WRITER &writer, const int32_t value)
{
    WriteUVarint32(writer, static_cast<uint32_t>(value));
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
template <typename READER>
inline int64_t ReadVarint64(READER &reader)
{
    return static_cast<int64_t>(ReadUVarint64(reader));
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteVarint64(WRITER &writer, const int64_t value)
{
    WriteUVarint64(writer, static_cast<uint64_t>(value));
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
template <typename READER>
inline int32_t ReadSVarint32(READER &reader)
{
    const uint32_t var = ReadUVarint32(reader);
    return static_cast<int32_t>((var >> 1) ^ (~(var & 1) + 1));
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteSVarint32(WRITER &writer, const int64_t value)
{
    const uint32_t var = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 63);
    WriteUVarint32(writer, var);
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
template <typename READER>
inline int64_t ReadSVarint64(READER &reader)
{
    const uint64_t var = ReadUVarint64(reader);
    return static_cast<int64_t>((var >> 1) ^ (~(var & 1) + 1));
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
template <typename WRITER>
inline void WriteSVarint64(WRITER &writer, const int64_t value)
{
    const uint64_t var = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    WriteUVarint64(writer, var);
}

//******************************************************************************

/**
 * @brief Read a fixed-size byte buffer from the passed reader.
 *
 * @param outBegin Start of byte buffer to write to (begin iterator).
 * @param outEnd One past end of byte buffer to write to (end iterator).
 * @param reader Reader to operate on.
 */
template <typename READER, typename IT>
inline void ReadBytes(IT outBegin, IT outEnd, READER &reader)
{
    const size_t length = outEnd - outBegin;
    const size_t count = Read(ByteSpanT(&(*outBegin), length), reader);
    if (count != length)
    {
        throw std::runtime_error("could not read entire byte buffer");
    }
}

/**
 * @brief Write a fixed-size data buffer to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param begin Start of byte buffer to read from (begin iterator).
 * @param end One past end of byte buffer to read from (end iterator).
 */
template <typename WRITER, typename IT>
inline void WriteBytes(WRITER &writer, IT begin, IT end)
{
    const size_t length = end - begin;
    const size_t count = Write(writer, ConstByteSpanT(&(*begin), length));
    if (count != length)
    {
        throw std::runtime_error("could not write entire byte buffer");
    }
}

//******************************************************************************

/**
 * @brief Read a fixed-size data buffer from the passed reader.
 *
 * @param outData Pointer to output data buffer.
 * @param length Length of data in bytes.
 * @param reader Reader to operate on.
 */
template <typename READER>
inline void ReadData(void *outData, const size_t length, READER &reader)
{
    uint8_t *castData = Detail::BitCast<uint8_t *>(outData);

    const size_t count = Read(ByteSpanT(castData, length), reader);
    if (count != length)
    {
        throw std::runtime_error("could not read entire buffer");
    }
}

/**
 * @brief Write a fixed-size data buffer to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param data Pointer to data.
 * @param length Length of data in bytes.
 */
template <typename WRITER>
inline void WriteData(WRITER &writer, const void *data, const size_t length)
{
    const uint8_t *castData = Detail::BitCast<const uint8_t *>(data);

    const size_t count = Write(writer, ConstByteSpanT(castData, length));
    if (count != length)
    {
        throw std::runtime_error("could not write entire buffer");
    }
}

//******************************************************************************

/**
 * @brief Read a fixed-size string from the passed reader.
 *
 * @param outBegin Start of char buffer to write to (begin iterator).
 * @param outEnd One past end of char buffer to write to (end iterator).
 * @param reader Reader to operate on.
 * @throws std::runtime_error if the entire string could not be read.
 */
template <typename READER, typename IT>
inline void ReadString(IT outBegin, IT outEnd, READER &reader)
{
    uint8_t *castBegin = Detail::BitCast<uint8_t *>(&(*outBegin));
    const size_t length = outEnd - outBegin;

    const size_t count = Read(ByteSpanT(castBegin, length), reader);
    if (count != length)
    {
        throw std::runtime_error("could not read entire string");
    }
}

/**
 * @brief Write a fixed-size string to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param begin Start of char buffer to read from (begin iterator).
 * @param end One past end of char buffer to read from (end iterator).
 */
template <typename WRITER, typename IT>
inline void WriteString(WRITER &writer, IT begin, IT end)
{
    const uint8_t *castBegin = Detail::BitCast<const uint8_t *>(&(*begin));
    const size_t length = end - begin;

    const size_t count = Write(writer, ConstByteSpanT(castBegin, length));
    if (count != length)
    {
        throw std::runtime_error("could not write entire string");
    }
}

} // namespace LexIO

#endif
