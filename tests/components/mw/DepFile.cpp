/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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
#include "../../../lib/mw/DepFiles.hpp"

TEST_CASE("MWTP::DepFile")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    DepFile df;

    REQUIRE( df.name.empty() );
    REQUIRE( df.size == -1 );
    REQUIRE( df.modified == static_cast<std::time_t>(-1) );
  }

  SECTION("constructor with parameter")
  {
    DepFile df("example.esp");

    REQUIRE( df.name == "example.esp" );
    REQUIRE( df.size == -1 );
    REQUIRE( df.modified == static_cast<std::time_t>(-1) );
  }

  SECTION("constructor with parameter (string)")
  {
    const std::string fn("example.esp");
    DepFile df(fn);

    REQUIRE( df.name == "example.esp" );
    REQUIRE( df.size == -1 );
    REQUIRE( df.modified == static_cast<std::time_t>(-1) );
  }

  SECTION("equality operator")
  {
    DepFile a;
    DepFile b;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("name mismatch")
      {
        a.name = "one.esm";
        b.name = "another_one.esm";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("size mismatch")
      {
        a.size = 12345;
        b.size = 55555;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("modified mismatch")
      {
        a.modified = static_cast<std::time_t>(-1);
        b.modified = static_cast<std::time_t>(123);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("isMasterFile")
  {
    DepFile df;

    df.name = "example.esp";
    REQUIRE_FALSE( df.isMasterFile() );
    df.name = "example.ESP";
    REQUIRE_FALSE( df.isMasterFile() );
    df.name = "example.Esp";
    REQUIRE_FALSE( df.isMasterFile() );
    df.name = "example.esm";
    REQUIRE( df.isMasterFile() );
    df.name = "example.ESM";
    REQUIRE( df.isMasterFile() );
    df.name = "example.Esm";
    REQUIRE( df.isMasterFile() );

    df.name = "foo";
    REQUIRE_FALSE( df.isMasterFile() );

    df.name = "";
    REQUIRE_FALSE( df.isMasterFile() );
  }
}
