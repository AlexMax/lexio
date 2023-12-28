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

//******************************************************************************

#if defined(_WIN32)
using FileGeneric = LexIO::FileWin32;
#else
using FileGeneric = LexIO::FilePOSIX;
#endif

TEST(File, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<FileGeneric>);
}

TEST(File, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<FileGeneric>);
}

TEST(File, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<FileGeneric>);
}

TEST(File, ReadMode)
{
    constexpr const char *firstLine = "The quick brown fox";

    auto file = LexIO::Open(LEXIO_TEST_DIR "/test_file.txt", LexIO::OpenMode::read);

    // Test reading.
    uint8_t readBuffer[32];
    LexIO::Read(readBuffer, file);
    size_t testLen = strlen(firstLine);
    readBuffer[testLen] = '\0';                                                      // NOLINT
    EXPECT_EQ(0, strcmp(reinterpret_cast<const char *>(&readBuffer[0]), firstLine)); // NOLINT

    // Test writing.
    uint8_t writeBuffer[32] = {0x00};
    EXPECT_ANY_THROW(LexIO::Write(file, writeBuffer));

    // Test seeking.
    size_t pos = LexIO::Seek(file, 2, LexIO::Whence::start);
    EXPECT_EQ(pos, 2);

    pos = LexIO::Seek(file, 6, LexIO::Whence::current);
    EXPECT_EQ(pos, 8);

    pos = LexIO::Seek(file, 2, LexIO::Whence::end);

#if defined(_WIN32)
    EXPECT_EQ(pos, 49);
#else
    EXPECT_EQ(pos, 47);
#endif
}
