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

#pragma once

#include "./tryvarint.hpp"

namespace LexIO
{

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
inline int32_t ReadSVarint32(READER &reader)
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

} // namespace LexIO