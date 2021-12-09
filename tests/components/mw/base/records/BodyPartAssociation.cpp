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
#include "../../../../../mw/base/records/BodyPartAssociation.hpp"

TEST_CASE("MWTP::BodyPartAssociation")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    BodyPartAssociation bpa;

    REQUIRE( bpa.Index == 0 );
    REQUIRE( bpa.MaleBodyPart.empty() );
    REQUIRE( bpa.FemaleBodyPart.empty() );
  }

  SECTION("equality operator")
  {
    BodyPartAssociation a;
    BodyPartAssociation b;

    SECTION("equal")
    {
      a.Index = 1;
      a.MaleBodyPart = "foo";
      a.FemaleBodyPart = "bar";

      b.Index = 1;
      b.MaleBodyPart = "foo";
      b.FemaleBodyPart = "bar";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("index mismatch")
      {
        a.Index = 1;
        b.Index = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("MaleBodyPart mismatch")
      {
        a.MaleBodyPart = "foo";
        b.MaleBodyPart = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FemaleBodyPart mismatch")
      {
        a.FemaleBodyPart = "foo";
        b.FemaleBodyPart = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
