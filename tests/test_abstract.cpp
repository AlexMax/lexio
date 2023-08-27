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

#include "./test.h"
#include "doctest.h"

#include <memory>

TEST_SUITE_BEGIN("abstract");

//******************************************************************************

TEST_CASE("Test LexIO::MakeAbstractR")
{
    struct GoodR
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodR{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractR(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_THROWS(abstr->GetBufferSize());
    CHECK_THROWS(abstr->FillBuffer(0));
    CHECK_THROWS(abstr->ConsumeBuffer(0));
    CHECK_THROWS(abstr->RawWrite(span));
    CHECK_THROWS(abstr->Flush());
    CHECK_THROWS(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractBR")
{
    struct GoodBR
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t GetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT FillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void ConsumeBuffer(const size_t) {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBR{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBR(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_NOTHROW(abstr->GetBufferSize());
    CHECK_NOTHROW(abstr->FillBuffer(0));
    CHECK_NOTHROW(abstr->ConsumeBuffer(0));
    CHECK_THROWS(abstr->RawWrite(span));
    CHECK_THROWS(abstr->Flush());
    CHECK_THROWS(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractW")
{
    struct GoodW
    {
        size_t RawWrite(LexIO::ConstByteSpanT) { return 0; }
        void Flush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractW(&test));

    CHECK_THROWS(abstr->RawRead(span));
    CHECK_THROWS(abstr->GetBufferSize());
    CHECK_THROWS(abstr->FillBuffer(0));
    CHECK_THROWS(abstr->ConsumeBuffer(0));
    CHECK_NOTHROW(abstr->RawWrite(span));
    CHECK_NOTHROW(abstr->Flush());
    CHECK_THROWS(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractRW")
{
    struct GoodRW
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t RawWrite(LexIO::ConstByteSpanT) { return 0; }
        void Flush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRW(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_THROWS(abstr->GetBufferSize());
    CHECK_THROWS(abstr->FillBuffer(0));
    CHECK_THROWS(abstr->ConsumeBuffer(0));
    CHECK_NOTHROW(abstr->RawWrite(span));
    CHECK_NOTHROW(abstr->Flush());
    CHECK_THROWS(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractBRW")
{
    struct GoodBRW
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t GetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT FillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void ConsumeBuffer(const size_t) {}
        size_t RawWrite(LexIO::ConstByteSpanT) { return 0; }
        void Flush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRW(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_NOTHROW(abstr->GetBufferSize());
    CHECK_NOTHROW(abstr->FillBuffer(0));
    CHECK_NOTHROW(abstr->ConsumeBuffer(0));
    CHECK_NOTHROW(abstr->RawWrite(span));
    CHECK_NOTHROW(abstr->Flush());
    CHECK_THROWS(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_THROWS(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractRS")
{
    struct GoodRS
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t Seek(const LexIO::WhenceStart) { return 0; }
        size_t Seek(const LexIO::WhenceCurrent) { return 0; }
        size_t Seek(const LexIO::WhenceEnd) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRS(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_THROWS(abstr->GetBufferSize());
    CHECK_THROWS(abstr->FillBuffer(0));
    CHECK_THROWS(abstr->ConsumeBuffer(0));
    CHECK_THROWS(abstr->RawWrite(span));
    CHECK_THROWS(abstr->Flush());
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractBRS")
{
    struct GoodBRS
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t GetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT FillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void ConsumeBuffer(const size_t) {}
        size_t Seek(const LexIO::WhenceStart) { return 0; }
        size_t Seek(const LexIO::WhenceCurrent) { return 0; }
        size_t Seek(const LexIO::WhenceEnd) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRS(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_NOTHROW(abstr->GetBufferSize());
    CHECK_NOTHROW(abstr->FillBuffer(0));
    CHECK_NOTHROW(abstr->ConsumeBuffer(0));
    CHECK_THROWS(abstr->RawWrite(span));
    CHECK_THROWS(abstr->Flush());
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractRWS")
{
    struct GoodRWS
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t RawWrite(LexIO::ConstByteSpanT) { return 0; }
        void Flush() {}
        size_t Seek(const LexIO::WhenceStart) { return 0; }
        size_t Seek(const LexIO::WhenceCurrent) { return 0; }
        size_t Seek(const LexIO::WhenceEnd) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRWS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRWS(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_THROWS(abstr->GetBufferSize());
    CHECK_THROWS(abstr->FillBuffer(0));
    CHECK_THROWS(abstr->ConsumeBuffer(0));
    CHECK_NOTHROW(abstr->RawWrite(span));
    CHECK_NOTHROW(abstr->Flush());
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_CASE("Test LexIO::MakeAbstractBRWS")
{
    struct GoodBRWS
    {
        size_t RawRead(LexIO::ByteSpanT) { return 0; }
        size_t GetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT FillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void ConsumeBuffer(const size_t) {}
        size_t RawWrite(LexIO::ConstByteSpanT) { return 0; }
        void Flush() {}
        size_t Seek(const LexIO::WhenceStart) { return 0; }
        size_t Seek(const LexIO::WhenceCurrent) { return 0; }
        size_t Seek(const LexIO::WhenceEnd) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRWS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRWS(&test));

    CHECK_NOTHROW(abstr->RawRead(span));
    CHECK_NOTHROW(abstr->GetBufferSize());
    CHECK_NOTHROW(abstr->FillBuffer(0));
    CHECK_NOTHROW(abstr->ConsumeBuffer(0));
    CHECK_NOTHROW(abstr->RawWrite(span));
    CHECK_NOTHROW(abstr->Flush());
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceStart{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceCurrent{0}));
    CHECK_NOTHROW(abstr->Seek(LexIO::WhenceEnd{0}));
}

TEST_SUITE_END();
