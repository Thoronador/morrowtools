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
#include "../../../../../sr/base/records/FactionRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("FactionRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    FactionRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE( record.relations.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.flags == 0 );
    REQUIRE( record.exteriorJailMarkerRefID == 0 );
    REQUIRE( record.followerWaitMarkerRefID == 0 );
    REQUIRE( record.stolenGoodsContainerRefID == 0 );
    REQUIRE( record.playerInventoryContainerRefID == 0 );
    REQUIRE( record.crimeFactionListFormID == 0 );
    REQUIRE( record.jailOutfitFormID == 0 );
    REQUIRE_FALSE( record.unknownCRVA.isPresent() );
    REQUIRE( record.ranks.empty() );
    REQUIRE( record.vendorListFormID == 0 );
    REQUIRE( record.vendorContainterFormID == 0 );
    REQUIRE_FALSE( record.vendorStuff.isPresent );
    REQUIRE_FALSE( record.unknownPLVD.isPresent() );
    REQUIRE( record.conditions.empty() );
  }

  SECTION("equals")
  {
    FactionRecord a;
    FactionRecord b;

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

      SECTION("relations mismatch")
      {
        a.relations.push_back(FactionRecord::InterfactionRelation());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("flags mismatch")
      {
        a.flags = 1;
        b.flags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("exteriorJailMarkerRefID mismatch")
      {
        a.exteriorJailMarkerRefID = 1;
        b.exteriorJailMarkerRefID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("followerWaitMarkerRefID mismatch")
      {
        a.followerWaitMarkerRefID = 1;
        b.followerWaitMarkerRefID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("stolenGoodsContainerRefID mismatch")
      {
        a.stolenGoodsContainerRefID = 1;
        b.stolenGoodsContainerRefID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("playerInventoryContainerRefID mismatch")
      {
        a.playerInventoryContainerRefID = 1;
        b.playerInventoryContainerRefID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("crimeFactionListFormID mismatch")
      {
        a.crimeFactionListFormID = 1;
        b.crimeFactionListFormID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("jailOutfitFormID mismatch")
      {
        a.jailOutfitFormID = 1;
        b.jailOutfitFormID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CVRA mismatch")
      {
        a.unknownCRVA.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownCRVA.setPresence(false);
        b.unknownCRVA.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ranks mismatch")
      {
        a.ranks.push_back(FactionRecord::RankData());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("vendorListFormID mismatch")
      {
        a.vendorListFormID = 0x01234567;
        b.vendorListFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("vendorContainterFormID mismatch")
      {
        a.vendorContainterFormID = 0x01234567;
        b.vendorContainterFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("vendorStuff mismatch")
      {
        a.vendorStuff.isPresent = true;
        b.vendorStuff.isPresent = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PLVD mismatch")
      {
        a.unknownPLVD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownPLVD.setPresence(false);
        b.unknownPLVD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("conditions mismatch")
      {
        a.conditions.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.conditions.push_back(CTDA_CIS2_compound());
        b.conditions.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    FactionRecord record;

    REQUIRE( record.getRecordType() == cFACT );
  }

  SECTION("getWriteSize")
  {
    FactionRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 20 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 32 );
    }

    SECTION("size adjusts with length of relations")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.relations.push_back(FactionRecord::InterfactionRelation());
      REQUIRE( record.getWriteSize() == 38 );

      record.relations.push_back(FactionRecord::InterfactionRelation());
      REQUIRE( record.getWriteSize() == 56 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when exteriorJailMarkerRefID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.exteriorJailMarkerRefID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when followerWaitMarkerRefID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.followerWaitMarkerRefID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when stolenGoodsContainerRefID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.stolenGoodsContainerRefID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when playerInventoryContainerRefID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.playerInventoryContainerRefID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when crimeFactionListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.crimeFactionListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when jailOutfitFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.jailOutfitFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of CRVA")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "CRVA\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownCRVA.loadFromStream(stream, cCRVA, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "CRVA\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownCRVA.loadFromStream(stream, cCRVA, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when vendorListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.vendorListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when vendorContainterFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.vendorContainterFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when vendorStuff is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.vendorStuff.isPresent = true;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts with length of PLVD")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "PLVD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownPLVD.loadFromStream(stream, cPLVD, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "PLVD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownPLVD.loadFromStream(stream, cPLVD, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts with length of conditions")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.conditions.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 68 );

      record.conditions.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 106 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x0000F505, "foo");
    dummy_table.addString(0x00000132, "foo");
    dummy_table.addString(0x00002430, "foo");
    dummy_table.addString(0x0000D0A1, "foo");
    dummy_table.addString(0x0000D235, "foo");
    dummy_table.addString(0x0000D2E9, "foo");
    dummy_table.addString(0x00002C08, "foo");
    dummy_table.addString(0x0000087F, "foo");
    dummy_table.addString(0x0000E67C, "foo");
    dummy_table.addString(0x00008EEA, "foo");

    SECTION("default: load record")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FactionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001F259 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "CollegeofWinterholdFaction" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000F505 );
      REQUIRE( record.relations.size() == 6 );

      REQUIRE( record.relations[0].factionFormID == 0x000135A0 );
      REQUIRE( record.relations[0].mod == 0 );
      REQUIRE( record.relations[0].groupCombatReaction == 2 );

      REQUIRE( record.relations[1].factionFormID == 0x000DB386 );
      REQUIRE( record.relations[1].mod == 0 );
      REQUIRE( record.relations[1].groupCombatReaction == 0 );

      REQUIRE( record.relations[2].factionFormID == 0x0005C84E );
      REQUIRE( record.relations[2].mod == 0 );
      REQUIRE( record.relations[2].groupCombatReaction == 2 );

      REQUIRE( record.relations[3].factionFormID == 0x00103372 );
      REQUIRE( record.relations[3].mod == 0 );
      REQUIRE( record.relations[3].groupCombatReaction == 2 );

      REQUIRE( record.relations[4].factionFormID == 0x000180C5 );
      REQUIRE( record.relations[4].mod == 0 );
      REQUIRE( record.relations[4].groupCombatReaction == 2 );

      REQUIRE( record.relations[5].factionFormID == 0x0001F259 );
      REQUIRE( record.relations[5].mod == 0 );
      REQUIRE( record.relations[5].groupCombatReaction == 2 );

      REQUIRE( record.flags == 0x00008000 );
      REQUIRE( record.exteriorJailMarkerRefID == 0 );
      REQUIRE( record.followerWaitMarkerRefID == 0 );
      REQUIRE( record.stolenGoodsContainerRefID == 0 );
      REQUIRE( record.playerInventoryContainerRefID == 0 );
      REQUIRE( record.crimeFactionListFormID == 0 );
      REQUIRE( record.jailOutfitFormID == 0 );
      REQUIRE( record.unknownCRVA.isPresent() );
      const auto CRVA = std::string_view(reinterpret_cast<const char*>(record.unknownCRVA.data()), record.unknownCRVA.size());
      REQUIRE( CRVA == "\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );

      REQUIRE( record.ranks.size() == 7 );

      REQUIRE( record.ranks[0].index == 0 );
      REQUIRE( record.ranks[0].maleName.isPresent() );
      REQUIRE( record.ranks[0].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[0].maleName.getIndex() == 0x00000132 );
      REQUIRE_FALSE( record.ranks[0].femaleName.isPresent() );

      REQUIRE( record.ranks[1].index == 1 );
      REQUIRE( record.ranks[1].maleName.isPresent() );
      REQUIRE( record.ranks[1].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[1].maleName.getIndex() == 0x00002430 );
      REQUIRE_FALSE( record.ranks[1].femaleName.isPresent() );

      REQUIRE( record.ranks[2].index == 2 );
      REQUIRE( record.ranks[2].maleName.isPresent() );
      REQUIRE( record.ranks[2].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[2].maleName.getIndex() == 0x0000D0A1 );
      REQUIRE_FALSE( record.ranks[2].femaleName.isPresent() );

      REQUIRE( record.ranks[3].index == 3 );
      REQUIRE( record.ranks[3].maleName.isPresent() );
      REQUIRE( record.ranks[3].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[3].maleName.getIndex() == 0x000D2E9 );
      REQUIRE_FALSE( record.ranks[3].femaleName.isPresent() );

      REQUIRE( record.ranks[4].index == 4 );
      REQUIRE( record.ranks[4].maleName.isPresent() );
      REQUIRE( record.ranks[4].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[4].maleName.getIndex() == 0x00002C08 );
      REQUIRE_FALSE( record.ranks[4].femaleName.isPresent() );

      REQUIRE( record.ranks[5].index == 5 );
      REQUIRE( record.ranks[5].maleName.isPresent() );
      REQUIRE( record.ranks[5].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[5].maleName.getIndex() == 0x0000087F );
      REQUIRE_FALSE( record.ranks[5].femaleName.isPresent() );

      REQUIRE( record.ranks[6].index == 6 );
      REQUIRE( record.ranks[6].maleName.isPresent() );
      REQUIRE( record.ranks[6].maleName.getType() == LocalizedString::Type::Index );
      REQUIRE( record.ranks[6].maleName.getIndex() == 0x0000E67C );
      REQUIRE_FALSE( record.ranks[6].femaleName.isPresent() );

      REQUIRE( record.vendorListFormID == 0 );
      REQUIRE( record.vendorContainterFormID == 0 );
      REQUIRE( record.vendorStuff.isPresent );
      REQUIRE( record.vendorStuff.startHour == 0 );
      REQUIRE( record.vendorStuff.endHour == 0 );
      REQUIRE( record.vendorStuff.radius == 0 );
      REQUIRE( record.vendorStuff.flagsVendor == 0 );
      REQUIRE_FALSE( record.unknownPLVD.isPresent() );
      REQUIRE( record.conditions.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with JAIL + WAIT + CRGR")
    {
      const auto data = "FACT\x84\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FactionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000A0324 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0006 );
      // -- record data
      REQUIRE( record.editorID == "HonorhallKidsIgnoreCrimeFaction" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000D235 );
      REQUIRE( record.relations.empty() );

      REQUIRE( record.flags == 0x00003781 );
      REQUIRE( record.exteriorJailMarkerRefID == 0x0003EF58 );
      REQUIRE( record.followerWaitMarkerRefID == 0x0003EF58 );
      REQUIRE( record.stolenGoodsContainerRefID == 0 );
      REQUIRE( record.playerInventoryContainerRefID == 0 );
      REQUIRE( record.crimeFactionListFormID == 0x00026953 );
      REQUIRE( record.jailOutfitFormID == 0 );
      REQUIRE( record.unknownCRVA.isPresent() );
      const auto CRVA = std::string_view(reinterpret_cast<const char*>(record.unknownCRVA.data()), record.unknownCRVA.size());
      REQUIRE( CRVA == "\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0"sv );

      REQUIRE( record.ranks.empty() );

      REQUIRE( record.vendorListFormID == 0 );
      REQUIRE( record.vendorContainterFormID == 0 );
      REQUIRE( record.vendorStuff.isPresent );
      REQUIRE( record.vendorStuff.startHour == 0 );
      REQUIRE( record.vendorStuff.endHour == 0 );
      REQUIRE( record.vendorStuff.radius == 0 );
      REQUIRE( record.vendorStuff.flagsVendor == 0 );
      REQUIRE_FALSE( record.unknownPLVD.isPresent() );
      REQUIRE( record.conditions.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with STOL + PLCN + JOUT")
    {
      const auto data = "FACT\x95\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FactionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000E7C27 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "DBCrimeFactionMaro" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00008EEA );
      REQUIRE( record.relations.empty() );

      REQUIRE( record.flags == 0x00001041 );
      REQUIRE( record.exteriorJailMarkerRefID == 0x0003EF19 );
      REQUIRE( record.followerWaitMarkerRefID == 0x0003EF19 );
      REQUIRE( record.stolenGoodsContainerRefID == 0x0003EEFF );
      REQUIRE( record.playerInventoryContainerRefID == 0x0003EEFF );
      REQUIRE( record.crimeFactionListFormID == 0x00026953 );
      REQUIRE( record.jailOutfitFormID == 0x00028B60 );
      REQUIRE( record.unknownCRVA.isPresent() );
      const auto CRVA = std::string_view(reinterpret_cast<const char*>(record.unknownCRVA.data()), record.unknownCRVA.size());
      REQUIRE( CRVA == "\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0"sv );

      REQUIRE( record.ranks.empty() );

      REQUIRE( record.vendorListFormID == 0 );
      REQUIRE( record.vendorContainterFormID == 0 );
      REQUIRE( record.vendorStuff.isPresent );
      REQUIRE( record.vendorStuff.startHour == 0 );
      REQUIRE( record.vendorStuff.endHour == 0 );
      REQUIRE( record.vendorStuff.radius == 0 );
      REQUIRE( record.vendorStuff.flagsVendor == 0 );
      REQUIRE_FALSE( record.unknownPLVD.isPresent() );
      REQUIRE( record.conditions.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0FAIL\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\x02GollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x59\x01KollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "FACT\x63\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XNAM is not twelve")
    {
      {
        const std::string_view data = "FACT\x58\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0B\0\xA0\x35\x01\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x5A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0D\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XNAM can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XNAM's faction form ID is zero")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\0\0\0\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "FACT\x63\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not four")
    {
      {
        const std::string_view data = "FACT\x58\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x03\0\0\x80\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x5A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x05\0\0\x80\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "FACT\x4F\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple JAILs")
    {
      const auto data = "FACT\x8E\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of JAIL can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0JAIL\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: JAIL is zero")
    {
      const auto data = "FACT\x84\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0\0\0\0\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WAITs")
    {
      const auto data = "FACT\x8E\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of WAIT can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0WAIT\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: WAIT is zero")
    {
      const auto data = "FACT\x84\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0\0\0\0\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple STOLs")
    {
      const auto data = "FACT\x9F\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of STOL can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0STOL\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: STOL is zero")
    {
      const auto data = "FACT\x95\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\0\0\0\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PLCNs")
    {
      const auto data = "FACT\x9F\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PLCN can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0PLCN\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: PLCN is zero")
    {
      const auto data = "FACT\x95\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\0\0\0\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CRGRs")
    {
      const auto data = "FACT\x8E\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0Si\x02\0CRGR\x04\0Si\x02\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of CRGR can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0CRGR\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: CRGR is zero")
    {
      const auto data = "FACT\x84\0\0\0\0\0\0\0\x24\x03\x0A\0\x1B\x69\x55\0\x28\0\x06\0EDID\x20\0HonorhallKidsIgnoreCrimeFaction\0FULL\x04\0\x35\xD2\0\0DATA\x04\0\x81\x37\0\0JAIL\x04\0X\xEF\x03\0WAIT\x04\0X\xEF\x03\0CRGR\x04\0\0\0\0\0CRVA\x14\0\0\0\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple JOUTs")
    {
      const auto data = "FACT\x9F\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0`\x8B\x02\0JOUT\x04\0`\x8B\x02\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of JOUT can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0JOUT\x04\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: JOUT is zero")
    {
      const auto data = "FACT\x95\0\0\0\0\0\0\0\x27\x7C\x0E\0\x1B\x69\x55\0\x28\0\x05\0EDID\x13\0DBCrimeFactionMaro\0FULL\x04\0\xEA\x8E\0\0DATA\x04\0\x41\x10\0\0JAIL\x04\0\x19\xEF\x03\0WAIT\x04\0\x19\xEF\x03\0STOL\x04\0\xFF\xEE\x03\0PLCN\x04\0\xFF\xEE\x03\0CRGR\x04\0Si\x02\0JOUT\x04\0\0\0\0\0CRVA\x14\0\x01\x01\xE8\x03(\0\x05\0\x19\0\0\0\0\0\0?d\0d\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CRVAs")
    {
      const std::string_view data = "FACT\x73\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CRVA is not a multiple of four")
    {
      {
        const std::string_view data = "FACT\x58\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x13\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x5A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x15\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of CRVA can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of RNAM is not four")
    {
      {
        const std::string_view data = "FACT\x58\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x03\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x5A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x05\0\x01\0\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of RNAM can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MNAM can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FNAM can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0FNAM\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VEND can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0VEND\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VENC can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0VENC\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple VENVs")
    {
      const std::string_view data = "FACT\x6B\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of VENV is not twelve")
    {
      {
        const std::string_view data = "FACT\x58\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0B\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x5A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of VENV can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PLVDs")
    {
      const std::string_view data = "FACT\x7D\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0PLVD\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0PLVD\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PLVD can be read")
    {
      const std::string_view data = "FACT\x6B\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0PLVD\x0C\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PLVD is not twelve")
    {
      {
        const std::string_view data = "FACT\x6A\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0PLVD\x0B\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FACT\x6B\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0PLVD\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FACT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FactionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of CITC can be read")
    {
      const std::string_view data = "FACT\x59\x01\0\0\0\0\0\0\x59\xF2\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1B\0CollegeofWinterholdFaction\0FULL\x04\0\x05\xF5\0\0XNAM\x0C\0\xA0\x35\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x86\xB3\x0D\0\0\0\0\0\0\0\0\0XNAM\x0C\0\x4E\xC8\x05\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x72\x33\x10\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\xC5\x80\x01\0\0\0\0\0\x02\0\0\0XNAM\x0C\0\x59\xF2\x01\0\0\0\0\0\x02\0\0\0DATA\x04\0\0\x80\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0RNAM\x04\0\0\0\0\0MNAM\x04\0\x32\x01\0\0RNAM\x04\0\x01\0\0\0MNAM\x04\0\x30\x24\0\0RNAM\x04\0\x02\0\0\0MNAM\x04\0\xA1\xD0\0\0RNAM\x04\0\x03\0\0\0MNAM\x04\0\xE9\xD2\0\0RNAM\x04\0\x04\0\0\0MNAM\x04\0\x08\x2C\0\0RNAM\x04\0\x05\0\0\0MNAM\x04\0\x7F\x08\0\0RNAM\x04\0\x06\0\0\0MNAM\x04\0\x7C\xE6\0\0CITC\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FACT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FactionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("flag queries")
  {
    FactionRecord record;

    SECTION("isVendor")
    {
      REQUIRE_FALSE( record.isVendor() );
      record.flags = FactionRecord::cFlagVendor | 0xFF000000;
      REQUIRE( record.isVendor() );
    }
  }
}
