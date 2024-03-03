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

#include "./core.hpp"

#include <exception>

namespace LexIO
{

namespace Detail
{

std::exception_ptr &LastError() noexcept
{
    static thread_local std::exception_ptr ex;
    return ex;
}

} // namespace Detail

/**
 * @brief Set the "most recent" error for this thread.  Usually called from
 *        within a `catch` block of a LexIO function marked `noexcept`.
 *
 * @param ex Exception to set as most recent error.
 */
inline void SetLastError(std::exception_ptr ex) noexcept
{
    Detail::LastError() = ex;
}

/**
 * @brief Throw the most recent error for this thread.
 */
[[noreturn]] inline void ThrowLastError()
{
    std::rethrow_exception(Detail::LastError());
}

/**
 * @brief Clear the most recently set error for this thread.
 */
inline void ClearLastError() noexcept
{
    Detail::LastError() = nullptr;
}

/**
 * @brief Attempt to read data from the current offset, inserting it into
 *        the passed buffer.
 *
 * @param outActual Actual number of bytes read, or 0 if EOF-like condition
 *                  was encountered.
 * @param outDest Pointer to starting byte of output buffer.
 * @param reader Reader to operate on.
 * @param count Number of bytes to attempt to read.
 * @return True if successful, false if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.  To get specific
 *         error, call LexIO::ThrowLastError.
 */
inline bool TryRawRead(size_t &outActual, uint8_t *outDest, const ReaderRef &reader, size_t count) noexcept
{
    try
    {
        outActual = reader.LexRead(outDest, count);
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Fill the internal buffer of data to the requested size without
 *        advancing the offset.  If EOF is encountered, the rest of the
 *        data up to the EOF is buffered.
 *
 * @param outBuffer Span view of the internal buffer after buffering data
 *                  to the requested size.  Must be a span with a size between
 *                  0 and the given size.  A span of size 0 indicates EOF
 *                  was reached.
 * @param bufReader BufferedReader to operate on.
 * @param size Amount of data to buffer in bytes.
 * @return True if successful, false if an error with the read operation was
 *         encountered, or if too large of a buffer was requested.  To get
 *         specific error, call LexIO::ThrowLastError.
 */
inline bool TryFillBuffer(BufferView &outBuffer, const BufferedReaderRef &bufReader, size_t size) noexcept
{
    try
    {
        outBuffer = bufReader.LexFillBuffer(size);
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Signal to the reader that the given number of bytes have been
 *        "consumed" and should no longer be returned by FillBuffer.
 *
 * @param bufReader BufferedReader to operate on.
 * @param size Amount of data to consume in bytes.  Must be less than or
 *        equal to the amount of data in the visible buffer.
 * @return True if successful, false if a size greater than the amount of
 *         data in the visible buffer is passed to the function.  To get
 *         specific error, call LexIO::ThrowLastError.
 */
inline bool TryConsumeBuffer(const BufferedReaderRef &bufReader, size_t size) noexcept
{
    try
    {
        bufReader.LexConsumeBuffer(size);
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Attempt to write a buffer of data at the current offset.
 *
 * @param outActual Actual number of bytes written, or 0 if EOF-like condition
 *                  was encountered.
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @throws True if successful, false if an error with the write operation
 *         was encountered.  A partial write is _not_ considered an error.
 *         To get specific error, call LexIO::ThrowLastError.
 */
inline bool TryRawWrite(size_t &outActual, const WriterRef &writer, const uint8_t *src, size_t count) noexcept
{
    try
    {
        outActual = writer.LexWrite(src, count);
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Flushes data to underlying storage.  Can be a no-op.
 *
 * @param writer Writer to operate on.
 * @return True if successful, false if an error with the flush operation
 *         was encountered.  To get specific error, call LexIO::ThrowLastError.
 */
inline bool TryFlush(const WriterRef &writer) noexcept
{
    try
    {
        writer.LexFlush();
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Seek to a position in the underlying Seekable.
 *
 * @param outOffset Absolute position in stream after seek.
 * @param seekable Seekable to operate on.
 * @param pos Seek position.
 * @return True if successful, returns false if underlying seek operation
 *         goes past start of data, or has some other error condition.  To
 *         get specific error, call LexIO::ThrowLastError.
 */
inline bool TrySeek(size_t &outOffset, const SeekableRef &seekable, const SeekPos &pos) noexcept
{
    try
    {
        outOffset = seekable.LexSeek(pos);
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Seek to a position in the underlying Seekable.
 *
 * @param outOffset Absolute position in stream after seek.
 * @param seekable Seekable to operate on.
 * @param offset Seek offset.
 * @param whence Position to seek relative to.
 * @return True if successful, returns false if underlying seek operation
 *         goes past start of data, or has some other error condition.  To
 *         get specific error, call LexIO::ThrowLastError.
 */
inline size_t TrySeek(size_t &outOffset, const SeekableRef &seekable, ptrdiff_t offset, Whence whence) noexcept
{
    try
    {
        outOffset = seekable.LexSeek({offset, whence});
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer.  Calls LexIO::RawRead as many times as necessary to fill
 *        the output buffer until EOF is hit.
 *
 * @param outActual Actual number of bytes read, or 0 if EOF-like condition
 *                  was encountered.
 * @param outDest Pointer to starting byte of output buffer.
 * @param reader Reader to operate on.
 * @param count Number of bytes to attempt to read.
 * @return True if successful, false if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.  To get specific
 *         error, call LexIO::ThrowLastError.
 */
template <typename BYTE, typename = std::enable_if_t<sizeof(BYTE) == 1>>
inline bool TryRead(size_t &outActual, BYTE *outDest, const ReaderRef &reader, size_t count) noexcept
{
    try
    {
        uint8_t *dest = reinterpret_cast<uint8_t *>(outDest);
        size_t offset = 0, remain = count;
        while (offset != count)
        {
            const size_t read = reader.LexRead(dest + offset, remain);
            if (read == 0)
            {
                outActual = offset;
                return true;
            }

            offset += read;
            remain -= read;
        }

        outActual = count;
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Read data from the current offset, inserting it into the passed
 *        buffer.  Calls LexIO::RawRead as many times as necessary to fill
 *        the output buffer until EOF is hit.
 *
 * @param outActual Actual number of bytes read, or 0 if EOF-like condition
 *                  was encountered.
 * @param outArray Output buffer array.
 * @param reader Reader to operate on.
 * @return True if successful, false if an error with the read operation was
 *         encountered.  EOF is _not_ considered an error.  To get specific
 *         error, call LexIO::ThrowLastError.
 */
template <typename BYTE, size_t N, typename = std::enable_if_t<!Detail::IsConstV<BYTE> && sizeof(BYTE) == 1>>
LEXIO_FORCEINLINE bool TryRead(size_t &outActual, BYTE (&outArray)[N], const ReaderRef &reader) noexcept
{
    return TryRead(outActual, outArray, reader, N);
}

/**
 * @brief Write a buffer of data at the current offset.  Calls LexIO::RawWrite
 *        as many times as necessary to write the entire buffer unless EOF
 *        is hit.
 *
 * @param outActual Actual number of bytes written, or 0 if EOF-like condition
 *                  was encountered.
 * @param writer Writer to operate on.
 * @param src Pointer to starting byte of input buffer.
 * @param count Size of input buffer in bytes.
 * @return True if successful, false if an error with the write operation
 *         was encountered.  A partial write is _not_ considered an error.
 *         To get specific error, call LexIO::ThrowLastError.
 */
template <typename BYTE, typename = std::enable_if_t<sizeof(BYTE) == 1>>
inline bool TryWrite(size_t &outActual, const WriterRef &writer, const BYTE *src, size_t count) noexcept
{
    try
    {
        const uint8_t *srcByte = reinterpret_cast<const uint8_t *>(src);
        size_t offset = 0, remain = count;
        while (offset != count)
        {
            const size_t written = writer.LexWrite(srcByte + offset, remain);
            if (written == 0)
            {
                outActual = offset;
                return true;
            }

            offset += written;
            remain -= written;
        }

        outActual = count;
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Write a buffer of data at the current offset.  Calls LexIO::RawWrite
 *        as many times as necessary to write the entire buffer unless EOF
 *        is hit.
 *
 * @param outActual Actual number of bytes written, or 0 if EOF-like condition
 *                  was encountered.
 * @param writer Writer to operate on.
 * @param array Input buffer array.
 * @return Actual number of bytes written, or 0 if EOF-like condition was
 *         encountered.
 * @return True if successful, false if an error with the write operation
 *         was encountered.  A partial write is _not_ considered an error.
 *         To get specific error, call LexIO::ThrowLastError.
 */
template <typename BYTE, size_t N, typename = std::enable_if_t<sizeof(BYTE) == 1>>
LEXIO_FORCEINLINE bool TryWrite(size_t &outActual, const WriterRef &writer, const BYTE (&array)[N]) noexcept
{
    return TryWrite(outActual, writer, array, N);
}

/**
 * @brief Return the current offset position.
 *
 * @param outOffset Absolute position in stream.
 * @param seekable Seekable to operate on.
 * @return True if successful, false if Seek call throws, or some other error
 *         condition occurrs.  To get specific error, call
 *         LexIO::ThrowLastError.
 */
inline bool TryTell(size_t &outOffset, const SeekableRef &seekable) noexcept
{
    try
    {
        outOffset = seekable.LexSeek({0, Whence::current});
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Rewind the seekable stream to the beginning.
 *
 * @param outOffset Resulting position in stream, should be 0 on success.
 * @param seekable Seekable to operate on.
 * @return True if successful, false if Seek call throws, or some other error
 *         condition occurrs.  To get specific error, call
 *         LexIO::ThrowLastError.
 */
inline bool TryRewind(size_t &outOffset, const SeekableRef &seekable) noexcept
{
    try
    {
        outOffset = seekable.LexSeek({0, Whence::start});
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

/**
 * @brief Return length of underlying data.
 *
 * @param outLength Length of underlying data.
 * @param seekable Seekable to operate on.
 * @return True if successful, false if Seek call throws, or some other error
 *         condition occurrs.  To get specific error, call
 *         LexIO::ThrowLastError.
 */
inline bool TryLength(size_t &outLength, const SeekableRef &seekable) noexcept
{
    try
    {
        const size_t old = seekable.LexSeek({0, Whence::current});
        outLength = seekable.LexSeek({0, Whence::end});
        seekable.LexSeek({ptrdiff_t(old), Whence::start});
        return true;
    }
    catch (...)
    {
        SetLastError(std::current_exception());
        return false;
    }
}

} // namespace LexIO
