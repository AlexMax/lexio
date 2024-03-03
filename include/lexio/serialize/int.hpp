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
 * @file int.hpp
 * @brief Integer serialization functions that throw exceptions on failure.
 */

#pragma once

#include "../core.hpp"

#include "./tryint.hpp"

namespace LexIO
{

//******************************************************************************

/**
 * @brief Read a uint8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint8_t ReadU8(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    ReadExact(buf, reader);
    return buf[0];
}

/**
 * @brief Write a uint8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU8(const WriterRef &writer, uint8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a int8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int8_t Read8(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }
    return int8_t(buf[0]);
}

/**
 * @brief Write a int8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write8(const WriterRef &writer, int8_t value)
{
    const uint8_t buf[sizeof(uint8_t)] = {uint8_t(value)};
    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint16_t ReadU16LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint16_t out;
    std::memcpy(&out, buf, sizeof(uint16_t));
    out = LEXIO_IF_BE_BSWAP16(out);
    return out;
}

/**
 * @brief Read a big-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint16_t ReadU16BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint16_t out;
    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP16(out);
    return out;
}

/**
 * @brief Write a little-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU16LE(const WriterRef &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    value = LEXIO_IF_BE_BSWAP16(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU16BE(const WriterRef &writer, uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    value = LEXIO_IF_LE_BSWAP16(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int16_t Read16LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint16_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_BE_BSWAP16(bits);

    int16_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Read a big-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int16_t Read16BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint16_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_LE_BSWAP16(bits);

    int16_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Write a little-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write16LE(const WriterRef &writer, int16_t value)
{
    uint16_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_BE_BSWAP16(uvalue);

    uint8_t buf[sizeof(uint16_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write16BE(const WriterRef &writer, int16_t value)
{
    uint16_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_LE_BSWAP16(uvalue);

    uint8_t buf[sizeof(uint16_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint32_t ReadU32LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint32_t out;
    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_BE_BSWAP32(out);
    return out;
}

/**
 * @brief Read a big-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint32_t ReadU32BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint32_t out;
    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP32(out);
    return out;
}

/**
 * @brief Write a little-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU32LE(const WriterRef &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    value = LEXIO_IF_BE_BSWAP32(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU32BE(const WriterRef &writer, uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    value = LEXIO_IF_LE_BSWAP32(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int32_t Read32LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint32_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_BE_BSWAP32(bits);

    int32_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Read a big-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int32_t Read32BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint32_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_LE_BSWAP32(bits);

    int32_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Write a little-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write32LE(const WriterRef &writer, int32_t value)
{
    uint32_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_BE_BSWAP32(uvalue);

    uint8_t buf[sizeof(uint32_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write32BE(const WriterRef &writer, int32_t value)
{
    uint32_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_LE_BSWAP32(uvalue);

    uint8_t buf[sizeof(uint32_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint64_t ReadU64LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint64_t out;
    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_BE_BSWAP64(out);
    return out;
}

/**
 * @brief Read a big-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint64_t ReadU64BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint64_t out;
    std::memcpy(&out, buf, sizeof(out));
    out = LEXIO_IF_LE_BSWAP64(out);
    return out;
}

/**
 * @brief Write a little-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU64LE(const WriterRef &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    value = LEXIO_IF_BE_BSWAP64(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU64BE(const WriterRef &writer, uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    value = LEXIO_IF_LE_BSWAP64(value);
    std::memcpy(buf, &value, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int64_t Read64LE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint64_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_BE_BSWAP64(bits);

    int64_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Read a big-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int64_t Read64BE(const ReaderRef &reader)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = Read(buf, reader);
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not read");
    }

    uint64_t bits = 0;
    std::memcpy(&bits, buf, sizeof(bits));
    bits = LEXIO_IF_LE_BSWAP64(bits);

    int64_t out = 0;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

/**
 * @brief Write a little-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write64LE(const WriterRef &writer, int64_t value)
{
    uint64_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_BE_BSWAP64(uvalue);

    uint8_t buf[sizeof(uint64_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write64BE(const WriterRef &writer, int64_t value)
{
    uint64_t uvalue;
    std::memcpy(&uvalue, &value, sizeof(uvalue));
    uvalue = LEXIO_IF_LE_BSWAP64(uvalue);

    uint8_t buf[sizeof(uint64_t)] = {0};
    std::memcpy(buf, &uvalue, sizeof(buf));

    const size_t count = Write(writer, buf, sizeof(buf));
    if (count != sizeof(buf))
    {
        throw std::runtime_error("could not write");
    }
}

} // namespace LexIO
