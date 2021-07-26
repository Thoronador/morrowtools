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
#include "../../../base/UtilityFunctions.hpp"

TEST_CASE("UtilityFunctions")
{
  SECTION("intToString")
  {
    REQUIRE( intToString(0) == "0" );
    REQUIRE( intToString(1) == "1" );
    REQUIRE( intToString(12) == "12" );
    REQUIRE( intToString(123) == "123" );
    REQUIRE( intToString(1234) == "1234" );
    REQUIRE( intToString(12345) == "12345" );
    REQUIRE( intToString(32767) == "32767" );

    REQUIRE( intToString(-1) == "-1" );
    REQUIRE( intToString(-12) == "-12" );
    REQUIRE( intToString(-123) == "-123" );
    REQUIRE( intToString(-1234) == "-1234" );
    REQUIRE( intToString(-12345) == "-12345" );
    REQUIRE( intToString(-32768) == "-32768" );
  }

  SECTION("stringToFloat")
  {
    float value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToFloat("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToFloat("0", value));
      REQUIRE( value == 0.0 );

      REQUIRE( stringToFloat("0.0", value));
      REQUIRE( value == 0.0 );

      REQUIRE( stringToFloat("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToFloat("1.25", value));
      REQUIRE( value == 1.25 );

      REQUIRE( stringToFloat("123.45", value));
      REQUIRE( value == 123.45f );

      REQUIRE( stringToFloat("12345.75", value));
      REQUIRE( value == 12345.75f );

      REQUIRE( stringToFloat("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToFloat("-1.25", value));
      REQUIRE( value == -1.25 );

      REQUIRE( stringToFloat("-1.234", value));
      REQUIRE( value == -1.234f );

      REQUIRE( stringToFloat("-123.45", value));
      REQUIRE( value == -123.45f );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToFloat("-", value));
      REQUIRE_FALSE( stringToFloat("1.2a", value));
      REQUIRE_FALSE( stringToFloat("  1.2", value));
      REQUIRE_FALSE( stringToFloat("1abcdef", value));
      REQUIRE_FALSE( stringToFloat("Hello", value));
      REQUIRE_FALSE( stringToFloat("Hello12.3", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToFloat("350000000000000000000000000000000000000000000000", value));
      REQUIRE_FALSE( stringToFloat("-350000000000000000000000000000000000000000000000", value));
    }
  }

  SECTION("stringToLong")
  {
    int32_t value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToLong("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToLong("0", value));
      REQUIRE( value == 0 );

      REQUIRE( stringToLong("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToLong("1234", value));
      REQUIRE( value == 1234 );

      REQUIRE( stringToLong("12345", value));
      REQUIRE( value == 12345 );

      REQUIRE( stringToLong("2147483647", value));
      REQUIRE( value == 2147483647 );

      REQUIRE( stringToLong("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToLong("-12", value));
      REQUIRE( value == -12 );

      REQUIRE( stringToLong("-1234", value));
      REQUIRE( value == -1234 );

      REQUIRE( stringToLong("-12345", value));
      REQUIRE( value == -12345 );

      REQUIRE( stringToLong("-2147483647", value));
      REQUIRE( value == -2147483647 );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToLong("-", value));
      REQUIRE_FALSE( stringToLong("12a", value));
      REQUIRE_FALSE( stringToLong("  12", value));
      REQUIRE_FALSE( stringToLong("1abcdef", value));
      REQUIRE_FALSE( stringToLong("Hello", value));
      REQUIRE_FALSE( stringToLong("Hello123", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToLong("2147483648", value));
      REQUIRE_FALSE( stringToLong("-2147483649", value));
    }
  }

  SECTION("stringToShort")
  {
    int16_t value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToShort("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToShort("0", value));
      REQUIRE( value == 0 );

      REQUIRE( stringToShort("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToShort("1234", value));
      REQUIRE( value == 1234 );

      REQUIRE( stringToShort("12345", value));
      REQUIRE( value == 12345 );

      REQUIRE( stringToShort("32767", value));
      REQUIRE( value == 32767 );

      REQUIRE( stringToShort("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToShort("-12", value));
      REQUIRE( value == -12 );

      REQUIRE( stringToShort("-1234", value));
      REQUIRE( value == -1234 );

      REQUIRE( stringToShort("-12345", value));
      REQUIRE( value == -12345 );

      REQUIRE( stringToShort("-32767", value));
      REQUIRE( value == -32767 );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToShort("-", value));
      REQUIRE_FALSE( stringToShort("12a", value));
      REQUIRE_FALSE( stringToShort("  12", value));
      REQUIRE_FALSE( stringToShort("1abcdef", value));
      REQUIRE_FALSE( stringToShort("Hello", value));
      REQUIRE_FALSE( stringToShort("Hello123", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToShort("32768", value));
      REQUIRE_FALSE( stringToShort("-32769", value));
    }
  }
}
