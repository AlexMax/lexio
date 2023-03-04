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

/**
 * @brief SFINAE struct for detecting a valid "RawRead" method in a Reader type.
 *
 * @tparam T Type to check.
 */
template <typename, typename _ = void>
struct HasRawRead : std::false_type
{
};

template <typename T>
struct HasRawRead<T,                                                                                            //
                  std::enable_if_t<                                                                             //
                      std::is_same<size_t, decltype(std::declval<T>().RawRead(std::declval<SpanT>()))>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "RawWrite" method in a Writer type.
 *
 * @tparam T Type to check.
 */
template <typename T, typename _ = void>
struct HasRawWrite : std::false_type
{
};

template <typename T>
struct HasRawWrite<
    T,                                                                                                  //
    std::enable_if_t<                                                                                   //
        std::is_same<size_t, decltype(std::declval<T>().RawWrite(std::declval<ConstSpanT>()))>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "Flush" method in a Writer type.
 *
 * @tparam T Type to check.
 */
template <typename, typename _ = void>
struct HasFlush : std::false_type
{
};

template <typename T>
struct HasFlush<T,                                                                   //
                std::enable_if_t<                                                    //
                    std::is_same<void, decltype(std::declval<T>().Flush())>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "Seek" method in a Writer type.
 *
 * @tparam T Type to check.
 * @tparam U "Whence" overload to check with.
 */
template <typename T, typename U, typename _ = void>
struct HasSeek : std::false_type
{
};

template <typename T, typename U>
struct HasSeek<T, U,                                                                                  //
               std::enable_if_t<                                                                      //
                   std::is_same<size_t, decltype(std::declval<T>().Seek(std::declval<U>()))>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "GetBuffer" method in a BufferedReader type.
 *
 * @tparam T Type to check.
 */
template <typename, typename _ = void>
struct HasGetBuffer : std::false_type
{
};

template <typename T>
struct HasGetBuffer<T,                                                                                    //
                    std::enable_if_t<                                                                     //
                        std::is_same<LexIO::ConstSpanT, decltype(std::declval<T>().GetBuffer())>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "FillBuffer" method in a BufferedReader type.
 *
 * @tparam T Type to check.
 */
template <typename, typename _ = void>
struct HasFillBuffer : std::false_type
{
};

template <typename T>
struct HasFillBuffer<
    T,                                                                                                           //
    std::enable_if_t<                                                                                            //
        std::is_same<LexIO::ConstSpanT, decltype(std::declval<T>().FillBuffer(std::declval<size_t>()))>::value>> //
    : std::true_type
{
};

/**
 * @brief SFINAE struct for detecting a valid "ConsumeBuffer" method in a BufferedReader type.
 *
 * @tparam T Type to check.
 */
template <typename, typename _ = void>
struct HasConsumeBuffer : std::false_type
{
};

template <typename T>
struct HasConsumeBuffer<
    T,                                                                                                 //
    std::enable_if_t<                                                                                  //
        std::is_same<void, decltype(std::declval<T>().ConsumeBuffer(std::declval<size_t>()))>::value>> //
    : std::true_type
{
};

} // namespace Detail

/**
 * @brief Helper variable for IsReader trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsReaderV = Detail::HasRawRead<T>::value;

/**
 * @brief If the template parameter is a valid Reader, provides a member
 *        constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
struct IsReader : std::integral_constant<bool, IsReaderV<T>>
{
};

/**
 * @brief Helper variable for IsBufferedReader trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsBufferedReaderV =
    Detail::HasGetBuffer<T>::value && Detail::HasFillBuffer<T>::value && Detail::HasConsumeBuffer<T>::value;

/**
 * @brief If the template parameter is a valid BufferedReader, provides a
 *        member constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
struct IsBufferedReader : std::integral_constant<bool, IsBufferedReaderV<T>>
{
};

/**
 * @brief Helper variable for IsWriter trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsWriterV = Detail::HasRawWrite<T>::value && Detail::HasFlush<T>::value;

/**
 * @brief If the template parameter is a valid Writer, provides a member
 *        constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
struct IsWriter : std::integral_constant<bool, IsWriterV<T>>
{
};

/**
 * @brief Helper variable for IsSeekable trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsSeekableV =
    Detail::HasSeek<T, WhenceStart>::value && Detail::HasSeek<T, WhenceCurrent>::value &&
    Detail::HasSeek<T, WhenceEnd>::value;

/**
 * @brief If the template parameter is a valid SeekableReader, provides a
 *        member constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
struct IsSeekable : std::integral_constant<bool, IsSeekableV<T>>
{
};

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer and advancing the offset.
 *
 * @param reader Reader to operate on.
 * @param outBytes A span to read data into, the length of which is the number
 *                 of bytes to read.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER>
inline size_t RawRead(READER &reader, SpanT outBytes)
{
    return reader.RawRead(outBytes);
}

/**
 * @brief Return a view of the internal buffer used by the reader.
 *
 * @param bufReader BufferedReader to operate on.
 * @return Span view of the internal buffer.
 */
