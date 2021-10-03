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
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/CellRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("CellRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    CellRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.unknownDATA.isPresent() );
    REQUIRE_FALSE( record.unknownTVDT.isPresent() );
    REQUIRE_FALSE( record.unknownMHDT.isPresent() );
    REQUIRE_FALSE( record.gridLocation.presence );
    REQUIRE_FALSE( record.unknownXCLL.isPresent() );
    REQUIRE( record.lightingTemplateFormID == 0 );
    REQUIRE_FALSE( record.hasLNAM );
    REQUIRE( record.unknownLNAM == 0 );
    REQUIRE( record.unknownXCLW == 0.0f );
    REQUIRE( record.unknownXCLR.empty() );
    REQUIRE_FALSE( record.hasXNAM );
    REQUIRE( record.unknownXNAM == 0 );
    REQUIRE( record.locationFormID == 0 );
    REQUIRE_FALSE( record.hasXWCN );
    REQUIRE( record.unknownXWCN == 0 );
    REQUIRE_FALSE( record.hasXWCS );
    REQUIRE( record.unknownXWCS == 0 );
    REQUIRE_FALSE( record.unknownXWCU.isPresent() );
    REQUIRE( record.imageSpaceFormID == 0 );
    REQUIRE( record.encounterZoneFormID == 0 );
    REQUIRE_FALSE( record.hasXCWT );
    REQUIRE( record.unknownXCWT == 0 );
    REQUIRE( record.musicTypeFormID == 0 );
    REQUIRE( record.unknownXWEM.empty() );
    REQUIRE( record.ownerFactionFormID == 0 );
    REQUIRE( record.lockListFormID == 0 );
    REQUIRE( record.regionFormID == 0 );
    REQUIRE( record.defaultAcousticSpaceFormID == 0 );
  }

  SECTION("equals")
  {
    CellRecord a;
    CellRecord b;

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DATA mismatch")
      {
        a.unknownDATA.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDATA.setPresence(false);
        b.unknownDATA.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("TVDT mismatch")
      {
        a.unknownTVDT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownTVDT.setPresence(false);
        b.unknownTVDT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MHDT mismatch")
      {
        a.unknownMHDT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMHDT.setPresence(false);
        b.unknownMHDT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("grid location mismatch")
      {
        a.gridLocation.presence = true;
        b.gridLocation.presence = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.gridLocation.presence = true;
        b.gridLocation.presence = true;

        a.gridLocation.locationX = 1;
        a.gridLocation.locationY = 2;

        b.gridLocation.locationX = 1;
        b.gridLocation.locationY = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.gridLocation.locationX = 1;
        a.gridLocation.locationY = 2;

        b.gridLocation.locationX = 2;
        b.gridLocation.locationY = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCLL mismatch")
      {
        a.unknownXCLL.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXCLL.setPresence(false);
        b.unknownXCLL.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("lightingTemplateFormID mismatch")
      {
        a.lightingTemplateFormID = 0x01234567;
        b.lightingTemplateFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("LNAM mismatch")
      {
        a.hasLNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasLNAM = false;
        b.hasLNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasLNAM = true;
        a.unknownLNAM = 1;
        b.hasLNAM = true;
        a.unknownLNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCLW mismatch")
      {
        a.unknownXCLW = 1.0f;
        b.unknownXCLW = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCLR mismatch")
      {
        a.unknownXCLR.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownXCLR.push_back(0x01234567);
        b.unknownXCLR.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XNAM mismatch")
      {
        a.hasXNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXNAM = false;
        b.hasXNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXNAM = true;
        a.unknownXNAM = 1;
        b.hasXNAM = true;
        a.unknownXNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("locationFormID mismatch")
      {
        a.locationFormID = 0x01234567;
        b.locationFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCWN mismatch")
      {
        a.hasXWCN = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXWCN = false;
        b.hasXWCN = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXWCN = true;
        a.unknownXWCN = 1;
        b.hasXWCN = true;
        a.unknownXWCN = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCWS mismatch")
      {
        a.hasXWCS = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXWCS = false;
        b.hasXWCS = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXWCS = true;
        a.unknownXWCS = 1;
        b.hasXWCS = true;
        a.unknownXWCS = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XWCU mismatch")
      {
        a.unknownXWCU.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXWCU.setPresence(false);
        b.unknownXWCU.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("imageSpaceFormID mismatch")
      {
        a.imageSpaceFormID = 0x01234567;
        b.imageSpaceFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("encounterZoneFormID mismatch")
      {
        a.encounterZoneFormID = 0x01234567;
        b.encounterZoneFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCWT mismatch")
      {
        a.hasXCWT = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXCWT = false;
        b.hasXCWT = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasXCWT = true;
        a.unknownXCWT = 1;
        b.hasXCWT = true;
        a.unknownXCWT = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("musicTypeFormID mismatch")
      {
        a.musicTypeFormID = 0x01234567;
        b.musicTypeFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XWEM mismatch")
      {
        a.unknownXWEM = "foo";
        b.unknownXWEM = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ownerFactionFormID mismatch")
      {
        a.ownerFactionFormID = 0x01234567;
        b.ownerFactionFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("lockListFormID mismatch")
      {
        a.lockListFormID = 0x01234567;
        b.lockListFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("regionFormID mismatch")
      {
        a.regionFormID = 0x01234567;
        b.regionFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("defaultAcousticSpaceFormID mismatch")
      {
        a.defaultAcousticSpaceFormID = 0x01234567;
        b.defaultAcousticSpaceFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    CellRecord record;

    REQUIRE( record.getRecordType() == cCELL );
  }

  SECTION("getWriteSize")
  {
    CellRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 30 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 42 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of DATA")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "DATA\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownDATA.loadFromStream(stream, cDATA, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "DATA\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownDATA.loadFromStream(stream, cDATA, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts with length of TVDT")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "TVDT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownTVDT.loadFromStream(stream, cTVDT, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "TVDT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownTVDT.loadFromStream(stream, cTVDT, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts with length of MHDT")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MHDT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMHDT.loadFromStream(stream, cMHDT, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "MHDT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMHDT.loadFromStream(stream, cMHDT, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when grid location is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.gridLocation.presence = true;
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts with length of XCLL")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "XCLL\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownXCLL.loadFromStream(stream, cXCLL, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "XCLL\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownXCLL.loadFromStream(stream, cXCLL, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when LNAM is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasLNAM = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of XCLR")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXCLR.push_back(1.0f);
      REQUIRE( record.getWriteSize() == 30 );

      record.unknownXCLR.push_back(1.5f);
      REQUIRE( record.getWriteSize() == 34 );

      record.unknownXCLR.push_back(1.25f);
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when XNAM is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasXNAM = true;
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when locationFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.locationFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when XWCN is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasXWCN = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when XWCS is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasXWCS = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of XWCU")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "XWCU\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownXWCU.loadFromStream(stream, cXWCU, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "XWCU\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownXWCU.loadFromStream(stream, cXWCU, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when imageSpaceFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.imageSpaceFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when encounterZoneFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.encounterZoneFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when XCWT is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasXCWT = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when musicTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.musicTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of XWEM")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXWEM = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 30 );

      record.unknownXWEM = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 42 );
    }

    SECTION("size adjusts when ownerFactionFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.ownerFactionFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when lockListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.lockListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when regionFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.regionFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when defaultAcousticSpaceFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.defaultAcousticSpaceFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
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

    SECTION("default: load uncompressed record")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00106666 );
      REQUIRE( record.headerRevision == 0x000A661D );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID.empty() );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0xFFFFFFFF );
      REQUIRE( record.gridLocation.locationY == 0xFFFFFFFD );
      REQUIRE( record.gridLocation.unknownThird == 0x0053BA00 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.hasLNAM );
      REQUIRE( record.unknownLNAM == 0 );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.size() == 1 );
      REQUIRE( record.unknownXCLR[0] == 0x00106668 );
      REQUIRE_FALSE( record.hasXNAM );
      REQUIRE( record.unknownXNAM == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE_FALSE( record.hasXWCN );
      REQUIRE( record.unknownXWCN == 0 );
      REQUIRE_FALSE( record.hasXWCS );
      REQUIRE( record.unknownXWCS == 0 );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.hasXCWT );
      REQUIRE( record.unknownXCWT == 0 );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.unknownXWEM.empty() );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load compressed record")
    {
      const auto data = "CELL\x85\0\0\0\0\0\x04\0\xBC\xA1\x10\0\x0B\x68\x0E\0\x27\0\x03\0\xE0\x02\0\0x\xDAsq\x0Cqdb`b\x88p\xF6q\xE6\x61P```\xE0\0b\x86+\xC1\x0C!a.!k\x98\x98\x19\x06\x16\x30\xC1Y\x0D`\x92\x65\x41\x1A\x03\x83\x01\x83%\xFB\x82\xE5\xDF\xFF\xFC\xF8\xFF_\x9F\x1F(\xF8\x61\xF9~F\x86!\x04$\x10L\x16\x06\x06\x46\x06\x41\x20\xCD\x8E])\x1F\x43G\xFD\xEB\xAF\x1A\x0C:\x0C\x0D\xFF\xEB\xED\x19\x38YX\x19\x46\x01\x0E\xE0\x13\xE2\x1B\xC0\x02\x66\x01Ss8\x0B\xC3\xFF\xFF\xF5\xF5\0\x04\x62\x1B\xA8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00040000 );
      REQUIRE( record.headerFormID == 0x0010A1BC );
      REQUIRE( record.headerRevision == 0x000E680B );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0003 );
      // -- record data
      REQUIRE( record.editorID.empty() );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE( record.unknownTVDT.isPresent() );
      REQUIRE( record.unknownTVDT.size() == 684 );
      const auto TVDT = std::string_view(reinterpret_cast<const char*>(record.unknownTVDT.data()), record.unknownTVDT.size());
      REQUIRE( TVDT == "\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\0\0\0\0\0\0\x80\0\0\0\0\0\x04\xA0\x66\0\0\x30\09\x07\xA0\xA7\xF7\xFC\xF8\xFF\xFF/\x0F\0\0\0\xF0\xA7\xBF\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x18\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x11\0\0\x04\x07\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x0E\0\x88\x7F\xEB\xF5(\0,\0\x80\xFF\x7F?\0\x09\x04\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 32 );
      REQUIRE( record.gridLocation.locationY == 8 );
      REQUIRE( record.gridLocation.unknownThird == 0x0053D400 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.hasLNAM );
      REQUIRE( record.unknownLNAM == 0 );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.hasXNAM );
      REQUIRE( record.unknownXNAM == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE_FALSE( record.hasXWCN );
      REQUIRE( record.unknownXWCN == 0 );
      REQUIRE_FALSE( record.hasXWCS );
      REQUIRE( record.unknownXWCS == 0 );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.hasXCWT );
      REQUIRE( record.unknownXCWT == 0 );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.unknownXWEM.empty() );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );
    }

    SECTION("default: load record with editor ID")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00104220 );
      REQUIRE( record.headerRevision == 0x00276715 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "MazeStart" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == 0xFFFFFFFF );
      REQUIRE( record.gridLocation.unknownThird == 0x0000170F );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.hasLNAM );
      REQUIRE( record.unknownLNAM == 0 );
      REQUIRE( record.unknownXCLW == -2147483648.0f );
      REQUIRE( record.unknownXCLR.size() == 1 );
      REQUIRE( record.unknownXCLR[0] == 0x00106668 );
      REQUIRE_FALSE( record.hasXNAM );
      REQUIRE( record.unknownXNAM == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE_FALSE( record.hasXWCN );
      REQUIRE( record.unknownXWCN == 0 );
      REQUIRE_FALSE( record.hasXWCS );
      REQUIRE( record.unknownXWCS == 0 );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.hasXCWT );
      REQUIRE( record.unknownXCWT == 0 );
      REQUIRE( record.musicTypeFormID == 0x0002D4C2 );
      REQUIRE( record.unknownXWEM.empty() );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "CELL\0\0\0\0\x20\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x00106666 );
      REQUIRE( record.headerRevision == 0x000A661D );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before decompressed size of a compressed record can be read")
    {
      const auto data = "CELL\x85\0\0\0\0\0\x04\0\xBC\xA1\x10\0\x0B\x68\x0E\0\x27\0\x03\0\xE0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: record size is too small to contain compressed data")
    {
      const auto data = "CELL\x04\0\0\0\0\0\x04\0\xBC\xA1\x10\0\x0B\x68\x0E\0\x27\0\x03\0\xE0\x02\0\0x\xDAsq\x0Cqdb`b\x88p\xF6q\xE6\x61P```\xE0\0b\x86+\xC1\x0C!a.!k\x98\x98\x19\x06\x16\x30\xC1Y\x0D`\x92\x65\x41\x1A\x03\x83\x01\x83%\xFB\x82\xE5\xDF\xFF\xFC\xF8\xFF_\x9F\x1F(\xF8\x61\xF9~F\x86!\x04$\x10L\x16\x06\x06\x46\x06\x41\x20\xCD\x8E])\x1F\x43G\xFD\xEB\xAF\x1A\x0C:\x0C\x0D\xFF\xEB\xED\x19\x38YX\x19\x46\x01\x0E\xE0\x13\xE2\x1B\xC0\x02\x66\x01Ss8\x0B\xC3\xFF\xFF\xF5\xF5\0\x04\x62\x1B\xA8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all compressed data of a record can be read")
    {
      const auto data = "CELL\x85\0\0\0\0\0\x04\0\xBC\xA1\x10\0\x0B\x68\x0E\0\x27\0\x03\0\xE0\x02\0\0x\xDAsq\x0Cqd"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: invalid compressed data in a compressed record")
    {
      const auto data = "CELL\x85\0\0\0\0\0\x04\0\xBC\xA1\x10\0\x0B\x68\x0E\0\x27\0\x03\0\xE0\x02\0\0\0\xFFsq\x0Cqdb`b\x88p\xF6q\xE6\x61P```\xE0\0b\x86+\xC1\x0C!a.!k\x98\x98\x19\x06\x16\x30\xC1Y\x0D`\x92\x65\x41\x1A\x03\x83\x01\x83%\xFB\x82\xE5\xDF\xFF\xFC\xF8\xFF_\x9F\x1F(\xF8\x61\xF9~F\x86!\x04$\x10L\x16\x06\x06\x46\x06\x41\x20\xCD\x8E])\x1F\x43G\xFD\xEB\xAF\x1A\x0C:\x0C\x0D\xFF\xEB\xED\x19\x38YX\x19\x46\x01\x0E\xE0\x13\xE2\x1B\xC0\x02\x66\x01Ss8\x0B\xC3\xFF\xFF\xF5\xF5\0\x04\x62\x1B\xA8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0FAIL\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple EDIDs")
    {
      const auto data = "CELL\x62\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\x02MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x52\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCLCs")
    {
      const auto data = "CELL\x4A\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCLC is not 12")
    {
      {
        const auto data = "CELL\x37\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0B\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0D\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of XCLC can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple LTMPs")
    {
      const auto data = "CELL\x42\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing LTMP")
    {
      const auto data = "CELL\x2E\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of LTMP is not four")
    {
      {
        const auto data = "CELL\x37\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x03\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x05\0\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of LTMP can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCLWs")
    {
      const auto data = "CELL\x42\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCLW is not four")
    {
      {
        const auto data = "CELL\x37\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x03\0\xFF\xFF\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x05\0\xFF\xFF\x7F\x7F\0XCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCLW can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCLRs")
    {
      const auto data = "CELL\x42\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0XCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCLR is not a multiple of four or it's zero")
    {
      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x05\0hf\x10\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x34\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCLR can be read")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCMOs")
    {
      const auto data = "CELL\x5C\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCMO is not four")
    {
      {
        const auto data = "CELL\x51\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x03\0\xC2\xD4\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x53\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x05\0\xC2\xD4\x02\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CELL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCMO can be read")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XCMO is zero")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CELL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      CellRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x00106666;
      record.headerRevision = 0x000A661D;
      record.headerVersion = 37;
      record.headerUnknown5 = 0x0001;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CELL\0\0\0\0\x20\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
