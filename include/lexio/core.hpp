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
 * @file core.hpp
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

#include <cstdint>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

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

/**
 * @brief View of a contiguous buffer of bytes.
 */
class BufferView
{
  public:
    constexpr BufferView() = default;
    constexpr BufferView(const uint8_t *data, size_t size) : m_data(data), m_size(size) {}
    constexpr const uint8_t *Data() const noexcept { return m_data; }
    constexpr size_t Size() const noexcept { return m_size; }

  protected:
    const uint8_t *m_data = nullptr;
    size_t m_size = 0;
};

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
 * @see https://en.cppreference.com/w/cpp/types/is_const
 */
template <typename TYPE>
LEXIO_INLINE_VAR constexpr bool IsConstV = std::is_const<TYPE>::value;

/**
 * @see https://en.cppreference.com/w/cpp/algorithm/min
 */
template <typename TYPE>
constexpr const TYPE &Min(const TYPE &a, const TYPE &b)
{
    return a < b ? a : b;
}

/**
 * @see https://en.cppreference.com/w/cpp/numeric/bit_cast
 */
template <class TO, class FROM>
inline std::enable_if_t<
    sizeof(TO) == sizeof(FROM) && std::is_trivially_copyable<FROM>::value && std::is_trivially_copyable<TO>::value, TO>
