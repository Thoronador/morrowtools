/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2025  Dirk Stolle

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

#include "../locate_catch.hpp"
#include <cstdint>
#include <fstream>
#include "../limited_streambuf.hpp"

TEST_CASE("MWTP::limited_streambuf")
{
  using namespace MWTP;

  SECTION("buffer limited to five characters, single character puts")
  {
    limited_streambuf<5> buf;
    std::ostream stream(&buf);

    REQUIRE( stream.good() );

    stream << '1';
    REQUIRE( stream.good() );

    stream << '2';
    REQUIRE( stream.good() );

    stream << '3';
    REQUIRE( stream.good() );

    stream << '4';
    REQUIRE( stream.good() );

    stream << '5';
    REQUIRE( stream.good() );

    // Writing a sixth character will change the stream to bad state.
    stream << '5';
    REQUIRE_FALSE( stream.good() );
    REQUIRE( stream.bad() );
  }

  SECTION("buffer limited to five characters, string writes")
  {
    limited_streambuf<5> buf;
    std::ostream stream(&buf);

    REQUIRE( stream.good() );

    stream << "Hi :)";
    REQUIRE( stream.good() );
    REQUIRE_FALSE( stream.bad() );

    stream << " fail";
    REQUIRE_FALSE( stream.good() );
    REQUIRE( stream.bad() );
  }

  SECTION("buffer limited to five characters, binary writes")
  {
    limited_streambuf<5> buf;
    std::ostream stream(&buf);

    REQUIRE( stream.good() );

    const std::uint32_t four = 0xF00BA121;
    stream.write(reinterpret_cast<const char*>(&four), 4);
    REQUIRE( stream.good() );
    REQUIRE_FALSE( stream.bad() );

    stream.write(reinterpret_cast<const char*>(&four), 1);
    REQUIRE( stream.good() );
    REQUIRE_FALSE( stream.bad() );

    stream.write(reinterpret_cast<const char*>(&four), 4);
    REQUIRE_FALSE( stream.good() );
    REQUIRE( stream.bad() );
  }

  SECTION("buffer limited to 23 characters, string writes")
  {
    limited_streambuf<23> buf;
    std::ostream stream(&buf);

    REQUIRE( stream.good() );

    stream << "Hi there. :)";
    REQUIRE( stream.good() );
    REQUIRE_FALSE( stream.bad() );

    stream << "A nice day.";
    REQUIRE( stream.good() );
    REQUIRE_FALSE( stream.bad() );

    stream << "fail";
    REQUIRE_FALSE( stream.good() );
    REQUIRE( stream.bad() );
  }
}
