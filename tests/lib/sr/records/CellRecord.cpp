/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2026  Dirk Stolle

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
#include <limits>
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/CellRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

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
    REQUIRE_FALSE( record.unknownLNAM.has_value() );
    REQUIRE( record.unknownXCLW == 0.0f );
    REQUIRE( record.unknownXCLR.empty() );
    REQUIRE_FALSE( record.unknownXNAM.has_value() );
    REQUIRE_FALSE( record.unknownXWCN.has_value() );
    REQUIRE_FALSE( record.unknownXWCS.has_value() );
    REQUIRE_FALSE( record.unknownXWCU.isPresent() );
    REQUIRE( record.imageSpaceFormID == 0 );
    REQUIRE( record.locationFormID == 0 );
    REQUIRE( record.encounterZoneFormID == 0 );
    REQUIRE_FALSE( record.unknownXCWT.has_value() );
    REQUIRE( record.musicTypeFormID == 0 );
    REQUIRE( record.ownerFactionFormID == 0 );
    REQUIRE( record.lockListFormID == 0 );
    REQUIRE( record.regionFormID == 0 );
    REQUIRE( record.environmentMap.empty() );
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
        a.unknownLNAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownLNAM.reset();
        b.unknownLNAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownLNAM = 1;
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
        a.unknownXNAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXNAM = std::nullopt;
        b.unknownXNAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXNAM = 1;
        a.unknownXNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCWN mismatch")
      {
        a.unknownXWCN = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXWCN.reset();
        b.unknownXWCN = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXWCN = 1;
        a.unknownXWCN = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XCWS mismatch")
      {
        a.unknownXWCS = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXWCS.reset();
        b.unknownXWCS = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXWCS = 1;
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

      SECTION("locationFormID mismatch")
      {
        a.locationFormID = 0x01234567;
        b.locationFormID = 0x0FEDCBA9;

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
        a.unknownXCWT = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXCWT.reset();
        b.unknownXCWT = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownXCWT = 1;
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
        a.environmentMap = "foo.dds";
        b.environmentMap = "bar.dds";

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

      record.unknownLNAM = 0;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of XCLR")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXCLR.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 30 );

      record.unknownXCLR.push_back(0x89ABCDEF);
      REQUIRE( record.getWriteSize() == 34 );

      record.unknownXCLR.push_back(0x0F00BA12);
      REQUIRE( record.getWriteSize() == 38 );
    }

    SECTION("size adjusts when XNAM is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXNAM = 0;
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when XWCN is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXWCN = 0;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when XWCS is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXWCS = 0;
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

    SECTION("size adjusts when locationFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.locationFormID = 0x01234567;
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

      record.unknownXCWT = 0;
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

      record.environmentMap = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 30 );

      record.environmentMap = "foobarfoobarbaz"; // 15 characters
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
    StringTable dummy_table;
    dummy_table.addString(0x0000ED3E, "foo");
    dummy_table.addString(0x00000155, "bar");

    SECTION("default: load uncompressed record")
    {
      const auto data = "CELL\x38\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE( record.gridLocation.locationX == static_cast<std::int32_t>(0xFFFFFFFF) );
      REQUIRE( record.gridLocation.locationY == static_cast<std::int32_t>(0xFFFFFFFD) );
      REQUIRE( record.gridLocation.unknownThird == 0x0053BA00 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.size() == 1 );
      REQUIRE( record.unknownXCLR[0] == 0x00106668 );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );
    }

    SECTION("default: load record with editor ID")
    {
      const auto data = "CELL\x52\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE( record.gridLocation.locationY == static_cast<std::int32_t>(0xFFFFFFFF) );
      REQUIRE( record.gridLocation.unknownThird == 0x0000170F );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == -2147483648.0f );
      REQUIRE( record.unknownXCLR.size() == 1 );
      REQUIRE( record.unknownXCLR[0] == 0x00106668 );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0x0002D4C2 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with XCIM + XLCN + XCAS")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000C97F7 );
      REQUIRE( record.headerRevision == 0x00276714 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0003 );
      // -- record data
      REQUIRE( record.editorID.empty() );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == static_cast<std::int32_t>(0xFFFFFFFE) );
      REQUIRE( record.gridLocation.unknownThird == 0x0053CF00 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0x0001560E );
      REQUIRE( record.locationFormID == 0x000192B8 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0x00094BDD );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0x0010CB83 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with LNAM and XNAM")
    {
      const auto data = "CELL\x3F\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000B4603 );
      REQUIRE( record.headerRevision == 0x00645E1B );
      REQUIRE( record.headerVersion == 30 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.editorID.empty() );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == 0 );
      REQUIRE( record.gridLocation.unknownThird == 0 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownLNAM.value() == 0x0000039F );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE( record.unknownXNAM.has_value() );
      REQUIRE( record.unknownXNAM == 0x34 );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with MHDT, XWCN + XWCU")
    {
      const auto data = "CELL\x87\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00009BE0 );
      REQUIRE( record.headerRevision == 0x000A6817 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "EvergreenGroveExterior" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "B\0"sv );
      REQUIRE( record.unknownTVDT.isPresent() );
      const auto TVDT = std::string_view(reinterpret_cast<const char*>(record.unknownTVDT.data()), record.unknownTVDT.size());
      REQUIRE( TVDT == "\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.unknownMHDT.isPresent() );
      const auto MHDT = std::string_view(reinterpret_cast<const char*>(record.unknownMHDT.data()), record.unknownMHDT.size());
      REQUIRE( MHDT == "\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09"sv );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == static_cast<std::int32_t>(0xFFFFFFF3) );
      REQUIRE( record.gridLocation.locationY == static_cast<std::int32_t>(0xFFFFFFEF) );
      REQUIRE( record.gridLocation.unknownThird == 0x0053FD00 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == -1400.0f );
      REQUIRE( record.unknownXCLR.size() == 8 );
      REQUIRE( record.unknownXCLR[0] == 0x0002A72D );
      REQUIRE( record.unknownXCLR[1] == 0x000629B4 );
      REQUIRE( record.unknownXCLR[2] == 0x0008EED7 );
      REQUIRE( record.unknownXCLR[3] == 0x000BD798 );
      REQUIRE( record.unknownXCLR[4] == 0x00083864 );
      REQUIRE( record.unknownXCLR[5] == 0x0008FB97 );
      REQUIRE( record.unknownXCLR[6] == 0x000F7C40 );
      REQUIRE( record.unknownXCLR[7] == 0x000C5859 );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE( record.unknownXWCN.has_value() );
      REQUIRE( record.unknownXWCN.value() == 0x00000004 );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE( record.unknownXWCU.isPresent() );
      const auto XWCU = std::string_view(reinterpret_cast<const char*>(record.unknownXWCU.data()), record.unknownXWCU.size());
      REQUIRE( XWCU == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0x00019174 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str().size() == data.size() );
      // TODO: Fix write order of XLCN and XWCN so the following line passes.
      // REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with XCLL")
    {
      const auto data = "CELL\xA9\0\0\0\0\0\0\0\x4C\xFA\x01\0\x07\x65\x0E\0\x25\0\x0A\0EDID\x11\0CWGuardTemplates\0DATA\x02\0\x01\0XCLL\x5C\0\xFF\xFF\xFF\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\x18\xFF\x0A\0\xE5\xFF\xF3\0\xFF\x0A\xFF\0\xFF\xF3\xFF\0yys\0\x84\x84\x8A\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9F\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\xFFXCIM\x04\0\x87&\x0A\0XLCN\x04\0\xC8\x84\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001FA4C );
      REQUIRE( record.headerRevision == 0x000E6507 );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x000A );
      // -- record data
      REQUIRE( record.editorID == "CWGuardTemplates" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x01\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE_FALSE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == 0 );
      REQUIRE( record.gridLocation.unknownThird == 0 );
      REQUIRE( record.unknownXCLL.isPresent() );
      const auto XCLL = std::string_view(reinterpret_cast<const char*>(record.unknownXCLL.data()), record.unknownXCLL.size());
      REQUIRE( XCLL == "\xFF\xFF\xFF\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\x18\xFF\x0A\0\xE5\xFF\xF3\0\xFF\x0A\xFF\0\xFF\xF3\xFF\0yys\0\x84\x84\x8A\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9F\0\0\0"sv );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == -std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0x000A2687 );
      REQUIRE( record.locationFormID == 0x000884C8 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with XWCS")
    {
      const auto data = "CELL\x78\0\0\0\0\0\0\0\x17\x55\x01\0\x0A\x63\x36\0\x22\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\x03\0\0\0\x01\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXWCS\x04\0\x03\0\0\0XWCU\x30\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0XCIM\x04\0\x0EV\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00015517 );
      REQUIRE( record.headerRevision == 0x0036630A );
      REQUIRE( record.headerVersion == 34 );
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
      REQUIRE( record.gridLocation.locationX == 3 );
      REQUIRE( record.gridLocation.locationY == 1 );
      REQUIRE( record.gridLocation.unknownThird == 0 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == std::numeric_limits<float>::max() );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE( record.unknownXWCS.has_value() );
      REQUIRE( record.unknownXWCS.value() == 3 );
      REQUIRE( record.unknownXWCU.isPresent() );
      const auto XWCU = std::string_view(reinterpret_cast<const char*>(record.unknownXWCU.data()), record.unknownXWCU.size());
      REQUIRE( XWCU == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.imageSpaceFormID == 0x0001560E );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with XCWT")
    {
      const auto data = "CELL\x62\0\0\0\0\0\0\0\xC1\x37\x02\0\x0D\x65\x4F\0\x23\0\x06\0EDID\x1A\0BlackreachSinderionsShack\0DATA\x02\0\x02\0XCLC\x0C\0\x04\0\0\0\x04\0\0\0\0\x80S\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\xC0Z\xC6XCLR\x04\0.\x8C\x04\0XCWT\x04\0\x18\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000237C1 );
      REQUIRE( record.headerRevision == 0x004F650D );
      REQUIRE( record.headerVersion == 35 );
      REQUIRE( record.headerUnknown5 == 0x0006 );
      // -- record data
      REQUIRE( record.editorID == "BlackreachSinderionsShack" );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x02\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 4 );
      REQUIRE( record.gridLocation.locationY == 4 );
      REQUIRE( record.gridLocation.unknownThird == 0x00538000 );
      REQUIRE_FALSE( record.unknownXCLL.isPresent() );
      REQUIRE( record.lightingTemplateFormID == 0 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == -14000.0f );
      REQUIRE( record.unknownXCLR.size() == 1 );
      REQUIRE( record.unknownXCLR[0] == 0x00048C2E );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0 );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE( record.unknownXCWT.has_value() );
      REQUIRE( record.unknownXCWT.value() == 0x00000018 );
      REQUIRE( record.musicTypeFormID == 0 );
      REQUIRE( record.ownerFactionFormID == 0 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with FULL, XEZN, XOWN, XCCM, XWEM")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00016BCF );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0009 );
      // -- record data
      REQUIRE( record.editorID == "RiftenRaggedFlagon" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000ED3E );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\xA3\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE_FALSE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == 0 );
      REQUIRE( record.gridLocation.unknownThird == 0 );
      REQUIRE( record.unknownXCLL.isPresent() );
      const auto XCLL = std::string_view(reinterpret_cast<const char*>(record.unknownXCLL.data()), record.unknownXCLL.size());
      REQUIRE( XCLL == "\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0"sv );
      REQUIRE( record.lightingTemplateFormID == 0x0005C734 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == 0.0f );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0x0005C73D );
      REQUIRE( record.locationFormID == 0x0002264A );
      REQUIRE( record.encounterZoneFormID == 0x0009FBB9 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0x0005615B );
      REQUIRE( record.ownerFactionFormID == 0x000A2522 );
      REQUIRE( record.lockListFormID == 0 );
      REQUIRE( record.regionFormID == 0x0007042E );
      REQUIRE( record.environmentMap == "Data\\Textures\\Cubemaps\\WRTemple_e.dds" );
      REQUIRE( record.defaultAcousticSpaceFormID == 0x00108E60 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str().size() == data.size() );
      // TODO: Fix write order of XEZN and surrounding subrecords so the following line passes.
      // REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with FULL + XILL")
    {
      const auto data = "CELL\xD0\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x04\0]\x84\x01\x02XCMO\x04\0\x81\x42\x03\x02XCAS\x04\0m[\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x02018277 );
      REQUIRE( record.headerRevision == 0x00557707 );
      REQUIRE( record.headerVersion == 43 );
      REQUIRE( record.headerUnknown5 == 0x0008 );
      // -- record data
      REQUIRE( record.editorID == "DLC2RRIenthFarm" );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00000155 );
      REQUIRE( record.unknownDATA.isPresent() );
      const auto DATA = std::string_view(reinterpret_cast<const char*>(record.unknownDATA.data()), record.unknownDATA.size());
      REQUIRE( DATA == "\x01\0"sv );
      REQUIRE_FALSE( record.unknownTVDT.isPresent() );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE_FALSE( record.gridLocation.presence );
      REQUIRE( record.gridLocation.locationX == 0 );
      REQUIRE( record.gridLocation.locationY == 0 );
      REQUIRE( record.gridLocation.unknownThird == 0 );
      REQUIRE( record.unknownXCLL.isPresent() );
      const auto XCLL = std::string_view(reinterpret_cast<const char*>(record.unknownXCLL.data()), record.unknownXCLL.size());
      REQUIRE( XCLL == "#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0"sv );
      REQUIRE( record.lightingTemplateFormID == 0x000A1196 );
      REQUIRE_FALSE( record.unknownLNAM.has_value() );
      REQUIRE( record.unknownXCLW == 0.0f );
      REQUIRE( record.unknownXCLR.empty() );
      REQUIRE_FALSE( record.unknownXNAM.has_value() );
      REQUIRE_FALSE( record.unknownXWCN.has_value() );
      REQUIRE_FALSE( record.unknownXWCS.has_value() );
      REQUIRE_FALSE( record.unknownXWCU.isPresent() );
      REQUIRE( record.imageSpaceFormID == 0 );
      REQUIRE( record.locationFormID == 0x02019A1F );
      REQUIRE( record.encounterZoneFormID == 0 );
      REQUIRE_FALSE( record.unknownXCWT.has_value() );
      REQUIRE( record.musicTypeFormID == 0x02034281 );
      REQUIRE( record.ownerFactionFormID == 0x02018426 );
      REQUIRE( record.lockListFormID == 0x0201845D );
      REQUIRE( record.regionFormID == 0 );
      REQUIRE( record.environmentMap.empty() );
      REQUIRE( record.defaultAcousticSpaceFormID == 0x000C5B6D );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str().size() == data.size() );
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "CELL\0\0\0\0\x20\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULLs")
    {
      const auto data = "CELL\x1D\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "CELL\x40\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not one or two")
    {
      {
        const auto data = "CELL\x36\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\0\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x03\0\x02\0\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple XCLCs")
    {
      const auto data = "CELL\x4A\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0D\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TVDTs")
    {
      const auto data = "CELL\x39\x0A\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of TVDT can be read")
    {
      const auto data = "CELL\x87\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of TVDT is not 684")
    {
      {
        const auto data = "CELL\x86\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAB\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x88\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAD\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple MHDTs")
    {
      const auto data = "CELL\x91\x0B\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MHDT is not 1028")
    {
      {
        const auto data = "CELL\x86\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x03\x04\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x88\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x05\x04\0\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of MHDT can be read")
    {
      const auto data = "CELL\x87\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCLLs")
    {
      const auto data = "CELL\x0B\x01\0\0\0\0\0\0\x4C\xFA\x01\0\x07\x65\x0E\0\x25\0\x0A\0EDID\x11\0CWGuardTemplates\0DATA\x02\0\x01\0XCLL\x5C\0\xFF\xFF\xFF\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\x18\xFF\x0A\0\xE5\xFF\xF3\0\xFF\x0A\xFF\0\xFF\xF3\xFF\0yys\0\x84\x84\x8A\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9F\0\0\0XCLL\x5C\0\xFF\xFF\xFF\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\x18\xFF\x0A\0\xE5\xFF\xF3\0\xFF\x0A\xFF\0\xFF\xF3\xFF\0yys\0\x84\x84\x8A\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9F\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\xFFXCIM\x04\0\x87&\x0A\0XLCN\x04\0\xC8\x84\x08\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream end before all of XCLL can be read")
    {
      const auto data = "CELL\xA9\0\0\0\0\0\0\0\x4C\xFA\x01\0\x07\x65\x0E\0\x25\0\x0A\0EDID\x11\0CWGuardTemplates\0DATA\x02\0\x01\0XCLL\x5C\0\xFF\xFF\xFF\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x05\0\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple LNAMs")
    {
      const auto data = "CELL\x49\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of LNAM is not four")
    {
      {
        const auto data = "CELL\x3E\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x03\0\x9F\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0\x34"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x40\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x05\0\x9F\x03\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0\x34"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of LNAM can be read")
    {
      const auto data = "CELL\x3F\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x39\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x05\0\xFF\xFF\x7F\x7F\0XCLR\x04\0hf\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x34\0\0\0\0\0\0\0\x66\x66\x10\0\x1D\x66\x0A\0\x25\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\xFF\xFF\xFF\xFF\xFD\xFF\xFF\xFF\0\xBAS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCLR\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XNAMs")
    {
      const auto data = "CELL\x46\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0\x34XNAM\x01\0\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XNAM is not one")
    {
      {
        const auto data = "CELL\x3E\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x40\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x02\0\x34\x12"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XNAM can be read")
    {
      const auto data = "CELL\x3F\0\0\0\0\0\0\0\x03\x46\x0B\0\x1B\x5E\x64\0\x1E\0\0\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0LNAM\x04\0\x9F\x03\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXNAM\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XWCNs")
    {
      const auto data = "CELL\x91\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XWCN is not four")
    {
      {
        const auto data = "CELL\x86\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x03\0\x04\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x88\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x05\0\x04\0\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XWCN can be read")
    {
      const auto data = "CELL\x87\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XWCSs")
    {
      const auto data = "CELL\x82\0\0\0\0\0\0\0\x17\x55\x01\0\x0A\x63\x36\0\x22\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\x03\0\0\0\x01\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXWCS\x04\0\x03\0\0\0XWCS\x04\0\x03\0\0\0XWCU\x30\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0XCIM\x04\0\x0EV\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XWCS is not four")
    {
      {
        const auto data = "CELL\x77\0\0\0\0\0\0\0\x17\x55\x01\0\x0A\x63\x36\0\x22\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\x03\0\0\0\x01\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXWCS\x03\0\x03\0\0XWCU\x30\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0XCIM\x04\0\x0EV\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x79\0\0\0\0\0\0\0\x17\x55\x01\0\x0A\x63\x36\0\x22\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\x03\0\0\0\x01\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXWCS\x05\0\0\x03\0\0\0XWCU\x30\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0XCIM\x04\0\x0EV\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XWCS can be read")
    {
      const auto data = "CELL\x78\0\0\0\0\0\0\0\x17\x55\x01\0\x0A\x63\x36\0\x22\0\x01\0DATA\x02\0\x02\0XCLC\x0C\0\x03\0\0\0\x01\0\0\0\0\0\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXWCS\x04\0\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XWCUs")
    {
      const auto data = "CELL\xCD\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?XWCU@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XWCU is not a multiple of 16")
    {
      {
        const auto data = "CELL\x47\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x88\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU\x41\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x8CP9\xC4\xE9\x83o\xC4r\xFB\xB9\xC0\x03\0\0\0\0\0\0\0\0\0\x80\xBF\0\0\0\x80\x1F\x16j?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XWCU can be read")
    {
      const auto data = "CELL\x87\x07\0\0\0\0\0\0\xE0\x9B\0\0\x17\x68\x0A\0\x28\0\x05\0EDID\x17\0EvergreenGroveExterior\0DATA\x02\0B\0XCLC\x0C\0\xF3\xFF\xFF\xFF\xEF\xFF\xFF\xFF\0\xFDS\0TVDT\xAC\x02\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x10\0\0\0\0\0\0\0\0\0\0\x04\0\0\x01\0\x1B\0\0\x04\x17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x1E#\xF8\x13\xF8`\xFF\xFF\xFF\xE0\x03\0\0\0\0\0\0\0\0\0\0\xF0\xFB\xFF\xFF\xE0\xAF\x03\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0MHDT\x04\x04\0\0\x90\xC4\x01\x01\x01\x02\x02\x02\x01\x01\x01\x01\x01\x01\x01\x02\x02\x0B\x02\x02\x03\x03\x05\x05\x05\x05\x04\x05\x05\x05\x05\x06\x06\x07\x01\x0B\x0B\x01\x01\x01\x01\x01\x01\x01\x01\x01\x02\x02\x02\x0B\x02\x03\x03\x03\x05\x05\x04\x04\x04\x05\x05\x05\x05\x06\x07\x08\0\x01\x01\x01\x01\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x05\x04\x04\x04\x04\x05\x05\x05\x05\x06\x06\x08\0\x01\x01\x01\x02\x02\x02\x02\x0C\x0C\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x04\x04\x04\x04\x04\x05\x05\x05\x06\x07\x07\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x04\x04\x04\x05\x04\x04\x05\x05\x05\x06\x06\x06\x08\0\x0B\x01\x02\x12\x12\x12\x02\x02\x02\x02\x02\x03\x14\x03\x02\x02\x03\x03\x05\x05\x05\x05\x05\x04\x05\x05\x06\x06\x06\x08\x08\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x15\x02\x02\x03\x03\x05\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x14\x02\x02\x15\x15\x14\x05\x05\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x0C\x0C\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x15\x15\x0C\x02\x03\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x01\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x0B\x02\x02\x02\x02\x10\x10\x03\x03\x04\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x05\x07\x02\x02\x04\x02\x02\x02\x07\x04\x09\x03\x03\x05\x05\x05\x05\x06\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x07\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x08\x04\x03\x03\x03\x05\x05\x05\x05\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x03\x05\x05\x05\x06\x08\x08\x08\x08\x02\x02\x02\x02\x0D\x0D\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x04\x06\x06\x06\x08\x08\x08\x08\x08\x02\x02\x02\x02\x02\x02\x02\x0C\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x03\x06\x06\x06\x07\x08\x08\x08\x08\x09\x02\x02\x0D\x0D\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x03\x02\x02\x02\x02\x04\x06\x06\x07\x07\x08\x08\x08\x08\x09\x02\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x06\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x06\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x08\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x05\x07\x08\x09\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x14\x14\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x10\x03\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x03\x02\x02\x02\x02\x10\x10\x06\x07\x08\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x04\x02\x02\x02\x02\x02\x02\x03\x03\x06\x07\x07\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x0C\x03\x03\x03\x03\x02\x02\x02\x02\x03\x04\x02\x02\x02\x02\x03\x03\x03\x03\x06\x07\x08\x08\x09\x09\x09\x09\x09\x09\x03\x02\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x02\x02\x02\x02\x03\x03\x02\x05\x06\x0D\x0D\x13\x13\x09\x09\x09\x09\x09\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x02\x02\x03\x02\x02\x02\x05\x05\x05\x05\x07\x08\x0C\x0C\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x02\x02\x02\x03\x03\x03\x03\x02\x05\x05\x05\x05\x06\x07\x08\x09\x09\x09\x09\x09\x09\x0D\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x08\x08\x09\x09\x09\x09\x09\x03\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x04\x05\x05\x05\x05\x05\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x02\x03\x03\x13\x13\x03\x03\x03\x03\x03\x03\x03\x03\x04\x04\x04\x05\x05\x05\x05\x0B\x05\x06\x06\x06\x06\x09\x09\x09\x09\x09\x09\x02\x03\x03\x03\x03\x03\x03\x0E\x03\x03\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0B\x0B\x06\x06\x07\x09\x09\x09\x09\x09\x09\x09\x02\x02\x03\x03\x03\x03\x04\x0E\x04\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x0F\x0F\x06\x0F\x07\x09\x09\x09\x09\x09\x09\x09\x09LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\xAF\xC4XCLR\x20\0-\xA7\x02\0\xB4)\x06\0\xD7\xEE\x08\0\x98\xD7\x0B\0d8\x08\0\x97\xFB\x08\0@|\x0F\0YX\x0C\0XLCN\x04\0t\x91\x01\0XWCN\x04\0\x04\0\0\0XWCU@\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCIMs")
    {
      const auto data = "CELL\x60\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCIM is not four")
    {
      {
        const auto data = "CELL\x55\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x03\0\x0EV\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x57\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x05\0\x0EV\x01\0\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCIM can be read")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0E"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XCIM is zero")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\0\0\0\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XLCNs")
    {
      const auto data = "CELL\x60\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XLCN is not four")
    {
      {
        const auto data = "CELL\x55\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x03\0\xB8\x92\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x57\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x05\0\xB8\x92\x01\0\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XLCN can be read")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XLCN is zero")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\0\0\0\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XEZNs")
    {
      const auto data = "CELL\x1D\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XEZN is not four")
    {
      {
        const auto data = "CELL\x12\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x03\0\xB9\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x14\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x05\0\xB9\xFB\x09\0\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XEZN can be read")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XEZN is zero")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\0\0\0\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCWTs")
    {
      const auto data = "CELL\x6C\0\0\0\0\0\0\0\xC1\x37\x02\0\x0D\x65\x4F\0\x23\0\x06\0EDID\x1A\0BlackreachSinderionsShack\0DATA\x02\0\x02\0XCLC\x0C\0\x04\0\0\0\x04\0\0\0\0\x80S\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\xC0Z\xC6XCLR\x04\0.\x8C\x04\0XCWT\x04\0\x18\0\0\0XCWT\x04\0\x18\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCWT is not four")
    {
      {
        const auto data = "CELL\x61\0\0\0\0\0\0\0\xC1\x37\x02\0\x0D\x65\x4F\0\x23\0\x06\0EDID\x1A\0BlackreachSinderionsShack\0DATA\x02\0\x02\0XCLC\x0C\0\x04\0\0\0\x04\0\0\0\0\x80S\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\xC0Z\xC6XCLR\x04\0.\x8C\x04\0XCWT\x03\0\x18\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x62\0\0\0\0\0\0\0\xC1\x37\x02\0\x0D\x65\x4F\0\x23\0\x06\0EDID\x1A\0BlackreachSinderionsShack\0DATA\x02\0\x02\0XCLC\x0C\0\x04\0\0\0\x04\0\0\0\0\x80S\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\xC0Z\xC6XCLR\x04\0.\x8C\x04\0XCWT\x05\0\x18\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCWT can be read")
    {
      const auto data = "CELL\x62\0\0\0\0\0\0\0\xC1\x37\x02\0\x0D\x65\x4F\0\x23\0\x06\0EDID\x1A\0BlackreachSinderionsShack\0DATA\x02\0\x02\0XCLC\x0C\0\x04\0\0\0\x04\0\0\0\0\x80S\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\xC0Z\xC6XCLR\x04\0.\x8C\x04\0XCWT\x04\0\x18\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x53\0\0\0\0\0\0\0\x20\x42\x10\0\x15\x67\x27\0\x27\0\x02\0EDID\x0A\0MazeStart\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFF\xFF\xFF\xFF\x0F\x17\0\0LTMP\x04\0\0\0\0\0XCLW\x04\0\0\0\0\xCFXCLR\x04\0hf\x10\0XCMO\x05\0\xC2\xD4\x02\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XOWNs")
    {
      const auto data = "CELL\x1D\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XOWN is not four")
    {
      {
        const auto data = "CELL\x12\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x03\0\x22\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x14\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x05\0\x22\x25\x0A\0\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XOWN can be read")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XOWN is zero")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\0\0\0\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XILLs")
    {
      const auto data = "CELL\xDA\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x04\0]\x84\x01\x02XILL\x04\0]\x84\x01\x02XCMO\x04\0\x81\x42\x03\x02XCAS\x04\0m[\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XILL is not four")
    {
      {
        const auto data = "CELL\xCF\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x03\0]\x01\x02XCMO\x04\0\x81\x42\x03\x02XCAS\x04\0m[\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\xD1\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x05\0]\x84\x01\x02\0XCMO\x04\0\x81\x42\x03\x02XCAS\x04\0m[\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XILL can be read")
    {
      const auto data = "CELL\xD0\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x04\0]\x84"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XILL is zero")
    {
      const auto data = "CELL\xD0\0\0\0\0\0\0\0\x77\x82\x01\x02\x07\x77\x55\0\x2B\0\x08\0EDID\x10\0DLC2RRIenthFarm\0FULL\x04\0\x55\x01\0\0DATA\x02\0\x01\0XCLL\x5C\0#&&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80?&&'\0\x1F&$\0#'&\0#$&\0\x10\x12\x11\0\x35\x39:\0\0\0\0\0\0\0\x80?\0\0\0\0\0\0\x80?\0\0\0\0\0\0\0\0\x9E\x07\0\0LTMP\x04\0\x96\x11\x0A\0XCLW\x04\0\0\0\0\0XLCN\x04\0\x1F\x9A\x01\x02XOWN\x04\0&\x84\x01\x02XILL\x04\0\0\0\0\0XCMO\x04\0\x81\x42\x03\x02XCAS\x04\0m[\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCCMs")
    {
      const auto data = "CELL\x1D\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCCM is not four")
    {
      {
        const auto data = "CELL\x12\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x03\0.\x04\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x14\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x05\0.\x04\x07\0\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCCM can be read")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XCCM is zero")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0\0\0\0\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XWEMs")
    {
      const auto data = "CELL\x3F\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XWEM\x26\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XWEM > 512")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x26\x02Gata\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XWEM is beyond stream")
    {
      const auto data = "CELL\x13\x01\0\0\0\0\0\0\xCF\x6B\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x13\0RiftenRaggedFlagon\0FULL\x04\0>\xED\0\0DATA\x02\0\xA3\0XCLL\x5C\0\x22\x1F\x13\0?GI\0`fH\0\0\0\xAA\x43\0\0zE\0\0\0\0Z\0\0\0\0\0\x80?\0@\x9C\x45\xCD\xCCL?%\x1F\x13\0\x1E\x1F\x12\0\"\x20\x13\0\"\x1D\x13\0\x10\x0E\x08\0\x33/\x1D\0`fH\0\0\0\x80?`fH\0\0\0\x80?\0\0zE\0@\x9C\x45\xEF\x03\0\0LTMP\x04\0\x34\xC7\x05\0XCLW\x04\0\0\0\0\0XLCN\x04\0J&\x02\0XCIM\x04\0=\xC7\x05\0XCMO\x04\0[a\x05\0XEZN\x04\0\xB9\xFB\x09\0XOWN\x04\0\x22\x25\x0A\0XCCM\x04\0.\x04\x07\0XWEM\x31\0Data\\Textures\\Cubemaps\\WRTemple_e.dds\0XCAS\x04\0\x60\x8E\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XCASs")
    {
      const auto data = "CELL\x60\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB\x10\0XCAS\x04\0\x83\xCB\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XCAS is not four")
    {
      {
        const auto data = "CELL\x55\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x03\0\x83\xCB\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "CELL\x57\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x05\0\x83\xCB\x10\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XCAS can be read")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\x83\xCB"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XCAS is zero")
    {
      const auto data = "CELL\x56\0\0\0\0\0\0\0\xF7\x97\x0C\0\x14\x67\x27\0\x27\0\x03\0DATA\x02\0\x02\0XCLC\x0C\0\0\0\0\0\xFE\xFF\xFF\xFF\0\xCFS\0LTMP\x04\0\0\0\0\0XCLW\x04\0\xFF\xFF\x7F\x7FXCIM\x04\0\x0EV\x01\0XLCN\x04\0\xB8\x92\x01\0XCMO\x04\0\xDDK\x09\0XCAS\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
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
