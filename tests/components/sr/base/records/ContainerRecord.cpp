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
#include "../../../../../sr/base/records/ContainerRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("ContainerRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ContainerRecord record;

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
    REQUIRE( record.contents.empty() );
    REQUIRE_FALSE( record.unknownCOED.isPresent() );
    REQUIRE( record.flags == 0 );
    REQUIRE( record.weight == 0 );
    REQUIRE( record.openSoundFormID == 0 );
    REQUIRE( record.closeSoundFormID == 0 );
  }

  SECTION("equals")
  {
    ContainerRecord a;
    ContainerRecord b;

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

      SECTION("contents mismatch")
      {
        ComponentData cd;
        cd.formID = 0x01234567;
        cd.count = 2;
        a.contents.push_back(cd);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.contents.push_back(cd);
        b.contents.push_back(cd);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("COED mismatch")
      {
        a.unknownCOED.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownCOED.setPresence(false);
        b.unknownCOED.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("flags mismatch")
      {
        a.flags = 0xAB;
        b.flags = 0xCD;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("weight mismatch")
      {
        a.weight = 1.0f;
        b.weight = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("openSoundFormID mismatch")
      {
        a.openSoundFormID = 0x01234567;
        b.openSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("closeSoundFormID mismatch")
      {
        a.closeSoundFormID = 0x01234567;
        b.closeSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    ContainerRecord record;

    REQUIRE( record.getRecordType() == cCONT );
  }

  SECTION("getWriteSize")
  {
    ContainerRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 39 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 51 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 46 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 49 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 57 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 46 );
    }

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 46 );
    }

    SECTION("size adjusts with length of contents")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      ComponentData cd;
      cd.formID = 0x01234567;
      cd.count = 2;

      record.contents.push_back(cd);
      REQUIRE( record.getWriteSize() == 63 );

      record.contents.push_back(cd);
      REQUIRE( record.getWriteSize() == 77 );
    }

    SECTION("size adjusts with length of COED")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "COED\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownCOED.loadFromStream(stream, cCOED, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );

      {
        const std::string_view data = "COED\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownCOED.loadFromStream(stream, cCOED, true) );
      }
      REQUIRE( record.getWriteSize() == 46 );
    }

    SECTION("size adjusts when openSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      record.openSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 49 );
    }

    SECTION("size adjusts when closeSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 39 );

      record.closeSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 49 );
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
    dummy_table.addString(0x0000DE55, "foo");
    dummy_table.addString(0x0000724D, "bar");

    SECTION("default: load record without content and sounds")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x08000000 );
      REQUIRE( record.headerFormID == 0x000267E7 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000A );
      // -- record data
      REQUIRE( record.editorID == "EvidenceChestStolenGoods" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xD0 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xE7 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x30 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x18 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x30 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000724D );
      REQUIRE( record.modelPath == "Clutter\\Containers\\Chest01\\Chest01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.contents.empty() );
      REQUIRE_FALSE( record.unknownCOED.isPresent() );
      REQUIRE( record.flags == 0x00 );
      REQUIRE( record.weight == 0.0f );
      REQUIRE( record.openSoundFormID == 0 );
      REQUIRE( record.closeSoundFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with sounds")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x08000000 );
      REQUIRE( record.headerFormID == 0x00000845 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000D );
      // -- record data
      REQUIRE( record.editorID == "BarrelFood01" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xE5 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xE5 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x1B );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x1B );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x50 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000DE55 );
      REQUIRE( record.modelPath == "Clutter\\Barrel01.NIF" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.contents.size() == 1 );
      REQUIRE( record.contents[0].formID == 0x0010FCFB );
      REQUIRE( record.contents[0].count == 1 );
      REQUIRE_FALSE( record.unknownCOED.isPresent() );
      REQUIRE( record.flags == 0x02 );
      REQUIRE( record.weight == 0.0f );
      REQUIRE( record.openSoundFormID == 0x0003C86D );
      REQUIRE( record.closeSoundFormID == 0x0003C870 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "CONT\0\0\0\0\x20\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x08000020 );
      REQUIRE( record.headerFormID == 0x00000845 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000D );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0FAIL\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\x02EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x9D\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0FAIL\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "CONT\x8B\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "CONT\xAF\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0B\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0D\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of FULL can be read")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "CONT\xA7\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\x02Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "CONT\xCA\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "CONT\x77\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "CONT\xC7\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of COCT is not four")
    {
      {
        const std::string_view data = "CONT\xAA\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x03\0\x01\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CONT\xAC\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x06\0\x02\0\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: length of DATA is not five")
    {
      {
        const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x04\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x06\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "CONT\x9D\0\0\0\0\0\0\x08\xE7\x67\x02\0\x1B\x69\x55\0\x28\0\x0A\0EDID\x19\0EvidenceChestStolenGoods\0OBND\x0C\0\xD0\xFF\xE7\xFF\0\0\x30\0\x18\0\x30\0FULL\x04\0\x4D\x72\0\0MODL\x27\0Clutter\\Containers\\Chest01\\Chest01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x45\xDA\xC1\xAB\x64\x64\x73\0\xB3\xB8\x24\x94\x8F\x26\x21\x7C\x64\x64\x73\0\xB3\xB8\x24\x94\x44\x41TA\x05\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const std::string_view data = "CONT\xB5\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SNAM can be read")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\0\0\0\0QNAM\x04\0\x70\xC8\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple QNAMs")
    {
      const std::string_view data = "CONT\xB5\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of QNAM can be read")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\x70\xC8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: QNAM is zero")
    {
      const std::string_view data = "CONT\xAB\0\0\0\0\0\0\x08\x45\x08\0\0\x1B\x69\x55\0\x28\0\x0D\0EDID\x0D\0BarrelFood01\0OBND\x0C\0\xE5\xFF\xE5\xFF\0\0\x1B\0\x1B\0\x50\0FULL\x04\0\x55\xDE\0\0MODL\x15\0Clutter\\Barrel01.NIF\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x4E\xFC\x5A\xB9\x64\x64\x73\0\xBF\xFA\x25\xDA\x22\x68\xD8\x4D\x64\x64\x73\0\xBF\xFA\x25\xDA\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\xFB\xFC\x10\0\x01\0\0\0DATA\x05\0\x02\0\0\0\0SNAM\x04\0\x6D\xC8\x03\0QNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      ContainerRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x000B91F0;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0007;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CONT\0\0\0\0\x20\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
