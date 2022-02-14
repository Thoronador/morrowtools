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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/AIPackages.hpp"

TEST_CASE("MWTP::NPC_AIActivate")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPC_AIActivate package;

    REQUIRE( package.TargetID.empty() );
    REQUIRE( package.Reset == 0 );
  }

  SECTION("equals")
  {
    NPC_AIActivate a;
    NPC_AIActivate b;

    SECTION("equal")
    {
      a.TargetID = "foo";
      a.Reset = 1;

      b.TargetID = "foo";
      b.Reset = 1;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("TargetID mismatch")
      {
        a.TargetID = "foo";
        b.TargetID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Reset mismatch")
      {
        a.Reset = 1;
        b.Reset = 42;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getPackageType")
  {
    NPC_AIActivate package;

    REQUIRE( package.getPackageType() == PackageType::ptActivate );
  }

  SECTION("getStreamSize")
  {
    NPC_AIActivate package;

    REQUIRE( package.getStreamSize() == 41 );
  }

  SECTION("saveToStream")
  {
    SECTION("default: save package")
    {
      NPC_AIActivate package;
      package.TargetID = "foo";
      package.Reset = 42;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_A\x21\0\0\0foo\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x2A"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("default: save package with very long target ID")
    {
      NPC_AIActivate package;
      package.TargetID = "foobarbazquux4567890abcdefghij123456789";
      package.Reset = 42;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( package.saveToStream(stream) );
      // Check written data.
      const auto data = "AI_A\x21\0\0\0foobarbazquux4567890abcdefghij1\0\x2A"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
