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
#include "../../../../../sr/base/records/TreeRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("TreeRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    TreeRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.ingredientFormID == 0 );
    REQUIRE( record.harvestSoundFormID == 0 );
    REQUIRE( record.unknownPFPC == 0 );
    REQUIRE_FALSE( record.name.isPresent() );
    for (unsigned int i = 0; i < 48; ++i)
    {
      REQUIRE( record.unknownCNAM[i] == 0 );
    }
  }

  SECTION("equals")
  {
    TreeRecord a;
    TreeRecord b;

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CNAM mismatch")
      {
        a.unknownCNAM[47] = 12;
        b.unknownCNAM[47] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    TreeRecord record;

    REQUIRE( record.getRecordType() == cTREE );
  }

  SECTION("getWriteSize")
  {
    TreeRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 92 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 104 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 92 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 110 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 119 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 92 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 116 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 99 );
    }

    SECTION("size adjusts when ingredientFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 92 );

      record.ingredientFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 102 );
    }

    SECTION("size adjusts when harvestSoundFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 92 );

      record.harvestSoundFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 102 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 92 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 102 );
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
    dummy_table.addString(0x000022AC, "foo");

    SECTION("default: load record without ingredient")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TreeRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001306D );
      REQUIRE( record.headerRevision == 0x000A6815 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "TreePineForest01" );
      REQUIRE( record.unknownOBND[0] == 0x35 );
      REQUIRE( record.unknownOBND[1] == 0xFE );
      REQUIRE( record.unknownOBND[2] == 0x16 );
      REQUIRE( record.unknownOBND[3] == 0xFE );
      REQUIRE( record.unknownOBND[4] == 0xBE );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0xE9 );
      REQUIRE( record.unknownOBND[7] == 0x01 );
      REQUIRE( record.unknownOBND[8] == 0xE6 );
      REQUIRE( record.unknownOBND[9] == 0x01 );
      REQUIRE( record.unknownOBND[10] == 0xC2 );
      REQUIRE( record.unknownOBND[11] == 0x09 );
      REQUIRE( record.modelPath == "Landscape\\Trees\\TreePineForest01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0A"sv );
      REQUIRE( record.ingredientFormID == 0 );
      REQUIRE( record.harvestSoundFormID == 0 );
      REQUIRE( record.unknownPFPC == 0 );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::None );
      const auto CNAM = std::string_view(reinterpret_cast<const char*>(&(record.unknownCNAM[0])), 48);
      REQUIRE( CNAM == "\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with ingredient")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TreeRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000B91F0 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0007 );
      // -- record data
      REQUIRE( record.editorID == "TreeFloraThistle01" );
      REQUIRE( record.unknownOBND[0] == 0xB0 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xB8 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xED );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x47 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x45 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x72 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.modelPath == "Plants\\FloraThistle01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFA"sv );
      REQUIRE( record.ingredientFormID == 0x000134AA );
      REQUIRE( record.harvestSoundFormID == 0x000519D5 );
      REQUIRE( record.unknownPFPC == 0x64646464 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x000022AC );
      const auto CNAM = std::string_view(reinterpret_cast<const char*>(&(record.unknownCNAM[0])), 48);
      REQUIRE( CNAM == "\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "TREE\0\0\0\0\x20\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TreeRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 32 );
      REQUIRE( record.headerFormID == 0x0001306D );
      REQUIRE( record.headerRevision == 0x000A6815 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0FAIL\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\x02TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\xD6\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0FAIL\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const std::string_view data = "TREE\xC4\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "TREE\xE8\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0B\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TREE, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TreeRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0D\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TREE, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TreeRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\x02Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "TREE\x01\x01\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "TREE\xB2\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x01\0\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "TREE\x18\x01\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0AMODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of MODT can be read")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PFIGs")
    {
      const std::string_view data = "TREE\xDD\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0PFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PFIG can be read")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: PFIG is zero")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\0\0\0\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const std::string_view data = "TREE\xDD\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19\x05\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SNAM can be read")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SNAM is zero")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\0\0\0\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PFPCs")
    {
      const std::string_view data = "TREE\xE0\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0PFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of PFPC can be read")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of FULL can be read")
    {
      const std::string_view data = "TREE\xD3\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "TREE\xDD\0\0\0\0\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0EDID\x13\0TreeFloraThistle01\0OBND\x0C\0\xB0\xFF\xB8\xFF\xED\xFF\x47\0\x45\0\x72\0MODL\x1A\0Plants\\FloraThistle01.nif\0MODT\x24\0\x02\0\0\0\x02\0\0\0\0\0\0\0\x38\x6E\xFF\x80\x64\x64\x73\0\x13\x4C\x1E\xFA\x9F\x7D\xBC\x2A\x64\x64\x73\0\x13\x4C\x1E\xFAPFIG\x04\0\xAA\x34\x01\0SNAM\x04\0\xD5\x19\x05\0PFPC\x04\0\x64\x64\x64\x64\x46ULL\x04\0\xAC\x22\0\0FULL\x04\0\xAC\x22\0\0CNAM\x30\0\0\0\x80\x3F\0\0\x80\x3F\x0B\xD7\x23\x3D\x90\xC2\xF5\x3C\x0B\xD7\x23\x3D\x96\x43\x0B\x3D\0\0\0\x3F\0\0\0\x3F\xCD\xCC\xCC\x3E\0\0\x80\x3F\0\0\x20\x40\0\0\0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const std::string_view data = "TREE\x0C\x01\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F\x43NAM\x30\0\0\0\0\0\0\0\x90\x40\0\0\0\0\x71\x3D\x0A\x3E\xEC\x51\x38\x3E\x09\xAC\x1C\x3E\0\0\x10\x40\0\0\x10\x40\x67\x66\xE6\x3F\0\0\x80\x3F\0\0\x90\x40\0\0\xE0\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of CNAM can be read")
    {
      const std::string_view data = "TREE\xD6\0\0\0\0\0\0\0\x6D\x30\x01\0\x15\x68\x0A\0\x28\0\x0C\0EDID\x11\0TreePineForest01\0OBND\x0C\0\x35\xFE\x16\xFE\xBE\xFF\xE9\x01\xE6\x01\xC2\x09MODL\x25\0Landscape\\Trees\\TreePineForest01.nif\0MODT\x3C\0\x02\0\0\0\x04\0\0\0\0\0\0\0\xFA\x66\x29\xB9\x64\x64\x73\0\x3B\xAC\x5C\x0A\xB7\x79\xAB\xDD\x64\x64\x73\0\x3B\xAC\x5C\x0A\x26\x1B\x51\x60\x64\x64\x73\0\x3B\xAC\x5C\x0A\x5D\x27\x92\xDE\x64\x64\x73\0\x3B\xAC\x5C\x0APFPC\x04\0\0\0\0\0CNAM\x30\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TREE, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TreeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      TreeRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x000B91F0;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0007;
      // -- record data
      record.editorID = "TreeFloraThistle01";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "TREE\0\0\0\0\x20\0\0\0\xF0\x91\x0B\0\x1B\x69\x55\0\x28\0\x07\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
