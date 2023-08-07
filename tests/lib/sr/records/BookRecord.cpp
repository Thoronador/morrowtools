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
#include "../../../../lib/sr/records/BookRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("BookRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    BookRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE_FALSE( record.title.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE_FALSE( record.text.isPresent() );
    REQUIRE( record.pickupSoundFormID == 0 );
    REQUIRE( record.putdownSoundFormID == 0 );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.bookFlags == 0 );
    REQUIRE( record.spellOrSkillID == 0 );
    REQUIRE( record.bookValue == 0 );
    REQUIRE( record.weight == 0.0f );
    REQUIRE( record.inventoryArtFormID == 0 );
    REQUIRE( record.unknownCNAM == 0 );
  }

  SECTION("equals")
  {
    BookRecord a;
    BookRecord b;

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

      SECTION("title mismatch")
      {
        a.title = LocalizedString(LocalizedString::Type::Index, 2, "");

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

      SECTION("text mismatch")
      {
        a.text = LocalizedString(LocalizedString::Type::Index, 2, "");

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

      SECTION("flags mismatch")
      {
        a.bookFlags = 1;
        b.bookFlags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("spellOrSkillID mismatch")
      {
        a.spellOrSkillID = 1;
        b.spellOrSkillID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("value mismatch")
      {
        a.bookValue = 1;
        b.bookValue = 2;

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

      SECTION("inventoryArtFormID mismatch")
      {
        a.inventoryArtFormID = 1;
        b.inventoryArtFormID = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CNAM mismatch")
      {
        a.unknownCNAM = 1;
        b.unknownCNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    BookRecord record;

    REQUIRE( record.getRecordType() == cBOOK );
  }

  SECTION("getWriteSize")
  {
    BookRecord record;

    record.editorID = "foo";
    record.modelPath = "foo\\bar.nif";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 78 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 90 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 78 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 102 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 85 );
    }

    SECTION("size adjusts when title is present")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.title = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 88 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 78 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 87 );
    }

    SECTION("size adjusts with length of MODT")
    {
      REQUIRE( record.getWriteSize() == 78 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 102 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 85 );
    }

    SECTION("size adjusts with length of MODS")
    {
      REQUIRE( record.getWriteSize() == 78 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 102 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 85 );
    }

    SECTION("size adjusts when text is present")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.text = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 88 );
    }

    SECTION("size adjusts when pickupSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.pickupSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 88 );
    }

    SECTION("size adjusts when putdownSoundFormID is present")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.putdownSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 88 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 98 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 102 );
    }

    SECTION("size adjusts when inventoryArtFormID is present")
    {
      REQUIRE( record.getWriteSize() == 78 );

      record.inventoryArtFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 88 );
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
    dummy_table.addString(0x00001C4C, "foo");
    dummy_table.addString(0x0000DC0A, "bar");
    dummy_table.addString(0x00009D98, "baz");
    dummy_table.addString(0x0000FD83, "quux");
    dummy_table.addString(0x000000F6, "fish");
    dummy_table.addString(0x000001F0, "fishy");

    SECTION("default: load record with keyword + INAM")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001ACCD );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "Book2CommonGalerionTheMystic" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xF7 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xF5 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFF );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x09 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x0B );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x02 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.title.isPresent() );
      REQUIRE( record.title.getType() == LocalizedString::Type::Index );
      REQUIRE( record.title.getIndex() == 0x00001C4C );
      REQUIRE( record.modelPath == "Clutter\\Books\\BasicBook06.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.text.isPresent() );
      REQUIRE( record.text.getType() == LocalizedString::Type::Index );
      REQUIRE( record.text.getIndex() == 0x0000DC0A );
      REQUIRE( record.pickupSoundFormID == 0 );
      REQUIRE( record.putdownSoundFormID == 0 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000937A2 );
      REQUIRE( record.bookFlags == 0 );
      REQUIRE( record.spellOrSkillID == 0xFFFFFFFF );
      REQUIRE( record.bookValue == 6 );
      REQUIRE( record.weight == 1.0f );
      REQUIRE( record.inventoryArtFormID == 0x000153F9 );
      REQUIRE( record.unknownCNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with VMAD + YNAM")
    {
      const std::string_view data = "BOOK\xFD\0\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x04\0\x54\x7A\x0C\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0INAM\x04\0\x88\x77\x09\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00021683 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "MS11ButcherFlyer" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0"sv );
      REQUIRE( record.unknownOBND[0] == 0xF8 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xFB );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x08 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x05 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.title.isPresent() );
      REQUIRE( record.title.getType() == LocalizedString::Type::Index );
      REQUIRE( record.title.getIndex() == 0x00009D98 );
      REQUIRE( record.modelPath == "Clutter\\Books\\Note01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE( record.text.isPresent() );
      REQUIRE( record.text.getType() == LocalizedString::Type::Index );
      REQUIRE( record.text.getIndex() == 0x0000FD83 );
      REQUIRE( record.pickupSoundFormID == 0x000C7A54 );
      REQUIRE( record.putdownSoundFormID == 0 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x0000937A2 );
      REQUIRE( record.bookFlags == 0 );
      REQUIRE( record.spellOrSkillID == 0xFFFFFFFF );
      REQUIRE( record.bookValue == 0 );
      REQUIRE( record.weight == 0.0f );
      REQUIRE( record.inventoryArtFormID == 0x00097788 );
      REQUIRE( record.unknownCNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with MODS")
    {
      const auto data = "BOOK\xE6\0\0\0\0\0\0\0\x7F\x0E\0\x05\x3D\x2B\x1E\0\x2C\0\x01\0EDID\x17\0ccBGSSSE001_FishGuide1\0OBND\x0C\0\xF7\xFF\xF4\xFF\xFE\xFF\x08\0\x0C\0\x02\0FULL\x04\0\xF6\0\0\0MODL$\0Clutter\\Books\\Book02MG03LowPoly.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODS\x19\0\x01\0\0\0\x09\0\0\0Book01a:0\xB9\x30\x11\x05\0\0\0\0DESC\x04\0\xF0\x01\0\0YNAM\x04\0Tz\x0C\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x08\0\0\0\0\0\0\0INAM\x04\0\xB7\x30\x11\x05\x43NAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x05000E7F );
      REQUIRE( record.headerRevision == 0x001E2B3D );
      REQUIRE( record.headerVersion == 44 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "ccBGSSSE001_FishGuide1" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xF7 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xF4 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xFE );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x08 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x0C );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x02 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.title.isPresent() );
      REQUIRE( record.title.getType() == LocalizedString::Type::Index );
      REQUIRE( record.title.getIndex() == 0x000000F6 );
      REQUIRE( record.modelPath == "Clutter\\Books\\Book02MG03LowPoly.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.unknownMODS.isPresent() );
      const auto MODS = std::string_view(reinterpret_cast<const char*>(record.unknownMODS.data()), record.unknownMODS.size());
      REQUIRE( MODS == "\x01\0\0\0\x09\0\0\0Book01a:0\xB9\x30\x11\x05\0\0\0\0"sv );
      REQUIRE( record.text.isPresent() );
      REQUIRE( record.text.getType() == LocalizedString::Type::Index );
      REQUIRE( record.text.getIndex() == 0x000001F0 );
      REQUIRE( record.pickupSoundFormID == 0x000C7A54 );
      REQUIRE( record.putdownSoundFormID == 0 );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x000937A2 );
      REQUIRE( record.bookFlags == 0 );
      REQUIRE( record.spellOrSkillID == 0xFFFFFFFF );
      REQUIRE( record.bookValue == 8 );
      REQUIRE( record.weight == 0.0f );
      REQUIRE( record.inventoryArtFormID == 0x051130B7 );
      REQUIRE( record.unknownCNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "BOOK\0\0\0\0\x20\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x0001ACCD );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0FAIL\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\x02Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\xED\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0VMAD\x64\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0FAIL\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "BOOK\xDB\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "BOOK\xFF\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0B\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0D\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "BOOK\xF7\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\x02Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "BOOK\x11\x01\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "BOOK\xD0\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x01\0\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "BOOK\x2F\x01\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODSs")
    {
      const auto data = "BOOK\x05\x01\0\0\0\0\0\0\x7F\x0E\0\x05\x3D\x2B\x1E\0\x2C\0\x01\0EDID\x17\0ccBGSSSE001_FishGuide1\0OBND\x0C\0\xF7\xFF\xF4\xFF\xFE\xFF\x08\0\x0C\0\x02\0FULL\x04\0\xF6\0\0\0MODL$\0Clutter\\Books\\Book02MG03LowPoly.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODS\x19\0\x01\0\0\0\x09\0\0\0Book01a:0\xB9\x30\x11\x05\0\0\0\0MODS\x19\0\x01\0\0\0\x09\0\0\0Book01a:0\xB9\x30\x11\x05\0\0\0\0DESC\x04\0\xF0\x01\0\0YNAM\x04\0Tz\x0C\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x08\0\0\0\0\0\0\0INAM\x04\0\xB7\x30\x11\x05\x43NAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODS can be read")
    {
      const auto data = "BOOK\xE6\0\0\0\0\0\0\0\x7F\x0E\0\x05\x3D\x2B\x1E\0\x2C\0\x01\0EDID\x17\0ccBGSSSE001_FishGuide1\0OBND\x0C\0\xF7\xFF\xF4\xFF\xFE\xFF\x08\0\x0C\0\x02\0FULL\x04\0\xF6\0\0\0MODL$\0Clutter\\Books\\Book02MG03LowPoly.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODS\x19\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DESCs")
    {
      const std::string_view data = "BOOK\xF7\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0DESC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DESC is not four")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x03\0\x0A\xDC\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x05\0\x0A\xDC\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before DESC can be read completely")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple YNAMs")
    {
      const std::string_view data = "BOOK\x08\x01\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x04\0\x54\x7A\x0C\0YNAM\x04\0\x54\x7A\x0C\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0INAM\x04\0\x88\x77\x09\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of YNAM is not four")
    {
      {
        const std::string_view data = "BOOK\xFC\0\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x03\0\x54\x7A\x0CKSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0INAM\x04\0\x88\x77\x09\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xFE\0\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x05\0\x54\x7A\x0C\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0INAM\x04\0\x88\x77\x09\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of YNAM can be read")
    {
      const std::string_view data = "BOOK\xFD\0\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x04\0\x54\x7A"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: YNAM is zero")
    {
      const std::string_view data = "BOOK\xFD\0\0\0\0\0\0\0\x83\x16\x02\0\x1B\x69\x55\0\x28\0\x02\0EDID\x11\0MS11ButcherFlyer\0VMAD\x29\0\x05\0\x02\0\x01\0\x0E\0ms11cluescript\0\x01\0\x04\0MS11\x01\x01\0\0\xFF\xFF\xA3\xF7\x01\0OBND\x0C\0\xF8\xFF\xFB\xFF\0\0\x08\0\x05\0\0\0FULL\x04\0\x98\x9D\0\0MODL\x19\0Clutter\\Books\\Note01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x7F\x86\x45\x0D\x64\x64\x73\0\xB5\x13\x9F\x09\xAD\x43\x55\x3B\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x83\xFD\0\0YNAM\x04\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\0\0\0\0INAM\x04\0\x88\x77\x09\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "BOOK\x01\x01\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x03\0\x01\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\0\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "BOOK\xE3\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\0\0\0\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "BOOK\xE3\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x05\0\xA2\x37\x09\0\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xE9\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\0\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "BOOK\x03\x01\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3F\x44\x41TA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not sixteen")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x0F\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x11\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3\0FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "BOOK\xD7\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0INAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple INAMs")
    {
      const std::string_view data = "BOOK\xF7\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0INAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of INAM is not four")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x03\0\xF9\x53\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x05\0\xF9\x53\x01\0\0CNAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of INAM can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: INAM is zero")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\0\0\0\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const std::string_view data = "BOOK\xF7\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0\0\0CNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CNAM is not four")
    {
      {
        const std::string_view data = "BOOK\xEC\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x03\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "BOOK\xEE\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x05\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of CNAM can be read")
    {
      const std::string_view data = "BOOK\xED\0\0\0\0\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0EDID\x1D\0Book2CommonGalerionTheMystic\0OBND\x0C\0\xF7\xFF\xF5\xFF\xFF\xFF\x09\0\x0B\0\x02\0FULL\x04\0\x4C\x1C\0\0MODL\x1E\0Clutter\\Books\\BasicBook06.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\x33\x2E\x65\x69\x64\x64\x73\0\xB5\x13\x9F\x09\x5E\x0C\x94\xBF\x64\x64\x73\0\xB5\x13\x9F\x09\xCD\x64\x72\xC9\x64\x64\x73\0\xB5\x13\x9F\x09\xF4\xC8\xA2\xA3\x64\x64\x73\0\xB5\x13\x9F\x09\x44\x45SC\x04\0\x0A\xDC\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0\xA2\x37\x09\0DATA\x10\0\0\0\0\0\xFF\xFF\xFF\xFF\x06\0\0\0\0\0\x80\x3FINAM\x04\0\xF9\x53\x01\0CNAM\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      BookRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x0001ACCD;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "BOOK\0\0\0\0\x20\0\0\0\xCD\xAC\x01\0\x1B\x69\x55\0\x28\0\x01\0"sv;
      REQUIRE( stream.str() == data );
    }
  }

  SECTION("flag status checks")
  {
    BookRecord record;

    SECTION("isSkillBook")
    {
      REQUIRE_FALSE( record.isSkillBook() );
      record.bookFlags = BookRecord::cSkillBookFlag;
      REQUIRE( record.isSkillBook() );
      record.bookFlags = 0xFFFFFFFF & !BookRecord::cSkillBookFlag;
      REQUIRE_FALSE( record.isSkillBook() );
    }

    SECTION("isSpellTome")
    {
      REQUIRE_FALSE( record.isSpellTome() );
      record.bookFlags = BookRecord::cSpellTomeFlag;
      REQUIRE( record.isSpellTome() );
      record.bookFlags = 0xFFFFFFFF & !BookRecord::cSpellTomeFlag;
      REQUIRE_FALSE( record.isSpellTome() );
    }
  }
}
