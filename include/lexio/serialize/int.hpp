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

namespace LexIO
{

namespace Detail
{

//******************************************************************************

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

} // namespace Detail

//******************************************************************************

/**
 * @brief Read a uint8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint8_t ReadU8(READER &reader)
{
    return Detail::ReadWithExcept<uint8_t>(reader, TryReadU8<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU8(WRITER &writer, const uint8_t value)
{
    Detail::WriteWithExcept<uint8_t>(writer, value, TryWriteU8<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a int8_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int8_t Read8(READER &reader)
{
    return Detail::ReadWithExcept<int8_t>(reader, TryRead8<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write8(WRITER &writer, const int8_t value)
{
    Detail::WriteWithExcept<int8_t>(writer, value, TryWrite8<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint16_t ReadU16LE(READER &reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16LE<READER>);
}

/**
 * @brief Read a big-endian uint16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint16_t ReadU16BE(READER &reader)
{
    return Detail::ReadWithExcept<uint16_t>(reader, TryReadU16BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU16LE(WRITER &writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU16BE(WRITER &writer, const uint16_t value)
{
    Detail::WriteWithExcept<uint16_t>(writer, value, TryWriteU16BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int16_t Read16LE(READER &reader)
{
    return Detail::ReadWithExcept<int16_t>(reader, TryRead16LE<READER>);
}

/**
 * @brief Read a big-endian int16_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int16_t Read16BE(READER &reader)
{
    return Detail::ReadWithExcept<int16_t>(reader, TryRead16BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write16LE(WRITER &writer, const int16_t value)
{
    Detail::WriteWithExcept<int16_t>(writer, value, TryWrite16LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write16BE(WRITER &writer, const int16_t value)
{
    Detail::WriteWithExcept<int16_t>(writer, value, TryWrite16BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint32_t ReadU32LE(READER &reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32LE<READER>);
}

/**
 * @brief Read a big-endian uint32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint32_t ReadU32BE(READER &reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadU32BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU32LE(WRITER &writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU32BE(WRITER &writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteU32BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int32_t Read32LE(READER &reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryRead32LE<READER>);
}

/**
 * @brief Read a big-endian int32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int32_t Read32BE(READER &reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryRead32BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write32LE(WRITER &writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWrite32LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write32BE(WRITER &writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWrite32BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint64_t ReadU64LE(READER &reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64LE<READER>);
}

/**
 * @brief Read a big-endian uint64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint64_t ReadU64BE(READER &reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadU64BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU64LE(WRITER &writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteU64BE(WRITER &writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteU64BE<WRITER>);
}

//******************************************************************************

/**
 * @brief Read a little-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int64_t Read64LE(READER &reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryRead64LE<READER>);
}

/**
 * @brief Read a big-endian int64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Integer that was read.
 * @throws std::runtime_error if stream could not be read.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int64_t Read64BE(READER &reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryRead64BE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write64LE(WRITER &writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWrite64LE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Write64BE(WRITER &writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWrite64BE<WRITER>);
}

} // namespace LexIO
