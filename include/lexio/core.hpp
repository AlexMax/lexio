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
 * @file core.h
 * Core interfaces and functions needed by LexIO streams.
 *
 * LexIO streams are not derived from abstract classes, but by classes that
 * adhere to traits that are enforced by the type traits in this file.  There
 * are four basic types of stream.
 *
 * - Reader: These classes can read from a data source.
 * - BufferedReader: These classes read from a data source and keep said
 *                   data in an internal buffer.
 * - Writer: These classes can write to a data source.
 * - Seekable: These classes can seek to various points in the stream.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

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

#if (LEXIO_HAS_INLINE_VARS == 1)
#define LEXIO_INLINE_VAR inline
#else
#define LEXIO_INLINE_VAR
#endif

/**
 * @brief LexIO library namespace.
 */
namespace LexIO
{

using BufferView = std::pair<const uint8_t *, size_t>;

/**
 * @brief Possible seek directions.
 */
enum class Whence
{
    start,   // Relative to start of stream.
    current, // Relative to current stream position.
    end,     // Relative to end of stream.
};

/**
 * @brief Parameter for Seek() that dictates the desired seek.
 */
struct SeekPos
{
    ptrdiff_t offset = 0;
    Whence whence = Whence::start;

    SeekPos() = default;
    SeekPos(ptrdiff_t offset_) : offset(offset_) {}
    SeekPos(ptrdiff_t offset_, Whence whence_) : offset(offset_), whence(whence_) {}
};

namespace Detail
{

/**
 * @see https://en.cppreference.com/w/cpp/numeric/bit_cast
 */
template <class TO, class FROM>
inline TO BitCast(const FROM &src) noexcept
{
    static_assert(sizeof(TO) == sizeof(FROM), "BitCast requires equal size.");
    static_assert(std::is_trivially_copyable<FROM>::value, "BitCast FROM must be trivially copyable.");
    static_assert(std::is_trivially_copyable<TO>::value, "BitCast TO must be trivially copyable.");
    static_assert(std::is_trivially_constructible<TO>::value, "BitCast TO must be trivially constructible.");

    TO dst;
    std::memcpy(&dst, &src, sizeof(TO));
    return dst;
}

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
using ReaderType =
    decltype(std::declval<size_t &>() = std::declval<T>().LexRead(std::declval<uint8_t *>(), std::declval<size_t>()));

/**
 * @brief This type exists if the passed T conforms to BufferedReader.
 */
template <typename T>
using BufferedReaderType =
    decltype(std::declval<BufferView &>() = std::declval<T>().LexFillBuffer(std::declval<size_t>()),
             std::declval<T>().LexConsumeBuffer(std::declval<size_t>()));

/**
 * @brief This type exists if the passed T conforms to Writer.
 */
template <typename T>
using WriterType = decltype(std::declval<size_t &>() =
                                std::declval<T>().LexWrite(std::declval<const uint8_t *>(), std::declval<size_t>()),
                            std::declval<T>().LexFlush());

/**
 * @brief This type exists if the passed T conforms to Seekable.
 */
template <typename T>
using SeekableType = decltype(std::declval<size_t &>() = std::declval<T>().LexSeek(std::declval<SeekPos>()));

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
 * @brief Assert that a parameter conforms to Reader.
 */
#define LEXIO_ASSERT_READER(param)                                                                                     \
    static_assert(LexIO::IsReaderV<decltype(param)>, #param " does not conform to a LexIO::Reader");                   \
    static_assert(true, "")

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
 * @brief Assert that a parameter conforms to BufferedReader.
 */
#define LEXIO_ASSERT_BUFFERED_READER(param)                                                                            \
    static_assert(LexIO::IsBufferedReaderV<decltype(param)>, #param " does not conform to a LexIO::BufferedReader");   \
    static_assert(true, "")

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
 * @brief Assert that a parameter conforms to Writer.
 */
#define LEXIO_ASSERT_WRITER(param)                                                                                     \
    static_assert(LexIO::IsWriterV<decltype(param)>, #param " does not conform to a LexIO::Writer");                   \
    static_assert(true, "")

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

/**
 * @brief Assert that a parameter conforms to Seekable.
 */
#define LEXIO_ASSERT_SEEKABLE(param)                                                                                   \
    static_assert(LexIO::IsSeekableV<decltype(param)>, #param " does not conform to a LexIO::Seekable");               \
    static_assert(true, "")

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
 * @param outDest Pointer to starting byte of output buffer.
 * @param count Size of output buffer in bytes.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t Read(uint8_t *outDest, const size_t count, READER &reader)
{
    return reader.LexRead(outDest, count);
}

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        array and advancing the offset.
 *
 * @param outArray Output buffer.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read.  Must be between 0 and the requested
 *         length.  0 can mean EOF or empty buffer.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename READER, size_t N, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t Read(uint8_t (&outArray)[N], READER &reader)
{
    return reader.LexRead(&outArray[0], N);
}

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t Read(IT outStart, IT outEnd, READER &reader)
{
    const size_t count = std::distance(outStart, outEnd);
    return reader.LexRead(&(*outStart), count);
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
template <typename BUFFERED_READER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
inline BufferView FillBuffer(BUFFERED_READER &bufReader, const size_t size)
{
    return bufReader.LexFillBuffer(size);
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
template <typename BUFFERED_READER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
inline void ConsumeBuffer(BUFFERED_READER &bufReader, const size_t size)
{
    bufReader.LexConsumeBuffer(size);
}

/**
 * @brief Write a buffer of data at the current offset, overwriting any
 *        existing data.
 *
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @return Actual number of bytes written.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t Write(WRITER &writer, const uint8_t *src, const size_t count)
{
    return writer.LexWrite(src, count);
}

/**
 * @brief Write an array of data at the current offset, overwriting any
 *        existing data.
 *
 * @param writer Writer to operate on.
 * @param array Input array.
 * @return Actual number of bytes written.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, size_t N, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t Write(WRITER &writer, const uint8_t (&array)[N])
{
    return writer.LexWrite(&array[0], N);
}

/**
 * @brief Write a pointer or contiguous iterator pair covering a range of
 *        data at the current offset, overwriting any existing data.
 *
 * @param writer Writer to operate on.
 * @param start Iterator or pointer to start of input buffer.
 * @param end Iterator or pointer to end of input buffer.
 * @return Actual number of bytes written.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline size_t Write(WRITER &writer, IT start, IT end)
{
    const size_t count = std::distance(start, end);
    return Write<WRITER>(writer, &(*start), count);
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param writer Writer to operate on.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void Flush(WRITER &writer)
{
    return writer.LexFlush();
}

/**
 * @brief Seek to a position in the underlying Seekable.
 *
 * @param seekable Seekable to operate on.
 * @param pos Seek position.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
inline size_t Seek(SEEKABLE &seekable, const SeekPos pos)
{
    return seekable.LexSeek(pos);
}

/**
 * @brief Seek to a position in the underlying Seekable.
 *
 * @param seekable Seekable to operate on.
 * @param offset Seek offset.
 * @param whence Position to seek relative to.
 * @return Absolute position in stream after seek.
 * @throws std::runtime_error if underlying seek operation goes past start
 *         of data, or has some other error condition.
 */
template <typename SEEKABLE, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
inline size_t Seek(SEEKABLE &seekable, const ptrdiff_t offset, const Whence whence)
{
    return seekable.LexSeek(SeekPos(offset, whence));
}

//******************************************************************************
//
// The following functions are default implementations of common stream
// functionality that builds off of the basic functions.  It is expected that
// these functions can be specialized if need be.
//
//******************************************************************************

template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
inline void ReadAll(uint8_t *outDest, const size_t count, READER &reader)
{
    size_t offset = 0, remain = count;
    while (offset != count)
    {
        const size_t written = Read<READER>(outDest + offset, remain, reader);
        offset += written;
        remain -= written;
    }
}

template <typename READER, size_t N, typename = std::enable_if_t<IsReaderV<READER>>>
inline void ReadAll(uint8_t (&outArray)[N], READER &reader)
{
    size_t offset = 0, remain = N;
    while (offset != N)
    {
        const size_t written = Read<READER>(&outArray[offset], remain, reader);
        offset += written;
        remain -= written;
    }
}

template <typename READER, typename IT, typename = std::enable_if_t<IsReaderV<READER>>>
inline size_t ReadAll(IT outStart, IT outEnd, READER &reader)
{
    IT iter = outStart;
    while (iter != outEnd)
    {
        const size_t written = Read<READER>(outStart, outEnd, reader);
        iter += written;
    }
}

/**
 * @brief Get the current contents of the buffer.
 *
 * @param bufReader BufferedReader to operate on.
 * @return Span view of the internal buffer.
 */
template <typename BUFFERED_READER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
inline BufferView GetBuffer(BUFFERED_READER &bufReader)
{
    return FillBuffer(bufReader, 0);
}

/**
 * @brief Read the entire contents of the stream using the buffer.
 *
 * @param outIt Output iterator to write result into.
 * @param bufReader BufferedReader to operate on.
 * @param bufSize Number of bytes to buffer at a time.
 * @return Total number of bytes read.
 */
template <typename BUFFERED_READER, typename OUT_ITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
inline size_t ReadToEOF(OUT_ITER outIt, BUFFERED_READER &bufReader, const size_t bufSize = 8192)
{
    size_t total = 0;
    for (;;)
    {
        BufferView buf = FillBuffer(bufReader, bufSize);
        if (buf.second == 0)
        {
            // Read all data there was to read.
            return total;
        }

        // Copy the buffered data into the vector.
        std::copy(buf.first, buf.first + buf.second, outIt);

        // Consume what we've read.
        ConsumeBuffer(bufReader, buf.second);
        total += buf.second;
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
 * @param bufSize Number of bytes to buffer at a time.
 * @return Total number of bytes read.
 */
template <typename BUFFERED_READER, typename OUT_ITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
inline size_t ReadUntil(OUT_ITER outIt, BUFFERED_READER &bufReader, const uint8_t term, const size_t bufSize = 8192)
{
    size_t size = 0;
    for (;;)
    {
        BufferView buf = FillBuffer(bufReader, bufSize);
        if (buf.second == 0)
        {
            // Read all data there was to read.
            return size;
        }

        // Copy the buffered data into the iterator until we hit the passed byte.
        const uint8_t *it = buf.first;
        for (size_t i = 0; i < buf.second; i++)
        {
            if (*it == term)
            {
                // Found the terminator, append it and stop.
                *outIt++ = *it++;
                ConsumeBuffer(bufReader, i + 1);
                size += i + 1;
                return size;
            }
            *outIt++ = *it++;
        }

        // Consume what we've read.
        ConsumeBuffer(bufReader, bufSize);
        size += bufSize;
    }
}

/**
 * @brief Write a complete buffer of data to the stream, calling LexIO::Write
 *        multiple times if necessary until the entire buffer is written
 *        or an error is encountered.
 *
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteAll(WRITER &writer, const uint8_t *src, const size_t count)
{
    size_t offset = 0, remain = count;
    while (offset != count)
    {
        const size_t written = Write<WRITER>(writer, src + offset, remain);
        offset += written;
        remain -= written;
    }
}

template <typename WRITER, size_t N, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteAll(WRITER &writer, const uint8_t (&array)[N])
{
    size_t offset = 0, remain = N;
    while (offset != N)
    {
        const size_t written = Write<WRITER>(writer, array[offset], remain);
        offset += written;
        remain -= written;
    }
}

template <typename WRITER, typename IT, typename = std::enable_if_t<IsWriterV<WRITER>>>
inline void WriteAll(WRITER &writer, IT start, IT end)
{
    IT iter = start;
    while (iter != end)
    {
        const size_t written = Write<WRITER>(writer, iter, end);
        iter += written;
    }
}

/**
 * @brief Return the current offset position.
 *
 * @param seekable Seekable to operate on.
 * @return Absolute position in stream.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
inline size_t Tell(SEEKABLE &seekable)
{
    return Seek(seekable, 0, Whence::current);
}

/**
 * @brief Rewind the seekable stream to the beginning.
 *
 * @param seekable Seekable to operate on.
 * @return Resulting position in stream, should be 0 on success.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
inline size_t Rewind(SEEKABLE &seekable)
{
    return Seek(seekable, 0, Whence::start);
}

/**
 * @brief Return length of underlying data.
 *
 * @param seekable Seekable to operate on.
 * @return Length of underlying data.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
template <typename SEEKABLE, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
inline size_t Length(SEEKABLE &seekable)
{
    const size_t old = Seek(seekable, 0, Whence::current);
    const size_t len = Seek(seekable, 0, Whence::end);
    Seek(seekable, ptrdiff_t(old), Whence::start);
    return len;
}

//******************************************************************************
//
// Utility functions.
//
//******************************************************************************

/**
 * @brief Copy the contents of a buffered reader to a writer until EOF is hit
 *        on the reader.
 *
 * @param writer Writer to copy to.
 * @param bufReader Buffered read to read from.
 * @param bufSize Number of bytes to buffer per individual copy.
 * @return Number of bytes copied.
 */
template <typename WRITER, typename BUFFERED_READER, typename = std::enable_if_t<IsWriterV<WRITER>>,
          typename = std::enable_if_t<IsReaderV<BUFFERED_READER>>>
inline size_t Copy(WRITER &writer, BUFFERED_READER &bufReader, const size_t bufSize = 8192)
{
    size_t count = 0;
    for (;;)
    {
        const BufferView buffer = FillBuffer<BUFFERED_READER>(bufReader, bufSize);
        if (buffer.second == 0)
        {
            return count;
        }

        const size_t written = Write<WRITER>(writer, buffer.first, buffer.second);
        ConsumeBuffer<BUFFERED_READER>(bufReader, written);
        count += written;
    }
}

} // namespace LexIO
