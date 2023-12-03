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
 * @file serialize.h
 *
 * Serialization functions that throw exceptions on failure.
 */

#ifndef LEXIO_SERIALIZE_H
#define LEXIO_SERIALIZE_H

#include "./core.h"

#include "./tryserialize.h"

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

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline float ReadFloatLE(READER &reader)
{
    return Detail::ReadWithExcept<float>(reader, TryReadFloatLE<READER>);
}

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline float ReadFloatBE(READER &reader)
{
    return Detail::ReadWithExcept<float>(reader, TryReadFloatBE<READER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteFloatLE(WRITER &writer, const float value)
{
    Detail::WriteWithExcept<float>(writer, value, TryWriteFloatLE<WRITER>);
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteFloatBE(WRITER &writer, const float value)
{
    Detail::WriteWithExcept<float>(writer, value, TryWriteFloatBE<WRITER>);
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

//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline double ReadDoubleLE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64LE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteDoubleLE(WRITER &writer, const double value)
{
    uint64_t out;
    std::memcpy(&out, &value, sizeof(out));
    WriteU64LE(writer, out);
}

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline double ReadDoubleBE(READER &reader)
{
    double rvo;
    uint64_t bits = ReadU64BE(reader);
    std::memcpy(&rvo, &bits, sizeof(rvo));
    return rvo;
}

template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
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
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint32_t ReadUVarint32(READER &reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadUVarint32<READER>);
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
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteUVarint32(WRITER &writer, const uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteUVarint32<WRITER>);
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int32_t ReadVarint32(READER &reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryReadVarint32<READER>);
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteVarint32(WRITER &writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWriteVarint32<WRITER>);
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint32_t ReadSVarint32(READER &reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryReadSVarint32<READER>);
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteSVarint32(WRITER &writer, const int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWriteSVarint32<WRITER>);
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
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline uint64_t ReadUVarint64(READER &reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadUVarint64<READER>);
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
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteUVarint64(WRITER &writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteUVarint64<WRITER>);
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int64_t ReadVarint64(READER &reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryReadVarint64<READER>);
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteVarint64(WRITER &writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWriteVarint64<WRITER>);
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline int64_t ReadSVarint64(READER &reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryReadSVarint64<READER>);
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteSVarint64(WRITER &writer, const int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWriteSVarint64<WRITER>);
}

/**
 * @brief Read a fixed-size byte buffer from the passed reader.
 *
 * @param outBegin Start of byte buffer to write to (begin iterator).
 * @param outEnd One past end of byte buffer to write to (end iterator).
 * @param reader Reader to operate on.
 */
template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline void ReadBytes(IT outBegin, IT outEnd, READER &reader)
{
    if (!TryReadBytes<READER, IT>(outBegin, outEnd, reader))
    {
        throw std::runtime_error("could not read");
    }
}

/**
 * @brief Write a fixed-size data buffer to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param begin Start of byte buffer to read from (begin iterator).
 * @param end One past end of byte buffer to read from (end iterator).
 */
template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteBytes(WRITER &writer, IT begin, IT end)
{
    if (!TryWriteBytes<WRITER, IT>(writer, begin, end))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a fixed-size data buffer from the passed reader.
 *
 * @param outData Pointer to output data buffer.
 * @param length Length of data in bytes.
 * @param reader Reader to operate on.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline void ReadData(void *outData, const size_t length, READER &reader)
{
    if (!TryReadData<READER>(outData, length, reader))
    {
        throw std::runtime_error("could not read");
    }
}

/**
 * @brief Write a fixed-size data buffer to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param data Pointer to data.
 * @param length Length of data in bytes.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteData(WRITER &writer, const void *data, const size_t length)
{
    if (!TryWriteData<WRITER>(writer, data, length))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a fixed-size string from the passed reader.
 *
 * @param outBegin Start of char buffer to write to (begin iterator).
 * @param outEnd One past end of char buffer to write to (end iterator).
 * @param reader Reader to operate on.
 * @throws std::runtime_error if the entire string could not be read.
 */
template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline void ReadString(IT outBegin, IT outEnd, READER &reader)
{
    if (!TryReadString<READER, IT>(outBegin, outEnd, reader))
    {
        throw std::runtime_error("could not read");
    }
}

/**
 * @brief Write a fixed-size string to the passed writer.
 *
 * @param writer Writer to operate on.
 * @param begin Start of char buffer to read from (begin iterator).
 * @param end One past end of char buffer to read from (end iterator).
 */
template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteString(WRITER &writer, IT begin, IT end)
{
    if (!TryWriteString<WRITER, IT>(writer, begin, end))
    {
        throw std::runtime_error("could not write");
    }
}

} // namespace LexIO

#endif
