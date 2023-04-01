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

//******************************************************************************
//
// Core interfaces and functions needed by LexIO streams.
//
// LexIO streams are not derived from abstract classes, but by classes that
// adhere to traits that are enforced by the type traits in this file.  There
// are four basic types of stream.
//
// - Reader: These classes can read from a data source.
// - BufferedReader: These classes read from a data source and keep said
//                   data in an internal buffer.
// - Writer: These classes can write to a data source.
// - Seekable: These classes can seek to various points in the stream.
//
//******************************************************************************

#pragma once

#include <cstdint>
#include <stdexcept>

// Feature detection - since we support C++14 use cppreference to see when
// features were added to the compiler.

#if defined(__clang__)
#define LEXIO_CLANG_MAJOR (__clang_major__)
#define LEXIO_CLANG_MINOR (__clang_minor__)
#define LEXIO_GNUC (0)
#define LEXIO_MSC_VER (0)
#define LEXIO_CPLUSPLUS (__cplusplus)
#elif defined(__GNUC__) || defined(__GNUG__)
#define LEXIO_CLANG_MAJOR (0)
#define LEXIO_CLANG_MINOR (0)
#define LEXIO_GNUC (__GNUC__)
#define LEXIO_MSC_VER (0)
#define LEXIO_CPLUSPLUS (__cplusplus)
#elif defined(_MSC_VER)
#define LEXIO_CLANG_MAJOR (0)
#define LEXIO_CLANG_MINOR (0)
#define LEXIO_GNUC (0)
#define LEXIO_MSC_VER (_MSC_VER)
#define LEXIO_CPLUSPLUS (_MSVC_LANG)
#endif

#if (LEXIO_CPLUSPLUS < 201402L)
#error "LexIO requires support for at least C++14"
#endif

// Inline variables were added in GCC 7, Clang 3.9, VS 2017 15.5

#if !defined(LEXIO_HAS_INLINE_VARS)
#if (LEXIO_CPLUSPLUS >= 201703L) &&                                                                                    \
    (LEXIO_GCC >= 7 || (LEXIO_CLANG_MAJOR >= 3 && LEXIO_CLANG_MINOR >= 9) || LEXIO_MSC_VER >= 1912)
#define LEXIO_HAS_INLINE_VARS 1
#else
#define LEXIO_HAS_INLINE_VARS 0
#endif
#endif

#if !defined(LEXIO_SPAN)
#include <span>
#define LEXIO_SPAN(T) std::span<T>
#endif

#if (LEXIO_HAS_INLINE_VARS == 1)
#define LEXIO_INLINE_VAR inline
#else
#define LEXIO_INLINE_VAR
#endif

namespace LexIO
{

using ByteSpanT = LEXIO_SPAN(uint8_t);
using ConstByteSpanT = LEXIO_SPAN(const uint8_t);

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
 * @see https://en.cppreference.com/w/cpp/types/void_t
 */
template <class...>
using VoidT = void;

/**
 * @see https://en.cppreference.com/w/cpp/experimental/is_detected
 */
template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
struct Detector
{
    using value_t = std::false_type;
    using type = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct Detector<Default, VoidT<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type = Op<Args...>;
};

/**
 * @brief Indicates detection failure from IsDetected.
 *
 * @see https://en.cppreference.com/w/cpp/experimental/nonesuch
 */
struct Nonesuch
{
    ~Nonesuch() = delete;
    Nonesuch(Nonesuch const &) = delete;
    void operator=(Nonesuch const &) = delete;
};

/**
 * @brief Check to see if the template Op<Args...> exists and aliases
 *        std::true_type if so, otherwise it alises std::false_type.
 *
 * @see https://en.cppreference.com/w/cpp/experimental/is_detected
 */
template <template <class...> class Op, class... Args>
using IsDetected = typename Detector<Nonesuch, void, Op, Args...>::value_t;

/**
 * @brief This type exists if the passed T conforms to Reader.
 */
template <typename T>
using ReaderType = decltype(std::declval<size_t &>() = std::declval<T>().RawRead(std::declval<ByteSpanT>()));

/**
 * @brief This type exists if the passed T conforms to BufferedReader.
 */
template <typename T>
using BufferedReaderType =
    decltype(std::declval<size_t &>() = std::declval<T>().GetBufferSize(),
             std::declval<ConstByteSpanT &>() = std::declval<T>().FillBuffer(std::declval<size_t>()),
             std::declval<T>().ConsumeBuffer(std::declval<size_t>()));

/**
 * @brief This type exists if the passed T conforms to Writer.
 */
template <typename T>
using WriterType = decltype(std::declval<size_t &>() = std::declval<T>().RawWrite(std::declval<ConstByteSpanT>()),
                            std::declval<T>().Flush());

/**
 * @brief This type exists if the passed T conforms to Seekable.
 */
template <typename T>
using SeekableType = decltype(std::declval<size_t &>() = std::declval<T>().Seek(std::declval<WhenceStart>()),
                              std::declval<size_t &>() = std::declval<T>().Seek(std::declval<WhenceCurrent>()),
                              std::declval<size_t &>() = std::declval<T>().Seek(std::declval<WhenceEnd>()));

} // namespace Detail

