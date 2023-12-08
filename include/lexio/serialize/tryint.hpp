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
 * @file tryint.h
 *
 * Integer serialization functions that return false on failure.
 */

#pragma once

#include "../core.hpp"

#include <cstring>

namespace LexIO
{

namespace Detail
{

#if defined(_MSC_VER) && !defined(__clang__)
#define LEXIO_BSWAP16_(v) _byteswap_ushort(v)
#define LEXIO_BSWAP32_(v) _byteswap_ulong(v)
#define LEXIO_BSWAP64_(v) _byteswap_uint64(v)
#else
#define LEXIO_BSWAP16_(v) __builtin_bswap16(v)
#define LEXIO_BSWAP32_(v) __builtin_bswap32(v)
#define LEXIO_BSWAP64_(v) __builtin_bswap64(v)
#endif

enum class endian
{
#if defined(_MSC_VER) && !defined(__clang__)
    little = 0,
    big = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

//******************************************************************************

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

} // namespace Detail

//******************************************************************************

/**
 * @brief Try to read a uint8_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU8(uint8_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint8_t))
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
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU8(WRITER &writer, const uint8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint8_t);
}

//******************************************************************************

/**
 * @brief Try to read a int8_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead8(int8_t &out, READER &reader)
{
    return Detail::ReadSigned<int8_t>(out, reader, TryReadU8<READER>);
}

/**
 * @brief Try to write a int8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite8(WRITER &writer, const int8_t value)
{
    return Detail::WriteSigned<int8_t>(writer, value, TryWriteU8<WRITER>);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU16LE(uint16_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint16_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint16_t));
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        out = LEXIO_BSWAP16_(out);
    }
    return true;
}

/**
 * @brief Try to read a big-endian uint16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU16BE(uint16_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint16_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint16_t));
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        out = LEXIO_BSWAP16_(out);
    }
    return true;
}

/**
 * @brief Try to write a little-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU16LE(WRITER &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        value = LEXIO_BSWAP16_(value);
    }
    std::memcpy(buf, &value, sizeof(uint16_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint16_t);
}

/**
 * @brief Try to write a big-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU16BE(WRITER &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        value = LEXIO_BSWAP16_(value);
    }
    std::memcpy(buf, &value, sizeof(uint16_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint16_t);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead16LE(int16_t &out, READER &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16LE<READER>);
}

/**
 * @brief Try to read a big-endian int16_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead16BE(int16_t &out, READER &reader)
{
    return Detail::ReadSigned<int16_t>(out, reader, TryReadU16BE<READER>);
}

/**
 * @brief Try to write a little-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite16LE(WRITER &writer, const int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16LE<WRITER>);
}

/**
 * @brief Try to write a big-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite16BE(WRITER &writer, const int16_t value)
{
    return Detail::WriteSigned<int16_t>(writer, value, TryWriteU16BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU32LE(uint32_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint32_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint32_t));
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        out = LEXIO_BSWAP32_(out);
    }
    return true;
}

/**
 * @brief Try to read a big-endian uint32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU32BE(uint32_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint32_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint32_t));
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        out = LEXIO_BSWAP32_(out);
    }
    return true;
}

/**
 * @brief Try to write a little-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU32LE(WRITER &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        value = LEXIO_BSWAP32_(value);
    }
    std::memcpy(buf, &value, sizeof(uint32_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint32_t);
}

/**
 * @brief Try to write a big-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU32BE(WRITER &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        value = LEXIO_BSWAP32_(value);
    }
    std::memcpy(buf, &value, sizeof(uint32_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint32_t);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead32LE(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32LE<READER>);
}

/**
 * @brief Try to read a big-endian int32_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead32BE(int32_t &out, READER &reader)
{
    return Detail::ReadSigned<int32_t>(out, reader, TryReadU32BE<READER>);
}

/**
 * @brief Try to write a little-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite32LE(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32LE<WRITER>);
}

/**
 * @brief Try to write a big-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite32BE(WRITER &writer, const int32_t value)
{
    return Detail::WriteSigned<int32_t>(writer, value, TryWriteU32BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian uint64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU64LE(uint64_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint64_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint64_t));
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        out = LEXIO_BSWAP64_(out);
    }
    return true;
}

/**
 * @brief Try to read a big-endian uint64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryReadU64BE(uint64_t &out, READER &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read<READER>(buf, reader);
    if (count != sizeof(uint64_t))
    {
        return false;
    }

    std::memcpy(&out, buf, sizeof(uint64_t));
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        out = LEXIO_BSWAP64_(out);
    }
    return true;
}

/**
 * @brief Try to write a little-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU64LE(WRITER &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::big)
    {
        value = LEXIO_BSWAP64_(value);
    }
    std::memcpy(buf, &value, sizeof(uint64_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint64_t);
}

/**
 * @brief Try to write a big-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWriteU64BE(WRITER &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    if constexpr (Detail::endian::native == Detail::endian::little)
    {
        value = LEXIO_BSWAP64_(value);
    }
    std::memcpy(buf, &value, sizeof(uint64_t));

    const size_t count = Write<WRITER>(writer, buf, sizeof(buf));
    return count == sizeof(uint64_t);
}

//******************************************************************************

/**
 * @brief Try to read a little-endian int64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead64LE(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64LE<READER>);
}

/**
 * @brief Try to read a big-endian int64_t from a stream.
 *
 * @param out Integer that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline bool TryRead64BE(int64_t &out, READER &reader)
{
    return Detail::ReadSigned<int64_t>(out, reader, TryReadU64BE<READER>);
}

/**
 * @brief Try to write a little-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite64LE(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64LE<WRITER>);
}

/**
 * @brief Try to write a big-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline bool TryWrite64BE(WRITER &writer, const int64_t value)
{
    return Detail::WriteSigned<int64_t>(writer, value, TryWriteU64BE<WRITER>);
}

} // namespace LexIO

#undef LEXIO_BSWAP16_
#undef LEXIO_BSWAP32_
#undef LEXIO_BSWAP64_
