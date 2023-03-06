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

namespace LexIO
{

enum class LOpenMode
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

class LFileWin32
{
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;

    LFileWin32(const HANDLE fileHandle) : m_fileHandle(fileHandle) {}

  public:
    LFileWin32(const LFileWin32 &other) = delete;
    LFileWin32 &operator=(const LFileWin32 &other) = delete;

    LFileWin32(LFileWin32 &&other) noexcept
    {
        m_fileHandle = other.m_fileHandle;
        other.m_fileHandle = INVALID_HANDLE_VALUE;
    }

    LFileWin32 &operator=(LFileWin32 &&other) noexcept
    {
        m_fileHandle = other.m_fileHandle;
        other.m_fileHandle = INVALID_HANDLE_VALUE;
        return *this;
    }

    /**
     * @brief Destructor closes file handle with no error handling.
     */
    ~LFileWin32()
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
     * @return A constructed LFileWin32 object.
     * @throws Win32Error if error was encountered.
     */
    static LFileWin32 Open(const char *path, const DWORD desiredAccess, const DWORD shareMode,
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

        return LFileWin32(fileHandle);
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

    size_t RawRead(SpanT buffer)
    {
        DWORD bytesToRead = static_cast<DWORD>(buffer.size());
        DWORD bytesRead = 0;
        const BOOL ok = ReadFile(m_fileHandle, buffer.data(), bytesToRead, &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not read file.", GetLastError());
        }
        return bytesRead;
    }

    size_t RawWrite(ConstSpanT buffer)
    {
        DWORD bytesToRead = static_cast<DWORD>(buffer.size());
        DWORD bytesRead = 0;
        const BOOL ok = WriteFile(m_fileHandle, buffer.data(), bytesToRead, &bytesRead, NULL);
        if (ok == FALSE)
        {
            throw Win32Error("Could not write file.", GetLastError());
        }
        return bytesRead;
    }

    void Flush()
    {
        const BOOL ok = FlushFileBuffers(m_fileHandle);
        if (ok == FALSE)
        {
            throw Win32Error("Could not flush file.", GetLastError());
        }
    }

    size_t Seek(const WhenceStart whence)
    {
        LARGE_INTEGER offset, newOffset;
        offset.QuadPart = whence.offset;
        const BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_BEGIN);
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
        const BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_CURRENT);
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
        const BOOL ok = SetFilePointerEx(m_fileHandle, offset, &newOffset, FILE_END);
        if (ok == 0)
        {
            throw Win32Error("Could not seek file.", GetLastError());
        }
        return static_cast<size_t>(newOffset.QuadPart);
    }
};

static_assert(IsReaderV<LFileWin32>, "LFileWin32 is not a Reader");
static_assert(IsWriterV<LFileWin32>, "LFileWin32 is not a Writer");
static_assert(IsSeekableV<LFileWin32>, "LFileWin32 is not a Seekable");

using LFile = LFileWin32;

