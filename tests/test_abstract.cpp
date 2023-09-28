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
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodR{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractR(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_THROWS(LexIO::GetBufferSize(*abstr));
    CHECK_THROWS(LexIO::FillBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::Write(*abstr, span));
    CHECK_THROWS(LexIO::Flush(*abstr));
    CHECK_THROWS(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractBR")
{
    struct GoodBR
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexGetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT LexFillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void LexConsumeBuffer(const size_t) {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBR{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBR(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_NOTHROW(LexIO::GetBufferSize(*abstr));
    CHECK_NOTHROW(LexIO::FillBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::Write(*abstr, span));
    CHECK_THROWS(LexIO::Flush(*abstr));
    CHECK_THROWS(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractW")
{
    struct GoodW
    {
        size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
        void LexFlush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractW(&test));

    CHECK_THROWS(LexIO::Read(span, *abstr));
    CHECK_THROWS(LexIO::GetBufferSize(*abstr));
    CHECK_THROWS(LexIO::FillBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::Write(*abstr, span));
    CHECK_NOTHROW(LexIO::Flush(*abstr));
    CHECK_THROWS(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractRW")
{
    struct GoodRW
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
        void LexFlush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRW(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_THROWS(LexIO::GetBufferSize(*abstr));
    CHECK_THROWS(LexIO::FillBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::Write(*abstr, span));
    CHECK_NOTHROW(LexIO::Flush(*abstr));
    CHECK_THROWS(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractBRW")
{
    struct GoodBRW
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexGetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT LexFillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
        void LexFlush() {}
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRW{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRW(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_NOTHROW(LexIO::GetBufferSize(*abstr));
    CHECK_NOTHROW(LexIO::FillBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::Write(*abstr, span));
    CHECK_NOTHROW(LexIO::Flush(*abstr));
    CHECK_THROWS(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractRS")
{
    struct GoodRS
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRS(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_THROWS(LexIO::GetBufferSize(*abstr));
    CHECK_THROWS(LexIO::FillBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::Write(*abstr, span));
    CHECK_THROWS(LexIO::Flush(*abstr));
    CHECK_NOTHROW(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractBRS")
{
    struct GoodBRS
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexGetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT LexFillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRS(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_NOTHROW(LexIO::GetBufferSize(*abstr));
    CHECK_NOTHROW(LexIO::FillBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::Write(*abstr, span));
    CHECK_THROWS(LexIO::Flush(*abstr));
    CHECK_NOTHROW(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractRWS")
{
    struct GoodRWS
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
        void LexFlush() {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodRWS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractRWS(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_THROWS(LexIO::GetBufferSize(*abstr));
    CHECK_THROWS(LexIO::FillBuffer(*abstr, 0));
    CHECK_THROWS(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::Write(*abstr, span));
    CHECK_NOTHROW(LexIO::Flush(*abstr));
    CHECK_NOTHROW(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_CASE("Test LexIO::MakeAbstractBRWS")
{
    struct GoodBRWS
    {
        size_t LexRead(LexIO::ByteSpanT) { return 0; }
        size_t LexGetBufferSize() const { return 0; }
        LexIO::ConstByteSpanT LexFillBuffer(const size_t) { return LexIO::ConstByteSpanT{}; }
        void LexConsumeBuffer(const size_t) {}
        size_t LexWrite(LexIO::ConstByteSpanT) { return 0; }
        void LexFlush() {}
        size_t LexSeek(const LexIO::SeekPos) { return 0; }
    };

    LexIO::ByteSpanT span{};
    auto test = GoodBRWS{};
    std::unique_ptr<LexIO::Abstract> abstr(LexIO::MakeAbstractBRWS(&test));

    CHECK_NOTHROW(LexIO::Read(span, *abstr));
    CHECK_NOTHROW(LexIO::GetBufferSize(*abstr));
    CHECK_NOTHROW(LexIO::FillBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::ConsumeBuffer(*abstr, 0));
    CHECK_NOTHROW(LexIO::Write(*abstr, span));
    CHECK_NOTHROW(LexIO::Flush(*abstr));
    CHECK_NOTHROW(LexIO::Seek(*abstr, LexIO::SeekPos{}));
}

TEST_SUITE_END();
