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
 * @file tryint.hpp
 * @brief Integer serialization functions that return false on failure.
 */

#pragma once

#include "../core.hpp"

#include <cstring>

namespace LexIO
{

namespace Detail
{

//******************************************************************************

template <typename TYPE, typename TRY_READ>
inline constexpr bool ReadSigned(TYPE &out, const ReaderRef &reader, TRY_READ &tryRead)
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

template <typename TYPE, typename TRY_WRITE>
inline constexpr bool WriteSigned(const WriterRef &writer, const TYPE &value, TRY_WRITE &tryWrite)
{
    return tryWrite(writer, static_cast<TYPE>(value));
}

} // namespace Detail

//******************************************************************************

/**
 * @brief Try to read a uint8_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU8(uint8_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }
    out = buf[0];
    return true;
}

/**
 * @brief Try to write a uint8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU8(const WriterRef &writer, uint8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

//******************************************************************************

/**
 * @brief Try to read a int8_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead8(int8_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }
    out = int8_t(buf[0]);
    return true;
}

/**
 * @brief Try to write a int8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite8(const WriterRef &writer, int8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {uint8_t(value)};
    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU16LE(uint16_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_BE_BSWAP16(out);
    return true;
}

/**
 * @brief Try to read a big-endian uint16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU16BE(uint16_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP16(out);
    return true;
}

/**
 * @brief Try to write a little-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU16LE(const WriterRef &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    value = LEXIO_IF_BE_BSWAP16(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

/**
 * @brief Try to write a big-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU16BE(const WriterRef &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    value = LEXIO_IF_LE_BSWAP16(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead16LE(int16_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16LE);
}

/**
 * @brief Try to read a big-endian int16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead16BE(int16_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16BE);
}

/**
 * @brief Try to write a little-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite16LE(const WriterRef &writer, int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16LE);
}

/**
 * @brief Try to write a big-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite16BE(const WriterRef &writer, int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16BE);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU32LE(uint32_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_BE_BSWAP32(out);
    return true;
}

/**
 * @brief Try to read a big-endian uint32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU32BE(uint32_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP32(out);
    return true;
}

/**
 * @brief Try to write a little-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU32LE(const WriterRef &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    value = LEXIO_IF_BE_BSWAP32(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

/**
 * @brief Try to write a big-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU32BE(const WriterRef &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    value = LEXIO_IF_LE_BSWAP32(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead32LE(int32_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32LE);
}

/**
 * @brief Try to read a big-endian int32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead32BE(int32_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32BE);
}

/**
 * @brief Try to write a little-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite32LE(const WriterRef &writer, int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32LE);
}

/**
 * @brief Try to write a big-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite32BE(const WriterRef &writer, int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32BE);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU64LE(uint64_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_BE_BSWAP64(out);
    return true;
}

/**
 * @brief Try to read a big-endian uint64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadU64BE(uint64_t &out, const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP64(out);
    return true;
}

/**
 * @brief Try to write a little-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU64LE(const WriterRef &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    value = LEXIO_IF_BE_BSWAP64(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

/**
 * @brief Try to write a big-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteU64BE(const WriterRef &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    value = LEXIO_IF_LE_BSWAP64(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    return count == sizeof(buf);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead64LE(int64_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64LE);
}

/**
 * @brief Try to read a big-endian int64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryRead64BE(int64_t &out, const ReaderRef &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64BE);
}

/**
 * @brief Try to write a little-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite64LE(const WriterRef &writer, int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64LE);
}

/**
 * @brief Try to write a big-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWrite64BE(const WriterRef &writer, int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64BE);
}

} // namespace LexIO
