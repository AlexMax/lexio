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

#include <utility>

namespace LexIO
{

/**
 * @brief Add buffering to any Writer, with a fixed-length buffer.
 *
 * @tparam WRITER Writer type to wrap.
 */
template <typename WRITER, typename = std::enable_if_t<IsWriterV<WRITER>>>
class FixedBufWriter
{
    static constexpr size_t DEFAULT_ALLOC_SIZE = 8192;

    WRITER m_writer;
    uint8_t *m_buffer = nullptr;
    size_t m_allocSize = DEFAULT_ALLOC_SIZE;
    size_t m_size = 0;

  protected:
    void FlushBuffer()
    {
        size_t offset = 0;
        size_t countLeft = m_size;
        const size_t totalSize = m_size;
        while (offset < totalSize)
        {
            const size_t written = Write(m_writer, m_buffer + offset, countLeft);
            offset += written;
            countLeft -= written;
        }

        m_size = 0;
    }

  public:
    /**
     * @brief Default constructor.
     *
     * @param bufSize Size of write buffer in bytes.
     */
    FixedBufWriter(size_t bufSize = DEFAULT_ALLOC_SIZE) : m_buffer(::new uint8_t[bufSize]), m_allocSize(bufSize) {}

    /**
     * @brief Copy constructor.
     */
    FixedBufWriter(const FixedBufWriter &other)
        : m_writer(other.m_writer), m_buffer(::new uint8_t[other.m_allocSize]), m_allocSize(other.m_allocSize),
          m_size(other.m_size)
    {
        std::memcpy(m_buffer, &other.m_buffer[0], m_size);
    }

    /**
     * @brief Move constructor.
     */
    FixedBufWriter(FixedBufWriter &&other) noexcept
        : m_writer(std::move(other.m_writer)), m_buffer(std::exchange(other.m_buffer, nullptr)),
          m_allocSize(other.m_allocSize), m_size(other.m_size)
    {
    }

    /**
     * @brief Constructor from existing Writer.
     *
     * @param writer Writer to wrap with a buffer.
     * @param bufSize Size of write buffer in bytes.
     */
    FixedBufWriter(WRITER &&writer, size_t bufSize = DEFAULT_ALLOC_SIZE)
        : m_writer(writer), m_buffer(::new uint8_t[bufSize]), m_allocSize(bufSize)
    {
    }

    /**
     * @brief Destructor.
     */
    ~FixedBufWriter()
    {
        if (m_buffer == nullptr)
        {
            // Writer is moved-from, don't operate on it.
            return;
        }

        LexFlush();
        ::delete[] m_buffer;
    }

    /**
     * @brief Copy assignment operator.
     */
    FixedBufWriter &operator=(const FixedBufWriter &other)
    {
        if (this == &other)
        {
            return *this;
        }

        FixedBufWriter copy{other};
        std::swap(m_writer, copy.m_writer);
        std::swap(m_buffer, copy.m_buffer);
        std::swap(m_allocSize, copy.m_allocSize);
        std::swap(m_size, copy.m_size);
        return *this;
    }

    /**
     * @brief Move assignment operator.
     */
    FixedBufWriter &operator=(FixedBufWriter &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ::delete[] m_buffer;
        m_writer = std::move(other.m_writer);
        m_buffer = std::exchange(other.m_buffer, nullptr);
        m_allocSize = other.m_allocSize;
        m_size = other.m_size;
        return *this;
    }

    /**
     * @brief Return underlying Writer.
     */
    const WRITER &Writer() const & { return m_writer; }

    /**
     * @brief Obtain the underlying wrapped Writer while moving-from the
     *        FixedBufWriter.
     */
    WRITER Writer() && { return m_writer; }

    template <typename READER = WRITER, typename = std::enable_if_t<IsReaderV<READER>>>
    size_t LexRead(uint8_t *outDest, size_t count)
    {
        return Read(m_writer, outDest, count);
    }

    template <typename BUFFERED_READER = WRITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
    BufferView LexFillBuffer(size_t count)
    {
        return FillBuffer(m_writer, count);
    }

    template <typename BUFFERED_READER = WRITER, typename = std::enable_if_t<IsBufferedReaderV<BUFFERED_READER>>>
    void LexConsumeBuffer(size_t count)
    {
        ConsumeBuffer(m_writer, count);
    }

    size_t LexWrite(const uint8_t *src, size_t count)
    {
        const size_t wantSize = m_size + count;
        if (wantSize < m_allocSize)
        {
            // Fast path, just append to the buffer.
            std::memcpy(&m_buffer[m_size], src, count);
            m_size = wantSize;
            return count;
        }

        // Flush the current contents of the buffer.
        LexFlush();

        if (count < m_allocSize)
        {
            // Room in the buffer, write to it.
            std::memcpy(&m_buffer[0], src, count);
            m_size = count;
            return count;
        }

        // Write is too large for buffer, pass through.
        return Write(m_writer, src, count);
    }

    void LexFlush()
    {
        FlushBuffer();
        Flush(m_writer);
    }

    template <typename SEEKABLE = WRITER, typename = std::enable_if_t<IsSeekableV<SEEKABLE>>>
    size_t LexSeek(SeekPos pos)
    {
        LexFlush();
        return Seek(m_writer, pos);
    }
};

} // namespace LexIO