template <typename BUFFERED_READER>
inline LexIO::ConstSpanT GetBuffer(BUFFERED_READER &bufReader) noexcept
{
    return bufReader.GetBuffer();
}

/**
 * @brief Fill the internal buffer of data to the requested size without
 *        advancing the offset.  If EOF is encountered, the rest of the
 *        data up to the EOF is buffered.
 *
 * @param bufReader BufferedReader to operate on.
 * @param size Amount of data to buffer in bytes.
 * @return Span view of the internal buffer after buffering data to the
 *         requested size.  Must be a span with a size between 0 and the
 *         given size.  A span of size 0 indicates EOF was reached.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename BUFFERED_READER>
inline LexIO::ConstSpanT FillBuffer(BUFFERED_READER &bufReader, const size_t size)
{
    return bufReader.FillBuffer(size);
}

/**
 * @brief Signal to the reader that the given number of bytes have been
 *        "consumed" and should no longer be returned by
 *
 * @param bufReader BufferedReader to operate on.
 * @param size Amount of data to consume in bytes.  Must be less than or
 *        equal to the amount of data in the visible buffer.
 * @throws std::runtime_error if a size greater than the amount of data
 *         in the visible buffer is passed to the function.
 */
template <typename BUFFERED_READER>
inline void ConsumeBuffer(BUFFERED_READER &bufReader, const size_t size)
{
    bufReader.ConsumeBuffer(size);
}

/**
 * @brief Write a span of data at the current offset, overwriting any
 *        existing data.
 *
 * @param writer Writer to operate on.
 * @param bytes Bytes to write into the data stream.
 * @return Actual number of bytes written.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER>
inline size_t RawWrite(WRITER &writer, ConstSpanT bytes)
{
    return writer.RawWrite(bytes);
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param writer Writer to operate on.
 */
template <typename WRITER>
inline void Flush(WRITER &writer)
{
    return writer.Flush();
}

/**
 * @brief Seek to a position relative to the start of the underlying data.
 *
 * @param seekable Seekable to operate on.
 * @param whence Offset from start to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &seekable, const WhenceStart whence)
{
    return seekable.Seek(whence);
}

/**
 * @brief Seek to a position relative to the current offset.
 *
 * @param seekable Seekable to operate on.
 * @param whence Offset from current position to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &seekable, const WhenceCurrent whence)
{
    return seekable.Seek(whence);
}

/**
 * @brief Seek to a position relative to the end of the underlying data.
 *
 * @param seekable Seekable to operate on.
 * @param whence Offset from end to seek.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE>
inline size_t Seek(SEEKABLE &seekable, const WhenceEnd whence)
{
    return seekable.Seek(whence);
}

/**
 * @brief Return the current offset position.
 *
 * @param seekable Seekable to operate on.
 * @return Absolute position in stream.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE>
inline size_t Tell(SEEKABLE &seekable)
{
    return seekable.Seek(WhenceCurrent(0));
}

/**
 * @brief Return length of underlying data.
 *
 * @param seekable Seekable to operate on.
 * @return Length of underlying data.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE>
inline size_t Length(SEEKABLE &seekable)
{
    const size_t old = seekable.Seek(WhenceCurrent(0));
    const size_t len = seekable.Seek(WhenceEnd(0));
    seekable.Seek(WhenceStart(size_t(old)));
    return len;
}

} // namespace LexIO
