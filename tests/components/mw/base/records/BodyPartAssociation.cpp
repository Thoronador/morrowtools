/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

  SECTION("getWriteSize")
  {
    BodyPartAssociation assoc;
    assoc.Index = 1;

    SECTION("size adjusts with length of male part name")
    {
      assoc.MaleBodyPart = "foo";
      REQUIRE( assoc.getWriteSize() == 20 );

      assoc.MaleBodyPart = "foo12345";
      REQUIRE( assoc.getWriteSize() == 25 );

      assoc.MaleBodyPart = "some_name_that_is_made_up";
      REQUIRE( assoc.getWriteSize() == 42 );
    }

    SECTION("size adjusts with length of female part name")
    {
      assoc.FemaleBodyPart = "foo";
      REQUIRE( assoc.getWriteSize() == 20 );

      assoc.FemaleBodyPart = "foo12345";
      REQUIRE( assoc.getWriteSize() == 25 );

      assoc.FemaleBodyPart = "some_name_that_is_made_up";
      REQUIRE( assoc.getWriteSize() == 42 );
    }

    SECTION("both part names go into the size calculation")
    {
      assoc.MaleBodyPart = "foo";
      assoc.FemaleBodyPart = "bar";
      REQUIRE( assoc.getWriteSize() == 31 );

      assoc.MaleBodyPart = "foo12345";
      assoc.FemaleBodyPart = "bar123456";
      REQUIRE( assoc.getWriteSize() == 42 );
    }
  }

  SECTION("save to stream")
  {
    using namespace std::string_view_literals;

    SECTION("known example")
    {
      BodyPartAssociation assoc;
      assoc.Index = 1;
      assoc.MaleBodyPart = "A_Gondolier_M_Helmet";
      assoc.FemaleBodyPart.clear();

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( assoc.saveToStream(streamOut) );
      // Check written data.
      const auto data = "INDX\x01\0\0\0\x01\x42NAM\x14\0\0\0A_Gondolier_M_Helmet"sv;
      REQUIRE( streamOut.str() == data );
    }

    SECTION("with both parts")
    {
      BodyPartAssociation assoc;
      assoc.Index = 1;
      assoc.MaleBodyPart = "part_one";
      assoc.FemaleBodyPart = "part_two";

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( assoc.saveToStream(streamOut) );
      // Check written data.
      const auto data = "INDX\x01\0\0\0\x01\x42NAM\x08\0\0\0part_oneCNAM\x08\0\0\0part_two"sv;
      REQUIRE( streamOut.str() == data );
    }
  }
}
