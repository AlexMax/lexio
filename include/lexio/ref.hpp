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
} // namespace Detail

class ReaderRef
{
  protected:
    void *m_ptr;
    Detail::lexRead_t m_lexRead;

    ReaderRef(void *ptr, Detail::lexRead_t read) : m_ptr(ptr), m_lexRead(read) {}

  public:
    template <typename READER>
    ReaderRef(READER &reader)
        : m_ptr(&reader), m_lexRead([](void *ptr, uint8_t *outDest, const size_t count) -> size_t {
              return static_cast<READER *>(ptr)->LexRead(outDest, count);
          })
    {
    }

    size_t LexRead(uint8_t *outDest, const size_t count) const { return m_lexRead(m_ptr, outDest, count); }
};

class BufferedReaderRef : public ReaderRef
{
  protected:
    void *m_ptr;
    Detail::lexFillBuffer_t m_lexFillBuffer;
    Detail::lexConsumeBuffer_t m_lexConsumeBuffer;

    BufferedReaderRef(void *ptr, Detail::lexRead_t lexRead, Detail::lexFillBuffer_t lexFillBuffer,
                      Detail::lexConsumeBuffer_t lexConsumeBuffer)
        : ReaderRef(ptr, lexRead), m_ptr(ptr), m_lexFillBuffer(lexFillBuffer), m_lexConsumeBuffer(lexConsumeBuffer)
    {
    }

  public:
    template <typename BUFFERED_READER>
    BufferedReaderRef(BUFFERED_READER &bufReader)
        : ReaderRef(bufReader), m_ptr(&bufReader), m_lexFillBuffer([](void *ptr, const size_t size) -> BufferView { //
              return static_cast<BUFFERED_READER *>(ptr)->LexFillBuffer(size);
          }),
          m_lexConsumeBuffer([](void *ptr, const size_t size) -> void { //
              static_cast<BUFFERED_READER *>(ptr)->LexConsumeBuffer(size);
          })
    {
    }

    BufferView LexFillBuffer(const size_t size) const { return m_lexFillBuffer(m_ptr, size); }
    void LexConsumeBuffer(const size_t size) const { m_lexConsumeBuffer(m_ptr, size); }
};

class WriterRef
{
  protected:
    void *m_ptr;
    Detail::lexWrite_t m_lexWrite;
    Detail::lexFlush_t m_lexFlush;

    WriterRef(void *ptr, Detail::lexWrite_t lexWrite, Detail::lexFlush_t lexFlush)
        : m_ptr(ptr), m_lexWrite(lexWrite), m_lexFlush(lexFlush)
    {
    }

  public:
    template <typename WRITER>
    WriterRef(WRITER &writer)
        : m_ptr(&writer), m_lexWrite([](void *ptr, const uint8_t *src, const size_t count) -> size_t { //
              return static_cast<WRITER *>(ptr)->LexWrite(src, count);
          }),
          m_lexFlush([](void *ptr) -> void { //
              static_cast<WRITER *>(ptr)->LexFlush();
          })
    {
    }

    size_t LexWrite(const uint8_t *src, const size_t count) const { return m_lexWrite(m_ptr, src, count); }
    void LexFlush() const { m_lexFlush(m_ptr); }
};

class SeekableRef
{
  protected:
    void *m_ptr;
    Detail::lexSeek_t m_lexSeek;

    SeekableRef(void *ptr, Detail::lexSeek_t lexSeek) : m_ptr(ptr), m_lexSeek(lexSeek) {}

  public:
    template <typename SEEKABLE>
    SeekableRef(SEEKABLE &seekable)
        : m_ptr(&seekable), m_lexSeek([](void *ptr, const SeekPos pos) -> size_t { //
              return static_cast<SEEKABLE *>(ptr)->LexSeek(pos);
          })
    {
    }

    size_t LexSeek(const SeekPos pos) const { return m_lexSeek(m_ptr, pos); }
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

    ReaderSeekableRef(void *ptr, Detail::lexRead_t lexRead, Detail::lexSeek_t lexSeek)
        : ReaderRef(ptr, lexRead), SeekableRef(ptr, lexSeek)
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

    operator ReaderSeekableRef() { return ReaderSeekableRef{ReaderRef::m_ptr, m_lexRead, m_lexSeek}; }
};

class ReaderWriterSeekableRef : public ReaderWriterRef, public SeekableRef
{
  public:
    template <typename READER_WRITER_SEEKABLE>
    ReaderWriterSeekableRef(READER_WRITER_SEEKABLE &readerWriterSeekable)
        : ReaderWriterRef(readerWriterSeekable), SeekableRef(readerWriterSeekable)
    {
    }

    operator ReaderSeekableRef() { return ReaderSeekableRef{ReaderRef::m_ptr, m_lexRead, m_lexSeek}; }
};

class BufferedReaderWriterSeekableRef : public BufferedReaderWriterRef, public SeekableRef
{
  public:
    template <typename BUFFERED_READER_WRITER_SEEKABLE>
    BufferedReaderWriterSeekableRef(BUFFERED_READER_WRITER_SEEKABLE &bufferedReaderWriterSeekable)
        : BufferedReaderWriterRef(bufferedReaderWriterSeekable), SeekableRef(bufferedReaderWriterSeekable)
    {
    }

    operator ReaderSeekableRef() { return ReaderSeekableRef{ReaderRef::m_ptr, m_lexRead, m_lexSeek}; }
};

} // namespace LexIO