BitCast(const FROM &src) noexcept
{
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

/**
 * @brief Function that calls a wrapped LexRead.
 */
template <typename READER>
inline size_t WrapRead(void *ptr, uint8_t *outDest, size_t count)
{
    return static_cast<READER *>(ptr)->LexRead(outDest, count);
}

template <typename BUFFERED_READER>
inline BufferView WrapFillBuffer(void *ptr, size_t size)
{
    return static_cast<BUFFERED_READER *>(ptr)->LexFillBuffer(size);
}

template <typename BUFFERED_READER>
inline void WrapConsumeBuffer(void *ptr, size_t size)
{
    static_cast<BUFFERED_READER *>(ptr)->LexConsumeBuffer(size);
}

template <typename WRITER>
inline size_t WrapWrite(void *ptr, const uint8_t *src, size_t count)
{
    return static_cast<WRITER *>(ptr)->LexWrite(src, count);
}

template <typename WRITER>
inline void WrapFlush(void *ptr)
{
    static_cast<WRITER *>(ptr)->LexFlush();
}

template <typename SEEKABLE>
inline size_t WrapSeek(void *ptr, SeekPos pos)
{
    return static_cast<SEEKABLE *>(ptr)->LexSeek(pos);
}

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

template <typename T>
struct IsRef : std::false_type
{
};

template <typename T>
LEXIO_INLINE_VAR constexpr bool IsRefV = IsRef<T>::value;

/**
 * @brief A type-erased reference to a stream that implements Reader.
 */
class ReaderRef
{
    friend class UnbufferedReaderRef;

  public:
    using WrapReadFunc = size_t (*)(void *, uint8_t *, size_t);

    template <typename READER>
    using EnableIfWrappable = std::enable_if_t<!IsRefV<READER> && IsReaderV<READER>>;

    ReaderRef() = delete;

    /**
     * @brief Copy constructor.
     */
    ReaderRef(const ReaderRef &other) : m_ptr(other.m_ptr), m_lexRead(other.m_lexRead) {}

    /**
     * @brief Construct and wrap any Reader that isn't a Ref.
     */
    template <typename READER, typename = EnableIfWrappable<READER>>
    ReaderRef(READER &reader) : m_ptr(&reader), m_lexRead(Detail::WrapRead<READER>)
    {
    }

    /**
     * @brief Member-wise constructor.  Useful if you want to construct a
     *        ReaderRef from some other type of Ref.
     */
    ReaderRef(void *ptr, WrapReadFunc lexRead) : m_ptr(ptr), m_lexRead(lexRead) {}

    /**
     * @brief Copy assignment operator.
     */
    ReaderRef &operator=(const ReaderRef &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_ptr = other.m_ptr;
        m_lexRead = other.m_lexRead;
        return *this;
    }

    /**
     * @brief Copy-assign and wrap any Reader that isn't a Ref.
     */
    template <typename READER, typename = EnableIfWrappable<READER>>
    ReaderRef &operator=(READER &reader)
    {
        m_ptr = &reader;
        m_lexRead = Detail::WrapRead<READER>;
        return *this;
    }

    size_t LexRead(uint8_t *outDest, size_t count) const { return m_lexRead(m_ptr, outDest, count); }

  protected:
    void *m_ptr;
    WrapReadFunc m_lexRead;
};

template <>
struct IsRef<ReaderRef> : std::true_type
{
};

/**
 * @brief A type-erased reference to a stream that implements BufferedReader.
 */
class BufferedReaderRef
{
  public:
    using WrapFillBufferFunc = BufferView (*)(void *, size_t);
    using WrapConsumeBufferFunc = void (*)(void *, size_t);

    template <typename BUFFERED_READER>
    using EnableIfWrappable = std::enable_if_t<!IsRefV<BUFFERED_READER> && IsBufferedReaderV<BUFFERED_READER>>;

    BufferedReaderRef() = delete;

    /**
     * @brief Copy constructor.
     */
    BufferedReaderRef(const BufferedReaderRef &other)
        : m_ptr(other.m_ptr), m_lexRead(other.m_lexRead), m_lexFillBuffer(other.m_lexFillBuffer),
          m_lexConsumeBuffer(other.m_lexConsumeBuffer)
    {
    }

    /**
     * @brief Construct and wrap any BufferedReader that isn't a Ref.
     */
    template <typename BUFFERED_READER, typename = EnableIfWrappable<BUFFERED_READER>>
    BufferedReaderRef(BUFFERED_READER &bufReader)
        : m_ptr(&bufReader), m_lexRead(Detail::WrapRead<BUFFERED_READER>),
          m_lexFillBuffer(Detail::WrapFillBuffer<BUFFERED_READER>),
          m_lexConsumeBuffer(Detail::WrapConsumeBuffer<BUFFERED_READER>)
    {
    }

    /**
     * @brief Member-wise constructor.  Useful if you want to construct a
     *        BufferedReaderRef from some other type of Ref.
     */
    BufferedReaderRef(void *ptr, ReaderRef::WrapReadFunc lexRead, WrapFillBufferFunc lexFillBuffer,
                      WrapConsumeBufferFunc lexConsumeBuffer)
        : m_ptr(ptr), m_lexRead(lexRead), m_lexFillBuffer(lexFillBuffer), m_lexConsumeBuffer(lexConsumeBuffer)
    {
    }

    /**
     * @brief Copy assignment operator.
     */
    BufferedReaderRef &operator=(const BufferedReaderRef &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_ptr = other.m_ptr;
        m_lexRead = other.m_lexRead;
        m_lexFillBuffer = other.m_lexFillBuffer;
        m_lexConsumeBuffer = other.m_lexConsumeBuffer;
        return *this;
    }

    /**
     * @brief Copy-assign and wrap any BufferedReader that isn't a Ref.
     */
    template <typename BUFFERED_READER, typename = EnableIfWrappable<BUFFERED_READER>>
    BufferedReaderRef &operator=(BUFFERED_READER &bufReader)
    {
        m_ptr = &bufReader;
        m_lexRead = Detail::WrapRead<BUFFERED_READER>;
        m_lexFillBuffer = Detail::WrapFillBuffer<BUFFERED_READER>;
        m_lexConsumeBuffer = Detail::WrapConsumeBuffer<BUFFERED_READER>;
        return *this;
    }

    /**
     * @brief User-defined conversion that directly converts to a ReaderRef,
     *        avoiding an extra indirection.
     */
    operator ReaderRef() const { return ReaderRef{m_ptr, m_lexRead}; };

    size_t LexRead(uint8_t *outDest, size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    BufferView LexFillBuffer(size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(size_t size) const { m_lexConsumeBuffer(m_ptr, size); }

  protected:
    void *m_ptr;
    ReaderRef::WrapReadFunc m_lexRead;
    WrapFillBufferFunc m_lexFillBuffer;
    WrapConsumeBufferFunc m_lexConsumeBuffer;
};

template <>
struct IsRef<BufferedReaderRef> : std::true_type
{
};

/**
 * @brief A type-erased reference to a stream that implements Reader, but not
 *        BufferedReader.  Useful if you want to make functions that overload
 *        based on the presence or absence of BufferedReader.
 */
class UnbufferedReaderRef
{
  public:
    template <typename READER>
    using EnableIfWrappable = std::enable_if_t<!IsRefV<READER> && IsReaderV<READER> && !IsBufferedReaderV<READER>>;

    UnbufferedReaderRef() = delete;

    /**
     * @brief Copy constructor.
     */
    UnbufferedReaderRef(const UnbufferedReaderRef &other) : m_ptr(other.m_ptr), m_lexRead(other.m_lexRead) {}

    /**
     * @brief Construct and wrap any Reader that isn't a Ref or BufferedReader.
     */
    template <typename READER, typename = EnableIfWrappable<READER>>
    UnbufferedReaderRef(READER &reader) : m_ptr(&reader), m_lexRead(Detail::WrapRead<READER>)
    {
    }

    /**
     * @brief Construct from a ReaderRef.  Avoids an extra indirection.
     */
    UnbufferedReaderRef(const ReaderRef &reader) : m_ptr(reader.m_ptr), m_lexRead(reader.m_lexRead) {}

    /**
     * @brief Member-wise constructor.  Useful if you want to construct an
     *        UnbufferedReaderRef from some other type of Ref.
     */
    UnbufferedReaderRef(void *ptr, ReaderRef::WrapReadFunc lexRead) : m_ptr(ptr), m_lexRead(lexRead) {}

    /**
     * @brief Copy assignment operator.
     */
    UnbufferedReaderRef &operator=(const UnbufferedReaderRef &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_ptr = other.m_ptr;
        m_lexRead = other.m_lexRead;
        return *this;
    }

    /**
     * @brief Copy-assign and wrap any Reader that isn't a Ref or
     *        BufferedReader.
     */
    template <typename READER, typename = EnableIfWrappable<READER>>
    UnbufferedReaderRef &operator=(READER &reader)
    {
        m_ptr = &reader;
        m_lexRead = Detail::WrapRead<READER>;
        return *this;
    }

    /**
     * @brief Convert to ReaderRef.
     */
    operator ReaderRef() const { return ReaderRef{m_ptr, m_lexRead}; };

    size_t LexRead(uint8_t *outDest, size_t count) const { return m_lexRead(m_ptr, outDest, count); }

  protected:
    void *m_ptr;
    ReaderRef::WrapReadFunc m_lexRead;
};

template <>
struct IsRef<UnbufferedReaderRef> : std::true_type
{
};

/**
 * @brief A type-erased reference to a stream that implements Writer.
 */
class WriterRef
{
  public:
    using WrapWriteFunc = size_t (*)(void *, const uint8_t *, size_t);
    using WrapFlushFunc = void (*)(void *);

    template <typename WRITER>
    using EnableIfWrappable = std::enable_if_t<!IsRefV<WRITER> && IsWriterV<WRITER>>;

    WriterRef() = delete;

    /**
     * @brief Copy constructor.
     */
    WriterRef(const WriterRef &other) : m_ptr(other.m_ptr), m_lexWrite(other.m_lexWrite), m_lexFlush(other.m_lexFlush)
    {
    }

    /**
     * @brief Construct and wrap any Writer that isn't a Ref.
     */
    template <typename WRITER, typename = EnableIfWrappable<WRITER>>
    WriterRef(WRITER &writer)
        : m_ptr(&writer), m_lexWrite(Detail::WrapWrite<WRITER>), m_lexFlush(Detail::WrapFlush<WRITER>)
    {
    }

    /**
     * @brief Member-wise constructor.  Useful if you want to construct an
     *        WriterRef from some other type of Ref.
     */
    WriterRef(void *ptr, WrapWriteFunc lexWrite, WrapFlushFunc lexFlush)
        : m_ptr(ptr), m_lexWrite(lexWrite), m_lexFlush(lexFlush)
    {
    }

    /**
     * @brief Copy assignment operator.
     */
    WriterRef &operator=(const WriterRef &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_ptr = other.m_ptr;
        m_lexWrite = other.m_lexWrite;
        m_lexFlush = other.m_lexFlush;
        return *this;
    }

    /**
     * @brief Copy-assign and wrap any Writer that isn't a Ref.
     */
    template <typename WRITER, typename = EnableIfWrappable<WRITER>>
    WriterRef &operator=(WRITER &writer)
    {
        m_ptr = &writer;
        m_lexWrite = Detail::WrapWrite<WRITER>;
        m_lexFlush = Detail::WrapFlush<WRITER>;
        return *this;
    }

    size_t LexWrite(const uint8_t *src, size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }

  protected:
    void *m_ptr;
    WrapWriteFunc m_lexWrite;
    WrapFlushFunc m_lexFlush;
};

template <>
struct IsRef<WriterRef> : std::true_type
{
};

/**
 * @brief A type-erased reference to a stream that implements Seekable.
 */
class SeekableRef
{
  public:
    using WrapSeekFunc = size_t (*)(void *, const SeekPos);

    template <typename SEEKABLE>
    using EnableIfWrappable = std::enable_if_t<!IsRefV<SEEKABLE> && IsSeekableV<SEEKABLE>>;

    SeekableRef() = delete;

    /**
     * @brief Copy constructor.
     */
    SeekableRef(const SeekableRef &other) : m_ptr(other.m_ptr), m_lexSeek(other.m_lexSeek) {}

    /**
     * @brief Construct and wrap any Seekable that isn't a Ref.
     */
    template <typename SEEKABLE, typename = EnableIfWrappable<SEEKABLE>>
    SeekableRef(SEEKABLE &seekable) : m_ptr(&seekable), m_lexSeek(Detail::WrapSeek<SEEKABLE>)
    {
    }

    /**
     * @brief Member-wise constructor.  Useful if you want to construct an
     *        SeekableRef from some other type of Ref.
     */
    SeekableRef(void *ptr, WrapSeekFunc lexSeek) : m_ptr(ptr), m_lexSeek(lexSeek) {}

    /**
     * @brief Copy assignment operator.
     */
    SeekableRef &operator=(const SeekableRef &other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_ptr = other.m_ptr;
        m_lexSeek = other.m_lexSeek;
        return *this;
    }

    /**
     * @brief Copy-assign and wrap any Seekable that isn't a Ref.
     */
    template <typename SEEKABLE, typename = EnableIfWrappable<SEEKABLE>>
    SeekableRef &operator=(SEEKABLE &seekable)
    {
        m_ptr = &seekable;
        m_lexSeek = Detail::WrapSeek<SEEKABLE>;
        return *this;
    }

    size_t LexSeek(SeekPos pos) const { return m_lexSeek(m_ptr, pos); }

  protected:
    void *m_ptr;
    WrapSeekFunc m_lexSeek;
};

template <>
struct IsRef<SeekableRef> : std::true_type
{
};

//******************************************************************************
//
// The following functions are used to call basic stream functionality that
// all streams that adhere to a given trait must provide.
//
//******************************************************************************

/**
 * @brief Attempt to read data from the current offset, inserting it into
 *        the passed buffer.
 *
 * @detail For a Reader, the LexRead method is expected to
 *         call the underlying read method a single time unless a retryable
 *         error is returned, such as EINTR.  Partial reads are an acceptable
 *         return condition, use LexIO::Read for a full read.
 *
 *         For a BufferedReader, this is usually implemented in terms of
 *         FillBuffer and ConsumeBuffer.
 *
 * @param outDest Pointer to starting byte of output buffer.
 * @param reader Reader to operate on.
 * @param count Number of bytes to attempt to read.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
inline size_t RawRead(uint8_t *outDest, ReaderRef reader, size_t count)
{
    return reader.LexRead(outDest, count);
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
 *         encountered, or if too large of a buffer was requested.
 */
inline BufferView FillBuffer(BufferedReaderRef bufReader, size_t size)
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
inline void ConsumeBuffer(BufferedReaderRef bufReader, size_t size)
{
    bufReader.LexConsumeBuffer(size);
}

/**
 * @brief Attempt to write a buffer of data at the current offset.
 *
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
inline size_t RawWrite(WriterRef writer, const uint8_t *src, size_t count)
{
    return writer.LexWrite(src, count);
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param writer Writer to operate on.
 */
inline void Flush(WriterRef writer)
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
inline size_t Seek(SeekableRef seekable, SeekPos pos)
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
inline size_t Seek(SeekableRef seekable, const ptrdiff_t offset, const Whence whence)
{
    return seekable.LexSeek(SeekPos(offset, whence));
}

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer.  Calls LexIO::RawRead as many times as necessary to fill
 *        the output buffer until EOF is hit.
 *
 * @param outDest Pointer to starting byte of output buffer.
 * @param reader Reader to operate on.
 * @param count Number of bytes to attempt to read.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename BYTE, typename = std::enable_if_t<!Detail::IsConstV<BYTE> && sizeof(BYTE) == 1>>
inline size_t Read(BYTE *outDest, ReaderRef reader, size_t count)
{
    uint8_t *dest = reinterpret_cast<uint8_t *>(outDest);
    size_t offset = 0, remain = count;
    while (offset != count)
    {
        const size_t read = reader.LexRead(dest + offset, remain);
        if (read == 0)
        {
            return offset;
        }

        offset += read;
        remain -= read;
    }

    return count;
}

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer.  Calls LexIO::RawRead as many times as necessary to fill
 *        the output buffer until EOF is hit.
 *
 * @param outArray Output buffer array.
 * @param reader Reader to operate on.
 * @return Actual number of bytes read, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.
 */
template <typename BYTE, size_t N, typename = std::enable_if_t<!Detail::IsConstV<BYTE> && sizeof(BYTE) == 1>>
inline size_t Read(BYTE (&outArray)[N], ReaderRef reader)
{
    uint8_t *dest = reinterpret_cast<uint8_t *>(&outArray[0]);
    size_t offset = 0, remain = N;
    while (offset != N)
    {
        const size_t read = reader.LexRead(dest + offset, remain);
        if (read == 0)
        {
            return offset;
        }

        offset += read;
        remain -= read;
    }

    return N;
}

/**
 * @brief Get the current contents of the buffer.
 *
 * @param bufReader BufferedReader to operate on.
 * @return Span view of the internal buffer.
 */
inline BufferView GetBuffer(BufferedReaderRef bufReader)
{
    return FillBuffer(bufReader, 0);
}

/**
 * @brief Write a buffer of data at the current offset.  Calls LexIO::RawWrite
 *        as many times as necessary to write the entire buffer unless EOF
 *        is hit.
 *
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename BYTE, typename = std::enable_if_t<sizeof(BYTE) == 1>>
inline size_t Write(WriterRef writer, const BYTE *src, size_t count)
{
    const uint8_t *srcByte = reinterpret_cast<const uint8_t *>(src);
    size_t offset = 0, remain = count;
    while (offset != count)
    {
        const size_t written = writer.LexWrite(srcByte + offset, remain);
        if (written == 0)
        {
            return offset;
        }

        offset += written;
        remain -= written;
    }

    return count;
}

/**
 * @brief Write a buffer of data at the current offset.  Calls LexIO::RawWrite
 *        as many times as necessary to write the entire buffer unless EOF
 *        is hit.
 *
 * @param writer Writer to operate on.
 * @param array Input buffer array.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @throws std::runtime_error if an error with the write operation was
 *         encountered.  A partial write is _not_ considered an error.
 */
template <typename BYTE, size_t N, typename = std::enable_if_t<sizeof(BYTE) == 1>>
inline size_t Write(WriterRef writer, const BYTE (&array)[N])
{
    const uint8_t *srcByte = reinterpret_cast<const uint8_t *>(&array[0]);
    size_t offset = 0, remain = N;
    while (offset != N)
    {
        const size_t written = writer.LexWrite(srcByte + offset, remain);
        if (written == 0)
        {
            return offset;
        }

        offset += written;
        remain -= written;
    }

    return N;
}

/**
 * @brief Return the current offset position.
 *
 * @param seekable Seekable to operate on.
 * @return Absolute position in stream.
 * @throws std::runtime_error if Seek call throws, or some other error
 *         condition occurrs.
 */
inline size_t Tell(SeekableRef seekable)
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
inline size_t Rewind(SeekableRef seekable)
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
inline size_t Length(SeekableRef seekable)
{
    const size_t old = Seek(seekable, 0, Whence::current);
    const size_t len = Seek(seekable, 0, Whence::end);
    Seek(seekable, ptrdiff_t(old), Whence::start);
    return len;
}

} // namespace LexIO
