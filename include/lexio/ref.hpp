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

#include <memory>

namespace LexIO
{

namespace Detail
{

using lexRead_t = size_t (*)(void *, uint8_t *, const size_t);
using lexFillBuffer_t = BufferView (*)(void *, const size_t);
using lexConsumeBuffer_t = void (*)(void *, const size_t);
using lexWrite_t = size_t (*)(void *, const uint8_t *, const size_t);
using lexFlush_t = void (*)(void *);
using lexSeek_t = size_t (*)(void *, const SeekPos);

template <typename READER>
inline size_t WrapRead(void *ptr, uint8_t *outDest, const size_t count)
{
    return static_cast<READER *>(ptr)->LexRead(outDest, count);
}

template <typename BUFFERED_READER>
inline BufferView WrapFillBuffer(void *ptr, const size_t size)
{
    return static_cast<BUFFERED_READER *>(ptr)->LexFillBuffer(size);
}

template <typename BUFFERED_READER>
inline void WrapConsumeBuffer(void *ptr, const size_t size)
{
    static_cast<BUFFERED_READER *>(ptr)->LexConsumeBuffer(size);
}

template <typename WRITER>
inline size_t WrapWrite(void *ptr, const uint8_t *src, const size_t count)
{
    return static_cast<WRITER *>(ptr)->LexWrite(src, count);
}

template <typename WRITER>
inline void WrapFlush(void *ptr)
{
    static_cast<WRITER *>(ptr)->LexFlush();
}

template <typename SEEKABLE>
inline size_t WrapSeek(void *ptr, const SeekPos pos)
{
    return static_cast<SEEKABLE *>(ptr)->LexSeek(pos);
}

} // namespace Detail

/**
 * @brief A type-erased reference to a stream that implements Reader.
 */
class ReaderRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;

  public:
    template <typename READER, typename = std::enable_if_t<IsReaderV<READER>>>
    ReaderRef(READER &reader) : m_ptr(&reader), m_lexRead(Detail::WrapRead<READER>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
};

/**
 * @brief A type-erased reference to a stream that implements BufferedReader.
 */
class BufferedReaderRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;

  public:
    template <typename BUFFERED_READER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
    BufferedReaderRef(BUFFERED_READER &bufReader)
        : m_ptr(&bufReader), m_lexRead(Detail::WrapRead<BUFFERED_READER>),
          m_lexFillBuffer(Detail::WrapFillBuffer<BUFFERED_READER>),
          m_lexConsumeBuffer(Detail::WrapConsumeBuffer<BUFFERED_READER>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    BufferView LexFillBuffer(const size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) const { m_lexConsumeBuffer(m_ptr, size); }
};

/**
 * @brief A type-erased reference to a stream that implements Reader, but not
 *        BufferedReader.
 */
class UnbufferedReaderRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;

  public:
    template <typename READER, typename = std::enable_if_t<IsReaderV<READER> && !IsBufferedReaderV<READER>>>
    UnbufferedReaderRef(READER &reader) : m_ptr(&reader), m_lexRead(Detail::WrapRead<READER>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
};

/**
 * @brief A type-erased reference to a stream that implements Writer.
 */
class WriterRef
{
  protected:
    void *m_ptr;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;

  public:
    template <typename WRITER>
    WriterRef(WRITER &writer)
        : m_ptr(&writer), m_lexWrite(Detail::WrapWrite<WRITER>), m_lexFlush(Detail::WrapFlush<WRITER>)
    {
    }

    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
};

/**
 * @brief A type-erased reference to a stream that implements Seekable.
 */
class SeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename SEEKABLE>
    SeekableRef(SEEKABLE &seekable) : m_ptr(&seekable), m_lexSeek(Detail::WrapSeek<SEEKABLE>)
    {
    }

    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
};

/**
 * @brief A type-erased reference to a stream that implements Reader,
 *        and Writer.
 */
class ReaderWriterRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;

  public:
    template <typename STREAM>
    ReaderWriterRef(STREAM &readerWriter)
        : m_ptr(&readerWriter), m_lexRead(Detail::WrapRead<STREAM>), m_lexWrite(Detail::WrapWrite<STREAM>),
          m_lexFlush(Detail::WrapFlush<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
};

/**
 * @brief A type-erased reference to a stream that implements BufferedReader,
 *        and Writer.
 */
class BufferedReaderWriterRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;

  public:
    template <typename STREAM>
    BufferedReaderWriterRef(STREAM &bufferedReaderWriter)
        : m_ptr(&bufferedReaderWriter), m_lexRead(Detail::WrapRead<STREAM>),
          m_lexFillBuffer(Detail::WrapFillBuffer<STREAM>), m_lexConsumeBuffer(Detail::WrapConsumeBuffer<STREAM>),
          m_lexWrite(Detail::WrapWrite<STREAM>), m_lexFlush(Detail::WrapFlush<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    BufferView LexFillBuffer(const size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) const { m_lexConsumeBuffer(m_ptr, size); }
    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
};

/**
 * @brief A type-erased reference to a stream that implements Reader,
 *        and Seekable.
 */
class ReaderSeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename STREAM>
    ReaderSeekableRef(STREAM &readerSeekable)
        : m_ptr(&readerSeekable), m_lexRead(Detail::WrapRead<STREAM>), m_lexSeek(Detail::WrapSeek<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
};

/**
 * @brief A type-erased reference to a stream that implements BufferedReader,
 *        and Seekable.
 */
class BufferedReaderSeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename STREAM>
    BufferedReaderSeekableRef(STREAM &bufferedReaderSeekable)
        : m_ptr(&bufferedReaderSeekable), m_lexRead(Detail::WrapRead<STREAM>),
          m_lexFillBuffer(Detail::WrapFillBuffer<STREAM>), m_lexConsumeBuffer(Detail::WrapConsumeBuffer<STREAM>),
          m_lexSeek(Detail::WrapSeek<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    BufferView LexFillBuffer(const size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) const { m_lexConsumeBuffer(m_ptr, size); }
    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
};

/**
 * @brief A type-erased reference to a stream that implements Reader, Writer,
 *        and Seekable.
 */
class ReaderWriterSeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename STREAM>
    ReaderWriterSeekableRef(STREAM &readerWriterSeekable)
        : m_ptr(&readerWriterSeekable), m_lexRead(Detail::WrapRead<STREAM>), m_lexWrite(Detail::WrapWrite<STREAM>),
          m_lexFlush(Detail::WrapFlush<STREAM>), m_lexSeek(Detail::WrapSeek<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
};

/**
 * @brief A type-erased reference to a stream that implements BufferedReader,
 *        Writer, and Seekable.
 */
class BufferedReaderWriterSeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename STREAM>
    BufferedReaderWriterSeekableRef(STREAM &bufferedReaderWriterSeekable)
        : m_ptr(&bufferedReaderWriterSeekable), m_lexRead(Detail::WrapRead<STREAM>),
          m_lexFillBuffer(Detail::WrapFillBuffer<STREAM>), m_lexConsumeBuffer(Detail::WrapConsumeBuffer<STREAM>),
          m_lexWrite(Detail::WrapWrite<STREAM>), m_lexFlush(Detail::WrapFlush<STREAM>),
          m_lexSeek(Detail::WrapSeek<STREAM>)
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
    BufferView LexFillBuffer(const size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) const { m_lexConsumeBuffer(m_ptr, size); }
    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
};

} // namespace LexIO
