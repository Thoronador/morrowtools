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
#include "../../../../../sr/base/records/FloraRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("FloraRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    FloraRecord record;

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
    REQUIRE( record.unknownPNAM == 0 );
    REQUIRE_FALSE( record.activateTextOverride.isPresent() );
    REQUIRE( record.unknownFNAM == 0 );
    REQUIRE( record.ingredientFormID == 0 );
    REQUIRE( record.harvestSoundFormID == 0 );
    REQUIRE( record.unknownPFPC == 0 );
  }

  SECTION("equals")
  {
    FloraRecord a;
    FloraRecord b;

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

      SECTION("PNAM mismatch")
      {
        a.unknownPNAM = 1;
        b.unknownPNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("activateTextOverride mismatch")
      {
        a.activateTextOverride = LocalizedString(LocalizedString::Type::Index, 2, "");
        b.activateTextOverride = LocalizedString(LocalizedString::Type::Index, 3, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FNAM mismatch")
      {
        a.unknownFNAM = 1;
        b.unknownFNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ingredientFormID mismatch")
      {
        a.ingredientFormID = 0x01234567;
        b.ingredientFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("harvestSoundFormID mismatch")
      {
        a.harvestSoundFormID = 0x01234567;
        b.harvestSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PFPC mismatch")
      {
        a.unknownPFPC = 1;
        b.unknownPFPC = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    FloraRecord record;

    REQUIRE( record.getRecordType() == cFLOR );
  }

  SECTION("getWriteSize")
  {
    FloraRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 56 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts when VMAD is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.unknownVMAD.setPresence(true);
      REQUIRE( record.getWriteSize() == 62 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 74 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 83 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts when activateTextOverride is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.activateTextOverride = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts when ingredientFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.ingredientFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts when harvestSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      record.harvestSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 66 );
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
    dummy_table.addString(0x00000DBE, "foo");
    dummy_table.addString(0x0001052F, "bar");

    SECTION("default: load record")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FloraRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00034D2C );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000D );
      // -- record data
      REQUIRE( record.editorID == "HangingFrostMirriam" );
      REQUIRE( record.unknownOBND[0] == 0xEC );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xEC );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xD0 );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x14 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x0E );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x08 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00000DBE );
      REQUIRE( record.modelPath == "Plants\\HangingFrostMirriam01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFA"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.unknownPNAM == 0 );
      REQUIRE( record.activateTextOverride.isPresent() );
      REQUIRE( record.activateTextOverride.getType() == LocalizedString::Type::Index );
      REQUIRE( record.activateTextOverride.getIndex() == 0x0001052F );
      REQUIRE( record.unknownFNAM == 0 );
      REQUIRE( record.ingredientFormID == 0x00034D32 );
      REQUIRE( record.harvestSoundFormID == 0x00100F88 );
      REQUIRE( record.unknownPFPC == 0x64646464 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "FLOR\0\0\0\0\x20\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FloraRecord record;
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
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0FAIL\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\x02HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\xBC\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0FAIL\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "FLOR\xAF\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "FLOR\xD3\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0B\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FLOR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FloraRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0D\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FLOR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FloraRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of FULL can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\x02Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODL\x0A\0foo12.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "FLOR\xA1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "FLOR\xEB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAMODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of MODT can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PNAMs")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0PNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PNAM can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple RNAMs")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of RNAM can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing FNAM")
    {
      const std::string_view data = "FLOR\xB9\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const std::string_view data = "FLOR\xC9\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FNAM is not two")
    {
      {
        const std::string_view data = "FLOR\xC0\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x01\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FLOR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FloraRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FLOR\xC2\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x03\0\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FLOR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FloraRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of FNAM can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PFIGs")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PFIG can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: PFIG is zero")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\0\0\0\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SNAM can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\0\0\0\0PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PFPCs")
    {
      const std::string_view data = "FLOR\xCB\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64\x64\x64PFPC\x04\0\x64\x64\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of PFPC can be read")
    {
      const std::string_view data = "FLOR\xC1\0\0\0\0\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x14\0HangingFrostMirriam\0OBND\x0C\0\xEC\xFF\xEC\xFF\xD0\xFF\x14\0\x0E\0\x08\0FULL\x04\0\xBE\x0D\0\0MODL\x21\0Plants\\HangingFrostMirriam01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x8C\xF0\x72\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA\xED\x02\xA2\xBD\x64\x64\x73\0\x13\x4C\x1E\xFAPNAM\x04\0\0\0\0\0RNAM\x04\0\x2F\x05\x01\0FNAM\x02\0\0\0PFIG\x04\0\x32\x4D\x03\0SNAM\x04\0\x88\x0F\x10\0PFPC\x04\0\x64\x64"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FLOR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FloraRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      FloraRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x00034D2C;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000D;
      // -- record data
      record.editorID = "HangingFrostMirriam";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "FLOR\0\0\0\0\x20\0\0\0\x2C\x4D\x03\0\x1B\x69\x55\0\x28\0\x0D\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
