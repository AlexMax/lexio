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
     * @brief Read data from the current offset, inserting it into the passed
     *        buffer and advancing the offset.
     *
     * @param buffer A span to read data into, the size of which is the number
     *               of bytes to read.
     * @return Actual number of bytes read.  Must be between 0 and the requested
     *         length.  0 can mean EOF or empty buffer.
     * @throws std::runtime_error if an error with the read operation was
     *         encountered.  EOF is _not_ considered an error.
     */
    virtual size_t Read(span_type buffer) = 0;
};

/**
 * @brief Writer interface.
 */
class Writer
{
  public:
    virtual ~Writer() {}

    /**
     * @brief Flushes data to underlying storage.  Can be a no-op.
     */
    virtual void Flush() = 0;

    /**
     * @brief Write a span of data at the current offset, overwriting any
     *        existing data.
     *
     * @param buffer Bytes to write into the data stream.
     * @return Actual number of bytes written.
     * @throws std::runtime_error if an error with the write operation was
     *         encountered.  A partial write is _not_ considered an error.
     */
    virtual size_t Write(const_span_type buffer) = 0;
};

/**
 * @brief Seekable interface.
 */
class Seekable
{
  public:
    virtual ~Seekable() {}

    /**
     * @brief Seek to a position relative to the start of the underlying data.
     *
     * @param whence Offset from start to seek.
     * @return Absolute position in stream after seek.
     * @throws std::runtime_error if underlying seek operation goes past start
     *         of data, or has some other error condition.
     */
    virtual size_t Seek(const WhenceStart whence) = 0;

    /**
     * @brief Seek to a position relative to the current offset.
     *
     * @param whence Offset from current position to seek.
     * @return Absolute position in stream after seek.
     * @throws std::runtime_error if underlying seek operation goes past start
     *         of data, or has some other error condition.
     */
    virtual size_t Seek(const WhenceCurrent whence) = 0;

    /**
     * @brief Seek to a position relative to the end of the underlying data.
     *
     * @param whence Offset from end to seek.
     * @return Absolute position in stream after seek.
     * @throws std::runtime_error if underlying seek operation goes past start
     *         of data, or has some other error condition.
     */
    virtual size_t Seek(const WhenceEnd whence) = 0;

    /**
     * @brief Return the current offset position.
     *
     * @return Absolute position in stream.
     * @throws std::runtime_error if Seek call throws, or some other error
     *         condition occurrs.
     */
    size_t Tell() { return Seek(WhenceCurrent(0)); }

    /**
     * @brief Return length of underlying data.
     *
     * @return Length of underlying data.
     * @throws std::runtime_error if Seek call throws, or some other error
     *         condition occurrs.
     */
    size_t Length()
    {
        const size_t old = Seek(WhenceCurrent(0));
        const size_t len = Seek(WhenceEnd(0));
        Seek(WhenceStart(old));
        return len;
    }
};

/**
 * @brief Reader interface for seekable data.
 */
class SeekableReader : public Reader
{
  public:
    /**
     * @brief Return a read-only view of the entire data stream.
     *
     * @return A span of the entire data stream, from offset 0 to EOF.
     */
    virtual const_span_type Data() const noexcept = 0;
};

/**
 * @brief Writer interface for seekable data.
 */
class SeekableWriter : public Writer
{
  public:
    virtual span_type Data() noexcept = 0;
};

} // namespace Type

template <typename T> size_t Seek(T &buffer, const WhenceStart whence)
{
    return buffer.Seek(whence);
}

template <typename T> size_t Seek(T &buffer, const WhenceCurrent whence)
{
    return buffer.Seek(whence);
}

template <typename T> size_t Seek(T &buffer, const WhenceEnd whence)
{
    return buffer.Seek(whence);
}

template <typename T> size_t Length(T &buffer)
{
    return buffer.Length();
}

template <typename T> size_t Tell(T &buffer)
{
    return buffer.Tell();
}

template <typename T> uint8_t ReadU8(T &buffer)
{
    uint8_t buf[sizeof(uint8_t)] = {0};
    const size_t count = buffer.Read(buf);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not read 1 byte");
    }
    return buf[0];
}

template <typename T> uint16_t ReadU16LE(T &buffer)
{
    uint8_t buf[sizeof(uint16_t)] = {0};
    const size_t count = buffer.Read(buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not read 2 bytes");
    }
    return buf[0] | (buf[1] << 8);
}

template <typename T> uint32_t ReadU32LE(T &buffer)
{
    uint8_t buf[sizeof(uint32_t)] = {0};
    const size_t count = buffer.Read(buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not read 4 bytes");
    }
    return buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
}

template <typename T> uint64_t ReadU64LE(T &buffer)
{
    uint8_t buf[sizeof(uint64_t)] = {0};
    const size_t count = buffer.Read(buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not read 8 bytes");
    }
    return static_cast<uint64_t>(buf[0]) | (static_cast<uint64_t>(buf[1]) << 8) |
           (static_cast<uint64_t>(buf[2]) << 16) | (static_cast<uint64_t>(buf[3]) << 24) |
           (static_cast<uint64_t>(buf[4]) << 32) | (static_cast<uint64_t>(buf[5]) << 40) |
           (static_cast<uint64_t>(buf[6]) << 48) | (static_cast<uint64_t>(buf[7]) << 56);
}

template <typename T> void WriteU8(T &buffer, const uint8_t value)
{
    uint8_t buf[sizeof(uint8_t)] = {value};
    const size_t count = buffer.Write(buf);
    if (count != sizeof(uint8_t))
    {
        throw std::runtime_error("could not write 1 byte");
    }
}

template <typename T> void WriteU16LE(T &buffer, const uint16_t value)
{
    uint8_t buf[sizeof(uint16_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
    };
    const size_t count = buffer.Write(buf);
    if (count != sizeof(uint16_t))
    {
        throw std::runtime_error("could not write 2 bytes");
    }
}

template <typename T> void WriteU32LE(T &buffer, const uint32_t value)
{
    uint8_t buf[sizeof(uint32_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
        static_cast<uint8_t>((value & 0xff0000) >> 16),
        static_cast<uint8_t>((value & 0xff000000) >> 24),
    };
    const size_t count = buffer.Write(buf);
    if (count != sizeof(uint32_t))
    {
        throw std::runtime_error("could not write 4 bytes");
    }
}

template <typename T> void WriteU64LE(T &buffer, const uint64_t value)
{
    uint8_t buf[sizeof(uint64_t)] = {
        static_cast<uint8_t>(value & 0xff),
        static_cast<uint8_t>((value & 0xff00) >> 8),
        static_cast<uint8_t>((value & 0xff0000) >> 16),
        static_cast<uint8_t>((value & 0xff000000) >> 24),
        static_cast<uint8_t>((value & 0xff00000000) >> 32),
        static_cast<uint8_t>((value & 0xff0000000000) >> 40),
        static_cast<uint8_t>((value & 0xff000000000000) >> 48),
        static_cast<uint8_t>((value & 0xff00000000000000) >> 56),
    };
    const size_t count = buffer.Write(buf);
    if (count != sizeof(uint64_t))
    {
        throw std::runtime_error("could not write 8 bytes");
    }
}

} // namespace LexIO
