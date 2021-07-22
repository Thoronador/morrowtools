/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
 -------------------------------------------------------------------------------
*/

#include <catch.hpp>
#include <cstring>
#include "../../../base/BufferStream.hpp"

TEST_CASE("BufferStream")
{
  using namespace MWTP;

  SECTION("initialize and read")
  {
    char* ptr = new char[11];
    std::memcpy(ptr, "Hey there!", 11);
    BufferStream stream(ptr, 11);

    char c = '\0';
    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'H' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'e' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'y' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == ' ' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 't' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'h' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'e' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'r' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == 'e' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == '!' );

    stream.read(&c, 1);
    REQUIRE( stream.good() );
    REQUIRE( c == '\0' );

    // Reading more data than the buffer has should switch stream from good to
    // failed.
    stream.read(&c, 1);
    REQUIRE_FALSE( stream.good() );
    REQUIRE( stream.fail() );
  }
}
