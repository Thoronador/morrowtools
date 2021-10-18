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
#include <string_view>
#include <sstream>
#include "../../../../../sr/base/records/QuestRecord.hpp"

TEST_CASE("QuestRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    QuestRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    REQUIRE_FALSE( record.name.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownDNAM[i] == 0 );
    }
    REQUIRE_FALSE( record.hasENAM );
    REQUIRE( record.unknownENAM == 0 );
    REQUIRE( record.unknownQTGLs.empty() );
    REQUIRE( record.unknownCTDA_CIS2s.empty() );
    REQUIRE( record.filter.empty() );
    REQUIRE( record.indices.empty() );
    REQUIRE( record.theQOBJs.empty() );
    REQUIRE( record.unknownANAM == 0 );
    REQUIRE( record.aliases.empty() );
  }

  SECTION("equals")
  {
    QuestRecord a;
    QuestRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("VMAD mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownVMAD.setPresence(false);
        b.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM[11] = 12;
        b.unknownDNAM[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ENAM mismatch")
      {
        a.hasENAM = true;
        b.hasENAM = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasENAM = false;
        b.hasENAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasENAM = true;
        a.unknownENAM = 1;
        b.hasENAM = true;
        b.unknownENAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("QTGLs mismatch")
      {
        a.unknownQTGLs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownQTGLs.push_back(0x01234567);
        b.unknownQTGLs.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("filter mismatch")
      {
        a.filter = "foo";
        b.filter = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("indices mismatch")
      {
        a.indices.push_back(IndexEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("QOBJs mismatch")
      {
        a.theQOBJs.push_back(QOBJEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ANAM mismatch")
      {
        a.unknownANAM = 0x01234567;
        b.unknownANAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("aliases mismatch")
      {
        a.aliases.push_back(QuestRecord::AliasEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    QuestRecord record;

    REQUIRE( record.getRecordType() == cQUST );
  }

  SECTION("getWriteSize")
  {
    QuestRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 44 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 44 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 68 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 51 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 54 );
    }

    SECTION("size adjusts when ENAM is present")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.hasENAM = true;
      REQUIRE( record.getWriteSize() == 54 );
    }

    SECTION("size adjusts with length of QTGLs")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.unknownQTGLs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 54 );

      record.unknownQTGLs.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 64 );
    }

    SECTION("size adjusts with length of filter")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.filter = "foo";
      REQUIRE( record.getWriteSize() == 54 );

      record.filter = "foobarbaz";
      REQUIRE( record.getWriteSize() == 60 );
    }

    SECTION("size adjusts with CTDA/CIS2")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 82 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 120 );
    }

    // TODO: Add tests for indices, aliases and QOBJs as soon as they are
    //       calculated properly in getWriteSize().
  }
}
