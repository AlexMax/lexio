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

#pragma once

#include "./core.h"

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

} // namespace Detail

//******************************************************************************

template <typename READER>
inline uint8_t ReadU8(READER &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not read 1 byte");
    }
    return buf[0];
}

template <typename WRITER>
inline void WriteU8(WRITER &writer, const uint8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = Write(writer, buf);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not write 1 byte");
    }
}

//******************************************************************************

template <typename READER>
inline int8_t Read8(READER &reader)
{
    return static_cast<int8_t>(ReadU8(reader));
}

template <typename WRITER>
inline void Write8(WRITER &writer, const int8_t value)
{
    WriteU8(writer, static_cast<uint8_t>(value));
}

//******************************************************************************

template <typename READER>
inline uint16_t ReadU16LE(READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not read 2 bytes");
    }
    return Detail::FromByte<uint16_t>(buf[0], 0) | Detail::FromByte<uint16_t>(buf[1], 8);
}

template <typename WRITER>
inline void WriteU16LE(WRITER &writer, const uint16_t value)
{
    const uint8_t buf[sizeof(uint16_t)] = {
        Detail::ToByte(value, 0),
        Detail::ToByte(value, 8),
    };
    const size_t count = Write(writer, buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not write 2 bytes");
    }
}

template <typename READER>
inline uint16_t ReadU16BE(READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not read 2 bytes");
    }
    return Detail::FromByte<uint16_t>(buf[0], 8) | Detail::FromByte<uint16_t>(buf[1], 0);
}

template <typename WRITER>
inline void WriteU16BE(WRITER &writer, const uint16_t value)
{
    const uint8_t buf[sizeof(uint16_t)] = {
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 0),
    };
    const size_t count = Write(writer, buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not write 2 bytes");
    }
}

//******************************************************************************

template <typename READER>
inline int16_t Read16LE(READER &reader)
{
    return static_cast<int16_t>(ReadU16LE(reader));
}

template <typename WRITER>
inline void Write16LE(WRITER &writer, const int16_t value)
{
    WriteU16LE(writer, static_cast<uint16_t>(value));
}

template <typename READER>
inline int16_t Read16BE(READER &reader)
{
    return static_cast<int16_t>(ReadU16BE(reader));
}

template <typename WRITER>
inline void Write16BE(WRITER &writer, const int16_t value)
{
    WriteU16BE(writer, static_cast<uint16_t>(value));
}

//******************************************************************************

template <typename READER>
inline uint32_t ReadU32LE(READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not read 4 bytes");
    }
    return Detail::FromByte<uint32_t>(buf[0], 0) | Detail::FromByte<uint32_t>(buf[1], 8) |
           Detail::FromByte<uint32_t>(buf[2], 16) | Detail::FromByte<uint32_t>(buf[3], 24);
}

template <typename WRITER>
inline void WriteU32LE(WRITER &writer, const uint32_t value)
{
    const uint8_t buf[sizeof(uint32_t)] = {
        Detail::ToByte(value, 0),
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 16),
        Detail::ToByte(value, 24),
    };
    const size_t count = Write(writer, buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not write 4 bytes");
    }
}

template <typename READER>
inline uint32_t ReadU32BE(READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not read 4 bytes");
    }
    return Detail::FromByte<uint32_t>(buf[0], 24) | Detail::FromByte<uint32_t>(buf[1], 16) |
           Detail::FromByte<uint32_t>(buf[2], 8) | Detail::FromByte<uint32_t>(buf[3], 0);
}

template <typename WRITER>
inline void WriteU32BE(WRITER &writer, const uint32_t value)
{
    const uint8_t buf[sizeof(uint32_t)] = {
        Detail::ToByte(value, 24),
        Detail::ToByte(value, 16),
        Detail::ToByte(value, 8),
        Detail::ToByte(value, 0),
    };
    const size_t count = Write(writer, buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not write 4 bytes");
    }
}

//******************************************************************************

template <typename READER>
inline int32_t Read32LE(READER &reader)
{
    return static_cast<int32_t>(ReadU32LE(reader));
}

template <typename WRITER>
inline void Write32LE(WRITER &writer, const int32_t value)
{
    WriteU32LE(writer, static_cast<uint32_t>(value));
}

template <typename READER>
inline int32_t Read32BE(READER &reader)
{
    return static_cast<int32_t>(ReadU32BE(reader));
}

template <typename WRITER>
inline void Write32BE(WRITER &writer, const int32_t value)
{
    WriteU32BE(writer, static_cast<uint32_t>(value));
}

//******************************************************************************

template <typename READER>
inline uint64_t ReadU64LE(READER &buffer)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, buffer);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not read 8 bytes");
    }
    return Detail::FromByte<uint64_t>(buf[0], 0) | Detail::FromByte<uint64_t>(buf[1], 8) |
           Detail::FromByte<uint64_t>(buf[2], 16) | Detail::FromByte<uint64_t>(buf[3], 24) |
           Detail::FromByte<uint64_t>(buf[4], 32) | Detail::FromByte<uint64_t>(buf[5], 40) |
           Detail::FromByte<uint64_t>(buf[6], 48) | Detail::FromByte<uint64_t>(buf[7], 56);
}

template <typename WRITER>
inline void WriteU64LE(WRITER &buffer, const uint64_t value)
{
    const uint8_t buf[sizeof(uint64_t)] = {
        Detail::ToByte(value, 0),  Detail::ToByte(value, 8),  Detail::ToByte(value, 16), Detail::ToByte(value, 24),
        Detail::ToByte(value, 32), Detail::ToByte(value, 40), Detail::ToByte(value, 48), Detail::ToByte(value, 56),
    };
    const size_t count = Write(buffer, buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not write 8 bytes");
    }
}

template <typename READER>
inline uint64_t ReadU64BE(READER &buffer)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, buffer);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not read 8 bytes");
    }
    return Detail::FromByte<uint64_t>(buf[0], 56) | Detail::FromByte<uint64_t>(buf[1], 48) |
           Detail::FromByte<uint64_t>(buf[2], 40) | Detail::FromByte<uint64_t>(buf[3], 32) |
           Detail::FromByte<uint64_t>(buf[4], 24) | Detail::FromByte<uint64_t>(buf[5], 16) |
           Detail::FromByte<uint64_t>(buf[6], 8) | Detail::FromByte<uint64_t>(buf[7], 0);
}

template <typename WRITER>
inline void WriteU64BE(WRITER &buffer, const uint64_t value)
{
    const uint8_t buf[sizeof(uint64_t)] = {
        Detail::ToByte(value, 56), Detail::ToByte(value, 48), Detail::ToByte(value, 40), Detail::ToByte(value, 32),
        Detail::ToByte(value, 24), Detail::ToByte(value, 16), Detail::ToByte(value, 8),  Detail::ToByte(value, 0),
    };
    const size_t count = Write(buffer, buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not write 8 bytes");
    }
}

//******************************************************************************

template <typename READER>
inline int64_t Read64LE(READER &buffer)
{
    return static_cast<int64_t>(ReadU64LE(buffer));
}

template <typename WRITER>
inline void Write64LE(WRITER &buffer, const int64_t value)
{
    WriteU64LE(buffer, static_cast<uint64_t>(value));
}

template <typename READER>
inline int64_t Read64BE(READER &buffer)
{
    return static_cast<int64_t>(ReadU64BE(buffer));
}

template <typename WRITER>
inline void Write64BE(WRITER &buffer, const int64_t value)
{
    WriteU64BE(buffer, static_cast<uint64_t>(value));
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
 * @return 
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

//******************************************************************************

} // namespace LexIO
