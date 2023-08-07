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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/RaceRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("RaceRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    RaceRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.spellFormIDs.empty() );
    REQUIRE_FALSE( record.unknownWNAM.has_value() );
    REQUIRE_FALSE( record.unknownBODT.isPresent() );
    REQUIRE_FALSE( record.unknownBOD2.isPresent() );
    REQUIRE( record.keywords.empty() );
    // data
    for (unsigned int i = 0; i < 16; ++i)
    {
      REQUIRE( record.data.unknown16[i] == 0 );
    }
    REQUIRE( record.data.heightMale == 0.0f );
    REQUIRE( record.data.heightFemale == 0.0f );
    REQUIRE( record.data.weightMale == 0.0f );
    REQUIRE( record.data.weightFemale == 0.0f );
    for (unsigned int i = 0; i < 96; ++i)
    {
      REQUIRE( record.data.unknown96[i] == 0 );
    }
    REQUIRE_FALSE( record.data.unknown36.has_value() );
    // end of data
    REQUIRE( record.subBlocks.empty() );
  }

  SECTION("equals")
  {
    RaceRecord a;
    RaceRecord b;

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("spellFormIDs mismatch")
      {
        a.spellFormIDs.push_back(0x0F00BA12);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("WNAM mismatch")
      {
        a.unknownWNAM = 0x01234567;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownWNAM.reset();
        b.unknownWNAM = 0x089ABCDE;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownWNAM = 0x01234567;
        b.unknownWNAM = 0x089ABCDE;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BODT mismatch")
      {
        a.unknownBODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownBODT.setPresence(false);
        b.unknownBODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BOD2 mismatch")
      {
        a.unknownBOD2.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownBOD2.setPresence(false);
        b.unknownBOD2.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywords = { 0x01234567 };
        b.keywords = { 0x0FEDCBA9 };

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("data mismatch")
      {
        a.data.heightMale = 1.0f;
        b.data.heightMale = 2.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("subBlocks mismatch")
      {
        a.subBlocks.push_back(SubBlock());
        b.subBlocks.clear();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    RaceRecord record;

    REQUIRE( record.getRecordType() == cRACE );
  }

  SECTION("getWriteSize")
  {
    RaceRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 144 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 156 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 154 );
    }

    SECTION("size adjusts when description is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.description = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 154 );
    }

    SECTION("size adjusts with length of spell form IDs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.spellFormIDs = { 0x01234567 };
      REQUIRE( record.getWriteSize() == 164 );

      record.spellFormIDs = { 0x01234567, 0x0FEDCBA9 };
      REQUIRE( record.getWriteSize() == 174 );
    }

    SECTION("size adjusts when WNAM is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.unknownWNAM = 0xDEADBEEF;
      REQUIRE( record.getWriteSize() == 154 );
    }

    SECTION("size adjusts when BODT is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.unknownBODT.setPresence(true);
      REQUIRE( record.getWriteSize() == 150 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "BODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBODT.loadFromStream(stream, cBODT, true) );
      }
      REQUIRE( record.getWriteSize() == 168 );

      {
        const std::string_view data = "BODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBODT.loadFromStream(stream, cBODT, true) );
      }
      REQUIRE( record.getWriteSize() == 151 );
    }

    SECTION("size adjusts with length of BOD2")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "BOD2\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBOD2.loadFromStream(stream, cBOD2, true) );
      }
      REQUIRE( record.getWriteSize() == 168 );

      {
        const std::string_view data = "BOD2\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownBOD2.loadFromStream(stream, cBOD2, true) );
      }
      REQUIRE( record.getWriteSize() == 151 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      record.keywords = { 0x01234567 };
      REQUIRE( record.getWriteSize() == 164 );

      record.keywords = { 0x01234567, 0x01234568 };
      REQUIRE( record.getWriteSize() == 168 );
    }

    SECTION("size adjusts with length of data")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      // Size of data record was increased in version 43.
      record.data.unknown36 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0 };
      record.headerVersion = 43;
      REQUIRE( record.getWriteSize() == 180 );
    }

    SECTION("size adjusts with length of sub-blocks")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 144 );

      SubBlock block;
      block.subType = cTINC;
      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "TINC\x04\0_234"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( block.subData.loadFromStream(stream, cTINC, true) );
      }
      record.subBlocks.push_back(block);
      REQUIRE( record.getWriteSize() == 154 );

      block.subType = cVTCK;
      {
        const std::string_view data = "VTCK\x08\0_2345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( block.subData.loadFromStream(stream, cVTCK, true) );
      }
      record.subBlocks.push_back(block);
      REQUIRE( record.getWriteSize() == 168 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    StringTable dummy_table;
    dummy_table.addString(0x00004699, "foo");
    dummy_table.addString(0x0001052F, "bar");

    SECTION("default: load record with short DATA sub-record")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0009AA3C );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000A );
      // -- record data
      REQUIRE( record.editorID == "SwarmRace" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00004699 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0x00000000 );
      REQUIRE( record.spellFormIDs.size() == 1 );
      REQUIRE( record.spellFormIDs[0] == 0x0008CDD1 );
      REQUIRE( record.unknownWNAM.has_value() );
      REQUIRE( record.unknownWNAM.value() == 0x00086F42);
      REQUIRE( record.unknownBODT.isPresent() );
      const auto BODT = std::string_view(reinterpret_cast<const char*>(record.unknownBODT.data()), record.unknownBODT.size());
      REQUIRE( BODT == "\0\0\0\0\0/V\0\x02\0\0\0"sv );
      REQUIRE_FALSE( record.unknownBOD2.isPresent() );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x00013795 );
      REQUIRE( record.keywords[1] == 0x0006F6FB );
      REQUIRE( record.data.unknown16[0] == 0xFF );
      REQUIRE( record.data.unknown16[1] == 0x00 );
      REQUIRE( record.data.unknown16[2] == 0xFF );
      REQUIRE( record.data.unknown16[3] == 0x00 );
      REQUIRE( record.data.unknown16[4] == 0xFF );
      REQUIRE( record.data.unknown16[5] == 0x00 );
      REQUIRE( record.data.unknown16[6] == 0xFF );
      REQUIRE( record.data.unknown16[7] == 0x00 );
      REQUIRE( record.data.unknown16[8] == 0xFF );
      REQUIRE( record.data.unknown16[9] == 0x00 );
      REQUIRE( record.data.unknown16[10] == 0xFF );
      REQUIRE( record.data.unknown16[11] == 0x00 );
      REQUIRE( record.data.unknown16[12] == 0xFF );
      REQUIRE( record.data.unknown16[13] == 0x00 );
      REQUIRE( record.data.unknown16[14] == 0x00 );
      REQUIRE( record.data.unknown16[15] == 0x00 );
      REQUIRE( record.data.heightMale == 1.0f );
      REQUIRE( record.data.heightFemale == 1.0f );
      REQUIRE( record.data.weightMale == 1.0f );
      REQUIRE( record.data.weightFemale == 1.0f );
      const auto unk96 = std::string_view(reinterpret_cast<const char*>(&record.data.unknown96[0]), 96);
      REQUIRE( unk96 == "\0\x09\x04\0\0\0\x48\x42\0\0\x48\x42\0\0\x48\x42\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0"sv );
      REQUIRE_FALSE( record.data.unknown36.has_value() );
      REQUIRE( record.subBlocks.size() == 72 );
      REQUIRE( record.subBlocks[0].subType == cMNAM );
      REQUIRE( record.subBlocks[0].subData.size() == 0 );
      REQUIRE( record.subBlocks[1].subType == cANAM );
      REQUIRE( record.subBlocks[1].subData.size() == 48 );
      const auto ANAM = std::string_view(reinterpret_cast<const char*>(record.subBlocks[1].subData.data()), record.subBlocks[1].subData.size());
      REQUIRE( ANAM == "Actors\\Witchlight\\Character Assets\\skeleton.nif\0"sv );
      REQUIRE( record.subBlocks[2].subType == cMODT );
      REQUIRE( record.subBlocks[2].subData.size() == 12 );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.subBlocks[2].subData.data()), record.subBlocks[2].subData.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      // some more records, not checking all of them, just the last one
      REQUIRE( record.subBlocks[71].subType == 0x564D4E52 );
      REQUIRE( record.subBlocks[71].subData.size() == 4 );
      const auto RNMV = std::string_view(reinterpret_cast<const char*>(record.subBlocks[71].subData.data()), record.subBlocks[71].subData.size());
      REQUIRE( RNMV == "\x47\x6F\x08\0"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with long DATA sub-record")
    {
      const auto data = "RACE\x81\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x8E\x2B\x1E\0\x2C\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BOD2\x08\0\0\0\0\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\xA4\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0\x7F\xEA}\xC2\0\0\0\0\0\0\0\0\0\0H\xC2\0\0\0\0\0\0\x82\x42\0\0\0\0\0\0\x96\xC3\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xE2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0009AA3C );
      REQUIRE( record.headerRevision == 0x001E2B8E );
      REQUIRE( record.headerVersion == 44 );
      REQUIRE( record.headerUnknown5 == 0x000A );
      // -- record data
      REQUIRE( record.editorID == "SwarmRace" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00004699 );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0x00000000 );
      REQUIRE( record.spellFormIDs.size() == 1 );
      REQUIRE( record.spellFormIDs[0] == 0x0008CDD1 );
      REQUIRE( record.unknownWNAM.has_value() );
      REQUIRE( record.unknownWNAM.value() == 0x00086F42);
      REQUIRE_FALSE( record.unknownBODT.isPresent() );
      REQUIRE( record.unknownBOD2.isPresent() );
      const auto BOD2 = std::string_view(reinterpret_cast<const char*>(record.unknownBOD2.data()), record.unknownBOD2.size());
      REQUIRE( BOD2 == "\0\0\0\0\x02\0\0\0"sv );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x00013795 );
      REQUIRE( record.keywords[1] == 0x0006F6FB );
      REQUIRE( record.data.unknown16[0] == 0xFF );
      REQUIRE( record.data.unknown16[1] == 0x00 );
      REQUIRE( record.data.unknown16[2] == 0xFF );
      REQUIRE( record.data.unknown16[3] == 0x00 );
      REQUIRE( record.data.unknown16[4] == 0xFF );
      REQUIRE( record.data.unknown16[5] == 0x00 );
      REQUIRE( record.data.unknown16[6] == 0xFF );
      REQUIRE( record.data.unknown16[7] == 0x00 );
      REQUIRE( record.data.unknown16[8] == 0xFF );
      REQUIRE( record.data.unknown16[9] == 0x00 );
      REQUIRE( record.data.unknown16[10] == 0xFF );
      REQUIRE( record.data.unknown16[11] == 0x00 );
      REQUIRE( record.data.unknown16[12] == 0xFF );
      REQUIRE( record.data.unknown16[13] == 0x00 );
      REQUIRE( record.data.unknown16[14] == 0x00 );
      REQUIRE( record.data.unknown16[15] == 0x00 );
      REQUIRE( record.data.heightMale == 1.0f );
      REQUIRE( record.data.heightFemale == 1.0f );
      REQUIRE( record.data.weightMale == 1.0f );
      REQUIRE( record.data.weightFemale == 1.0f );
      const auto unk96 = std::string_view(reinterpret_cast<const char*>(&record.data.unknown96[0]), 96);
      REQUIRE( unk96 == "\0\x09\x04\0\0\0\x48\x42\0\0\x48\x42\0\0\x48\x42\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0"sv );
      REQUIRE( record.data.unknown36.has_value() );
      const auto unk36 = std::string_view(reinterpret_cast<const char*>(record.data.unknown36.value().data()), 36);
      REQUIRE( unk36 == "\x7F\xEA}\xC2\0\0\0\0\0\0\0\0\0\0H\xC2\0\0\0\0\0\0\x82\x42\0\0\0\0\0\0\x96\xC3\0\0\0\0"sv );
      REQUIRE( record.subBlocks.size() == 72 );
      REQUIRE( record.subBlocks[0].subType == cMNAM );
      REQUIRE( record.subBlocks[0].subData.size() == 0 );
      REQUIRE( record.subBlocks[1].subType == cANAM );
      REQUIRE( record.subBlocks[1].subData.size() == 48 );
      const auto ANAM = std::string_view(reinterpret_cast<const char*>(record.subBlocks[1].subData.data()), record.subBlocks[1].subData.size());
      REQUIRE( ANAM == "Actors\\Witchlight\\Character Assets\\skeleton.nif\0"sv );
      REQUIRE( record.subBlocks[2].subType == cMODT );
      REQUIRE( record.subBlocks[2].subData.size() == 12 );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.subBlocks[2].subData.data()), record.subBlocks[2].subData.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      // some more records, not checking all of them, just the last one
      REQUIRE( record.subBlocks[71].subType == 0x564D4E52 );
      REQUIRE( record.subBlocks[71].subData.size() == 4 );
      const auto RNMV = std::string_view(reinterpret_cast<const char*>(record.subBlocks[71].subData.data()), record.subBlocks[71].subData.size());
      REQUIRE( RNMV == "\x47\x6F\x08\0"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "RACE\0\0\0\0\x20\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 32 );
      REQUIRE( record.headerFormID == 0x00034D2C );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000D );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0FAIL\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\x02SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before EDID can be read completely")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0Swarm"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: unknown / invalid subrecord")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FAIL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of FULL can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "RACE\x6B\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of DESC can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESC entries")
    {
      const auto data = "RACE\x6B\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPCT is not four")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x03\0\x01\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x62\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x05\0\x01\0\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple SPCT")
    {
      const auto data = "RACE\x75\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SPCT can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }


    SECTION("corrupt data: SPCT is zero")
    {
      const auto data = "RACE\x57\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\0\0\0\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no SPLO after SPCT")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0FAIL\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPLO is not four")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x03\0\xD1\xCD\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x62\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x05\0\xD1\xCD\x08\0\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of SPLO can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WNAMs")
    {
      const auto data = "RACE\x6B\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of WNAM can be read")
    {
      const auto data = "RACE\x6B\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of BODT can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BODTs")
    {
      const auto data = "RACE\x73\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of BODT is not twelve")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0B\0\0\0\0\0\0/V\0\x02\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x62\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0D\0\0\0\0\0\0/V\0\x02\0\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of BOD2 can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BOD2\x0C\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BOD2s")
    {
      const auto data = "RACE\x73\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BOD2\x0C\0\0\0\0\0\0/V\0\x02\0\0\0BOD2\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const auto data = "RACE\x79\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x03\0\x02\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x62\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x05\0\x02\0\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\0\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const auto data = "RACE\x53\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\0\0\0\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0FAIL\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x07\0\x95\x37\x01\0\xFB\xF6\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x59\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\0\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "RACE\xB7\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not 128")
    {
      {
        const auto data = "RACE\x60\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x7F\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "RACE\x62\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x81\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WALKMTNM\x04\0RUN1MTNM\x04\0SNEKMTNM\x04\0BLDOMTNM\x04\0SWIMVTCK\x08\0\xA5\xF6\x01\0\xDD:\x01\0PNAM\x04\0\0\0\xA0@UNAM\x04\0\0\0@@ATKD,\0\0\0\x80?\0\0\x80?x\x1F\x09\0\0\0\0\0\0\0\0\0\0\0\x0C\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?ATKE\x14\0attackStart_Attack1\0NAM1\0\0MNAM\0\0INDX\x04\0\0\0\0\0MODL(\0Actors\\Character\\UpperBodyHumanMale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0INDX\x04\0\0\0\0\0MODL*\0Actors\\Character\\UpperBodyHumanFemale.egt\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0GNAM\x04\0Co\x08\0NAM3\0\0MNAM\0\0MODL(\0Actors\\Witchlight\\WitchlightProject.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MODL!\0Actors\\Character\\DefaultMale.hkx\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0NAM4\x04\0@/\x01\0NAM5\x04\0*\xF8\x01\0ONAM\x04\0\x13P\x0A\0LNAM\x04\0\x14P\x0A\0NAME\x05\0BODY\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0NAME\x01\0\0VNAM\x04\0\x01\xF2\xFF\xFFQNAM\x04\0B?\x01\0UNES\x04\0B?\x01\0WKMV\x04\0Go\x08\0RNMV\x04\0Go\x08\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of sub-blocks can be read")
    {
      const auto data = "RACE\x61\x04\0\0\0\0\0\0\x3C\xAA\x09\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x0A\0SwarmRace\0FULL\x04\0\x99\x46\0\0DESC\x04\0\0\0\0\0SPCT\x04\0\x01\0\0\0SPLO\x04\0\xD1\xCD\x08\0WNAM\x04\0Bo\x08\0BODT\x0C\0\0\0\0\0\0/V\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x95\x37\x01\0\xFB\xF6\x06\0DATA\x80\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0\x80?\0\0\x80?\0\x09\x04\0\0\0HB\0\0HB\0\0HB\0\0HC\xCD\xCC\xCC=\0\0\xC0?\0\0\0@\0\0\0\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0 A\0\0 A\0\0\x80?\0\0\x80\x42\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80>\0\0\xA0@\0\0\0\0MNAM\0\0ANAM0\0Actors\\Witchlight\\Character Assets\\skeleton.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0FNAM\0\0MTNM\x04\0WA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      RaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x00034D2C;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000D;
      // -- record data
      record.editorID = "WhatEver";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "RACE\0\0\0\0\x20\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0"sv;
      REQUIRE( stream.str() == data );
    }
  }

  SECTION("getDataLength")
  {
    RaceRecord record;

    SECTION("older / shorter data records")
    {
      for (uint16_t v = 27; v < 43; ++v)
      {
        record.headerVersion = v;
        REQUIRE( record.getDataLength() == 128 );
      }
    }

    SECTION("newer / longer data records")
    {
      for (uint16_t v = 44; v < 52; ++v)
      {
        record.headerVersion = v;
        REQUIRE( record.getDataLength() == 164 );
      }
    }
  }
}
