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
#include "../../../../../sr/base/records/ActivatorRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ActivatorRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ActivatorRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE_FALSE( record.hasDEST );
    REQUIRE( record.unknownDEST == 0 );
    REQUIRE( record.destructionStructures.empty() );
    REQUIRE( record.keywordArray.empty() );
    REQUIRE_FALSE( record.hasPNAM );
    REQUIRE( record.defaultPrimitiveColourRed == 0 );
    REQUIRE( record.defaultPrimitiveColourGreen == 0 );
    REQUIRE( record.defaultPrimitiveColourBlue == 0 );
    REQUIRE( record.loopingSoundFormID == 0 );
    REQUIRE( record.activateSoundFormID == 0 );
    REQUIRE( record.waterTypeFormID == 0 );
    REQUIRE_FALSE( record.activateTextOverride.isPresent() );
    REQUIRE_FALSE( record.hasFNAM );
    REQUIRE( record.unknownFNAM == 0 );
    REQUIRE( record.interactionKeywordFormID == 0 );
  }

  SECTION("equals")
  {
    ActivatorRecord a;
    ActivatorRecord b;

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

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 12;
        b.unknownOBND[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        a.modelPath = "foo\\bar.nif";
        b.modelPath = "foo2\\bar3.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODT mismatch")
      {
        a.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMODT.setPresence(false);
        b.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODS mismatch")
      {
        a.unknownMODS.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMODS.setPresence(false);
        b.unknownMODS.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DEST mismatch")
      {
        a.hasDEST = false;
        b.hasDEST = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDEST = true;
        a.unknownDEST = 1;
        b.hasDEST = true;
        b.unknownDEST = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("destructionStructures mismatch")
      {
        a.destructionStructures.push_back(ActivatorRecord::destStruct());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywordArray.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywordArray.push_back(0x01234567);
        b.keywordArray.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PNAM mismatch")
      {
        a.hasPNAM = false;
        b.hasPNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasPNAM = true;
        b.hasPNAM = true;

        a.defaultPrimitiveColourRed = 1;
        b.defaultPrimitiveColourRed = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.defaultPrimitiveColourRed = 1;
        a.defaultPrimitiveColourGreen = 1;
        b.defaultPrimitiveColourRed = 1;
        b.defaultPrimitiveColourGreen = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.defaultPrimitiveColourGreen = 1;
        b.defaultPrimitiveColourGreen = 1;
        a.defaultPrimitiveColourBlue = 2;
        b.defaultPrimitiveColourBlue = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("loopingSoundFormID mismatch")
      {
        a.loopingSoundFormID = 0x01234567;
        b.loopingSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("activateSoundFormID mismatch")
      {
        a.activateSoundFormID = 0x01234567;
        b.activateSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("waterTypeFormID mismatch")
      {
        a.waterTypeFormID = 0x01234567;
        b.waterTypeFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("activateTextOverride mismatch")
      {
        a.activateTextOverride = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FNAM mismatch")
      {
        a.hasFNAM = false;
        b.hasFNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("interactionKeywordFormID mismatch")
      {
        a.interactionKeywordFormID = 0x01234567;
        b.interactionKeywordFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    ActivatorRecord record;

    REQUIRE( record.getRecordType() == cACTI );
  }

  SECTION("getWriteSize")
  {
    ActivatorRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 28 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 40 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 52 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 35 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 46 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 55 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 52 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 35 );
    }

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 52 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 35 );
    }

    SECTION("size adjusts when DEST is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.hasDEST = true;

      REQUIRE( record.getWriteSize() == 42 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 48 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts when PNAM is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.hasPNAM = true;

      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when loopingSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.loopingSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when activateSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.activateSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when waterTypeFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.waterTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when activateTextOverride is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.activateTextOverride = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when FNAM is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.hasFNAM = true;

      REQUIRE( record.getWriteSize() == 36 );
    }

    SECTION("size adjusts when interactionKeywordFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.interactionKeywordFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00000FDA, "foo");

    SECTION("default: load record with keywords")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActivatorRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00120000 );
      REQUIRE( record.headerFormID == 0x00021513 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "NorLever01" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0"sv );
      REQUIRE( record.unknownOBND[0] == 0xD2 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xEC );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x2B );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x14 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x5E );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00000FDA );
      REQUIRE( record.modelPath == "Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE_FALSE( record.hasDEST );
      REQUIRE( record.destructionStructures.empty() );
      REQUIRE( record.keywordArray.size() == 2 );
      REQUIRE( record.keywordArray[0] == 0x0006DEAD );
      REQUIRE( record.keywordArray[1] == 0x00100769 );
      // PNAM\x04\0\xCC\x4C\x33\0
      REQUIRE( record.hasPNAM );
      REQUIRE( record.defaultPrimitiveColourRed == 0xCC );
      REQUIRE( record.defaultPrimitiveColourGreen == 0x4C );
      REQUIRE( record.defaultPrimitiveColourBlue == 0x33 );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.activateSoundFormID == 0 );
      REQUIRE( record.waterTypeFormID == 0 );
      REQUIRE_FALSE( record.activateTextOverride.isPresent() );
      REQUIRE( record.hasFNAM );
      REQUIRE( record.unknownFNAM == 0 );
      REQUIRE( record.interactionKeywordFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "ACTI\0\0\0\0\x20\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActivatorRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00120020 );
      REQUIRE( record.headerFormID == 0x00021513 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0FAIL\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\x02NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\xFC\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0FAIL\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "ACTI\xEA\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "ACTI\x0E\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "ACTI\xFB\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0B\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ACTI\xFD\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0D\0\xD2\xFF\0\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "ACTI\x06\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\x02Gungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "ACTI\x35\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "ACTI\xCA\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "ACTI\x26\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODS can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13MODS\x21\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "ACTI\x14\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "ACTI\xFB\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x03\0\x02\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x05\0\x02\0\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\0\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "ACTI\xE8\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\0\0\0\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0FAIL\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "ACTI\xEE\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "ACTI\xFD\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x09\0\xAD\xDE\x06\0\x69\x07\x10\0\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ACTI\xF4\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\0\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PNAMs")
    {
      const std::string_view data = "ACTI\x06\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PNAM is not four")
    {
      {
        const std::string_view data = "ACTI\xFB\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x03\0\xCC\x33\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x05\0\xCC\x4C\x33\0\0FNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of PNAM can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const std::string_view data = "ACTI\x04\x01\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0\0FNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FNAM is not two")
    {
      {
        const std::string_view data = "ACTI\xFB\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "ACTI\xFD\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x03\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ACTI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of FNAM can be read")
    {
      const std::string_view data = "ACTI\xFC\0\0\0\0\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x0B\0NorLever01\0VMAD\x3C\0\x05\0\x02\0\x02\0\x09\0TrapLever\0\x01\0\x0B\0TriggerType\x03\x01\x02\0\0\0\x10\0NorLever01SCRIPT\0\0\0OBND\x0C\0\xD2\xFF\xEC\xFF\0\0\x2B\0\x14\0\x5E\0FULL\x04\0\xDA\x0F\0\0MODL\x33\0Dungeons\\Nordic\\Levers\\RuinsLever01\\NorLever01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xB7\x4D\x67\xC4\x64\x64\x73\0\xE4\xF2\xA5\x13\xF1\xCC\xC0\x34\x64\x64\x73\0\xE4\xF2\xA5\x13KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xAD\xDE\x06\0\x69\x07\x10\0PNAM\x04\0\xCC\x4C\x33\0FNAM\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      ActivatorRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00120020;
      record.headerFormID = 0x00021513;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0002;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ACTI\0\0\0\0\x20\0\x12\0\x13\x15\x02\0\x1B\x69\x55\0\x28\0\x02\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
