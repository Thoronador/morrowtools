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
#include "../../../../../sr/base/records/PerkRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("PerkRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    PerkRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.subBlocks.empty() );
  }

  SECTION("equals")
  {
    PerkRecord a;
    PerkRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      a.name = LocalizedString(LocalizedString::Type::Index, 1234, "bar");
      a.description = LocalizedString(LocalizedString::Type::Index, 42, "foobar");

      b.editorID = "foo";
      b.name = LocalizedString(LocalizedString::Type::Index, 1234, "bar");
      b.description = LocalizedString(LocalizedString::Type::Index, 42, "foobar");

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
        a.name = LocalizedString(LocalizedString::Type::Index, 1234, "bar");
        b.name = LocalizedString(LocalizedString::Type::Index, 42, "bar bar");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 1234, "bar");
        b.description = LocalizedString(LocalizedString::Type::Index, 42, "bar bar");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("subblock mismatch")
      {
        a.subBlocks.clear();
        // Block is not filled, because just having a block more already is
        // enough to have inequality.
        b.subBlocks.push_back(SubBlock());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    PerkRecord record;

    REQUIRE( record.getRecordType() == cPERK );
  }

  SECTION("getWriteSize")
  {
    PerkRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 22 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 10 );
    }

    SECTION("size adjusts with presence of description and name")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 10 );

      record.description = LocalizedString(LocalizedString::Type::Index, 123, "");
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 321, "");
      REQUIRE( record.getWriteSize() == 30 );

      record.name = LocalizedString();
      REQUIRE( record.getWriteSize() == 20 );
    }

    SECTION("size adjusts with length of sub-blocks")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      uint32_t dummy = 0;
      StringTable dummy_table;
      dummy_table.addString(0x000011A2, "Some name is here.");
      dummy_table.addString(0x000043F9, "This is the description.");

      // read PERK, because header is handled before loadFromStream.
      streamIn.read((char*) &dummy, 4);
      REQUIRE( streamIn.good() );

      REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
      // Size of original record is 172 bytes.
      REQUIRE( record.getWriteSize() == 172 );
      // There are eight blocks.
      REQUIRE( record.subBlocks.size() == 8 );

      record.subBlocks.pop_back();
      // Block PRKF (zero / six bytes) was removed.
      REQUIRE( record.subBlocks.size() == 7 );
      REQUIRE( record.getWriteSize() == 166 );

      record.subBlocks.pop_back();
      // Block EPFD (four / ten bytes) was removed.
      REQUIRE( record.subBlocks.size() == 6 );
      REQUIRE( record.getWriteSize() == 156 );

      record.subBlocks.pop_back();
      // Block EPFT (one / seven bytes) was removed.
      REQUIRE( record.subBlocks.size() == 5 );
      REQUIRE( record.getWriteSize() == 149 );

      record.subBlocks.pop_back();
      // Block DATA (three / nine bytes) was removed.
      REQUIRE( record.subBlocks.size() == 4 );
      REQUIRE( record.getWriteSize() == 140 );

      record.subBlocks.pop_back();
      // Block PKRE (three / nine bytes) was removed.
      REQUIRE( record.subBlocks.size() == 3 );
      REQUIRE( record.getWriteSize() == 131 );

      record.subBlocks.pop_back();
      // Block DATA (five / eleven bytes) was removed.
      REQUIRE( record.subBlocks.size() == 2 );
      REQUIRE( record.getWriteSize() == 120 );

      record.subBlocks.pop_back();
      // Block CTDA (32 / 38 bytes) was removed.
      REQUIRE( record.subBlocks.size() == 1 );
      REQUIRE( record.getWriteSize() == 82 );

      record.subBlocks.pop_back();
      // Another CTDA (32 / 38 bytes) was removed.
      REQUIRE( record.subBlocks.size() == 0 );
      REQUIRE( record.getWriteSize() == 44 );
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

    dummy_table.addString(0x000011A2, "Some name is here.");
    dummy_table.addString(0x000043F9, "This is the description.");

    SECTION("default: load + save record")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream streamIn;
      streamIn.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      streamIn.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( streamIn.good() );

      // Reading should succeed.
      PerkRecord record;
      REQUIRE( record.loadFromStream(streamIn, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00051B12 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000F );
      // -- record data
      REQUIRE( record.editorID == "HuntersDiscipline" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x000011A2 );
      REQUIRE( record.name.getString() == "Some name is here." );
      REQUIRE( record.description.isPresent() );
      REQUIRE( record.description.getIndex() == 0x000043F9 );
      REQUIRE( record.description.getString() == "This is the description." );
      REQUIRE( record.subBlocks.size() == 8 );
      REQUIRE( record.subBlocks[0].subType == cCTDA );
      REQUIRE( record.subBlocks[7].subType == cPRKF );

      // Save it.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read completely")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0FAIL\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\x03HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\xAD\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL")
    {
      const std::string_view data = "PERK\xB6\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before FULL be read completely")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESC")
    {
      const std::string_view data = "PERK\xB6\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before DESC can be read completely")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: description (DESC) is missing")
    {
      const std::string_view data = "PERK\xA2\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0CTDA\x20\0\0\x85\x06\x26\0\0\x80\x3F\xC0\x01\x53\x43\x1C\x5F\x10\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43\x54\x44\x41\x20\0\x60\x85\x06\x26\0\0\x48\x42\x15\x01\x53\x43\x08\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x44\x41\x54\x41\x05\0\0\0\x01\x01\0\x50\x52\x4B\x45\x03\0\x02\0\0\x44\x41\x54\x41\x03\0\x15\x01\x01\x45\x50\x46\x54\x01\0\x01\x45\x50\x46\x44\x04\0\0\0\x84\x42\x50\x52\x4B\x46\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before CTDA be read completely")
    {
      const std::string_view data = "PERK\xAC\0\0\0\0\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x12\0HuntersDiscipline\0FULL\x04\0\xA2\x11\0\0DESC\x04\0\xF9\x43\0\0CTDA\x20\0\0\x85\x06"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PERK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PerkRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      PerkRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x00051B12;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000F;
      // -- record data
      record.editorID = "HuntersDiscipline";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "PERK\0\0\0\0\x20\0\0\0\x12\x1B\x05\0\x1B\x69\x55\0\x28\0\x0F\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