/**
 * @brief If the template parameter is a valid Reader, provides a member
 *        constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
using IsReader = Detail::IsDetected<Detail::ReaderType, T>;

/**
 * @brief Helper variable for IsReader trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsReaderV = IsReader<T>::value;

/**
 * @brief If the template parameter is a valid BufferedReader, provides a
 *        member constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
using IsBufferedReader = Detail::IsDetected<Detail::BufferedReaderType, T>;

/**
 * @brief Helper variable for IsBufferedReader trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsBufferedReaderV = IsBufferedReader<T>::value;

/**
 * @brief If the template parameter is a valid Writer, provides a member
 *        constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
using IsWriter = Detail::IsDetected<Detail::WriterType, T>;

/**
 * @brief Helper variable for IsWriter trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsWriterV = IsWriter<T>::value;

/**
 * @brief If the template parameter is a valid SeekableReader, provides a
 *        member constant "value" of true.  Otherwise, "value" is false.
 *
 * @tparam T Type to check.
 */
template <typename T>
using IsSeekable = Detail::IsDetected<Detail::SeekableType, T>;

/**
 * @brief Helper variable for IsSeekable trait.
 */
template <typename T>
LEXIO_INLINE_VAR constexpr bool IsSeekableV = IsSeekable<T>::value;

//******************************************************************************
//
// The following functions are used to call basic stream functionality that
// all streams that adhere to a given trait must provide.
//
//******************************************************************************

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer and advancing the offset.
 *
 * @param outBytes A span to read data into, the length of which is the number
 *                 of bytes to read.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER>
inline size_t Read(ByteSpanT outBytes, READER &reader)
{
    return reader.RawRead(outBytes);
}

/**
 * @brief Returns the size of the internal buffer.  This tells you how much
 *        data the buffer is currently capable of holding, regardless of
 *        how much data is actually buffered.
 *
 * @param bufReader BufferedReader to examine.
 * @return Size of internal buffer.
 */
