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
#include <stdexcept>

namespace LexIO
{

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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        const uint8_t buf[sizeof(uint8_t)] = {value};
        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        const uint8_t buf[sizeof(uint8_t)] = {uint8_t(value)};
        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint16_t)] = {0};
        value = LEXIO_IF_BE_BSWAP16(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint16_t)] = {0};
        value = LEXIO_IF_LE_BSWAP16(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint16_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint16_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_BE_BSWAP16(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint16_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint16_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_LE_BSWAP16(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint16_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_BE_BSWAP16(uvalue);

        uint8_t buf[sizeof(uint16_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint16_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_LE_BSWAP16(uvalue);

        uint8_t buf[sizeof(uint16_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint32_t)] = {0};
        value = LEXIO_IF_BE_BSWAP32(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint32_t)] = {0};
        value = LEXIO_IF_LE_BSWAP32(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint32_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint32_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_BE_BSWAP32(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint32_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint32_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_LE_BSWAP32(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint32_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_BE_BSWAP32(uvalue);

        uint8_t buf[sizeof(uint32_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint32_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_LE_BSWAP32(uvalue);

        uint8_t buf[sizeof(uint32_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
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
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint64_t)] = {0};
        value = LEXIO_IF_BE_BSWAP64(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint64_t)] = {0};
        value = LEXIO_IF_LE_BSWAP64(value);
        std::memcpy(buf, &value, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint64_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint64_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_BE_BSWAP64(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint8_t buf[sizeof(uint64_t)] = {0};
        const size_t count = Read(buf, reader);
        if (count != sizeof(buf))
        {
            return false;
        }

        uint64_t bits = 0;
        std::memcpy(&bits, buf, sizeof(bits));
        bits = LEXIO_IF_LE_BSWAP64(bits);
        std::memcpy(&out, &bits, sizeof(out));
        return true;
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint64_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_BE_BSWAP64(uvalue);

        uint8_t buf[sizeof(uint64_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
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
    try
    {
        uint64_t uvalue = 0;
        std::memcpy(&uvalue, &value, sizeof(uvalue));
        uvalue = LEXIO_IF_LE_BSWAP64(uvalue);

        uint8_t buf[sizeof(uint64_t)] = {0};
        std::memcpy(buf, &uvalue, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
}

} // namespace LexIO
