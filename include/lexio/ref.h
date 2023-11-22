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

#ifndef LEXIO_ABSTRACT_H
#define LEXIO_ABSTRACT_H

#include "./core.h"

#include <memory>

namespace LexIO
{

namespace Detail
{
using lexRead_t = size_t (*)(void *, ByteSpanT);
using lexGetBufferSize_t = size_t (*)(void *);
using lexFillBuffer_t = ConstByteSpanT (*)(void *, const size_t);
using lexConsumeBuffer_t = void (*)(void *, const size_t);
using lexWrite_t = size_t (*)(void *, ConstByteSpanT);
using lexFlush_t = void (*)(void *);
using lexSeek_t = size_t (*)(void *, const SeekPos);
} // namespace Detail

class ReaderRef
{
    void *m_ptr;
    Detail::lexRead_t m_lexRead;

  public:
    template <typename READER>
    ReaderRef(READER &reader)
        : m_ptr(&reader), m_lexRead([](void *ptr, ByteSpanT outBytes) -> size_t {
              return static_cast<READER *>(ptr)->LexRead(outBytes);
          })
    {
    }

    size_t LexRead(ByteSpanT outBytes) { return m_lexRead(m_ptr, outBytes); }
};

class BufferedReaderRef : public ReaderRef
{
    void *m_ptr;
    Detail::lexGetBufferSize_t m_lexGetBufferSize;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;

  public:
    template <typename BUFFERED_READER>
    BufferedReaderRef(BUFFERED_READER &bufReader)
        : ReaderRef(bufReader), m_ptr(&bufReader), m_lexGetBufferSize([](void *ptr) -> size_t { //
              return static_cast<BUFFERED_READER *>(ptr)->LexGetBufferSize();
          }),
          m_lexFillBuffer([](void *ptr, const size_t size) -> ConstByteSpanT { //
              return static_cast<BUFFERED_READER *>(ptr)->LexFillBuffer(size);
          }),
          m_lexConsumeBuffer([](void *ptr, const size_t size) -> void { //
              static_cast<BUFFERED_READER *>(ptr)->LexConsumeBuffer(size);
          })
    {
    }

    size_t LexGetBufferSize() const { return m_lexGetBufferSize(m_ptr); }
    ConstByteSpanT LexFillBuffer(const size_t size) { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) { m_lexConsumeBuffer(m_ptr, size); }
};

class WriterRef
{
    void *m_ptr;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;

  public:
    template <typename WRITER>
    WriterRef(WRITER &writer)
        : m_ptr(&writer), m_lexWrite([](void *ptr, ConstByteSpanT bytes) -> size_t { //
              return static_cast<WRITER *>(ptr)->LexWrite(bytes);
          }),
          m_lexFlush([](void *ptr) -> void { //
              static_cast<WRITER *>(ptr)->LexFlush();
          })
    {
    }

    size_t LexWrite(ConstByteSpanT bytes) { return m_lexWrite(m_ptr, bytes); }
    void LexFlush() { m_lexFlush(m_ptr); }
};

class SeekableRef
{
    void *m_ptr;
    Detail::lexSeek_t m_lexSeek;

  public:
    template <typename SEEKABLE>
    SeekableRef(SEEKABLE &seekable)
        : m_ptr(&seekable), m_lexSeek([](void *ptr, const SeekPos pos) -> size_t { //
              return static_cast<SEEKABLE *>(ptr)->LexSeek(pos);
          })
    {
    }

    size_t LexSeek(const SeekPos pos) { return m_lexSeek(m_ptr, pos); }
};

class ReaderWriterRef : public ReaderRef, public WriterRef
{
  public:
    template <typename READER_WRITER>
    ReaderWriterRef(READER_WRITER &readerWriter) : ReaderRef(readerWriter), WriterRef(readerWriter)
    {
    }
};

class BufferedReaderWriterRef : public BufferedReaderRef, public WriterRef
{
  public:
    template <typename BUFFERED_READER_WRITER>
    BufferedReaderWriterRef(BUFFERED_READER_WRITER &bufferedReaderWriter)
        : BufferedReaderRef(bufferedReaderWriter), WriterRef(bufferedReaderWriter)
    {
    }
};

class ReaderSeekableRef : public ReaderRef, public SeekableRef
{
  public:
    template <typename READER_SEEKABLE>
    ReaderSeekableRef(READER_SEEKABLE &readerSeekable) : ReaderRef(readerSeekable), SeekableRef(readerSeekable)
    {
    }
};

class BufferedReaderSeekableRef : public BufferedReaderRef, public SeekableRef
{
  public:
    template <typename BUFFERED_READER_SEEKABLE>
    BufferedReaderSeekableRef(BUFFERED_READER_SEEKABLE &bufferedReaderSeekable)
        : BufferedReaderRef(bufferedReaderSeekable), SeekableRef(bufferedReaderSeekable)
    {
    }
};

class ReaderWriterSeekableRef : public ReaderWriterRef, public SeekableRef
{
  public:
    template <typename READER_WRITER_SEEKABLE>
    ReaderWriterSeekableRef(READER_WRITER_SEEKABLE &readerWriterSeekable)
        : ReaderWriterRef(readerWriterSeekable), SeekableRef(readerWriterSeekable)
    {
    }
};

class BufferedReaderWriterSeekableRef : public BufferedReaderWriterRef, public SeekableRef
{
  public:
    template <typename BUFFERED_READER_WRITER_SEEKABLE>
    BufferedReaderWriterSeekableRef(BUFFERED_READER_WRITER_SEEKABLE &bufferedReaderWriterSeekable)
        : BufferedReaderWriterRef(bufferedReaderWriterSeekable), SeekableRef(bufferedReaderWriterSeekable)
    {
    }
};

} // namespace LexIO

#endif
