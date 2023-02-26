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

namespace LexIO
{

using span_type = LEXIO_SPAN_TYPE;
using const_span_type = LEXIO_CONST_SPAN_TYPE;

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

namespace Type
{

/**
 * @brief Basic reader interface.
 */
class Reader
{
  public:
    virtual ~Reader() {}

    /**
     * @brief See LexIO::RawRead
     */
    virtual size_t RawRead(span_type buffer) = 0;
};

/**
 * @brief Writer interface.
 */
class Writer
{
  public:
    virtual ~Writer() {}

    /**
     * @brief See LexIO::RawWrite
     */
    virtual size_t RawWrite(const_span_type buffer) = 0;

    /**
     * @brief See LexIO::Flush
     */
    virtual void Flush() = 0;
};

/**
 * @brief Seekable interface.
 */
class Seekable
{
  public:
    virtual ~Seekable() {}

    /**
     * @brief See LexIO::Seek
     */
    virtual size_t Seek(const WhenceStart whence) = 0;

    /**
     * @brief See LexIO::Seek
     */
    virtual size_t Seek(const WhenceCurrent whence) = 0;

    /**
     * @brief See LexIO::Seek
     */
    virtual size_t Seek(const WhenceEnd whence) = 0;
};

/**
 * @brief Reader interface for seekable data.
 */
class SeekableReader : public Reader
{
  public:
    /**
     * @brief See LexIO::Data
     */
    virtual const_span_type Data() const noexcept = 0;
};

/**
 * @brief Writer interface for seekable data.
 */
class SeekableWriter : public Writer
{
  public:
    /**
     * @brief See LexIO::Data
     */
    virtual span_type Data() noexcept = 0;
};

} // namespace Type

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
size_t RawRead(Type::Reader &buffer, span_type outBytes)
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
size_t RawWrite(Type::Writer &buffer, const_span_type bytes)
{
    return buffer.RawWrite(bytes);
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param buffer Buffer to operate on.
 */
void Flush(Type::Writer &buffer)
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
size_t Seek(Type::Seekable &buffer, const WhenceStart whence)
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
size_t Seek(Type::Seekable &buffer, const WhenceCurrent whence)
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
size_t Seek(Type::Seekable &buffer, const WhenceEnd whence)
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
size_t Tell(Type::Seekable &buffer)
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
size_t Length(Type::Seekable &buffer)
{
    const size_t old = buffer.Seek(WhenceCurrent(0));
    const size_t len = buffer.Seek(WhenceEnd(0));
    buffer.Seek(WhenceStart(old));
    return len;
}

/**
 * @brief Return a read-only view of the entire data stream.
 *
 * @param buffer Buffer to operate on.
 * @return A span of the entire data stream, from offset 0 to EOF.
 */
const_span_type Data(Type::SeekableReader &buffer) noexcept
{
    return buffer.Data();
}

/**
 * @brief Return a mutable view of the entire data stream.
 *
 * @param buffer Buffer to operate on.
 * @return A span of the entire data stream, from offset 0 to EOF.
 */
span_type Data(Type::SeekableWriter &buffer) noexcept
{
    return buffer.Data();
}

} // namespace LexIO