template <typename BUFFERED_READER>
inline size_t GetBufferSize(BUFFERED_READER &bufReader) noexcept
{
    return bufReader.GetBufferSize();
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
inline ConstByteSpanT FillBuffer(BUFFERED_READER &bufReader, const size_t size)
{
    return bufReader.FillBuffer(size);
}

/**
 * @brief Signal to the reader that the given number of bytes have been
 *        "consumed" and should no longer be returned by FillBuffer.
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
inline size_t Write(WRITER &writer, ConstByteSpanT bytes)
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

//******************************************************************************
//
// The following functions are default implementations of common stream
// functionality that builds off of the basic functions.  It is expected that
// these functions can be specialized if need be.
//
//******************************************************************************

/**
 * @brief Read function that writes into a char span.
 *
 * @param outChars A span of char to read data into, the length of which is
 *                 the number of bytes to read.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 */
template <typename READER>
inline size_t Read(LEXIO_SPAN(char) outChars, READER &reader)
{
    ByteSpanT outBytes{reinterpret_cast<uint8_t *>(outChars.data()), outChars.size()};
    const size_t actualSize = Read(outBytes, reader);
    return actualSize;
}

/**
 * @brief Read overload that writes into a void pointer/length combo.  Not
 *        recommended unless you're dealing with C coding style or libraries.
 *
 * @param outData An preallocated void pointer, assumed to have an underlying
 *                type of size 1.
 * @param outSize Size of preallocated buffer in bytes.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 */
template <typename READER>
inline size_t Read(void *outData, const size_t outSize, READER &reader)
{
    ByteSpanT outBytes{reinterpret_cast<uint8_t *>(outData), outSize};
    const size_t actualSize = Read(outBytes, reader);
    return actualSize;
}

/**
 * @brief Get the current contents of the buffer.
 *
 * @param bufReader BufferedReader to operate on.
 * @return Span view of the internal buffer.
 */
template <typename BUFFERED_READER>
inline ConstByteSpanT GetBuffer(BUFFERED_READER &bufReader)
{
    return FillBuffer(bufReader, 0);
}

/**
 * @brief Read the entire contents of the stream using the buffer.
 *
 * @param outIt Output iterator to write result into.
 * @param bufReader BufferedReader to operate on.
 * @return Total number of bytes read.
 */
template <typename OUT_ITER, typename BUFFERED_READER>
size_t ReadAll(OUT_ITER outIt, BUFFERED_READER &bufReader)
{
    size_t size = 0;
    for (;;)
    {
        ConstByteSpanT buf = FillBuffer(bufReader, GetBufferSize(bufReader));
        if (buf.size() == 0)
        {
            // Read all data there was to read.
            return size;
        }

        // Copy the buffered data into the vector.
        std::copy(buf.begin(), buf.end(), outIt);

        // Consume what we've read.
        ConsumeBuffer(bufReader, buf.size());
        size += buf.size();
    }
}

/**
 * @brief Read the entire contents of the stream until we hit a terminating byte
 *        or until EOF is hit.  The output will contain the terminator as the
 *        last character, if seen.
 *
 * @param outIt Output iterator to write result into.
 * @param bufReader BufferedReader to operate on.
 * @param term Byte to stop at.
 * @return Total number of bytes read.
 */
template <typename OUT_ITER, typename BUFFERED_READER>
size_t ReadUntil(OUT_ITER outIt, BUFFERED_READER &bufReader, const uint8_t term)
{
    size_t size = 0;
    for (;;)
    {
        ConstByteSpanT buf = FillBuffer(bufReader, GetBufferSize(bufReader));
        if (buf.size() == 0)
        {
            // Read all data there was to read.
            return size;
        }

        // Copy the buffered data into the vector until we hit the passed byte.
        auto it = buf.begin();
        while (it != buf.end())
        {
            if (*it == term)
            {
                // Found the terminator, append it and stop.
                *outIt++ = *it++;
                break;
            }
            *outIt++ = *it++;
        }

        // Consume what we've read.
        const size_t count = static_cast<size_t>(it - buf.begin());
        ConsumeBuffer(bufReader, count);
        size += count;
    }
}

template <typename WRITER>
inline size_t Write(WRITER &writer, LEXIO_SPAN(char) chars)
{
    ConstByteSpanT outBytes{reinterpret_cast<const uint8_t *>(chars.data()), chars.size()};
    return Write(writer, outBytes);
}

template <typename WRITER>
inline size_t Write(WRITER &writer, const void *data, const size_t size)
{
    ConstByteSpanT outBytes{reinterpret_cast<const uint8_t *>(data), size};
    return Write(writer, outBytes);
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
    return Seek(seekable, WhenceCurrent(0));
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
    const size_t old = Seek(seekable, WhenceCurrent(0));
    const size_t len = Seek(seekable, WhenceEnd(0));
    Seek(seekable, WhenceStart(size_t(old)));
    return len;
}

} // namespace LexIO
