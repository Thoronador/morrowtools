/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2025  Dirk Stolle

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
#include "../../../../lib/sr/records/TalkingActivatorRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("TalkingActivatorRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    TalkingActivatorRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.unknownPNAM == 0 );
    REQUIRE( record.loopingSoundFormID == 0 );
    REQUIRE( record.unknownFNAM == 0 );
    REQUIRE( record.voiceTypeFormID == 0 );
  }

  SECTION("equals")
  {
    TalkingActivatorRecord a;
    TalkingActivatorRecord b;

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

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 0xFF;
        b.unknownOBND[11] = 0x00;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 123, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.name = LocalizedString(LocalizedString::Type::Index, 456, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.name = LocalizedString(LocalizedString::Type::None, 0, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        a.modelPath = "foo.nif";
        b.modelPath = "bar\\foo.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODT mismatch")
      {
        a.unknownMODT.setPresence(true);
        b.unknownMODT.setPresence(false);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keyword array mismatch")
      {
        a.keywords.clear();
        b.keywords.push_back(0x12345678);
        b.keywords.push_back(0x90ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PNAM mismatch")
      {
        a.unknownPNAM = 1;
        b.unknownPNAM = 10;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("loopingSoundFormID mismatch")
      {
        a.loopingSoundFormID = 1;
        b.loopingSoundFormID = 10;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FNAM mismatch")
      {
        a.unknownFNAM = 1;
        b.unknownFNAM = 10;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("voiceTypeFormID mismatch")
      {
        a.voiceTypeFormID = 1;
        b.voiceTypeFormID = 10;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    TalkingActivatorRecord record;

    REQUIRE( record.getRecordType() == cTACT );
  }

  SECTION("getWriteSize")
  {
    TalkingActivatorRecord record;
    record.editorID = "foo";
    record.modelPath = "bar.nif";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 60 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 72 );
    }

    SECTION("size adjusts with presence of name")
    {
      REQUIRE( record.getWriteSize() == 60 );

      record.name = LocalizedString(LocalizedString::Type::Index, 0x000003B2, "");
      REQUIRE( record.getWriteSize() == 70 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.modelPath = "bar.nif";
      REQUIRE( record.getWriteSize() == 60 );

      record.modelPath = "barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.unknownMODT.setPresence(false);
      REQUIRE( record.getWriteSize() == 60 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 84 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 67 );
    }

    SECTION("size adjusts with presence of keywords")
    {
      record.keywords.clear();
      REQUIRE( record.getWriteSize() == 60 );

      record.keywords.push_back(0x00000001);
      REQUIRE( record.getWriteSize() == 80 );
      record.keywords.push_back(0x0000000F);
      REQUIRE( record.getWriteSize() == 84 );
      record.keywords.push_back(0x0000AFFE);
      REQUIRE( record.getWriteSize() == 88 );
    }

    SECTION("size adjusts with presence of loopingSoundFormID and voiceTypeFormID")
    {
      REQUIRE( record.getWriteSize() == 60 );

      record.loopingSoundFormID = 0xAFFEAFFE;
      REQUIRE( record.getWriteSize() == 70 );

      record.voiceTypeFormID = 0x12345678;
      REQUIRE( record.getWriteSize() == 80 );
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
    dummy_table.addString(0x0000A1E0, "foo");

    SECTION("default: load + save sane record")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      streamIn.seekg(4);
      REQUIRE( streamIn.good() );

      // Reading should succeed.
      TalkingActivatorRecord record;
      REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001C4E8 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0008 );
      // -- record data
      REQUIRE( record.editorID == "DA03ClavicusVileShrine" );
      REQUIRE( record.unknownOBND[0] == 0x74 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0x6D );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x03 );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x8C );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x93 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0xFD );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000A1E0 );
      REQUIRE( record.modelPath == "Clutter\\Statues\\ClavicusVileShrine01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      REQUIRE( record.unknownMODT.size() == 36 );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7"sv);
      REQUIRE( record.keywords.empty() );
      REQUIRE( record.unknownPNAM == 0 );
      REQUIRE( record.loopingSoundFormID == 0 );
      REQUIRE( record.unknownFNAM == 0 );
      REQUIRE( record.voiceTypeFormID == 0x00028CA4 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream too short to read even header data")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0FAIL\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\x02UU03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\xAE\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0FAIL\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0B\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0D\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before OBND's length")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before FULL can be read")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL records")
    {
      const std::string_view data = "TACT\xB8\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: FULL has index zero")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\0\0\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no MODL")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0FAIL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing MODL entry")
    {
      const std::string_view data = "TACT\x7F\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\x02Glutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\xAE\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODL entries")
    {
      const std::string_view data = "TACT\xB4\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: empty MODL")
    {
      const std::string_view data = "TACT\x86\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODT entries")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before MODT can be read completely")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PNAM is not four")
    {
      {
        const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x03\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "TACT\xAF\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x05\0\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple PNAM entries")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0PNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FNAM entries")
    {
      const std::string_view data = "TACT\xB6\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FNAM is not two")
    {
      {
        const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x01\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "TACT\xAF\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x03\0\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before FNAM can be read completely")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of VNAM is not four")
    {
      {
        const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x03\0\xA4\x8C\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "TACT\xAF\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x05\0\xA4\x8C\x02\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TACT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TalkingActivatorRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple VNAM entries")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7VNAM\x04\0\xA4\x8C\x02\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: VNAM is zero")
    {
      const std::string_view data = "TACT\xAE\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\xF9\x20\x9B\x23\x64\x64\x73\0\xE0\x4D\xF5\xD7\x63\x54\x39\x69\x64\x64\x73\0\xE0\x4D\xF5\xD7PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TACT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TalkingActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save record with keywords")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      TalkingActivatorRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0;
      record.headerFormID = 0x0001C4E8;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0008;
      // -- record data
      record.editorID = "DA03ClavicusVileShrine";
      record.unknownOBND[0] = 0x74;
      record.unknownOBND[1] = 0xFF;
      record.unknownOBND[2] = 0x6D;
      record.unknownOBND[3] = 0xFF;
      record.unknownOBND[4] = 0x03;
      record.unknownOBND[5] = 0xFF;
      record.unknownOBND[6] = 0x8C;
      record.unknownOBND[7] = 0x00;
      record.unknownOBND[8] = 0x93;
      record.unknownOBND[9] = 0x00;
      record.unknownOBND[10] = 0xFD;
      record.unknownOBND[11] = 0x00;

      record.name = LocalizedString(LocalizedString::Type::Index, 0x0000A1E0, "");
      record.modelPath = "Clutter\\Statues\\ClavicusVileShrine01.nif";
      record.unknownMODT.setPresence(false);
      REQUIRE( record.keywords.empty() );
      record.keywords.push_back(0x78563412);
      record.unknownPNAM = 0;
      record.loopingSoundFormID = 0;
      record.unknownFNAM = 0;
      record.voiceTypeFormID = 0x00028CA4;

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "TACT\x98\0\0\0\0\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0EDID\x17\0DA03ClavicusVileShrine\0OBND\x0C\0\x74\xFF\x6D\xFF\x03\xFF\x8C\0\x93\0\xFD\0FULL\x04\0\xE0\xA1\0\0MODL\x29\0Clutter\\Statues\\ClavicusVileShrine01.nif\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\x12\x34\x56\x78PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0VNAM\x04\0\xA4\x8C\x02\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      TalkingActivatorRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x0001C4E8;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0008;
      // -- record data
      record.editorID = "DA03ClavicusVileShrine";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "TACT\0\0\0\0\x20\0\0\0\xE8\xC4\x01\0\x1B\x69\x55\0\x28\0\x08\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
