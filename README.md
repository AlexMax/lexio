LexIO
=====
A library containing an API for reading and writing data to byte-based streams.

[![Testing](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml/badge.svg)](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml)

Requirements
------------
You need a C++14 compatible compiler.

Why?
----
I tend to run into situations where I need to read and write various data types to byte-based streams, but found many common C++ API designs made for this purpose cumbersome to use.  After doing some research and picking and choosing what I liked and disliked about previous API's I had used, this is what I came up with.

The base interfaces are heavily inspired by Rust's std::io traits, as they seem like a reasonable and flexible set of primitives to build on top of.

How To Use
----------
Reading and writing in LexIO involves using stream classes that have certain methods with specific signatures defined.  There is no subclassing or overriding virtual methods in the stream class, any class can be used so long as it provides the correct methods.  LexIO comes with several type traits to ensure that the method implementations you write adhere to the methods, as well as several classes that cover many common use cases.

Once you have a stream class, LexIO then gives you free functions that take the stream as a parameter and use those methods to read or write data.  The core library includes implementations of many common operations, such as reading an entire buffer or returning the current position in a seekable stream, but other functions are included for reading and writing many common data types, such as fixed-length integers, strings, and varints.

### Reader

```cpp
struct Reader
{
    size_t LexRead(uint8_t *outDest, const size_t count);
};
```

#### LexRead

This method should read `count` bytes into the buffer pointed to by `outDest`, which should be large enough to contain at most `count` bytes.

If the read operation encounters a temporary error that implies the operation can be retried - such as `EINTR` - the operation shuold be retried until a result is returned.  If the read operation is successful, return the number of bytes that were actually read.  If EOF was hit, return 0 bytes.  If an error was encountered, throw an exception.

### BufferedReader

```cpp
struct BufferedReader
{
    size_t LexRead(uint8_t *outDest, const size_t count);
    LexIO::BufferView LexFillBuffer(const size_t size);
    void LexConsumeBuffer(const size_t size);
};
```

A `BufferedReader` is assumed to have a contiguous internal byte buffer of some kind. 

#### LexRead

The semantics of this method are identical to a standard `Reader`.  Thus, any `BufferedReader` is also a `Reader`.  However, it should be implemented in terms of `LexFillBuffer` and `LexConsumeBuffer`.

The implementation is allowed to buffer more data than is requested by this reader, so long as only the requested amount of data is written to the output pointer and the proper count is returned from this method.

#### LexFillBuffer

This method fills the internal buffer to at most `count` bytes.

The return value of this method is always a `LexIO::BufferView` that contains a pointer to start of the internal buffer, as well as the current size of the buffer.  The pointer returned by the view should be assumed to be valid until `LexFillBuffer` grows the size of the buffer or `LexConsumeBuffer` shrinks it.

If `count` is less than or equal to the current size of the buffer, this method should immediately return a view of the buffer, including its current size.  0 is considered an acceptable `count` for this purpose, and is in fact how `LexIO::GetBuffer()` is implemented.

Otherwise, the method should read and append data to the internal buffer until it is at most `count` bytes or EOF is hit, and then return the buffer view.  For example, if the buffer size was previously 2 and this method is called with a `count` is 8, 6 bytes are read and appended to the original 2, and a view of the resulting buffer of size 8 is returned.

Note that the size of the buffer has no bearing on the allocated size of the buffer itself - that detail is left to the implementation's discretion.

#### LexConsumeBuffer

This method removes the first `count` bytes from the current buffer.

If `count` is equal to the current size of the buffer, then the buffer is cleared.  If `count` is less than the current size of the buffer, `count` bytes should be removed from the buffer, and the remaining contents should be shifted until the buffer points to first byte of the remaining contents.

An exception should be thrown if `count` is greater than the current size of the buffer.

Note that the means of how the buffer is cleared or points to any remaining data is left to the implementation's discretion.

### Writer

```cpp
struct Writer
{
    size_t LexWrite(const uint8_t *src, const size_t count);
    void LexFlush();
};
```

#### LexWrite

This method should write `count` bytes from the buffer pointed to by `src`.

If the write operation encounters a temporary error that implies the operation can be retried - such as `EINTR` - the operation shuold be retried until a result is returned.  If the write operation is successful, return the number of bytes that were actually written.  If an error was encountered, throw an exception.

#### LexFlush

This method should flush any data held in any buffers.  If the implementation has no flush behavior, a no-op is a valid implementation of this method.  For example, POSIX uses `fsync` for file descriptiors.

### Seekable

```cpp
struct Seekable
{
    size_t LexSeek(const LexIO::SeekPos pos);
};
```

This method should seek to a position in the implementation based on the `pos` parameters.  A `LexIO::SeekPos` contains a desired seek destination with semantics similar to POSIX `lseek`.

The return value of this method should be the absolute stream position in the implementation where the seek operation ended up.  A value of 0 for the offset and `current` for the whence is an acceptable way to get the current stream position from the implementation, and is in fact how `LexIO::Tell()` is implemented.

Seeking past the start of the stream implementation should throw an exception.  Seeking past the end of the stream is fine.
