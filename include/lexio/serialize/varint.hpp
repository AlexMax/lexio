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
 * @file varint.hpp
 * @brief Protobuf-style varint serialization functions that throw exceptions
 *        on failure.
 */

#pragma once

#include "./tryvarint.hpp"
#include <stdexcept>

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
inline uint32_t ReadUVarint32(const ReaderRef &reader)
{
    uint32_t rvo;
    if (!TryReadUVarint32(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
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
inline void WriteUVarint32(const WriterRef &writer, uint32_t value)
{
    if (!TryWriteUVarint32(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
inline int32_t ReadVarint32(const ReaderRef &reader)
{
    int32_t rvo;
    if (!TryReadVarint32(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
inline void WriteVarint32(const WriterRef &writer, int32_t value)
{
    if (!TryWriteVarint32(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 32-bit integer from the Reader.
 */
inline int32_t ReadSVarint32(const ReaderRef &reader)
{
    int32_t rvo;
    if (!TryReadSVarint32(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 32-bit integer to write to the Writer.
 */
inline void WriteSVarint32(const WriterRef &writer, int32_t value)
{
    if (!TryWriteSVarint32(writer, value))
    {
        throw std::runtime_error("could not write");
    }
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
inline uint64_t ReadUVarint64(const ReaderRef &reader)
{
    uint64_t rvo;
    if (!TryReadUVarint64(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
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
inline void WriteUVarint64(const WriterRef &writer, const uint64_t value)
{
    if (!TryWriteUVarint64(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function decodes negative numbers as large positive numbers.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
inline int64_t ReadVarint64(const ReaderRef &reader)
{
    int64_t rvo;
    if (!TryReadVarint64(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a signed integer encoded as a protobuf-style Varint.
 *
 * @detail This function encodes negative numbers as large positive numbers.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
inline void WriteVarint64(const WriterRef &writer, int64_t value)
{
    if (!TryWriteVarint64(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Read a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function decodes the Varint using zig-zag encoding.
 *
 * @param reader Reader to operate on.
 * @return An signed 64-bit integer from the Reader.
 */
inline int64_t ReadSVarint64(const ReaderRef &reader)
{
    int64_t rvo;
    if (!TryReadSVarint64(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a signed integer zig-zag encoded as a protobuf-style Varint.
 *
 * @detail This function encodes the Varint using zig-zag encoding.
 *
 * @param writer Writer to operate on.
 * @param value An unsigned 64-bit integer to write to the Writer.
 */
inline void WriteSVarint64(const WriterRef &writer, int64_t value)
{
    if (!TryWriteSVarint64(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

} // namespace LexIO
