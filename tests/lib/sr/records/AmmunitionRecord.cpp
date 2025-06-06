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
#include "../../../../lib/sr/records/AmmunitionRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("AmmunitionRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    AmmunitionRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE_FALSE( record.description.isPresent() );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.projectileFormID == 0 );
    REQUIRE( record.DATAflags == 0 );
    REQUIRE( record.baseDamage == 0 );
    REQUIRE( record.value == 0 );
    REQUIRE_FALSE( record.weight.has_value() );
  }

  SECTION("equals")
  {
    AmmunitionRecord a;
    AmmunitionRecord b;

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

      SECTION("pickupSoundFormID mismatch")
      {
        a.pickupSoundFormID = 0x01234567;
        b.pickupSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("putdownSoundFormID mismatch")
      {
        a.putdownSoundFormID = 0x01234567;
        b.putdownSoundFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywords.push_back(0xDEADBEEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("projectileFormID mismatch")
      {
        a.projectileFormID = 0x01234567;
        b.projectileFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DataFlags mismatch")
      {
        a.DATAflags = 12;
        b.DATAflags = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("baseDamage mismatch")
      {
        a.baseDamage = 12;
        b.baseDamage = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("value mismatch")
      {
        a.value = 12;
        b.value = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("weight mismatch")
      {
        a.weight.reset();
        b.weight = 1.25f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.weight = 1.25f;
        b.weight.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.weight = 2.0f;
        b.weight = 1.25f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    AmmunitionRecord record;

    REQUIRE( record.getRecordType() == cAMMO );
  }

  SECTION("getWriteSize")
  {
    AmmunitionRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 70 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 82 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 80 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 88 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 97 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 94 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 77 );
    }

    SECTION("size adjusts when description is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      record.description = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 80 );
    }

    SECTION("size adjusts with length of keyword array")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      record.keywords.push_back(0x00C0FFEE);
      REQUIRE( record.getWriteSize() == 90 );
      record.keywords.push_back(0x001EA1EA);
      REQUIRE( record.getWriteSize() == 94 );
      record.keywords.push_back(0x0F00BA12);
      REQUIRE( record.getWriteSize() == 98 );
    }

    SECTION("size adjusts when weight is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 70 );

      record.weight = 0.1f;
      REQUIRE( record.getWriteSize() == 74 );
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
    dummy_table.addString(0x00007DCD, "foo");
    dummy_table.addString(0x00012470, "bar");
    dummy_table.addString(0x0000004A, "baz");

    SECTION("default: load record")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AmmunitionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001397D );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000F );
      // -- record data
      REQUIRE( record.editorID == "IronArrow" );
      REQUIRE( record.unknownOBND[0] == 0xFF );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xC6 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFE );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x02 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x02 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00007DCD );
      REQUIRE( record.modelPath == "Weapons\\Iron\\IronArrow.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75"sv );
      REQUIRE( record.pickupSoundFormID == 0x0003E7B7 );
      REQUIRE( record.putdownSoundFormID == 0x0003E877 );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000917E7 );
      REQUIRE( record.projectileFormID == 0x0003BE11 );
      REQUIRE( record.DATAflags == 0 );
      REQUIRE( record.baseDamage == 8.0f );
      REQUIRE( record.value == 1 );
      REQUIRE_FALSE( record.weight.has_value() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with 20 byte DATA including weight")
    {
      const auto data = "AMMO\x82\0\0\0\0\0\0\0\x8C\xEC\x10\0\x6C\x27\x1E\0\x2C\0\x03\0EDID\x14\0DwarvenSphereBolt02\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0p$\x01\0YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0w\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x14\0\x36\xB9\x07\0\x06\0\0\0\0\0pA\0\0\0\0\xCD\xCC\xCC="sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AmmunitionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010EC8C );
      REQUIRE( record.headerRevision == 0x001E276C );
      REQUIRE( record.headerVersion == 44 );
      REQUIRE( record.headerUnknown5 == 0x0003 );
      // -- record data
      REQUIRE( record.editorID == "DwarvenSphereBolt02" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0x00 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00012470 );
      REQUIRE( record.modelPath.empty() );
      REQUIRE_FALSE( record.unknownMODT.isPresent() );
      REQUIRE( record.pickupSoundFormID == 0x0003E7B7 );
      REQUIRE( record.putdownSoundFormID == 0x0003E877 );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000917E7 );
      REQUIRE( record.projectileFormID == 0x0007B936 );
      REQUIRE( record.DATAflags == 0x00000006 );
      REQUIRE( record.baseDamage == 15.0f );
      REQUIRE( record.value == 0 );
      REQUIRE( record.weight.has_value() );
      REQUIRE( record.weight.value() == 0.1f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with multiple keywords")
    {
      const auto data = "AMMO\xD2\0\0\0\0\0\0\0\x3A\x08\0\x05\x1E-\x1E\0,\0\x01\0EDID\x15\0ccBGSSSE037_IronBolt\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0J\0\0\0MODL3\0CreationClub\\\x42GSSSE037\\Weapons\\Iron\\IronBolt01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0w\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xE7\x17\x09\0\x18\xE7\x01\0DATA\x14\0;\x08\0\x05\0\0\0\0\0\0\0A\x01\0\0\0\xCD\xCC\xCC="sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AmmunitionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0500083A );
      REQUIRE( record.headerRevision == 0x001E2D1E );
      REQUIRE( record.headerVersion == 44 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "ccBGSSSE037_IronBolt" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0x00 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000004A );
      REQUIRE( record.modelPath == "CreationClub\\BGSSSE037\\Weapons\\Iron\\IronBolt01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.pickupSoundFormID == 0x0003E7B7 );
      REQUIRE( record.putdownSoundFormID == 0x0003E877 );
      REQUIRE( record.description.getType() == LocalizedString::Type::Index );
      REQUIRE( record.description.getIndex() == 0 );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x000917E7 );
      REQUIRE( record.keywords[1] == 0x0001E718 );
      REQUIRE( record.projectileFormID == 0x0500083B );
      REQUIRE( record.DATAflags == 0x00000000 );
      REQUIRE( record.baseDamage == 8.0f );
      REQUIRE( record.value == 1 );
      REQUIRE( record.weight.has_value() );
      REQUIRE( record.weight.value() == 0.1f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "AMMO\0\0\0\0\x20\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AmmunitionRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 32 );
      REQUIRE( record.headerFormID == 0x0001397D );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000F );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "AMMO\0\0\0\0\x20\0\0\0\x7D\x39"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0FAIL\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\x02IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\xBF\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0FAIL\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "AMMO\xAD\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "AMMO\xD1\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0B\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0D\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "AMMO\xC9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\x02Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "AMMO\xE0\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "AMMO\xA5\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x01\0\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "AMMO\xE9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "AMMO\xC9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\0\0\0\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const std::string_view data = "AMMO\xC9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\0\0\0\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of DESC can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESC entries")
    {
      const std::string_view data = "AMMO\xC9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DESC subrecord")
    {
      const std::string_view data = "AMMO\xB5\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      const std::string_view data = "AMMO\xC0\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four")
    {
      const std::string_view data = "AMMO\xC2\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x07\0\xE7\x17\x09\0\0\0\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "AMMO\xD5\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0DATA\x10\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not 16")
    {
      {
        const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x0F\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x11\0\x11\xBE\x03\0\0\0\0\0\0\0\0\x41\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: length of DATA is not 20")
    {
      {
        const auto data = "AMMO\x81\0\0\0\0\0\0\0\x8C\xEC\x10\0\x6C\x27\x1E\0\x2C\0\x03\0EDID\x14\0DwarvenSphereBolt02\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0p$\x01\0YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0w\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x13\0\x36\xB9\x07\0\x06\0\0\0\0\0pA\0\0\0\0\xCD\xCC\xCC="sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "AMMO\x83\0\0\0\0\0\0\0\x8C\xEC\x10\0\x6C\x27\x1E\0\x2C\0\x03\0EDID\x14\0DwarvenSphereBolt02\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0p$\x01\0YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0w\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x15\0\x36\xB9\x07\0\x06\0\0\0\0\0pA\0\0\0\0\xCD\xCC\xCC=\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip AMMO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        AmmunitionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "AMMO\xBF\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0DATA\x10\0\x11\xBE\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA subrecord")
    {
      const std::string_view data = "AMMO\xA9\0\0\0\0\0\0\0\x7D\x39\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x0A\0IronArrow\0OBND\x0C\0\xFF\xFF\xC6\xFF\xFE\xFF\x02\0\0\0\x02\0FULL\x04\0\xCD\x7D\0\0MODL\x1B\0Weapons\\Iron\\IronArrow.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x42\x92\x9F\x41\x64\x64\x73\0\x68\xEF\xBA\x75\xB4\x93\x67\x4B\x64\x64\x73\0\x68\xEF\xBA\x75YNAM\x04\0\xB7\xE7\x03\0ZNAM\x04\0\x77\xE8\x03\0DESC\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xE7\x17\x09\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip AMMO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AmmunitionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      AmmunitionRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x000B91F0;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0007;
      // -- record data
      record.editorID = "ThrowingBanana";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "AMMO\0\0\0\0\x20\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
