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
 * @file tryfloat.hpp
 * @brief Floating-point serialization functions that return false on failure.
 */

#pragma once

#include "./tryint.hpp"

#include <stdexcept>

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
inline bool TryReadFloat32LE(float32_t &out, const ReaderRef &reader)
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
 * @brief Try to read a big-endian float32_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat32BE(float32_t &out, const ReaderRef &reader)
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
 * @brief Try to write a little-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat32LE(const WriterRef &writer, float32_t value)
{
    try
    {
        uint32_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));

        uint8_t buf[sizeof(bits)] = {0};
        bits = LEXIO_IF_BE_BSWAP32(bits);
        std::memcpy(buf, &bits, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
}

/**
 * @brief Try to write a big-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat32BE(const WriterRef &writer, float32_t value)
{
    try
    {
        uint32_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));

        uint8_t buf[sizeof(bits)] = {0};
        bits = LEXIO_IF_LE_BSWAP32(bits);
        std::memcpy(buf, &bits, sizeof(buf));

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
 * @brief Try to read a little-endian float64_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat64LE(float64_t &out, const ReaderRef &reader)
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
 * @brief Try to read a big-endian float64_t from a stream.
 *
 * @param out Float that was read.  Not modified if read failed.
 * @param reader Reader to read from.
 * @return True if the read was successful.
 */
inline bool TryReadFloat64BE(float64_t &out, const ReaderRef &reader)
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
 * @brief Try to write a little-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat64LE(const WriterRef &writer, float64_t value)
{
    try
    {
        uint64_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));

        uint8_t buf[sizeof(bits)] = {0};
        bits = LEXIO_IF_BE_BSWAP64(bits);
        std::memcpy(buf, &bits, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
}

/**
 * @brief Try to write a big-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @return True if the write was successful.
 */
inline bool TryWriteFloat64BE(const WriterRef &writer, float64_t value)
{
    try
    {
        uint64_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));

        uint8_t buf[sizeof(bits)] = {0};
        bits = LEXIO_IF_LE_BSWAP64(bits);
        std::memcpy(buf, &bits, sizeof(buf));

        const size_t count = Write(writer, buf, sizeof(buf));
        return count == sizeof(buf);
    }
    catch (std::runtime_error &)
    {
        return false;
    }
}

} // namespace LexIO
