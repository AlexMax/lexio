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

#pragma once

#include "../core.hpp"

namespace LexIO
{

enum class OpenMode
{
    // Read only, file must exist.
    read,

    // Write only, file is created or truncated.
    write,

    // Append only, file is created if it does not exist.
    append,

    // Read and Write, file must exist.
    readPlus,

    // Read and Write, file is created or truncated.
    writePlus,

    // Read and Append, file is create if it does not exist.
    appendPlus,
};

} // namespace LexIO

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

class FileWin32
{
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;

    FileWin32(const HANDLE fileHandle) : m_fileHandle(fileHandle) {}

  public:
    FileWin32(const FileWin32 &other) = delete;
    FileWin32 &operator=(const FileWin32 &other) = delete;

    FileWin32(FileWin32 &&other) noexcept
    {
        m_fileHandle = other.m_fileHandle;
        other.m_fileHandle = INVALID_HANDLE_VALUE;
    }

    FileWin32 &operator=(FileWin32 &&other) noexcept
    {
        m_fileHandle = other.m_fileHandle;
        other.m_fileHandle = INVALID_HANDLE_VALUE;
        return *this;
    }

    /**
     * @brief Destructor closes file handle with no error handling.
     */
    ~FileWin32()
    {
        if (m_fileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_fileHandle);
        }
        m_fileHandle = INVALID_HANDLE_VALUE;
    }

    /**
     * @brief Open a file for reading.
     *
     * @param path Path to filename, assumed to be a null-terminated string.
     * @param desiredAccess Desired access to pass to CreateFileW call.
     * @param shareMode Share mode to pass to CreateFileW call.
     * @param creationDisposition Creation disposition to pass to CreateFileW call.
     * @return A constructed FileWin32 object.
     * @throws Win32Error if error was encountered.
     */
    static FileWin32 Open(const char *path, const DWORD desiredAccess, const DWORD shareMode,
                          const DWORD creationDisposition)
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
        const HANDLE fileHandle = CreateFileW(wpath.c_str(), desiredAccess, shareMode, NULL, creationDisposition,
                                              FILE_ATTRIBUTE_NORMAL, NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            throw Win32Error("Could not open file.", GetLastError());
        }

        return FileWin32(fileHandle);
    }

    /**
     * @brief Close the file.
     */
    void Close()
    {
        if (m_fileHandle != INVALID_HANDLE_VALUE)
        {
            const BOOL ok = CloseHandle(m_fileHandle);
            if (ok == FALSE)
            {
                throw Win32Error("Could not close file.", GetLastError());
            }
            m_fileHandle = INVALID_HANDLE_VALUE;
        }
    }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        DWORD bytesToRead = static_cast<DWORD>(count);
        DWORD bytesRead = 0;
        const BOOL ok = ReadFile(m_fileHandle, outDest, bytesToRead, &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not read file.", GetLastError());
        }
        return bytesRead;
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        DWORD bytesToRead = static_cast<DWORD>(count);
        DWORD bytesRead = 0;
        const BOOL ok = WriteFile(m_fileHandle, src, bytesToRead, &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not write file.", GetLastError());
        }
        return bytesRead;
    }

    void LexFlush()
    {
        const BOOL ok = FlushFileBuffers(m_fileHandle);
        if (ok == FALSE)
        {
            throw Win32Error("Could not flush file.", GetLastError());
        }
    }

    size_t LexSeek(const SeekPos pos)
    {
        DWORD whence = 0;
        LARGE_INTEGER offset, newOffset;
        offset.QuadPart = pos.offset;

        switch (pos.whence)
        {
        case LexIO::Whence::start:
            whence = FILE_BEGIN;
            break;
        case LexIO::Whence::current:
            whence = FILE_CURRENT;
            break;
        case LexIO::Whence::end:
            whence = FILE_END;
            break;
        }

        const BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, whence);
        if (ok == 0)
        {
            throw Win32Error("Could not seek file.", GetLastError());
        }
        return static_cast<size_t>(newOffset.QuadPart);
    }
};

static_assert(IsReaderV<FileWin32>, "FileWin32 is not a Reader");
static_assert(IsWriterV<FileWin32>, "FileWin32 is not a Writer");
static_assert(IsSeekableV<FileWin32>, "FileWin32 is not a Seekable");

using File = FileWin32;

