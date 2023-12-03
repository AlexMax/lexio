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

#include "./trybytes.hpp"

namespace LexIO
{

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
