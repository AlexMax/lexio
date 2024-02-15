LexIO
=====
A library containing an API for reading and writing data to byte-based streams.

[![Testing](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml/badge.svg)](https://github.com/AlexMax/lexio/actions/workflows/cpp20.yml)

Requirements
------------
You need a C++14 compatible compiler with exceptions enabled.

Why?
----
I tend to run into situations where I need to read and write various data types to byte-based streams, but found many common C++ API designs made for this purpose cumbersome to use.  After doing some research and picking and choosing what I liked and disliked about previous API's I had used, this is what I came up with.

The base interfaces are heavily inspired by Rust's std::io traits, as they seem like a reasonable and flexible set of primitives to build on top of.  Streams throw exceptions when an unexpected or unrecoverable error happens, since each stream implementation can have its own unique failure conditions with additional stream-specific context.

How To Use
----------
Reading and writing in LexIO involves using stream classes that have certain methods with specific signatures defined.  There is no subclassing or overriding virtual methods in the stream class, any class can be used so long as it provides the correct methods.  LexIO comes with several type traits to ensure that the method implementations you write adhere to the methods, plus a collection of type-erased reference types that wrap any valid implementation.  The library also comes with several classes that adhere to these traits that cover many common use cases.

Once you have a stream class, LexIO then gives you free functions that take either a stream or a type-erased reference to a stream as a parameter and use those methods to read or write data.  The core library includes implementations of many common operations, such as reading an entire buffer or returning the current position in a seekable stream, but other functions are included for reading and writing many common data types, such as fixed-length integers, strings, and varints.

For more information on the available types of stream and their needed methods, read the documentation in the header of core.hpp.

License
-------
Licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).