inline File Open(const char *path, const OpenMode mode)
{
    switch (mode)
    {
    case OpenMode::read:
        return File::Open(path, GENERIC_READ, 0, OPEN_EXISTING);
    case OpenMode::write:
        return File::Open(path, GENERIC_WRITE, 0, CREATE_ALWAYS);
    case OpenMode::append:
        return File::Open(path, GENERIC_WRITE, 0, OPEN_ALWAYS);
    case OpenMode::readPlus:
        return File::Open(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING);
    case OpenMode::writePlus:
        return File::Open(path, GENERIC_READ | GENERIC_WRITE, 0, CREATE_ALWAYS);
    case OpenMode::appendPlus:
        return File::Open(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_ALWAYS);
    default:
        throw std::runtime_error("Unknown open mode type.");
    }
}

} // namespace LexIO

#else

#include <fcntl.h>
#include <unistd.h>

namespace LexIO
{

class POSIXError : public std::runtime_error
{
    int m_error = 0;

  public:
    POSIXError(const char *what, const int error) : std::runtime_error(what), m_error(error) {}
    int GetError() const noexcept { return m_error; }
};

class FilePOSIX
{
    int m_fd = -1;
    FilePOSIX(const int fd) : m_fd(fd) {}

  public:
    FilePOSIX(const FilePOSIX &other) = delete;
    FilePOSIX &operator=(const FilePOSIX &other) = delete;

    FilePOSIX(FilePOSIX &&other) noexcept : m_fd(other.m_fd) { other.m_fd = -1; }

    FilePOSIX &operator=(FilePOSIX &&other) noexcept
    {
        m_fd = other.m_fd;
        other.m_fd = -1;
        return *this;
    }

    /**
     * @brief Destructor closes file handle with no error handling.
     */
    ~FilePOSIX()
    {
        if (m_fd != -1)
        {
            close(m_fd);
        }
        m_fd = -1;
    }

    /**
     * @brief Open a file for reading.
     *
     * @param path Path to filename, assumed to be a null-terminated string.
     * @param flags flags to pass to open(2) call.
     * @param mode mode to pass to open(2) call.
     * @return A constructed FilePOSIX object.
     * @throws POSIXError if error was encountered.
     */
    static FilePOSIX Open(const char *path, const int flags, mode_t mode)
    {
        const int fd = open(path, flags, mode);
        if (fd == -1)
        {
            throw POSIXError("Could not open file.", errno);
        }

        return FilePOSIX(fd);
    }

    /**
     * @brief Close the file.
     */
    void Close()
    {
        if (m_fd != -1)
        {
            const int ok = close(m_fd);
            if (ok == -1)
            {
                throw POSIXError("Could not close file.", errno);
            }
            m_fd = -1;
        }
    }

    size_t LexRead(uint8_t *outDest, const size_t count)
    {
        const ssize_t bytesRead = read(m_fd, outDest, count);
        if (bytesRead == -1)
        {
            throw POSIXError("Could not read file.", errno);
        }
        return static_cast<size_t>(bytesRead);
    }

    size_t LexWrite(const uint8_t *src, const size_t count)
    {
        const ssize_t bytesRead = write(m_fd, src, count);
        if (bytesRead == -1)
        {
            throw POSIXError("Could not write file.", errno);
        }
        return static_cast<size_t>(bytesRead);
    }

    void LexFlush()
    {
        const int ok = fsync(m_fd);
        if (ok == -1)
        {
            throw POSIXError("Could not flush file.", errno);
        }
    }

    size_t LexSeek(const SeekPos pos)
    {
        int whence = 0;

        switch (pos.whence)
        {
        case LexIO::Whence::start:
            whence = SEEK_SET;
            break;
        case LexIO::Whence::current:
            whence = SEEK_CUR;
            break;
        case LexIO::Whence::end:
            whence = SEEK_END;
            break;
        }

        const off_t newOffset = lseek(m_fd, static_cast<off_t>(pos.offset), whence);
        if (newOffset == -1)
        {
            throw POSIXError("Could not seek file.", errno);
        }
        return static_cast<size_t>(newOffset);
    }
};

static_assert(IsReaderV<FilePOSIX>, "FilePOSIX is not a Reader");
static_assert(IsWriterV<FilePOSIX>, "FilePOSIX is not a Writer");
static_assert(IsSeekableV<FilePOSIX>, "FilePOSIX is not a Seekable");

using File = FilePOSIX;

inline File Open(const char *path, const OpenMode mode)
{
    switch (mode)
    {
    case OpenMode::read:
        return File::Open(path, O_RDONLY, 0666);
    case OpenMode::write:
        return File::Open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    case OpenMode::append:
        return File::Open(path, O_WRONLY | O_CREAT, 0666);
    case OpenMode::readPlus:
        return File::Open(path, O_RDWR, 0666);
    case OpenMode::writePlus:
        return File::Open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
    case OpenMode::appendPlus:
        return File::Open(path, O_RDWR | O_CREAT, 0666);
    default:
        throw std::runtime_error("Unknown open mode type.");
    }
}

} // namespace LexIO

#endif
