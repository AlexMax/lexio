LexIO
=====
A library containing an API for reading and writing data to byte-based streams.

[![Testing](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml/badge.svg)](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml)

Requirements
------------
You need a C++14 compatible compiler and an implementation of std::span.

You can use spans other than std::span with this library.  Define `LEXIO_SPAN_TYPE` and `LEXIO_CONST_SPAN_TYPE` to the type names the library should use before including `lexio.h`.

```cpp
#define LEXIO_SPAN_TYPE nonstd ::span<uint8_t>
#define LEXIO_CONST_SPAN_TYPE nonstd::span<const uint8_t>

#include "lexio.h"
```

Why?
----
I tend to run into situations where I need to read and write various data types to byte-based streams, but found many common API designs cumbersome to use.  After doing some research and picking and choosing what I liked and disliked about previous API's I had used, this is what I came up with.  It's heavily inspired by Rust's std::io traits.

Design
------
Here is the way LexIO reasons about streams.

- A stream should implement the bare minimum interface to read or write an arbitrary number of bytes, and nothing else.
    - This is to make writing your own stream wrappers easy.
    - Seekable buffers have more features for peeking at the underlying data.
- Reading and writing anything other than a span of bytes should be done in free functions that take the stream as a parameter.
    - With this design, you can easily write your own reader and writer functiosn for your own data types without having to stick the implementation in the stream class.
- Reading and writing functions should be uniquely named and not overloaded based on type.
    - This allows you to have multiple conventions for reading and writing a particular data type, such as writing integers as fixed-length integers versus as varints.
    - C++'s rules for overloading are too clever by half; better to be explicit than accidentally call the wrong overload.
- Reaching EOF is not an error, you just read less data than you expected.
- Actual errors are handled through exceptions.
    - Exceptions do have flaws, but I have found they are the best option for dealing with gnarly parsing code.
