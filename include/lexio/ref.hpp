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

namespace LexIO
{

/**
 * @brief A type-erased reference to a stream that implements Reader,
 *        and Writer.
 */
class ReaderWriterRef
{
  protected:
    void *m_ptr;
    ReaderRef::WrapReadFunc m_lexRead;
    WriterRef::WrapWriteFunc m_lexWrite;
    WriterRef::WrapFlushFunc m_lexFlush;

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
    ReaderRef::WrapReadFunc m_lexRead;
    BufferedReaderRef::WrapFillBufferFunc m_lexFillBuffer;
    BufferedReaderRef::WrapConsumeBufferFunc m_lexConsumeBuffer;
    WriterRef::WrapWriteFunc m_lexWrite;
    WriterRef::WrapFlushFunc m_lexFlush;

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
    ReaderRef::WrapReadFunc m_lexRead;
    SeekableRef::WrapSeekFunc m_lexSeek;

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
    ReaderRef::WrapReadFunc m_lexRead;
    BufferedReaderRef::WrapFillBufferFunc m_lexFillBuffer;
    BufferedReaderRef::WrapConsumeBufferFunc m_lexConsumeBuffer;
    SeekableRef::WrapSeekFunc m_lexSeek;

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
    ReaderRef::WrapReadFunc m_lexRead;
    WriterRef::WrapWriteFunc m_lexWrite;
    WriterRef::WrapFlushFunc m_lexFlush;
    SeekableRef::WrapSeekFunc m_lexSeek;

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
    ReaderRef::WrapReadFunc m_lexRead;
    BufferedReaderRef::WrapFillBufferFunc m_lexFillBuffer;
    BufferedReaderRef::WrapConsumeBufferFunc m_lexConsumeBuffer;
    WriterRef::WrapWriteFunc m_lexWrite;
    WriterRef::WrapFlushFunc m_lexFlush;
    SeekableRef::WrapSeekFunc m_lexSeek;

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