inline LFile LOpen(const char *path, const LOpenMode mode)
{
    switch (mode)
    {
    case LOpenMode::read:
        return LFile::Open(path, GENERIC_READ, 0, OPEN_EXISTING);
    case LOpenMode::write:
        return LFile::Open(path, GENERIC_WRITE, 0, CREATE_ALWAYS);
    case LOpenMode::append:
        return LFile::Open(path, GENERIC_WRITE, 0, OPEN_ALWAYS);
    case LOpenMode::readPlus:
        return LFile::Open(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING);
    case LOpenMode::writePlus:
        return LFile::Open(path, GENERIC_READ | GENERIC_WRITE, 0, CREATE_ALWAYS);
    case LOpenMode::appendPlus:
        return LFile::Open(path, GENERIC_READ | GENERIC_WRITE, 0, OPEN_ALWAYS);
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

class LFilePOSIX
{
    int m_fd = -1;
    LFilePOSIX(const int fd) : m_fd(fd) {}

  public:
    LFilePOSIX(const LFilePOSIX &other) = delete;
    LFilePOSIX &operator=(const LFilePOSIX &other) = delete;
    LFilePOSIX(LFilePOSIX &&other) noexcept = default;
    LFilePOSIX &operator=(LFilePOSIX &&other) noexcept = delete;

    /**
     * @brief Destructor closes file handle with no error handling.
     */
    ~LFilePOSIX() { close(m_fd); }

    /**
     * @brief Open a file for reading.
     *
     * @param path Path to filename, assumed to be a null-terminated string.
     * @param flags flags to pass to open(2) call.
     * @param mode mode to pass to open(2) call.
     * @return A constructed LFilePOSIX object.
     * @throws POSIXError if error was encountered.
     */
    static LFilePOSIX Open(const char *path, const int flags, mode_t mode)
    {
        const int fd = open(path, flags, mode);
        if (fd == -1)
        {
            throw POSIXError("Could not open file.", errno);
        }

        return LFilePOSIX(fd);
    }

    /**
     * @brief Close the file.
     */
    void Close()
    {
        const int ok = close(m_fd);
        if (ok == -1)
        {
            throw POSIXError("Could not close file.", errno);
        }
    }

    size_t RawRead(SpanT buffer)
    {
        const ssize_t bytesRead = read(m_fd, buffer.data(), buffer.size());
        if (bytesRead == -1)
        {
            throw POSIXError("Could not read file.", errno);
        }
        return static_cast<size_t>(bytesRead);
    }

    size_t RawWrite(ConstSpanT buffer)
    {
        const ssize_t bytesRead = write(m_fd, buffer.data(), buffer.size());
        if (bytesRead == -1)
        {
            throw POSIXError("Could not write file.", errno);
        }
        return static_cast<size_t>(bytesRead);
    }

    void Flush()
    {
        const int ok = fsync(m_fd);
        if (ok == -1)
        {
            throw POSIXError("Could not flush file.", errno);
        }
    }

    size_t Seek(const WhenceStart whence)
    {
        const off_t newOffset = lseek(m_fd, static_cast<off_t>(whence.offset), SEEK_SET);
        if (newOffset == -1)
        {
            throw POSIXError("Could not seek file.", errno);
        }
        return static_cast<size_t>(newOffset);
    }

    size_t Seek(const WhenceCurrent whence)
    {
        const off_t newOffset = lseek(m_fd, static_cast<off_t>(whence.offset), SEEK_CUR);
        if (newOffset == -1)
        {
            throw POSIXError("Could not seek file.", errno);
        }
        return static_cast<size_t>(newOffset);
    }

    size_t Seek(const WhenceEnd whence)
    {
        const off_t newOffset = lseek(m_fd, static_cast<off_t>(whence.offset), SEEK_END);
        if (newOffset == -1)
        {
            throw POSIXError("Could not seek file.", errno);
        }
        return static_cast<size_t>(newOffset);
    }
};

static_assert(IsReaderV<LFilePOSIX>, "LFilePOSIX is not a Reader");
static_assert(IsWriterV<LFilePOSIX>, "LFilePOSIX is not a Writer");
static_assert(IsSeekableV<LFilePOSIX>, "LFilePOSIX is not a Seekable");

using LFile = LFilePOSIX;

inline LFile LOpen(const char *path, const LOpenMode mode)
{
    switch (mode)
    {
    case LOpenMode::read:
        return LFile::Open(path, O_RDONLY, 0666);
    case LOpenMode::write:
        return LFile::Open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    case LOpenMode::append:
        return LFile::Open(path, O_WRONLY | O_CREAT, 0666);
    case LOpenMode::readPlus:
        return LFile::Open(path, O_RDWR, 0666);
    case LOpenMode::writePlus:
        return LFile::Open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
    case LOpenMode::appendPlus:
        return LFile::Open(path, O_RDWR | O_CREAT, 0666);
    default:
        throw std::runtime_error("Unknown open mode type.");
    }
}

} // namespace LexIO

#endif
