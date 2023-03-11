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

#include "test_nonstd.h"

#include "lexio.h"

//------------------------------------------------------------------------------

struct GoodReader
{
    size_t RawRead(LexIO::ByteSpanT buffer)
    {
        (void)buffer;
        return 0;
    }
};

static_assert(LexIO::IsReader<GoodReader>::value, "GoodReader does not fulfill IsReader");
static_assert(LexIO::IsReaderV<GoodReader>, "GoodReader does not fulfill IsReaderV");

//------------------------------------------------------------------------------

struct GoodBufferedReader : public GoodReader
{
    size_t GetBufferSize() const { return 0; }
    LexIO::ConstByteSpanT FillBuffer(const size_t size)
    {
        (void)size;
        return LexIO::ConstByteSpanT();
    }
    void ConsumeBuffer(const size_t size) { (void)size; }
};

static_assert(LexIO::IsBufferedReader<GoodBufferedReader>::value,
              "GoodBufferedReader does not fulfill IsBufferedReader");
static_assert(LexIO::IsBufferedReaderV<GoodBufferedReader>, "GoodBufferedReader does not fulfill IsBufferedReaderV");

//------------------------------------------------------------------------------

struct GoodWriter
{
    size_t RawWrite(LexIO::ConstByteSpanT buffer)
    {
        (void)buffer;
        return 0;
    }
    void Flush() {}
};

static_assert(LexIO::IsWriter<GoodWriter>::value, "GoodWriter does not fulfill IsWriter");
static_assert(LexIO::IsWriterV<GoodWriter>, "GoodWriter does not fulfill IsWriterV");

//------------------------------------------------------------------------------

struct GoodSeekable
{
    size_t Seek(const LexIO::WhenceStart whence)
    {
        (void)whence;
        return 0;
    }
    size_t Seek(const LexIO::WhenceCurrent whence)
    {
        (void)whence;
        return 0;
    }
    size_t Seek(const LexIO::WhenceEnd whence)
    {
        (void)whence;
        return 0;
    }
};

static_assert(LexIO::IsSeekable<GoodSeekable>::value, "GoodSeekable does not fulfill IsSeekable");
static_assert(LexIO::IsSeekableV<GoodSeekable>, "GoodSeekable does not fulfill IsSeekableV");

//------------------------------------------------------------------------------

struct BadReaderMissingClass
{
};

static_assert(!LexIO::IsReaderV<BadReaderMissingClass>, "BadReaderMissingClass incorrectly fulfills IsReaderV");

struct BadReaderBadParam
{
    size_t RawRead(uint8_t *&buffer, const size_t size)
    {
        (void)buffer;
        (void)size;
        return 0;
    }
};

static_assert(!LexIO::IsReaderV<BadReaderBadParam>, "BadReaderBadParam incorrectly fulfills IsReaderV");

struct BadReaderBadReturn
{
    void RawRead(LexIO::ByteSpanT buffer) { (void)buffer; }
};

static_assert(!LexIO::IsReaderV<BadReaderBadReturn>, "BadReaderBadReturn incorrectly fulfills IsReaderV");
