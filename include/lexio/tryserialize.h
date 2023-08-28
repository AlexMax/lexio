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
// "Try" Serialization functions.
//

#ifndef LEXIO_TRYSERIALIZE_H
#define LEXIO_TRYSERIALIZE_H

#include "./core.h"

#include <cstring>

namespace LexIO
{

namespace Detail
{

//******************************************************************************

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

//******************************************************************************

template <typename READER>
inline bool TryReadUVarint32(uint32_t &out, READER &reader)
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
        else if (!TryReadU8<READER>(b, reader))
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

template <typename WRITER>
inline bool TryWriteUVarint32(WRITER &writer, const uint32_t value)
{
    uint32_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v));
}

//******************************************************************************

template <typename READER>
inline bool TryReadVarint32(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadUVarint32<READER>);
}

template <typename WRITER>
inline bool TryWriteVarint32(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteUVarint32<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryReadSVarint32(int32_t &out, READER &reader)
{
    uint32_t outVal;
    if (!TryReadUVarint32<READER>(outVal, reader))
    {
        return false;
    }
    out = static_cast<int32_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

template <typename WRITER>
inline bool TryWriteSVarint32(WRITER &writer, const int32_t value)
{
    const uint32_t var = (static_cast<uint32_t>(value) << 1) ^ static_cast<uint32_t>(value >> 31);
    return TryWriteUVarint32<WRITER>(writer, var);
}

//******************************************************************************

template <typename READER>
inline bool TryReadUVarint64(uint64_t &out, READER &reader)
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
        if (!TryReadU8<READER>(b, reader))
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

template <typename WRITER>
inline bool TryWriteUVarint64(WRITER &writer, const uint64_t value)
{
    uint64_t v = value;
    while (v >= 0x80)
    {
        if (!TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v | 0x80)))
        {
            return false;
        }
        v >>= 7;
    }
    return TryWriteU8<WRITER>(writer, static_cast<uint8_t>(v));
}

//******************************************************************************

template <typename READER>
inline bool TryReadVarint64(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadUVarint64<READER>);
}

template <typename WRITER>
inline bool TryWriteVarint64(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteUVarint64<WRITER>);
}

//******************************************************************************

template <typename READER>
inline bool TryReadSVarint64(int64_t &out, READER &reader)
{
    uint64_t outVal;
    if (!TryReadUVarint64<READER>(outVal, reader))
    {
        return false;
    }
    out = static_cast<int64_t>((outVal >> 1) ^ (~(outVal & 1) + 1));
    return true;
}

template <typename WRITER>
inline bool TryWriteSVarint64(WRITER &writer, const int64_t value)
{
    const uint64_t var = (static_cast<uint64_t>(value) << 1) ^ static_cast<uint64_t>(value >> 63);
    return TryWriteUVarint64<WRITER>(writer, var);
}

} // namespace LexIO

#endif
