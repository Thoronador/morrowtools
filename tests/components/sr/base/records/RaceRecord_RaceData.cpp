/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#include "../../../locate_catch.hpp"
#include <string_view>
#include "../../../../../sr/base/records/RaceRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("RaceRecord::RaceData")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    RaceRecord::RaceData data;

    for (unsigned int i = 0; i < 16; ++i)
    {
      REQUIRE( data.unknown16[i] == 0 );
    }
    REQUIRE( data.heightMale == 0.0f );
    REQUIRE( data.heightFemale == 0.0f );
    REQUIRE( data.weightMale == 0.0f );
    REQUIRE( data.weightFemale == 0.0f );
    for (unsigned int i = 0; i < 96; ++i)
    {
      REQUIRE( data.unknown96[i] == 0 );
    }
    REQUIRE_FALSE( data.unknown36.has_value() );
  }

  SECTION("equality operator")
  {
    RaceRecord::RaceData a;
    RaceRecord::RaceData b;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("unknown16 mismatch")
      {
        a.unknown16[15] = 3;
        b.unknown16[15] = 4;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("heightMale mismatch")
      {
        a.heightMale = 1.0f;
        b.heightMale = 1.25f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("heightFemale mismatch")
      {
        a.heightFemale = 1.0f;
        b.heightFemale = 1.25f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("weightMale mismatch")
      {
        a.weightMale = 1.0f;
        b.weightMale = 1.25f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("weightFemale mismatch")
      {
        a.weightFemale = 1.0f;
        b.weightFemale = 1.25f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknown96 mismatch")
      {
        a.unknown96[95] = 3;
        a.unknown96[95] = 3;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknown36 mismatch")
      {
        a.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0 };

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknown36.reset();
        b.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0 };

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 12 };
        b.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 34 };

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    RaceRecord::RaceData data;

    // Set some data.
    for (unsigned int i = 0; i < 16; ++i)
    {
      data.unknown16[i] = i + 1;
    }
    data.heightMale = 3.5f;
    data.heightFemale = 3.0f;
    data.weightMale = 2.5f;
    data.weightFemale = 2.0f;
    for (unsigned int i = 0; i < 96; ++i)
    {
      data.unknown96[i] = i + 2;
    }
    data.unknown36 = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
                       30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56,
                       58, 60, 62, 64, 66, 68, 70 };

    // Clear and check data.
    data.clear();

    for (unsigned int i = 0; i < 16; ++i)
    {
      REQUIRE( data.unknown16[i] == 0 );
    }
    REQUIRE( data.heightMale == 0.0f );
    REQUIRE( data.heightFemale == 0.0f );
    REQUIRE( data.weightMale == 0.0f );
    REQUIRE( data.weightFemale == 0.0f );
    for (unsigned int i = 0; i < 96; ++i)
    {
      REQUIRE( data.unknown96[i] == 0 );
    }
    REQUIRE_FALSE( data.unknown36.has_value() );
  }
}
