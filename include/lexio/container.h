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
// Stream abstraction that wraps STL-like types.
//

#ifndef LEXIO_CONTAINER_H
#define LEXIO_CONTAINER_H

#include "./core.h"

#include <vector> // Used by StdBufReader

namespace LexIO
{

/**
 * @brief A seekable reader/writer that wraps a type that conforms to STL
 *        container methods.
 *
 * @details This class is very inconvenient to construct, prefer one of
 *          the other Container types.
 *
 * @tparam T Type to wrap.
 */
template <typename T>
class ContainerBase
{
  private:
    T m_container;
    size_t m_offset = 0;

  protected:
    T &Container() { return m_container; }
    size_t &Offset() { return m_offset; }

    void OffsetCheck(const ptrdiff_t offset)
    {
        if (offset < 0)
        {
            throw std::runtime_error("attempted seek to negative position");
        }
    }

  public:
    ContainerBase() = delete;
    ContainerBase(const T &buffer) : m_container(buffer) {}
    ContainerBase(T &&buffer) : m_container(buffer) {}

    /**
     * @brief A read-only reference of the wrapped container.
     */
    const T &Container() const { return m_container; }

    virtual size_t RawRead(ByteSpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_container.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(m_container.begin() + m_offset, m_container.begin() + m_offset + actualLength, buffer.begin());
        m_offset += actualLength;
        return actualLength;
    }

    virtual void Flush() {}

    virtual size_t RawWrite(ConstByteSpanT buffer)
    {
        const size_t wantedOffset = m_offset + buffer.size();
        const size_t destOffset = std::min(wantedOffset, m_container.size());
        const size_t actualLength = destOffset - m_offset;
        std::copy(buffer.begin(), buffer.begin() + actualLength, m_container.begin() + m_offset);
        m_offset += actualLength;
        return actualLength;
    }

    virtual size_t Seek(const WhenceStart whence)
    {
        OffsetCheck(whence.offset);
        m_offset = static_cast<size_t>(whence.offset);
        return m_offset;
    }

    virtual size_t Seek(const WhenceCurrent whence)
    {
        const ptrdiff_t offset = static_cast<ptrdiff_t>(m_offset) + whence.offset;
        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }

    virtual size_t Seek(const WhenceEnd whence)
    {
        const ptrdiff_t offset = static_cast<ptrdiff_t>(m_container.size()) - whence.offset;
        OffsetCheck(offset);
        m_offset = static_cast<size_t>(offset);
        return m_offset;
    }
};

/**
 * @brief Wraps a container that has a compile-time known size.
 *
 * @detail Useful for types like std::array.
 */
template <typename T>
class ContainerFixed : public ContainerBase<T>
{
  public:
    explicit ContainerFixed() : ContainerBase<T>(T()) {}
};

/**
 * @brief Wraps a dynamic container that does not grow if we write off the
 *        end.
 *
 * @detail Useful for types like std::vector if you don't want them to grow.
 */
template <typename T>
class ContainerStatic : public ContainerBase<T>
{
  public:
    explicit ContainerStatic(const size_t size) : ContainerBase<T>(T()) { this->Container().resize(size); }
};

/**
 * @brief Wraps a dynamic container that grows if we write off the end.
 *
 * @detail Useful for types like std::vector if you want them to grow.
 */
template <typename T>
class ContainerDynamic : public ContainerBase<T>
{
  public:
    /**
     * @brief Constructs an empty ContainerDynamic.
     */
    ContainerDynamic() : ContainerBase<T>(T()) {}

    /**
     * @brief Constructs a ContainerDynamic with a pre-allocated size.
     *
     * @param size Size of underlying type.
     */
    explicit ContainerDynamic(const size_t size) : ContainerBase<T>(T()) { this->Container().resize(size); }

    /**
     * @brief Construct a ContainerDynamic with data already written into the buffer.
     *
     * @param list Initializer list of data.
     */
    ContainerDynamic(std::initializer_list<uint8_t> list) : ContainerBase<T>(T())
    {
        this->Container().resize(list.size());
        std::copy(list.begin(), list.end(), this->Container().begin());
    }

    size_t RawWrite(ConstByteSpanT buffer) override
    {
        // Writes off the end of the burffer grow the buffer to fit.
        const size_t wantedOffset = this->Offset() + buffer.size();
        this->Container().resize(std::max(wantedOffset, this->Container().size()));
        std::copy(buffer.begin(), buffer.end(), this->Container().begin() + this->Offset());
        this->Offset() += buffer.size();
        return buffer.size();
    }
};

} // namespace LexIO

#endif
