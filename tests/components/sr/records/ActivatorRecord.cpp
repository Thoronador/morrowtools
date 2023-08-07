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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/ActivatorRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

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
    REQUIRE_FALSE( record.unknownDEST.has_value() );
    REQUIRE( record.destructionStructures.empty() );
    REQUIRE( record.keywords.empty() );
    REQUIRE_FALSE( record.defaultPrimitiveColour.has_value() );
    REQUIRE( record.loopingSoundFormID == 0 );
    REQUIRE( record.activateSoundFormID == 0 );
    REQUIRE( record.waterTypeFormID == 0 );
    REQUIRE_FALSE( record.activateTextOverride.isPresent() );
    REQUIRE_FALSE( record.unknownFNAM.has_value() );
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
        a.unknownDEST = {};
        b.unknownDEST = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDEST = 1;
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
        a.keywords.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywords.push_back(0x01234567);
        b.keywords.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PNAM mismatch")
      {
        a.defaultPrimitiveColour = {};
        b.defaultPrimitiveColour = ActivatorRecord::Colour();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.defaultPrimitiveColour = ActivatorRecord::Colour();
        b.defaultPrimitiveColour = ActivatorRecord::Colour();

        a.defaultPrimitiveColour.value().red = 1;
        b.defaultPrimitiveColour.value().red = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.defaultPrimitiveColour.value().red = 1;
        a.defaultPrimitiveColour.value().green = 1;
        b.defaultPrimitiveColour.value().red = 1;
        b.defaultPrimitiveColour.value().green = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.defaultPrimitiveColour.value().green = 1;
        b.defaultPrimitiveColour.value().green = 1;
        a.defaultPrimitiveColour.value().blue = 2;
        b.defaultPrimitiveColour.value().blue = 3;

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
        a.unknownFNAM = {};
        b.unknownFNAM = 1;

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

      record.unknownDEST = 1;

      REQUIRE( record.getWriteSize() == 42 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 48 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("size adjusts when PNAM is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 28 );

      record.defaultPrimitiveColour = ActivatorRecord::Colour();

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

      record.unknownFNAM = 5;

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
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.destructionStructures.empty() );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x0006DEAD );
      REQUIRE( record.keywords[1] == 0x00100769 );
      REQUIRE( record.defaultPrimitiveColour.has_value() );
      REQUIRE( record.defaultPrimitiveColour.value().red == 0xCC );
      REQUIRE( record.defaultPrimitiveColour.value().green == 0x4C );
      REQUIRE( record.defaultPrimitiveColour.value().blue == 0x33 );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.activateSoundFormID == 0 );
      REQUIRE( record.waterTypeFormID == 0 );
      REQUIRE_FALSE( record.activateTextOverride.isPresent() );
      REQUIRE( record.unknownFNAM.has_value() );
      REQUIRE( record.unknownFNAM.value() == 0 );
      REQUIRE( record.interactionKeywordFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with SNAM and WNAM")
    {
      const auto data = "ACTI\x88\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0\x03\0WNAM\x04\0)T\x01\0FNAM\x02\0\x01\0"sv;
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
      REQUIRE( record.headerFlags == 0x00010000 );
      REQUIRE( record.headerFormID == 0x0001608E );
      REQUIRE( record.headerRevision == 0x003A0204 );
      REQUIRE( record.headerVersion == 31 );
      REQUIRE( record.headerUnknown5 == 0x0004 );
      // -- record data
      REQUIRE( record.editorID == "TundraStreamStraight01WaterA" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0x33 );
      REQUIRE( record.unknownOBND[1] == 0xFE );
      REQUIRE( record.unknownOBND[2] == 0x22 );
      REQUIRE( record.unknownOBND[3] == 0xFB );
      REQUIRE( record.unknownOBND[4] == 0xCB );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x91 );
      REQUIRE( record.unknownOBND[7] == 0x01 );
      REQUIRE( record.unknownOBND[8] == 0x8B );
      REQUIRE( record.unknownOBND[9] == 0x04 );
      REQUIRE( record.unknownOBND[10] == 0x83 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.modelPath == "Water\\TundraStreamStraight01WaterA.nif" );
      REQUIRE_FALSE( record.unknownMODT.isPresent() );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.destructionStructures.empty() );
      REQUIRE( record.keywords.empty() );
      REQUIRE( record.defaultPrimitiveColour.has_value() );
      REQUIRE( record.defaultPrimitiveColour.value().red == 0xCC );
      REQUIRE( record.defaultPrimitiveColour.value().green == 0x4C );
      REQUIRE( record.defaultPrimitiveColour.value().blue == 0x33 );
      REQUIRE( record.loopingSoundFormID == 0x0003D0A7 );
      REQUIRE( record.activateSoundFormID == 0 );
      REQUIRE( record.waterTypeFormID == 0x00015429 );
      REQUIRE_FALSE( record.activateTextOverride.isPresent() );
      REQUIRE( record.unknownFNAM.has_value() );
      REQUIRE( record.unknownFNAM.value() == 1 );
      REQUIRE( record.interactionKeywordFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with KNAM")
    {
      const auto data = "ACTI\x8C\0\0\0\0\0\0\0\x45\xF2\x02\0\x13\x64\x36\0\x23\0\x07\0EDID\x19\0FXcameraAttachEffectsACT\0VMAD\x39\0\x04\0\x01\0\x01\0\x14\0FXCameraAttachScript\0\x01\0\x0E\0CameraAttachFX\x01\x01-Y\x05\0\xFF\xFF\0\0OBND\x0C\0\xA8\xFD\xF9\xFD\x9B\xFDX\x02\x07\x02\x65\x02PNAM\x04\0\xCCL3\0FNAM\x02\0\0\0KNAM\x04\0\x06\x93\x09\0"sv;
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
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0002F245 );
      REQUIRE( record.headerRevision == 0x00366413 );
      REQUIRE( record.headerVersion == 35 );
      REQUIRE( record.headerUnknown5 == 0x0007 );
      // -- record data
      REQUIRE( record.editorID == "FXcameraAttachEffectsACT" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x04\0\x01\0\x01\0\x14\0FXCameraAttachScript\0\x01\0\x0E\0CameraAttachFX\x01\x01-Y\x05\0\xFF\xFF\0\0"sv );
      REQUIRE( record.unknownOBND[0] == 0xA8 );
      REQUIRE( record.unknownOBND[1] == 0xFD );
      REQUIRE( record.unknownOBND[2] == 0xF9 );
      REQUIRE( record.unknownOBND[3] == 0xFD );
      REQUIRE( record.unknownOBND[4] == 0x9B );
      REQUIRE( record.unknownOBND[5] == 0xFD );
      REQUIRE( record.unknownOBND[6] == 0x58 );
      REQUIRE( record.unknownOBND[7] == 0x02 );
      REQUIRE( record.unknownOBND[8] == 0x07 );
      REQUIRE( record.unknownOBND[9] == 0x02 );
      REQUIRE( record.unknownOBND[10] == 0x65 );
      REQUIRE( record.unknownOBND[11] == 0x02 );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.modelPath.empty() );
      REQUIRE_FALSE( record.unknownMODT.isPresent() );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.destructionStructures.empty() );
      REQUIRE( record.keywords.empty() );
      REQUIRE( record.defaultPrimitiveColour.has_value() );
      REQUIRE( record.defaultPrimitiveColour.value().red == 0xCC );
      REQUIRE( record.defaultPrimitiveColour.value().green == 0x4C );
      REQUIRE( record.defaultPrimitiveColour.value().blue == 0x33 );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.activateSoundFormID == 0 );
      REQUIRE( record.waterTypeFormID == 0 );
      REQUIRE_FALSE( record.activateTextOverride.isPresent() );
      REQUIRE( record.unknownFNAM.has_value() );
      REQUIRE( record.unknownFNAM.value() == 0 );
      REQUIRE( record.interactionKeywordFormID == 0x00099306 );

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

    SECTION("corrupt data: multiple SNAMs")
    {
      const auto data = "ACTI\x92\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0\x03\0SNAM\x04\0\xA7\xD0\x03\0WNAM\x04\0)T\x01\0FNAM\x02\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SNAM can be read")
    {
      const auto data = "ACTI\x88\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const auto data = "ACTI\x88\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\0\0\0\0WNAM\x04\0)T\x01\0FNAM\x02\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WNAMs")
    {
      const auto data = "ACTI\x92\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0\x03\0WNAM\x04\0)T\x01\0WNAM\x04\0)T\x01\0FNAM\x02\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of WNAM can be read")
    {
      const auto data = "ACTI\x88\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0\x03\0WNAM\x04\0)T"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: WNAM is zero")
    {
      const auto data = "ACTI\x88\0\0\0\0\0\x01\0\x8E\x60\x01\0\x04\x02\x3A\0\x1F\0\x04\0EDID\x1D\0TundraStreamStraight01WaterA\0OBND\x0C\0\x33\xFE\"\xFB\xCB\xFF\x91\x01\x8B\x04\x83\0MODL\x27\0Water\\TundraStreamStraight01WaterA.nif\0PNAM\x04\0\xCCL3\0SNAM\x04\0\xA7\xD0\x03\0WNAM\x04\0\0\0\0\0FNAM\x02\0\x01\0"sv;
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

    SECTION("corrupt data: multiple KNAMs")
    {
      const auto data = "ACTI\x96\0\0\0\0\0\0\0\x45\xF2\x02\0\x13\x64\x36\0\x23\0\x07\0EDID\x19\0FXcameraAttachEffectsACT\0VMAD\x39\0\x04\0\x01\0\x01\0\x14\0FXCameraAttachScript\0\x01\0\x0E\0CameraAttachFX\x01\x01-Y\x05\0\xFF\xFF\0\0OBND\x0C\0\xA8\xFD\xF9\xFD\x9B\xFDX\x02\x07\x02\x65\x02PNAM\x04\0\xCCL3\0FNAM\x02\0\0\0KNAM\x04\0\x06\x93\x09\0KNAM\x04\0\x06\x93\x09\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of KNAM can be read")
    {
      const auto data = "ACTI\x8C\0\0\0\0\0\0\0\x45\xF2\x02\0\x13\x64\x36\0\x23\0\x07\0EDID\x19\0FXcameraAttachEffectsACT\0VMAD\x39\0\x04\0\x01\0\x01\0\x14\0FXCameraAttachScript\0\x01\0\x0E\0CameraAttachFX\x01\x01-Y\x05\0\xFF\xFF\0\0OBND\x0C\0\xA8\xFD\xF9\xFD\x9B\xFDX\x02\x07\x02\x65\x02PNAM\x04\0\xCCL3\0FNAM\x02\0\0\0KNAM\x04\0\x06\x93"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KNAM is zero")
    {
      const auto data = "ACTI\x8C\0\0\0\0\0\0\0\x45\xF2\x02\0\x13\x64\x36\0\x23\0\x07\0EDID\x19\0FXcameraAttachEffectsACT\0VMAD\x39\0\x04\0\x01\0\x01\0\x14\0FXCameraAttachScript\0\x01\0\x0E\0CameraAttachFX\x01\x01-Y\x05\0\xFF\xFF\0\0OBND\x0C\0\xA8\xFD\xF9\xFD\x9B\xFDX\x02\x07\x02\x65\x02PNAM\x04\0\xCCL3\0FNAM\x02\0\0\0KNAM\x04\0\0\0\0\0"sv;
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
