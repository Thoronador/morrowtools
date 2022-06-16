/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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
#include "../../../../../mw/base/records/DialogueInfoRecord.hpp"

TEST_CASE("MWTP::FuncVarRecord")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    FuncVarRecord item;

    REQUIRE( item.Index == 0 );
    REQUIRE( item.Type == 0 );
    REQUIRE( item.Function == 0 );
    REQUIRE( item.CompareOp == 0 );
    REQUIRE( item.Name.empty() );
    REQUIRE_FALSE( item.isFloat );
    REQUIRE( item.fVal == 0.0f );
    REQUIRE( item.iVal == 0 );
  }

  SECTION("equality operator")
  {
    FuncVarRecord a;
    FuncVarRecord b;

    SECTION("equal")
    {
      a.Index = 1;
      a.Name = "foo";

      b.Index = 1;
      b.Name = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("Index mismatch")
      {
        a.Index = 1;
        b.Index = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Type mismatch")
      {
        a.Type = 1;
        b.Type = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Function mismatch")
      {
        a.Function = 1;
        b.Function = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CompareOp mismatch")
      {
        a.CompareOp = 1;
        b.CompareOp = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Name mismatch")
      {
        a.Name = "foo";
        b.Name = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("isFloat mismatch")
      {
        a.isFloat = false;
        b.isFloat = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("float value mismatch")
      {
        a.isFloat = false;
        a.fVal = 1.0f;
        b.isFloat = false;
        b.fVal = 2.0f;

        REQUIRE( a == b );
        REQUIRE( b == a );

        a.isFloat = true;
        a.fVal = 1.0f;
        b.isFloat = true;
        b.fVal = 2.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("integer value mismatch")
      {
        a.isFloat = true;
        a.iVal = 1;
        b.isFloat = true;
        b.iVal = 2;

        REQUIRE( a == b );
        REQUIRE( b == a );

        a.isFloat = false;
        a.iVal = 1;
        b.isFloat = false;
        b.iVal = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
