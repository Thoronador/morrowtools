# Morrowind Tools Project

The Morrowind Tools Project is an attempt to provide some helpful utility
programs for the game The Elder Scrolls III: Morrowind. These tools will be
related to mods ("plugins") for Morrowind and allow things that would be
difficult to do with Morrowind's own modkit.

Some programs for The Elder Scrolls V: Skyrim were added more recently. So
despite its name the project also contains tools for Skyrim.

## Contents of repository

- **base**: This directory contains the source code for stuff that is used in
  more than one branch of morrowtools.
- **ci**: scripts and other files that are used as part of Continuous
  Integration (automated builds and tests, etc.)
- **mw**: contains code for Morrowind-related programs
- **sr**: contains code for Skyrim-related programs
- **tests**: contains code for test cases
- **win64-compile-test**: utility to test for 64 bit Windows OS when compiling

Within some directories you may find files named TODO or ToDo.txt. These files
list some of the work that still has to be done for that specific directory.

## Build status

[![GitHub CI Clang status](https://github.com/Thoronador/morrowtools/workflows/Clang/badge.svg)](https://github.com/Thoronador/morrowtools/actions)
[![GitHub CI GCC status](https://github.com/Thoronador/morrowtools/workflows/GCC/badge.svg)](https://github.com/Thoronador/morrowtools/actions)
[![GitHub CI Cross-compiling for Win32 status](https://github.com/Thoronador/morrowtools/workflows/Cross-compile%20for%20Win32/badge.svg)](https://github.com/Thoronador/morrowtools/actions)
[![GitLab CI Win64 cross-compile](https://gitlab.com/striezel/morrowtools/badges/master/pipeline.svg?ignore_skipped=true&key_text=Cross-compile%20for%20Win64&key_width=145)](https://gitlab.com/striezel/morrowtools/-/pipelines)

## Building from source

### Prerequisites

To build these tools from source you need a C++ compiler with support for C++17,
CMake 3.8 or later and the zlib library (>=1.2.7). pkg-config is required to
make it easier to find compiler options for the installed libraries.
Additionally, the program uses Catch (C++ Automated Test Cases in Headers) to
perform some tests.

It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All of that can usually be installed be typing

    apt-get install catch cmake g++ git pkg-config zlib1g-dev

or

    yum install catch cmake gcc-c++ git pkgconfig zlib-devel

into a root terminal.

_Note:_ Instructions for building on Windows systems are currently not included.
It it possible, but the setup is a bit complicated. The easiest way to do it is
probably to get a MinGW compiler (e. g. as part of
[Code::Blocks IDE](https://www.codeblocks.org/)) and download and compile
[zlib](https://www.zlib.net/) manually. A few hints on how to compile zlib
manually can be found in the file `zlib_info.txt`. After that is done, you can
use the Code::Blocks project files (*.cbp) in the directories to build the
applications.

### Getting the source code

Get the source directly from Git by cloning the Git repository and change to
the directory after the repository is completely cloned:

    git clone https://github.com/Thoronador/morrowtools.git ./mwtp
    cd mwtp

That's it, you should now have the current source code on your machine.

### Build process

The build process is relatively easy, because CMake does all the preparations.
Starting in the root directory of the source, you can do the following steps:

    mkdir build
    cd build
    cmake ../
    make -j4

If you want to run the test suite, too, then there is one more step:

    ctest -V

## Cross-compiling for Windows

There is an _experimental_ [Dockerfile](./Dockerfile) that uses Docker to
cross-compile the tools for 32 bit Windows (also runs on 64 bit Windows).
It will create a ZIP archive named `artifacts.zip` located in
`/opt/morrowtools/build/` when successful. This file contains the compiled
binaries after the Docker image has been built successfully. To start the build
type

    docker build . -t morrowtools

in the root directory of your source checkout.

## Copyright and Licensing

Copyright 2011-2022  Dirk Stolle

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
