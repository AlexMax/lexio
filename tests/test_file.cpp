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

#include "lexio/stream/file.hpp"

#include "./test.h"

//******************************************************************************

#if defined(_WIN32)

static std::string WideToUTF8(const wchar_t *buffer, int len)
{
    int bytes = WideCharToMultiByte(CP_UTF8, 0, buffer, len, NULL, 0, NULL, NULL);
    if (bytes == 0)
    {
        throw std::runtime_error("could not convert to utf8 string");
    }

    std::string str;
    str.resize(size_t(bytes) - 1);
    bytes = WideCharToMultiByte(CP_UTF8, 0, buffer, len, str.data(), bytes, NULL, NULL);
    if (bytes == 0)
    {
        throw std::runtime_error("could not convert to utf8 string");
    }

    return str;
}

static std::wstring UTF8ToWide(const std::string &str)
{
    // Request buffer size.
    int bytes = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (bytes == 0)
    {
        throw std::runtime_error("could not convert to wide string");
    }

    // Stuff filename into wide string.
    std::wstring wstr;
    wstr.resize(size_t(bytes) - 1);
    const int actual = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), bytes);
    if (actual == 0)
    {
        throw std::runtime_error("could not convert to wide string");
    }

    return wstr;
}

static std::string TempFile()
{
    WCHAR wTemp[MAX_PATH + 1] = {0};
    DWORD wSize = GetTempPathW(MAX_PATH + 1, &wTemp[0]);
    if (wSize == 0)
    {
        throw std::runtime_error("could not get temporary name");
    }

    WCHAR wfilename[MAX_PATH] = {0};
    UINT ok = GetTempFileNameW(&wTemp[0], L"lex", 0, &wfilename[0]);
    if (ok == 0)
    {
        throw std::runtime_error("could not get temporary name");
    }

    return WideToUTF8(&wfilename[0], -1);
}

class ScopeDelete
{
    std::string m_filename;

  public:
    ScopeDelete(const std::string &filename) : m_filename(filename) {}
    ~ScopeDelete()
    {
        std::wstring wfilename = UTF8ToWide(m_filename);
        if (DeleteFileW(wfilename.c_str()) == 0)
        {
            std::terminate();
        }
    }
};

#else

#endif

TEST(File, FulfillReader)
{
    EXPECT_TRUE(LexIO::IsReaderV<LexIO::File>);
}

TEST(File, FulfillWriter)
{
    EXPECT_TRUE(LexIO::IsWriterV<LexIO::File>);
}

TEST(File, FulfillSeekable)
{
    EXPECT_TRUE(LexIO::IsSeekableV<LexIO::File>);
}

TEST(File, DefaultCtor)
{
    auto file = LexIO::File{};
    EXPECT_EQ(LexIO::File::InvalidFileHandle(), std::move(file).FileHandle());
}

TEST(File, MoveCtor)
{
    auto file = LexIO::Open(LEXIO_TEST_DIR "/test_file.txt", LexIO::OpenMode::read);
    LexIO::File moveFIle{std::move(file)};
}

TEST(File, MoveAssign)
{
    auto file = LexIO::File{};
    file = LexIO::Open(LEXIO_TEST_DIR "/test_file.txt", LexIO::OpenMode::read);
}

TEST(File, Close)
{
    auto file = LexIO::Open(LEXIO_TEST_DIR "/test_file.txt", LexIO::OpenMode::read);
    EXPECT_NO_THROW(file.Close());
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

TEST(File, WriteMode)
{
    std::string filename = TempFile();
    ScopeDelete deleteMe{filename};

    {
        auto file = LexIO::Open(filename.c_str(), LexIO::OpenMode::write);

        // Test reading.
        uint8_t readBuffer[32] = {0x00};
        EXPECT_ANY_THROW(LexIO::Read(readBuffer, file));

        // Test writing.
        EXPECT_EQ(BUFFER_LENGTH, LexIO::Write(file, BUFFER_TEXT, BUFFER_LENGTH));
        EXPECT_NO_THROW(LexIO::Flush(file));

        // Test seeking.
        size_t pos = LexIO::Seek(file, 2, LexIO::Whence::start);
        EXPECT_EQ(pos, 2);

        pos = LexIO::Seek(file, 6, LexIO::Whence::current);
        EXPECT_EQ(pos, 8);

        pos = LexIO::Seek(file, 2, LexIO::Whence::end);
        EXPECT_EQ(pos, 47);
    }
}

#if defined(_WIN32)

TEST(File, Error)
{
    auto error = LexIO::Win32Error{"File not found", ERROR_FILE_NOT_FOUND};
    EXPECT_EQ(ERROR_FILE_NOT_FOUND, error.GetError());
}

#else

TEST(File, Error)
{
    auto error = LexIO::POSIXError{"File not found", ENOENT};
    EXPECT_EQ(ENOENT, error.GetError());
}

#endif
