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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/GroupData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("GroupData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    GroupData group;

    REQUIRE( group.getGroupSize() == 0 );
    REQUIRE( group.getGroupLabel() == 0 );
    REQUIRE( group.getGroupType() == 0 );
  }

  SECTION("equals")
  {
    GroupData a;
    GroupData b;

    SECTION("empty")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("equal")
    {
      a.setGroupSize(4567);
      a.setGroupLabel(cGMST);
      a.setGroupType(GroupData::cTopLevelGroup);

      b.setGroupSize(4567);
      b.setGroupLabel(cGMST);
      b.setGroupType(GroupData::cTopLevelGroup);

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("size mismatch")
      {
        a.setGroupSize(4567);
        a.setGroupLabel(cGMST);
        a.setGroupType(GroupData::cTopLevelGroup);

        b.setGroupSize(1234);
        b.setGroupLabel(cGMST);
        b.setGroupType(GroupData::cTopLevelGroup);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("label mismatch")
      {
        a.setGroupSize(4567);
        a.setGroupLabel(cGMST);
        a.setGroupType(GroupData::cTopLevelGroup);

        b.setGroupSize(4567);
        b.setGroupLabel(cAACT);
        b.setGroupType(GroupData::cTopLevelGroup);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("type mismatch")
      {
        a.setGroupSize(4567);
        a.setGroupLabel(cGMST);
        a.setGroupType(GroupData::cTopLevelGroup);

        b.setGroupSize(4567);
        b.setGroupLabel(cGMST);
        b.setGroupType(GroupData::cCellChildren);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("size")
  {
    GroupData group;

    REQUIRE( group.getGroupSize() == 0 );
    group.setGroupSize(0xABCD9876);
    REQUIRE( group.getGroupSize() == 0xABCD9876 );
  }

  SECTION("label")
  {
    GroupData group;

    REQUIRE( group.getGroupLabel() == 0 );
    group.setGroupLabel(0x87654321);
    REQUIRE( group.getGroupLabel() == 0x87654321 );
  }

  SECTION("type")
  {
    GroupData group;

    REQUIRE( group.getGroupType() == 0 );
    group.setGroupType(0xDEADBEEF);
    REQUIRE( group.getGroupType() == 0xDEADBEEF );
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("default: load group data")
    {
      const std::string_view data = "GRUP\x84\xA1\x00\x00\x4B\x59\x57\x44\x00\x00\x00\x00\x19\x4A\x23\x00\x00\x00\x00\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GRUP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GroupData group;
      REQUIRE( group.loadFromStream(stream) );
      // Check data.
      REQUIRE( group.getGroupSize() == 0x0000A184 );
      REQUIRE( group.getGroupLabel() == cKYWD );
      REQUIRE( group.getGroupType() == 0 );
    }

    SECTION("corrupt data: stream is too short")
    {
      const std::string_view data = "GRUP\x84\xA1\x00\x00\x4B\x59\x57\x44\x00\x00\x00\x00\x19\x4A\x23\x00\x00\x00\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GRUP, because header is handled before loadFromStream.
      stream.read((char*) &dummy, 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GroupData group;
      REQUIRE_FALSE( group.loadFromStream(stream) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("save group")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      GroupData group;
      // Fill data.
      // -- header
      group.setGroupSize(0xCDEFA184);
      group.setGroupLabel(cWOOP);
      group.setGroupType(0xAFFEBEEF);

      // Writing should succeed.
      REQUIRE( group.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "GRUP\x84\xA1\xEF\xCDWOOP\xEF\xBE\xFE\xAF\x00\x00\x00\x00\x00\x00\x00\x00"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("load + save roundtrip")
    {
      const std::string_view data = "GRUP\x84\xA1\xEF\xCDWOOP\xEF\xBE\xFE\xAF\x01\x23\x45\x67\x89\xAB\xCD\xEF"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // read GRUP, because header is handled before loadFromStream.
      streamIn.read((char*) &dummy, 4);
      REQUIRE( streamIn.good() );

      // Reading should succeed.
      GroupData group;
      REQUIRE( group.loadFromStream(streamIn) );

      std::ostringstream streamOut;
      REQUIRE( streamOut.good() );
      // Writing should succeed.
      REQUIRE( group.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }
  }
}
