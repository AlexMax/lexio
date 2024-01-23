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

#include "./tryfloat.hpp"
#include <stdexcept>

namespace LexIO
{

/**
 * @brief Read a little-endian float32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float32_t ReadFloat32LE(const ReaderRef &reader)
{
    float32_t rvo;
    if (!TryReadFloat32LE(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Read a big-endian float32_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float32_t ReadFloat32BE(const ReaderRef &reader)
{
    float32_t rvo;
    if (!TryReadFloat32BE(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a little-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat32LE(const WriterRef &writer, float32_t value)
{
    if (!TryWriteFloat32LE(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian float32_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat32BE(const WriterRef &writer, float32_t value)
{
    if (!TryWriteFloat32BE(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

//******************************************************************************

/**
 * @brief Read a little-endian float64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float64_t ReadFloat64LE(const ReaderRef &reader)
{
    float64_t rvo;
    if (!TryReadFloat64LE(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Read a big-endian float64_t from a stream.
 *
 * @param reader Reader to read from.
 * @return Float that was read.
 * @throws std::runtime_error if stream could not be read.
 */
inline float64_t ReadFloat64BE(const ReaderRef &reader)
{
    float64_t rvo;
    if (!TryReadFloat64BE(rvo, reader))
    {
        throw std::runtime_error("could not read");
    }
    return rvo;
}

/**
 * @brief Write a little-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat64LE(const WriterRef &writer, float64_t value)
{
    if (!TryWriteFloat64LE(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

/**
 * @brief Write a big-endian float64_t to a stream.
 *
 * @param writer Writer to write to.
 * @param value Float to write.
 * @throws std::runtime_error if stream could not be written.
 */
inline void WriteFloat64BE(const WriterRef &writer, float64_t value)
{
    if (!TryWriteFloat64BE(writer, value))
    {
        throw std::runtime_error("could not write");
    }
}

} // namespace LexIO
