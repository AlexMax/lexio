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
 * @file int.h
 *
 * Integer serialization functions that throw exceptions on failure.
 */

#pragma once

#include "../core.hpp"

#include "./tryint.hpp"
#include <stdexcept>

namespace LexIO
{

namespace Detail
{

//******************************************************************************

template <typename TYPE, typename TRY_READ>
inline constexpr TYPE ReadWithExcept(ReaderRef reader, TRY_READ &tryRead)
{
    TYPE rvo;
    if (!tryRead(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

template <typename TYPE, typename TRY_WRITE>
inline constexpr void WriteWithExcept(WriterRef writer, const TYPE &value, TRY_WRITE &tryWrite)
{
    if (!tryWrite(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

} // namespace Detail

//******************************************************************************

/**
 * @brief Read a uint8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint8_t ReadU8(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint8_t>(reader, TryReadU8);
}

/**
 * @brief Write a uint8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU8(WriterRef writer, const uint8_t value)
{
    Detail::WriteWithExcept<uint8_t>(writer, value, TryWriteU8);
}

//******************************************************************************

/**
 * @brief Read a int8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int8_t Read8(ReaderRef reader)
{
    return Detail::ReadWithExcept<int8_t>(reader, TryRead8);
}

/**
 * @brief Write a int8_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write8(WriterRef writer, const int8_t value)
{
    Detail::WriteWithExcept<int8_t>(writer, value, TryWrite8);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint16_t ReadU16LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16LE);
}

/**
 * @brief Read a big-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint16_t ReadU16BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16BE);
}

/**
 * @brief Write a little-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU16LE(WriterRef writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16LE);
}

/**
 * @brief Write a big-endian uint16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU16BE(WriterRef writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int16_t Read16LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int16_t>(reader, TryRead16LE);
}

/**
 * @brief Read a big-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int16_t Read16BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int16_t>(reader, TryRead16BE);
}

/**
 * @brief Write a little-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write16LE(WriterRef writer, const int16_t value)
{
    Detail::WriteWithExcept<int16_t>(writer, value, TryWrite16LE);
}

/**
 * @brief Write a big-endian int16_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write16BE(WriterRef writer, const int16_t value)
{
    Detail::WriteWithExcept<int16_t>(writer, value, TryWrite16BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint32_t ReadU32LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32LE);
}

/**
 * @brief Read a big-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint32_t ReadU32BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32BE);
}

/**
 * @brief Write a little-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU32LE(WriterRef writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32LE);
}

/**
 * @brief Write a big-endian uint32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU32BE(WriterRef writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int32_t Read32LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryRead32LE);
}

/**
 * @brief Read a big-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int32_t Read32BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryRead32BE);
}

/**
 * @brief Write a little-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write32LE(WriterRef writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWrite32LE);
}

/**
 * @brief Write a big-endian int32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write32BE(WriterRef writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWrite32BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint64_t ReadU64LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64LE);
}

/**
 * @brief Read a big-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline uint64_t ReadU64BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64BE);
}

/**
 * @brief Write a little-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU64LE(WriterRef writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64LE);
}

/**
 * @brief Write a big-endian uint64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteU64BE(WriterRef writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int64_t Read64LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryRead64LE);
}

/**
 * @brief Read a big-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline int64_t Read64BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryRead64BE);
}

/**
 * @brief Write a little-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write64LE(WriterRef writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWrite64LE);
}

/**
 * @brief Write a big-endian int64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Integer to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void Write64BE(WriterRef writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWrite64BE);
}

} // namespace LexIO
