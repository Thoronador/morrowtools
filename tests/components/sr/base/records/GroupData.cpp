/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include "../../../../../sr/base/records/GroupData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("GroupData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    GroupData group;

    REQUIRE( group.size() == 0 );
    REQUIRE( group.label() == 0 );
    REQUIRE( group.type() == 0 );
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
      a.setSize(4567);
      a.setLabel(cGMST);
      a.setType(GroupData::cTopLevelGroup);

      b.setSize(4567);
      b.setLabel(cGMST);
      b.setType(GroupData::cTopLevelGroup);

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("size mismatch")
      {
        a.setSize(4567);
        a.setLabel(cGMST);
        a.setType(GroupData::cTopLevelGroup);

        b.setSize(1234);
        b.setLabel(cGMST);
        b.setType(GroupData::cTopLevelGroup);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("label mismatch")
      {
        a.setSize(4567);
        a.setLabel(cGMST);
        a.setType(GroupData::cTopLevelGroup);

        b.setSize(4567);
        b.setLabel(cAACT);
        b.setType(GroupData::cTopLevelGroup);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("type mismatch")
      {
        a.setSize(4567);
        a.setLabel(cGMST);
        a.setType(GroupData::cTopLevelGroup);

        b.setSize(4567);
        b.setLabel(cGMST);
        b.setType(GroupData::cCellChildren);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknown data mismatch")
      {
        using namespace std::string_view_literals;
        uint32_t dummy = 0;

        {
          const std::string_view data = "GRUP\x84\xA1\xEF\xCDWOOP\xEF\xBE\xFE\xAF\x01\x23\x45\x67\x89\xAB\xCD\xEF"sv;
          std::istringstream streamIn;
          streamIn.str(std::string(data));
          // read GRUP, because header is handled before loadFromStream.
          streamIn.read((char*) &dummy, 4);
          // Reading should succeed.
          REQUIRE( a.loadFromStream(streamIn) );
        }

        {
          const std::string_view data = "GRUP\x84\xA1\xEF\xCDWOOP\xEF\xBE\xFE\xAF\x01\x23\x45\x67\x89\xAB\xCD\xFF"sv;
          std::istringstream streamIn;
          streamIn.str(std::string(data));
          // read GRUP, because header is handled before loadFromStream.
          streamIn.read((char*) &dummy, 4);
          // Reading should succeed.
          REQUIRE( b.loadFromStream(streamIn) );
        }

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("size")
  {
    GroupData group;

    REQUIRE( group.size() == 0 );
    group.setSize(0xABCD9876);
    REQUIRE( group.size() == 0xABCD9876 );
  }

  SECTION("label")
  {
    GroupData group;

    REQUIRE( group.label() == 0 );
    group.setLabel(0x87654321);
    REQUIRE( group.label() == 0x87654321 );
  }

  SECTION("type")
  {
    GroupData group;

    REQUIRE( group.type() == 0 );
    group.setType(0xDEADBEEF);
    REQUIRE( group.type() == 0xDEADBEEF );
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
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GroupData group;
      REQUIRE( group.loadFromStream(stream) );
      // Check data.
      REQUIRE( group.size() == 0x0000A184 );
      REQUIRE( group.label() == cKYWD );
      REQUIRE( group.type() == 0 );
    }

    SECTION("corrupt data: stream is too short")
    {
      const std::string_view data = "GRUP\x84\xA1\x00\x00\x4B\x59\x57\x44\x00\x00\x00\x00\x19\x4A\x23\x00\x00\x00\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GRUP, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
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
      group.setSize(0xCDEFA184);
      group.setLabel(cWOOP);
      group.setType(0xAFFEBEEF);

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

  SECTION("labelIsCellID")
  {
    GroupData group;

    group.setType(GroupData::cTopLevelGroup);
    REQUIRE_FALSE( group.labelIsCellID() );

    group.setType(GroupData::cWorldspaceChildren);
    REQUIRE_FALSE( group.labelIsCellID() );

    group.setType(GroupData::cCellChildren);
    REQUIRE( group.labelIsCellID() );

    group.setType(GroupData::cTopicChildren);
    REQUIRE_FALSE( group.labelIsCellID() );

    group.setType(GroupData::cCellPersistentChildren);
    REQUIRE( group.labelIsCellID() );

    group.setType(GroupData::cCellTemporaryChildren);
    REQUIRE( group.labelIsCellID() );

    group.setType(GroupData::cCellVisibleDistantChildren);
    REQUIRE( group.labelIsCellID() );
  }
}
