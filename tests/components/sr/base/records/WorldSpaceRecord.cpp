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

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00003EF4, "foo");

    SECTION("default: load record")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WorldSpaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001CDD3 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0009 );
      // -- record data
      REQUIRE( record.editorID == "LabyrinthianWorld" );
      REQUIRE( record.unknownRNAMs.size() == 4 );
      const auto RNAM_0 = std::string_view(reinterpret_cast<const char*>(record.unknownRNAMs[0].data()), record.unknownRNAMs[0].size());
      REQUIRE( RNAM_0 == "\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFF"sv );
      const auto RNAM_1 = std::string_view(reinterpret_cast<const char*>(record.unknownRNAMs[1].data()), record.unknownRNAMs[1].size());
      REQUIRE( RNAM_1 == "\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFF"sv );
      const auto RNAM_2 = std::string_view(reinterpret_cast<const char*>(record.unknownRNAMs[2].data()), record.unknownRNAMs[2].size());
      REQUIRE( RNAM_2 == "\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFF"sv );
      const auto RNAM_3 = std::string_view(reinterpret_cast<const char*>(record.unknownRNAMs[3].data()), record.unknownRNAMs[3].size());
      REQUIRE( RNAM_3 == "\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF"sv );
      REQUIRE_FALSE( record.unknownMHDT.isPresent() );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00003EF4 );
      REQUIRE( record.hasWCTR );
      REQUIRE( record.centerCellX == -3 );
      REQUIRE( record.centerCellY == 10 );
      REQUIRE( record.interiorLightingFormID == 0x0003128C );
      REQUIRE( record.encounterZoneFormID == 0x0003EC42 );
      REQUIRE( record.climateFormID == 0 );
      REQUIRE( record.waterFormID == 0x00000018 );
      REQUIRE( record.LODWaterTypeFormID == 0x00000018 );
      REQUIRE( record.hasNAM4 );
      REQUIRE( record.LODWaterHeight == 0.0f );
      REQUIRE( record.hasDNAM );
      REQUIRE( record.unknownDNAM == 0xC380000000000000 );
      REQUIRE( record.modelPath.empty() );
      REQUIRE_FALSE( record.unknownMODT.isPresent() );
      REQUIRE_FALSE( record.unknownMNAM.isPresent() );
      REQUIRE( record.locationFormID == 0x00019262 );
      REQUIRE( record.parentWorldSpaceFormID == 0x0000003C );
      REQUIRE( record.hasPNAM );
      REQUIRE( record.unknownPNAM == 0x0014 );
      REQUIRE( record.unknownONAM.isPresent() );
      const auto ONAM = std::string_view(reinterpret_cast<const char*>(record.unknownONAM.data()), record.unknownONAM.size());
      REQUIRE( ONAM == "\0\0\x80\x3F\0\0\0\0\0\0\0\0\0\0\x80\xC5"sv );
      REQUIRE( record.distantLODMultiplier == 1.0f );
      REQUIRE( record.hasDATA );
      REQUIRE( record.unknownDATA == 0x61 );
      REQUIRE( record.hasNAM0 );
      REQUIRE( record.unknownNAM0 == 0xC5800000C6800000 );
      REQUIRE( record.hasNAM9 );
      REQUIRE( record.unknownNAM9 == 0x4750000045800000 );
      REQUIRE( record.musicFormID == 0x0002D4C2 );
      REQUIRE( record.HD_LOD_DiffuseTexture.empty() );
      REQUIRE( record.HD_LOD_NormalTexture.empty() );
      REQUIRE( record.unknownXWEM.empty() );
      REQUIRE( record.unknownOFST.isPresent() );
      const auto OFST = std::string_view(reinterpret_cast<const char*>(record.unknownOFST.data()), record.unknownOFST.size());
      REQUIRE( OFST == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "WRLD\0\0\0\0\x20\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      WorldSpaceRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x0001CDD3 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0009 );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0FAIL\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\x02LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0Laby\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of RNAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no FULL")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46\x41IL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0FULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WCTR")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of WCTR is not 4")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x03\0\xFD\xFF\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x05\0\xFD\xFF\x0A\0\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of WCTR can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple LTMPs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of LTMP is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x03\0\x8C\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x05\0\x8C\x12\x03\0\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of LTMP can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: LTMP is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\0\0\0\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XEZNs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XEZN is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x03\0B\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x05\0B\xEC\x03\0\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XEZN can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XEZN is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0\0\0\0\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple XLCNs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of XLCN is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x03\0b\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x05\0b\x92\x01\0\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of XLCN can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: XLCN is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0\0\0\0\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WNAMs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of WNAM is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x03\0<\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x05\0<\0\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of WNAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: WNAM is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0\0\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PNAMs")
    {
      const auto data = "WRLD\xFB\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PNAM is not two")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x01\0\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x03\0\x14\0\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of PNAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM2s")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM2 is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x03\0\x18\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x05\0\x18\0\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM2 can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NAM2 is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\0\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM3s")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM3 is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x03\0\x18\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x05\0\x18\0\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM3 can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: NAM3 is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\0\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM4s")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM4 is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x03\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x05\0\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM4 can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "WRLD\x01\x03\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not eight")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x07\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x09\0\0\0\0\0\0\0\x80\xC3\0ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DNAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ONAMs")
    {
      const auto data = "WRLD\x09\x03\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of ONAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAMAs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAMA is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x03\0\0\0?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x05\0\0\0\x80?\0DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAMA can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing NAMA sub record")
    {
      const auto data = "WRLD\xE9\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5\x44\x41TA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "WRLD\xFA\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aDATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not one")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x00\0NAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x02\0a\0NAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM0s")
    {
      const auto data = "WRLD\x01\x03\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM0 is not eight")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x07\0\0\0\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x09\0\0\0\x80\xC6\0\0\x80\xC5\0NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM0 can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM9s")
    {
      const auto data = "WRLD\x01\x03\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGNAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM9 is not eight")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x07\0\0\0\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x09\0\0\0\x80\x45\0\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM9 can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const auto data = "WRLD\xFD\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0ZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ZNAM is not four")
    {
      {
        const auto data = "WRLD\xF2\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x03\0\xC2\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "WRLD\xF4\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x05\0\xC2\xD4\x02\0\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read WRLD, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        WorldSpaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\0\0\0\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OFSTs")
    {
      const auto data = "WRLD\xFA\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC2\x13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xDD\x12\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0L\x19\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD3\x39\0\0\x16l\x01\0\xDB\xB4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x80\x1A\0\0\x37\xDC\0\0>L\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xEB\x16\0\0I\x14\0\0\x44;\0\0S\x88\x01\0L\x13\0\0\0\0\0\0\0\0\0\0)\x1A\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0OFST\x01\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of OFST can be read")
    {
      const auto data = "WRLD\xF3\x02\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0OFSTh\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OFST")
    {
      const auto data = "WRLD\x85\x01\0\0\0\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0EDID\x12\0LabyrinthianWorld\0RNAM\x10\0\x0A\0\xFC\xFF\x01\0\0\0\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFD\xFF\x06\0\0\0\x0A\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0C\x07\x10\0\x0A\0\xFD\xFF\x0A\x07\x10\0\x0A\0\xFD\xFF\x0E\x07\x10\0\x0A\0\xFD\xFF\x04\xDB\x01\0\x0A\0\xFC\xFFRNAM8\0\x0A\0\xFE\xFF\x06\0\0\0\xFC\x06\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\x04\x07\x10\0\x0A\0\xFE\xFF\xFF\x06\x10\0\x0A\0\xFE\xFF\xFC\x06\x10\0\x0A\0\xFE\xFFRNAM\x18\0\x0B\0\xFE\xFF\x02\0\0\0\xFA\x06\x10\0\x0B\0\xFE\xFF\xFA\x06\x10\0\x0B\0\xFE\xFF\x46ULL\x04\0\xF4>\0\0WCTR\x04\0\xFD\xFF\x0A\0LTMP\x04\0\x8C\x12\x03\0XEZN\x04\0B\xEC\x03\0XLCN\x04\0b\x92\x01\0WNAM\x04\0<\0\0\0PNAM\x02\0\x14\0NAM2\x04\0\x18\0\0\0NAM3\x04\0\x18\0\0\0NAM4\x04\0\0\0\0\0DNAM\x08\0\0\0\0\0\0\0\x80\xC3ONAM\x10\0\0\0\x80?\0\0\0\0\0\0\0\0\0\0\x80\xC5NAMA\x04\0\0\0\x80?DATA\x01\0aNAM0\x08\0\0\0\x80\xC6\0\0\x80\xC5NAM9\x08\0\0\0\x80\x45\0\0PGZNAM\x04\0\xC2\xD4\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read WRLD, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      WorldSpaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      WorldSpaceRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x0001CDD3;
      record.headerRevision = 0x0055691B;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0009;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "WRLD\0\0\0\0\x20\0\0\0\xD3\xCD\x01\0\x1B\x69\x55\0\x28\0\x09\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
