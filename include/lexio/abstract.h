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

#ifndef LEXIO_ABSTRACT_H
#define LEXIO_ABSTRACT_H

#include "./core.h"

namespace LexIO
{

/**
 * @brief An abstract concrete class.  Wraps a specific implemtnation of a
 *        LexIO Reader/Writer/Seekable class in a type-erased struct.
 */
struct Abstract
{
    virtual ~Abstract() {}
    virtual size_t LexRead(ByteSpanT) { throw std::runtime_error("not implemented"); }
    virtual size_t LexGetBufferSize() const { throw std::runtime_error("not implemented"); }
    virtual ConstByteSpanT LexFillBuffer(const size_t) { throw std::runtime_error("not implemented"); }
    virtual void LexConsumeBuffer(const size_t) { throw std::runtime_error("not implemented"); }
    virtual size_t LexWrite(ConstByteSpanT) { throw std::runtime_error("not implemented"); }
    virtual void LexFlush() { throw std::runtime_error("not implemented"); }
    virtual size_t LexSeek(const SeekPos) { throw std::runtime_error("not implemented"); }
};

namespace Detail
{

#define READER_METHODS_                                                                                                \
    virtual size_t LexRead(ByteSpanT buffer) override                                                                  \
    {                                                                                                                  \
        return m_impl->LexRead(buffer);                                                                                \
    }

#define BUFFERED_READER_METHODS_                                                                                       \
    virtual size_t LexGetBufferSize() const override                                                                   \
    {                                                                                                                  \
        return m_impl->LexGetBufferSize();                                                                             \
    }                                                                                                                  \
    virtual ConstByteSpanT LexFillBuffer(const size_t size) override                                                   \
    {                                                                                                                  \
        return m_impl->LexFillBuffer(size);                                                                            \
    }                                                                                                                  \
    virtual void LexConsumeBuffer(const size_t size) override                                                          \
    {                                                                                                                  \
        m_impl->LexConsumeBuffer(size);                                                                                \
    }

#define WRITER_METHODS_                                                                                                \
    virtual size_t LexWrite(ConstByteSpanT buffer) override                                                            \
    {                                                                                                                  \
        return m_impl->LexWrite(buffer);                                                                               \
    }                                                                                                                  \
    virtual void LexFlush() override                                                                                   \
    {                                                                                                                  \
        m_impl->LexFlush();                                                                                            \
    }

#define SEEKABLE_METHODS_                                                                                              \
    virtual size_t LexSeek(const SeekPos pos) override                                                                 \
    {                                                                                                                  \
        return m_impl->LexSeek(pos);                                                                                   \
    }

template <typename T>
class AbstractR final : public Abstract
{
    T *m_impl;

  public:
    AbstractR(T *impl) : m_impl(impl) {}

    READER_METHODS_;
};

template <typename T>
class AbstractBR final : public Abstract
{
    T *m_impl;

  public:
    AbstractBR(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    BUFFERED_READER_METHODS_;
};

template <typename T>
class AbstractW final : public Abstract
{
    T *m_impl;

  public:
    AbstractW(T *impl) : m_impl(impl) {}

    WRITER_METHODS_;
};

template <typename T>
class AbstractRW final : public Abstract
{
    T *m_impl;

  public:
    AbstractRW(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    WRITER_METHODS_;
};

template <typename T>
class AbstractBRW final : public Abstract
{
    T *m_impl;

  public:
    AbstractBRW(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    BUFFERED_READER_METHODS_;
    WRITER_METHODS_;
};

template <typename T>
class AbstractRS final : public Abstract
{
    T *m_impl;

  public:
    AbstractRS(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    SEEKABLE_METHODS_;
};

template <typename T>
class AbstractBRS final : public Abstract
{
    T *m_impl;

  public:
    AbstractBRS(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    BUFFERED_READER_METHODS_;
    SEEKABLE_METHODS_;
};

template <typename T>
class AbstractRWS final : public Abstract
{
    T *m_impl;

  public:
    AbstractRWS(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    WRITER_METHODS_;
    SEEKABLE_METHODS_;
};

template <typename T>
class AbstractBRWS final : public Abstract
{
    T *m_impl;

  public:
    AbstractBRWS(T *impl) : m_impl(impl) {}

    READER_METHODS_;
    BUFFERED_READER_METHODS_;
    WRITER_METHODS_;
    SEEKABLE_METHODS_;
};

#undef READER_METHODS_
#undef BUFFERED_READER_METHODS_
#undef WRITER_METHODS_
#undef SEEKABLE_METHODS_

} // namespace Detail

/**
 * @brief Returns an allocated abstract class wrapping a Reader.  Caller owns
 *        the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractR(BASE *impl)
{
    return new Detail::AbstractR<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a BufferedReader.  Caller
 *        owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractBR(BASE *impl)
{
    return new Detail::AbstractBR<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a Writer.  Caller owns
 *        the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractW(BASE *impl)
{
    return new Detail::AbstractW<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a Reader/Writer.  Caller
 *        owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractRW(BASE *impl)
{
    return new Detail::AbstractRW<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a BufferedReader/Writer.
 *        Caller owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractBRW(BASE *impl)
{
    return new Detail::AbstractBRW<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a Reader/Seekable.
 *        Caller owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractRS(BASE *impl)
{
    return new Detail::AbstractRS<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a
 *        BufferedReader/Seekable.  Caller owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractBRS(BASE *impl)
{
    return new Detail::AbstractBRS<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a Reader/Writer/Seekable.
 *        Caller owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractRWS(BASE *impl)
{
    return new Detail::AbstractRWS<BASE>(impl);
}

/**
 * @brief Returns an allocated abstract class wrapping a
 *        BufferedReader/Writer/Seekable.  Caller owns the pointer.
 */
template <typename BASE>
Abstract *MakeAbstractBRWS(BASE *impl)
{
    return new Detail::AbstractBRWS<BASE>(impl);
}

} // namespace LexIO

#endif
