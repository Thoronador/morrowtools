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
#include "../../../../../sr/base/records/WorldSpaceRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("WorldSpaceRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    WorldSpaceRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE( record.unknownRNAMs.empty() );
    REQUIRE_FALSE( record.unknownMHDT.isPresent() );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.hasWCTR );
    REQUIRE( record.centerCellX == 0 );
    REQUIRE( record.centerCellY == 0 );
    REQUIRE( record.interiorLightingFormID == 0 );
    REQUIRE( record.encounterZoneFormID == 0 );
    REQUIRE( record.climateFormID == 0 );
    REQUIRE( record.waterFormID == 0 );
    REQUIRE( record.LODWaterTypeFormID == 0 );
    REQUIRE_FALSE( record.hasNAM4 );
    REQUIRE( record.LODWaterHeight == 0.0f );
    REQUIRE_FALSE( record.hasDNAM );
    REQUIRE( record.unknownDNAM == 0 );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE_FALSE( record.unknownMNAM.isPresent() );
    REQUIRE( record.locationFormID == 0 );
    REQUIRE( record.parentWorldSpaceFormID == 0 );
    REQUIRE_FALSE( record.hasPNAM );
    REQUIRE( record.unknownPNAM == 0 );
    REQUIRE_FALSE( record.unknownONAM.isPresent() );
    REQUIRE( record.distantLODMultiplier == 0.0f );
    REQUIRE_FALSE( record.hasDATA );
    REQUIRE( record.unknownDATA == 0 );
    REQUIRE_FALSE( record.hasNAM0 );
    REQUIRE( record.unknownNAM0 == 0 );
    REQUIRE_FALSE( record.hasNAM9 );
    REQUIRE( record.unknownNAM9 == 0 );
    REQUIRE( record.musicFormID == 0 );
    REQUIRE( record.HD_LOD_DiffuseTexture.empty() );
    REQUIRE( record.HD_LOD_NormalTexture.empty() );
    REQUIRE( record.unknownXWEM.empty() );
    REQUIRE_FALSE( record.unknownOFST.isPresent() );
  }

  SECTION("equals")
  {
    WorldSpaceRecord a;
    WorldSpaceRecord b;

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

      SECTION("RNAMs mismatch")
      {
        a.unknownRNAMs.push_back(BinarySubRecord());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownRNAMs.push_back(BinarySubRecord());
        b.unknownRNAMs.push_back(BinarySubRecord());

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

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("WCTR mismatch")
      {
        a.hasWCTR = true;
        b.hasWCTR = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasWCTR = true;
        a.centerCellX = 1;
        a.centerCellY = 2;
        b.hasWCTR = true;

        b.centerCellX = 1;
        b.centerCellY = 4;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.centerCellX = 5;
        b.centerCellY = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("interiorLightingFormID mismatch")
      {
        a.interiorLightingFormID = 0x01234567;
        b.interiorLightingFormID = 0x0FEDCBA9;

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

      SECTION("climateFormID mismatch")
      {
        a.climateFormID = 0x01234567;
        b.climateFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("waterFormID mismatch")
      {
        a.waterFormID = 0x01234567;
        b.waterFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("LODWaterTypeFormID mismatch")
      {
        a.LODWaterTypeFormID = 0x01234567;
        b.LODWaterTypeFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM4 mismatch")
      {
        a.hasNAM4 = true;
        b.hasNAM4 = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM4 = false;
        b.hasNAM4 = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM4 = true;
        a.LODWaterHeight = 1.0f;
        b.hasNAM4 = true;
        b.LODWaterHeight = 2.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.hasDNAM = true;
        b.hasDNAM = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDNAM = false;
        b.hasDNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDNAM = true;
        a.unknownDNAM = 1;
        b.hasDNAM = true;
        b.unknownDNAM = 2;

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

      SECTION("MNAM mismatch")
      {
        a.unknownMNAM.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMNAM.setPresence(false);
        b.unknownMNAM.setPresence(true);

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

      SECTION("parentWorldSpaceFormID mismatch")
      {
        a.parentWorldSpaceFormID = 0x01234567;
        b.parentWorldSpaceFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PNAM mismatch")
      {
        a.hasPNAM = true;
        b.hasPNAM = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasPNAM = false;
        b.hasPNAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasPNAM = true;
        a.unknownPNAM = 1;
        b.hasPNAM = true;
        b.unknownPNAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ONAM mismatch")
      {
        a.unknownONAM.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownONAM.setPresence(false);
        b.unknownONAM.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("distantLODMultiplier mismatch")
      {
        a.distantLODMultiplier = 1.0f;
        b.distantLODMultiplier = 2.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DATA mismatch")
      {
        a.hasDATA = true;
        b.hasDATA = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDATA = false;
        b.hasDATA = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDATA = true;
        a.unknownDATA = 1;
        b.hasDATA = true;
        b.unknownDATA = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM0 mismatch")
      {
        a.hasNAM0 = true;
        b.hasNAM0 = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM0 = false;
        b.hasNAM0 = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM0 = true;
        a.unknownNAM0 = 1;
        b.hasNAM0 = true;
        b.unknownNAM0 = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM9 mismatch")
      {
        a.hasNAM9 = true;
        b.hasNAM9 = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM9 = false;
        b.hasNAM9 = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasNAM9 = true;
        a.unknownNAM9 = 1;
        b.hasNAM9 = true;
        b.unknownNAM9 = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("musicFormID mismatch")
      {
        a.musicFormID = 0x01234567;
        b.musicFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HD_LOD_DiffuseTexture mismatch")
      {
        a.HD_LOD_DiffuseTexture = "foo\\bar.dds";
        b.HD_LOD_DiffuseTexture = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HD_LOD_NormalTexture mismatch")
      {
        a.HD_LOD_NormalTexture = "foo\\bar.dds";
        b.HD_LOD_NormalTexture = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("XWEM mismatch")
      {
        a.unknownXWEM = "foo\\bar.dds";
        b.unknownXWEM = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("OFST mismatch")
      {
        a.unknownOFST.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownOFST.setPresence(false);
        b.unknownOFST.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    WorldSpaceRecord record;

    REQUIRE( record.getRecordType() == cWRLD );
  }

  SECTION("getWriteSize")
  {
    WorldSpaceRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 20 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 32 );
    }

    SECTION("size adjusts with length of RNAMs")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "RNAM\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        BinarySubRecord rnam;
        REQUIRE( rnam.loadFromStream(stream, cRNAM, true) );
        record.unknownRNAMs.push_back(rnam);
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "RNAM\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        BinarySubRecord rnam;
        REQUIRE( rnam.loadFromStream(stream, cRNAM, true) );
        record.unknownRNAMs.push_back(rnam);
      }
      REQUIRE( record.getWriteSize() == 51 );
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

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when WCTR is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasWCTR = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when interiorLightingFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.interiorLightingFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when encounterZoneFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.encounterZoneFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when climateFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.climateFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when waterFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.waterFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when LODWaterTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.LODWaterTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when NAM4 is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasNAM4 = true;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when DNAM is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasDNAM = true;
      REQUIRE( record.getWriteSize() == 34 );
    }

    SECTION("size adjusts with length of model path")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 38 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 47 );
    }

    SECTION("size adjusts with length of MODT")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts with length of MNAM")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MNAM\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMNAM.loadFromStream(stream, cMNAM, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "MNAM\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMNAM.loadFromStream(stream, cMNAM, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when locationFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.locationFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when parentWorldSpaceFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.parentWorldSpaceFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts when PNAM is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasPNAM = true;
      REQUIRE( record.getWriteSize() == 28 );
    }

    SECTION("size adjusts with length of ONAM")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "ONAM\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownONAM.loadFromStream(stream, cONAM, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "ONAM\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownONAM.loadFromStream(stream, cONAM, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when DATA is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasDATA = true;
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts when NAM0 is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasNAM0 = true;
      REQUIRE( record.getWriteSize() == 34 );
    }

    SECTION("size adjusts when NAM9 is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.hasNAM9 = true;
      REQUIRE( record.getWriteSize() == 34 );
    }

    SECTION("size adjusts when musicFormID is present")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.musicFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 30 );
    }

    SECTION("size adjusts with length of HD_LOD_DiffuseTexture path")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.HD_LOD_DiffuseTexture = "foo\\bar.dds";
      REQUIRE( record.getWriteSize() == 38 );

      record.HD_LOD_DiffuseTexture = "foo\\barbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 47 );
    }

    SECTION("size adjusts with length of HD_LOD_NormalTexture path")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.HD_LOD_NormalTexture = "foo\\bar.dds";
      REQUIRE( record.getWriteSize() == 38 );

      record.HD_LOD_NormalTexture = "foo\\barbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 47 );
    }

    SECTION("size adjusts with length of XWEM path")
    {
      REQUIRE( record.getWriteSize() == 20 );

      record.unknownXWEM = "foo\\bar.dds";
      REQUIRE( record.getWriteSize() == 38 );

      record.unknownXWEM = "foo\\barbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 47 );
    }

    SECTION("size adjusts with length of OFST")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "OFST\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownOFST.loadFromStream(stream, cOFST, true) );
      }
      REQUIRE( record.getWriteSize() == 44 );

      {
        const std::string_view data = "OFST\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownOFST.loadFromStream(stream, cOFST, true) );
      }
      REQUIRE( record.getWriteSize() == 27 );
    }

    SECTION("size adjusts with length of OFST in oversized case")
    {
      REQUIRE( record.getWriteSize() == 20 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string data = std::string("\0\0", 2) + std::string(65536L, 'c');
        std::istringstream stream;
        stream.str(data);
        REQUIRE( record.unknownOFST.loadFromStreamExtended(stream, cOFST, false, 65536L) );
      }
      REQUIRE( record.getWriteSize() == 65572L );

      {
        const std::string data = std::string("\0\0", 2) + std::string(65542L, 'c');
        std::istringstream stream;
        stream.str(data);
        REQUIRE( record.unknownOFST.loadFromStreamExtended(stream, cOFST, false, 65542L) );
      }
      REQUIRE( record.getWriteSize() == 65578L );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }
}
