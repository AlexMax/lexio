LexIO
=====
A library containing an API for reading and writing data to byte-based streams.

[![Testing](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml/badge.svg)](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml)

Requirements
------------
You need a C++14 compatible compiler and an implementation of std::span.

You can use spans other than `std::span` with this library.  Define `LEXIO_SPAN` as a macro that names a span type of the passed parameter before including `lexio.h`.

```cpp
#include "nonstd/span.hpp"

#define LEXIO_SPAN(T) nonstd::span<T>

#include "lexio.h"
```

Why?
----
I tend to run into situations where I need to read and write various data types to byte-based streams, but found many common C++ API designs made for this purpose cumbersome to use.  After doing some research and picking and choosing what I liked and disliked about previous API's I had used, this is what I came up with.

The base interfaces are heavily inspired by Rust's std::io traits, as they seem like a reasonable and flexible set of primitives to build on top of.

How To Use
----------
Reading and writing in LexIO involves using stream classes that have certain methods with specific signatures defined.  There is no subclassing or overriding virtual methods in the stream class, any class can be used so long as it provides the correct methods.  LexIO comes with several type traits to ensure that the method implementations you write adhere to the methods, as well as several classes that cover many common use cases.

Once you have a stream class, LexIO then gives you free functions that take the stream as a parameter and use those methods to read or write data.  The core library includes implementations of many common operations, such as reading an entire buffer or returning the current position in a seekable stream, but other functions are included for reading and writing many common data types, such as fixed-length integers, strings, and varints.
