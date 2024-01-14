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
 * @file float.hpp
 * @brief Floating-point serialization functions that throw exceptions on
 *        failure.
 */

#pragma once

#include "./int.hpp"
#include "./tryfloat.hpp"

namespace LexIO
{

/**
 * @brief Read a little-endian float32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float32_t ReadFloat32LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<float32_t>(reader, TryReadFloat32LE);
}

/**
 * @brief Read a big-endian float32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float32_t ReadFloat32BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<float32_t>(reader, TryReadFloat32BE);
}

/**
 * @brief Write a little-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat32LE(WriterRef writer, float32_t value)
{
    Detail::WriteWithExcept<float32_t>(writer, value, TryWriteFloat32LE);
}

/**
 * @brief Write a big-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat32BE(WriterRef writer, float32_t value)
{
    Detail::WriteWithExcept<float32_t>(writer, value, TryWriteFloat32BE);
}

//******************************************************************************

/**
 * @brief Read a little-endian float64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float64_t ReadFloat64LE(ReaderRef reader)
{
    return Detail::ReadWithExcept<float64_t>(reader, TryReadFloat64LE);
}

/**
 * @brief Read a big-endian float64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float64_t ReadFloat64BE(ReaderRef reader)
{
    return Detail::ReadWithExcept<float64_t>(reader, TryReadFloat64BE);
}

/**
 * @brief Write a little-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat64LE(WriterRef writer, float64_t value)
{
    Detail::WriteWithExcept<float64_t>(writer, value, TryWriteFloat64LE);
}

/**
 * @brief Write a big-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat64BE(WriterRef writer, float64_t value)
{
    Detail::WriteWithExcept<float64_t>(writer, value, TryWriteFloat64BE);
}

} // namespace LexIO
