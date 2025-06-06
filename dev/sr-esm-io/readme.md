# sr-esm-io

This directory contains a C++ command line application that reads a Skyrim ESM
file and writes it back to a temporary directory. It's not intended for direct
use but only for development purposes.

It reads the data records from the file given as its first argument, parses the
content, and writes the records out as a separate ESM file, ideally producing
a file identical to the input.

## Building the application from source

### Prerequisites

To build this tools from source you need a C++ compiler with support for C++17,
CMake 3.8 or later, the LZ4 library and the zlib library (>=1.2.7). pkg-config
is required to make it easier to find compiler options for the installed
libraries.

All of that can usually be installed be typing

    # Debian-based Linux distribution
    apt-get install cmake g++ liblz4-dev pkg-config zlib1g-dev

or

    # CentOS 8
    yum install cmake gcc-c++ lz4-devel pkgconfig zlib-devel

or

    # Alpine
    apk add cmake g++ lz4-dev make pkgconf zlib-dev

into a root terminal.

### Build process

The build process is relatively easy, because CMake does all the preparations.
Starting in the root directory of the source, you can do the following steps:

    mkdir build
    cd build
    cmake ../
    cmake --build . -j4

That's it.

## Using the application

This is not intended for production use.

## Copyright and Licensing

Copyright 2025  Dirk Stolle

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
