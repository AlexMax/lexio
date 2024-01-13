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

#include "./int.hpp"
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
inline uint32_t ReadUVarint32(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint32_t>(reader, TryReadUVarint32);
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
inline void WriteUVarint32(WriterRef writer, uint32_t value)
{
    Detail::WriteWithExcept<uint32_t>(writer, value, TryWriteUVarint32);
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
inline int32_t ReadVarint32(ReaderRef reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryReadVarint32);
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
inline void WriteVarint32(WriterRef writer, int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWriteVarint32);
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
inline int32_t ReadSVarint32(ReaderRef reader)
{
    return Detail::ReadWithExcept<int32_t>(reader, TryReadSVarint32);
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
inline void WriteSVarint32(WriterRef writer, int32_t value)
{
    Detail::WriteWithExcept<int32_t>(writer, value, TryWriteSVarint32);
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
inline uint64_t ReadUVarint64(ReaderRef reader)
{
    return Detail::ReadWithExcept<uint64_t>(reader, TryReadUVarint64);
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
inline void WriteUVarint64(WriterRef writer, const uint64_t value)
{
    Detail::WriteWithExcept<uint64_t>(writer, value, TryWriteUVarint64);
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
inline int64_t ReadVarint64(ReaderRef reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryReadVarint64);
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
inline void WriteVarint64(WriterRef writer, int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWriteVarint64);
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
inline int64_t ReadSVarint64(ReaderRef reader)
{
    return Detail::ReadWithExcept<int64_t>(reader, TryReadSVarint64);
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
inline void WriteSVarint64(WriterRef writer, int64_t value)
{
    Detail::WriteWithExcept<int64_t>(writer, value, TryWriteSVarint64);
}

} // namespace LexIO
