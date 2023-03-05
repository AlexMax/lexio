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

//
// Stream abstraction that wraps basic file input and output.
//

#include "lexio_core.h"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace LexIO
{

class Win32Error : public std::runtime_error
{
    DWORD m_error = 0;

  public:
    Win32Error(const char *what, DWORD error) : std::runtime_error(what), m_error(error) {}
    DWORD GetError() const noexcept { return m_error; }
};

class LFileWin32
{
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;

    LFileWin32(const HANDLE fileHandle) : m_fileHandle(fileHandle) {}

  public:
    LFileWin32(const LFileWin32 &other) = delete;
    LFileWin32 &operator=(const LFileWin32 &other) = delete;
    LFileWin32(LFileWin32 &&other) noexcept = default;
    LFileWin32 &operator=(LFileWin32 &&other) noexcept = delete;

    /**
     * @brief Destructor closes file handle with no error handling.
     */
    ~LFileWin32() { CloseHandle(m_fileHandle); }

    /**
     * @brief Open a file for reading.
     *
     * @param path Path to filename, assumed to be a null-terminated UTF-8 string.
     */
    static LFileWin32 Open(const char *path)
    {
        // Request buffer size.
        const int wanted = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
        if (wanted == 0)
        {
            throw Win32Error("Could not open file.", GetLastError());
        }

        // Stuff filename into wide string.
        std::wstring wpath;
        wpath.resize(wanted - 1);
        const int actual = MultiByteToWideChar(CP_UTF8, 0, path, -1, &wpath[0], wanted);
        if (actual == 0)
        {
            throw Win32Error("Could not open file.", GetLastError());
        }

        // Open the file.
        const HANDLE fileHandle =
            CreateFileW(wpath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            throw Win32Error("Could not open file.", GetLastError());
        }

        return LFileWin32(fileHandle);
    }

    /**
     * @brief Close the file.
     */
    void Close()
    {
        BOOL ok = CloseHandle(m_fileHandle);
        if (ok == FALSE)
        {
            throw Win32Error("Could not close file.", GetLastError());
        }
    }

    size_t RawRead(SpanT buffer)
    {
        DWORD bytesRead = 0;
        BOOL ok = ReadFile(m_fileHandle, buffer.data(), buffer.size(), &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not read file.", GetLastError());
        }
        return bytesRead;
    }

    size_t RawWrite(ConstSpanT buffer)
    {
        DWORD bytesRead = 0;
        BOOL ok = WriteFile(m_fileHandle, buffer.data(), buffer.size(), &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not write file.", GetLastError());
        }
        return bytesRead;
    }

    void Flush() { FlushFileBuffers(m_fileHandle); }

    size_t Seek(const WhenceStart whence)
    {
        LARGE_INTEGER offset, newOffset;
        offset.QuadPart = whence.offset;
        BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_BEGIN);
        if (ok == 0)
        {
            throw Win32Error("Could not seek file.", GetLastError());
        }
        return static_cast<size_t>(newOffset.QuadPart);
    }

    size_t Seek(const WhenceCurrent whence)
    {
        LARGE_INTEGER offset, newOffset;
        offset.QuadPart = whence.offset;
        BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_CURRENT);
        if (ok == 0)
        {
            throw Win32Error("Could not seek file.", GetLastError());
        }
        return static_cast<size_t>(newOffset.QuadPart);
    }

    size_t Seek(const WhenceEnd whence)
    {
        LARGE_INTEGER offset, newOffset;
        offset.QuadPart = whence.offset;
        BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_END);
        if (ok == 0)
        {
            throw Win32Error("Could not seek file.", GetLastError());
        }
        return static_cast<size_t>(newOffset.QuadPart);
    }
};

static_assert(IsReaderV<LFileWin32>, "not a Reader");
static_assert(IsWriterV<LFileWin32>, "not a Writer");
static_assert(IsSeekableV<LFileWin32>, "not a Seekable");

} // namespace LexIO

#else

#endif
