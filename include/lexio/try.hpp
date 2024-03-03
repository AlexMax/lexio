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

inline void SetLastError(std::exception_ptr ex) noexcept
{
    Detail::LastError() = ex;
}

[[noreturn]] inline void ThrowLastError()
{
    std::rethrow_exception(Detail::LastError());
}

inline void ClearLastError() noexcept
{
    Detail::LastError() = nullptr;
}

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

template <typename BYTE, size_t N, typename = std::enable_if_t<!Detail::IsConstV<BYTE> && sizeof(BYTE) == 1>>
LEXIO_FORCEINLINE bool TryRead(size_t &outActual, BYTE (&outArray)[N], const ReaderRef &reader) noexcept
{
    return TryRead(outActual, outArray, reader, N);
}

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

template <typename BYTE, size_t N, typename = std::enable_if_t<sizeof(BYTE) == 1>>
LEXIO_FORCEINLINE bool TryWrite(size_t &outActual, const WriterRef &writer, const BYTE (&array)[N]) noexcept
{
    return TryWrite(outActual, writer, array, N);
}

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
