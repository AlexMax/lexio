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

//
// Core interfaces and functions needed by everything else.
//

#pragma once

#include <cstdint>
#include <stdexcept>

#if !defined(LEXIO_SPAN_TYPE) && !defined(LEXIO_CONST_SPAN_TYPE)
#include <span>
#define LEXIO_SPAN_TYPE std ::span<uint8_t>
#define LEXIO_CONST_SPAN_TYPE std::span<const uint8_t>
#endif

#ifdef __cpp_inline_variables
#define LEXIO_INLINE_VAR inline
#else
#define LEXIO_INLINE_VAR
#endif

namespace LexIO
{

using SpanT = LEXIO_SPAN_TYPE;
using ConstSpanT = LEXIO_CONST_SPAN_TYPE;

/**
 * @brief Parameter for Seek() that seeks from the start of the stream.
 */
struct WhenceStart
{
    const ptrdiff_t offset;
    WhenceStart(const ptrdiff_t off) : offset(off) {}
};

/**
 * @brief Parameter for Seek() that seeks from the current offset of the stream.
 */
struct WhenceCurrent
{
    const ptrdiff_t offset;
    WhenceCurrent(const ptrdiff_t off) : offset(off) {}
};

/**
 * @brief Parameter for Seek() that seeks from the end of the stream.
 */
struct WhenceEnd
{
    const ptrdiff_t offset;
    WhenceEnd(const ptrdiff_t off) : offset(off) {}
};

namespace Detail
{

template <typename, typename _ = void>
struct HasRawRead : std::false_type
{
};

template <typename T>
struct HasRawRead<T,                                                                                       //
                  std::enable_if_t<                                                                        //
                      std::is_same_v<size_t, decltype(std::declval<T>().RawRead(std::declval<SpanT>()))>>> //
    : std::true_type
{
};

} // namespace Detail

template <typename T>
LEXIO_INLINE_VAR constexpr bool IsReaderV = Detail::HasRawRead<T>::value;

template <typename T>
struct IsReader : std::bool_constant<IsReaderV<T>>
{
};

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer and advancing the offset.
 *
 * @param buffer Buffer to operate on.
 * @param outBytes A span to read data into, the size of which is the number
 *                 of bytes to read.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER>
inline size_t RawRead(READER &buffer, SpanT outBytes)
{
    return buffer.RawRead(outBytes);
}

/**
 * @brief Write a span of data at the current offset, overwriting any
 *        existing data.
 *
 * @param buffer Buffer to operate on.
 * @param bytes Bytes to write into the data stream.
 * @return Actual number of bytes written.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER>
inline size_t RawWrite(WRITER &buffer, ConstSpanT bytes)
{
    return buffer.RawWrite(bytes);
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param buffer Buffer to operate on.
 */
template <typename WRITER>
inline void Flush(WRITER &buffer)
{
    return buffer.Flush();
}

/**
 * @brief Seek to a position relative to the start of the underlying data.
 *
 * @param buffer Buffer to operate on.
 * @param whence Offset from start to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &buffer, const WhenceStart whence)
{
    return buffer.Seek(whence);
}

/**
 * @brief Seek to a position relative to the current offset.
 *
 * @param buffer Buffer to operate on.
 * @param whence Offset from current position to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &buffer, const WhenceCurrent whence)
{
    return buffer.Seek(whence);
}

/**
 * @brief Seek to a position relative to the end of the underlying data.
 *
 * @param buffer Buffer to operate on.
 * @param whence Offset from end to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &buffer, const WhenceEnd whence)
{
    return buffer.Seek(whence);
}

/**
 * @brief Return the current offset position.
 *
 * @param buffer Buffer to operate on.
 * @return Absolute position in stream.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE>
inline size_t Tell(SEEKABLE &buffer)
{
    return buffer.Seek(WhenceCurrent(0));
}

/**
 * @brief Return length of underlying data.
 *
 * @param buffer Buffer to operate on.
 * @return Length of underlying data.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE>
inline size_t Length(SEEKABLE &buffer)
{
    const size_t old = buffer.Seek(WhenceCurrent(0));
    const size_t len = buffer.Seek(WhenceEnd(0));
    buffer.Seek(WhenceStart(size_t(old)));
    return len;
}

/**
 * @brief Return a read-only view of the entire data stream.
 *
 * @param buffer Buffer to operate on.
 * @return A span of the entire data stream, from offset 0 to EOF.
 */
template <typename SEEKABLE_READER>
inline ConstSpanT Data(SEEKABLE_READER &buffer) noexcept
{
    return buffer.Data();
}

/**
 * @brief Return a mutable view of the entire data stream.
 *
 * @param buffer Buffer to operate on.
 * @return A span of the entire data stream, from offset 0 to EOF.
 */
template <typename SEEKABLE_WRITER>
inline SpanT Data(SEEKABLE_WRITER &buffer) noexcept
{
    return buffer.Data();
}

} // namespace LexIO
