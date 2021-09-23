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
#include "../../../../../sr/base/records/FurnitureRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("FurnitureRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    FurnitureRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    REQUIRE_FALSE( record.unknownOBND.isPresent() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMODS.isPresent() );
    REQUIRE_FALSE( record.destruction.isPresent );
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.unknownPNAM == 0 );
    REQUIRE( record.unknownFNAM == 0 );
    REQUIRE( record.interactionKeywordFormID == 0 );
    REQUIRE( record.unknownMNAM == 0 );
    REQUIRE( record.unknownWBDT == 0 );
    REQUIRE( record.unknownENAMs.empty() );
    REQUIRE( record.unknownNAM0s.empty() );
    REQUIRE( record.unknownFNMKs.empty() );
    REQUIRE( record.unknownFNPRs.empty() );
    REQUIRE( record.markerModelPath.empty() );
  }

  SECTION("equals")
  {
    FurnitureRecord a;
    FurnitureRecord b;

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
        a.unknownOBND.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownOBND.setPresence(false);
        b.unknownOBND.setPresence(true);

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

      SECTION("destruction mismatch")
      {
        a.destruction.isPresent = false;
        b.destruction.isPresent = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywords.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownPNAM mismatch")
      {
        a.unknownPNAM = 0x01234567;
        b.unknownPNAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownFNAM mismatch")
      {
        a.unknownFNAM = 0x0123;
        b.unknownFNAM = 0x0FED;

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

      SECTION("unknownMNAM mismatch")
      {
        a.unknownMNAM = 0x01234567;
        b.unknownMNAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownWBDT mismatch")
      {
        a.unknownWBDT = 0x0123;
        b.unknownWBDT = 0x0FED;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownENAMs mismatch")
      {
        a.unknownENAMs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownNAM0s mismatch")
      {
        a.unknownNAM0s.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownFNMKs mismatch")
      {
        a.unknownFNMKs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownFNPRs mismatch")
      {
        a.unknownFNPRs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("marker model path mismatch")
      {
        a.markerModelPath = "foo\\bar.nif";
        b.markerModelPath = "foo2\\bar3.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    FurnitureRecord record;

    REQUIRE( record.getRecordType() == cFURN );
  }

  SECTION("getWriteSize")
  {
    FurnitureRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 46 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 58 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 70 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 53 );
    }

    SECTION("size adjusts with length of OBND")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "OBND\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownOBND.loadFromStream(stream, cOBND, true) );
      }
      REQUIRE( record.getWriteSize() == 70 );

      {
        const std::string_view data = "OBND\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownOBND.loadFromStream(stream, cOBND, true) );
      }
      REQUIRE( record.getWriteSize() == 53 );
    }

    SECTION("size adjusts when name is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 64 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 73 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 70 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 53 );
    }

    SECTION("size adjusts with length of MODS")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODS\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 70 );

      {
        const std::string_view data = "MODS\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODS.loadFromStream(stream, cMODS, true) );
      }
      REQUIRE( record.getWriteSize() == 53 );
    }

    SECTION("size adjusts with length of keywords")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 70 );
    }

    SECTION("size adjusts when interactionKeywordFormID is present")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.interactionKeywordFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of ENAMs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.unknownENAMs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 56 );

      record.unknownENAMs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of NAM0s")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.unknownNAM0s.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 56 );

      record.unknownNAM0s.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of FNMKs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.unknownFNMKs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 56 );

      record.unknownFNMKs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of FNPRs")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.unknownFNPRs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 56 );

      record.unknownFNPRs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of marker model path")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 46 );

      record.markerModelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 64 );

      record.markerModelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 73 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x0000275C, "foo");

    SECTION("default: load record with name")
    {
      const std::string_view data = "FURN\xAA\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FurnitureRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00800000 );
      REQUIRE( record.headerFormID == 0x0006411B );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0009 );
      // -- record data
      REQUIRE( record.editorID == "InvisibleChairMarker" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND.isPresent() );
      const auto OBND = std::string_view(reinterpret_cast<const char*>(record.unknownOBND.data()), record.unknownOBND.size());
      REQUIRE( OBND == "\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000275C );
      REQUIRE( record.modelPath == "Furniture\\ChairInvisibleSingle.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE_FALSE( record.destruction.isPresent );
      REQUIRE( record.keywords.empty() );
      REQUIRE( record.unknownPNAM == 0 );
      REQUIRE( record.unknownFNAM == 0 );
      REQUIRE( record.interactionKeywordFormID == 0x00013794 );
      REQUIRE( record.unknownMNAM == 0x40000001 );
      REQUIRE( record.unknownWBDT == 0xFF00 );
      REQUIRE( record.unknownENAMs.empty() );
      REQUIRE( record.unknownNAM0s.empty() );
      REQUIRE( record.unknownFNMKs.empty() );
      REQUIRE( record.unknownFNPRs.size() == 1 );
      REQUIRE( record.unknownFNPRs[0] == 0x000D0001 );
      REQUIRE( record.markerModelPath.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with keywords")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      FurnitureRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00800000 );
      REQUIRE( record.headerFormID == 0x00052FF5 );
      REQUIRE( record.headerRevision == 0x003D660A );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0008 );
      // -- record data
      REQUIRE( record.editorID == "WallLeanMarker" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND.isPresent() );
      const auto OBND = std::string_view(reinterpret_cast<const char*>(record.unknownOBND.data()), record.unknownOBND.size());
      REQUIRE( OBND == "\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0"sv );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.modelPath == "Furniture\\WallLeanMarker.nif" );
      REQUIRE_FALSE( record.unknownMODT.isPresent() );
      REQUIRE_FALSE( record.unknownMODS.isPresent() );
      REQUIRE_FALSE( record.destruction.isPresent );
      REQUIRE( record.keywords.size() == 2 );
      REQUIRE( record.keywords[0] == 0x0006E9C7 );
      REQUIRE( record.keywords[1] == 0x000FD0E1 );
      REQUIRE( record.unknownPNAM == 0 );
      REQUIRE( record.unknownFNAM == 0 );
      REQUIRE( record.interactionKeywordFormID == 0x00013794 );
      REQUIRE( record.unknownMNAM == 0x10000001 );
      REQUIRE( record.unknownWBDT == 0xFF00 );
      REQUIRE( record.unknownENAMs.empty() );
      REQUIRE( record.unknownNAM0s.empty() );
      REQUIRE( record.unknownFNMKs.empty() );
      REQUIRE( record.unknownFNPRs.size() == 1 );
      REQUIRE( record.unknownFNPRs[0] == 0x00010004 );
      REQUIRE( record.markerModelPath.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0FAIL\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\x02WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x9A\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0VMAD\x64\0\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0FAIL\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const std::string_view data = "FURN\xAC\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const std::string_view data = "FURN\xB4\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const std::string_view data = "FURN\xAA\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\x02Gurniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "FURN\xBD\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "FURN\x7E\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x01\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "FURN\xBC\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "FURN\xAA\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const std::string_view data = "FURN\xB2\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const std::string_view data = "FURN\x99\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x03\0\x02\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x05\0\x02\0\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\0\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const std::string_view data = "FURN\x8C\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0FAIL\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const std::string_view data = "FURN\x8E\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const std::string_view data = "FURN\x99\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x07\0\xC7\xE9\x06\0\xE1\xD\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FURN\x92\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PNAMs")
    {
      const std::string_view data = "FURN\xB4\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of PNAM can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const std::string_view data = "FURN\xB2\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FNAM is not two")
    {
      {
        const std::string_view data = "FURN\xA9\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x01\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FURN\xAA\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x03\0\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of FNAM can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KNAMs")
    {
      const std::string_view data = "FURN\xA4\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KNAM is not four")
    {
      {
        const std::string_view data = "FURN\x99\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x03\0\x94\x37\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FURN\x9B\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x05\0\x94\x37\x01\0\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KNAM can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KNAM is zero")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\0\0\0\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MNAMs")
    {
      const std::string_view data = "FURN\xB4\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MNAM can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WBDTs")
    {
      const std::string_view data = "FURN\xB2\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x02\0\0\xFFWBDT\x02\0\0\xFF\x46NPR\x04\0\x01\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of WBDT is not two")
    {
      {
        const std::string_view data = "FURN\xA9\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x01\0\0\x46NPR\x04\0\x01\0\x0D\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "FURN\xAB\0\0\0\0\0\x80\0\x1B\x41\x06\0\x1B\x69\x55\0\x28\0\x09\0EDID\x15\0InvisibleChairMarker\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0FULL\x04\0\x5C\x27\0\0MODL\x23\0Furniture\\ChairInvisibleSingle.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x40WBDT\x03\0\0\xFF\0\x46NPR\x04\0\x01\0\x0D\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FURN, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        FurnitureRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of WBDT can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FNPR can be read")
    {
      const std::string_view data = "FURN\x9A\0\0\0\0\0\x80\0\xF5\x2F\x05\0\x0A\x66\x3D\0\x25\0\x08\0EDID\x0F\0WallLeanMarker\0OBND\x0C\0\xE7\xFF\xEE\xFF\xF2\xFF\x18\0\x1C\0\x96\0MODL\x1D\0Furniture\\WallLeanMarker.nif\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\xC7\xE9\x06\0\xE1\xD0\x0F\0PNAM\x04\0\0\0\0\0FNAM\x02\0\0\0KNAM\x04\0\x94\x37\x01\0MNAM\x04\0\x01\0\0\x10WBDT\x02\0\0\xFF\x46NPR\x04\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FURN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      FurnitureRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
