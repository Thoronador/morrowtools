# hex2sv

This directory contains a Rust command line application that can generate a C++
`std::string_view` literal from a given sequence of hexadecimal digits.

It reads the hexadecimal sequence from the standard input stream and prints the
result to the standard output.

[![GitHub CI](https://github.com/Thoronador/morrowtools/workflows/hex2sv/badge.svg)](https://github.com/Thoronador/morrowtools/actions)

## Building the application from source

### Prerequisites

[![minimum rustc 1.40.0](https://img.shields.io/badge/minimum%20rustc-1.40.0-c18170?logo=rust&style=for-the-badge)](https://www.whatrustisit.com/)

To build the application you need the Rust compiler. The Minimum Supported Rust
Version (MSRV) is Rust 1.40. Furthermore, you need Cargo (Rust's package
manager).

All of that can usually be installed be typing

    # Debian-based Linux distribution
    apt-get install cargo rustc

or

    # CentOS 8
    yum install cargo rust

or

    # Alpine
    apk add cargo rust

into a root terminal.

### Build process

The build process is relatively easy, because Cargo can handle that for you.
Starting in the root directory of the source, you can invoke the following
command in a terminal to build the application:

    cargo build

Or, if you want the optimized release version, type

    cargo build --release

instead.

That's it.

## Using the application

The application expects a hexadecimal sequence from standard input. You can use
`echo` or pipes (`|`) or input redirection to get the data into the program.
For example, a call like

```bash
echo "41 41 43 54 11 00 00 00 00 00 00 00 02 30 01 00 19 4B 0E 00 0F 00 01 00 45 44 49 44 0B 00 41 63 74 69 6F 6E 49 64 6C 65 00" | cargo run
```

should create an output like

```c++
const auto data = "AACT\x11\0\0\0\0\0\0\0\x02\x30\x01\0\x19\x4B\x0E\0\x0F\0\x01\0EDID\x0B\0ActionIdle\0"sv;
```

Note that the program expects the hex digits of each byte to be separated by a
space character from the next byte's hex digits.

## Copyright and Licensing

Copyright 2021  Dirk Stolle

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
