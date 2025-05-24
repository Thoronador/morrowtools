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
#include "../../../../lib/sr/records/CameraShotRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"
#include "../../limited_streambuf.hpp"

TEST_CASE("CameraShotRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    CameraShotRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.dataLen == CameraShotRecord::DataLengthType::dlt40Byte );
    REQUIRE( record.cameraAction == CameraShotRecord::cActionShoot );
    REQUIRE( record.cameraLocation == CameraShotRecord::cLocationAttacker );
    REQUIRE( record.cameraTarget == CameraShotRecord::cLocationTarget );
    REQUIRE( record.cameraFlags == 0 );
    REQUIRE( record.timeMultPlayer == 1.0f );
    REQUIRE( record.timeMultTarget == 1.0f );
    REQUIRE( record.timeMultGlobal == 1.0f );
    REQUIRE( record.maxTime == 20.0f );
    REQUIRE( record.minTime == 0.0f );
    REQUIRE( record.targetPercentBetweenActors == 0.0f );
    REQUIRE( record.nearTargetDistance == 0.0f );
    REQUIRE( record.imageSpaceModFormID == 0 );
  }

  SECTION("equals")
  {
    CameraShotRecord a;
    CameraShotRecord b;

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

      SECTION("dataLen mismatch")
      {
        a.dataLen = CameraShotRecord::DataLengthType::dlt40Byte;
        b.dataLen = CameraShotRecord::DataLengthType::dlt44Byte;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("cameraAction mismatch")
      {
        a.cameraAction = 1;
        b.cameraAction = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("cameraLocation mismatch")
      {
        a.cameraLocation = 1;
        b.cameraLocation = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("cameraTarget mismatch")
      {
        a.cameraTarget = 1;
        b.cameraTarget = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("cameraFlags mismatch")
      {
        a.cameraFlags = 1;
        b.cameraFlags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("timeMultPlayer mismatch")
      {
        a.timeMultPlayer = 1.0f;
        b.timeMultPlayer = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("timeMultTarget mismatch")
      {
        a.timeMultTarget = 1.0f;
        b.timeMultTarget = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("timeMultGlobal mismatch")
      {
        a.timeMultGlobal = 1.0f;
        b.timeMultGlobal = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("maxTime mismatch")
      {
        a.maxTime = 1.0f;
        b.maxTime = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("minTime mismatch")
      {
        a.minTime = 1.0f;
        b.minTime = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("targetPercentBetweenActors mismatch")
      {
        a.targetPercentBetweenActors = 1.0f;
        b.targetPercentBetweenActors = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("nearTargetDistance mismatch")
      {
        a.nearTargetDistance = 1.0f;
        b.nearTargetDistance = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("imageSpaceModFormID mismatch")
      {
        a.imageSpaceModFormID = 0x01234567;
        b.imageSpaceModFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    CameraShotRecord record;

    REQUIRE( record.getRecordType() == cCAMS );
  }

  SECTION("getWriteSize")
  {
    CameraShotRecord record;

    record.editorID = "foo";
    record.dataLen = CameraShotRecord::DataLengthType::dlt40Byte;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 56 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts with length of model path")
    {
      REQUIRE( record.getWriteSize() == 56 );

      record.modelPath = "foo\\bar.nif";
      REQUIRE( record.getWriteSize() == 74 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 83 );
    }

    SECTION("size adjusts with length of MODT")
    {
      record.editorID = "foo";
      REQUIRE( record.getWriteSize() == 56 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 80 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 63 );
    }

    SECTION("size adjusts with length of dataLen")
    {
      record.dataLen = CameraShotRecord::DataLengthType::dlt40Byte;
      REQUIRE( record.getWriteSize() == 56 );

      record.dataLen = CameraShotRecord::DataLengthType::dlt44Byte;
      REQUIRE( record.getWriteSize() == 60 );
    }

    SECTION("size adjusts when imageSpaceModFormID is present")
    {
      REQUIRE( record.getWriteSize() == 56 );

      record.imageSpaceModFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 66 );
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

    SECTION("default: load record where DATA.size() == 40")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CameraShotRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000E7735 );
      REQUIRE( record.headerRevision == 0x00256811 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "KillCam_LeftSideCloseUpA" );
      REQUIRE( record.modelPath == "Cameras\\KillCam_LeftSideCloseUpA.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.dataLen == CameraShotRecord::DataLengthType::dlt40Byte );
      REQUIRE( record.cameraAction == 0 );
      REQUIRE( record.cameraLocation == 0 );
      REQUIRE( record.cameraTarget == 2 );
      REQUIRE( record.cameraFlags == 0x00000027 );
      REQUIRE( record.timeMultPlayer == 1.0f );
      REQUIRE( record.timeMultTarget == 1.0f );
      REQUIRE( record.timeMultGlobal == 1.0f );
      REQUIRE( record.maxTime == 6.0f );
      REQUIRE( record.minTime == 0.1f );
      REQUIRE( record.targetPercentBetweenActors == 0.0f );
      REQUIRE( record.nearTargetDistance == 0.0f );
      REQUIRE( record.imageSpaceModFormID == 0x00035301 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record where DATA.size() == 44")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CameraShotRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0100084A );
      REQUIRE( record.headerRevision == 0x00256E11 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.editorID == "F02bLeftFlyCam" );
      REQUIRE( record.modelPath == "Cameras\\MissileFlyByCam01.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.dataLen == CameraShotRecord::DataLengthType::dlt44Byte );
      REQUIRE( record.cameraAction == 1 );
      REQUIRE( record.cameraLocation == 1 );
      REQUIRE( record.cameraTarget == 1 );
      REQUIRE( record.cameraFlags == 0x00000027 );
      REQUIRE( record.timeMultPlayer == 1.0f );
      REQUIRE( record.timeMultTarget == 0.1f );
      REQUIRE( record.timeMultGlobal == 0.5f );
      REQUIRE( record.maxTime == 4.0f );
      REQUIRE( record.minTime == 0.1f );
      REQUIRE( record.targetPercentBetweenActors == 0.0f );
      REQUIRE( record.nearTargetDistance == 0.0f );
      REQUIRE( record.imageSpaceModFormID == 0x00035301 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("special: load deleted record")
    {
      const std::string_view data = "CAMS\0\0\0\0\x20\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CameraShotRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x000E7735 );
      REQUIRE( record.headerRevision == 0x00256811 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID.empty() );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "CAMS\0\0\0\0\x20\0\0\0\x35\x77\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0FAIL\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\x02KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x94\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no MODL")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0FAIL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\x02Tameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const std::string_view data = "CAMS\xBE\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const std::string_view data = "CAMS\x70\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x01\0\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "CAMS\xA6\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "CAMS\xC2\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not 40")
    {
      {
        const std::string_view data = "CAMS\x93\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x27\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CAMS\x95\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x29\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: length of DATA is not 44")
    {
      {
        const std::string_view data = "CAMS\x86\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2B\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CAMS\x88\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2D\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA sub record")
    {
      const std::string_view data = "CAMS\x66\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs camera action is out of range")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x04\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs camera location is out of range")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x04\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs camera target is out of range")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x04\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs camera flags are out of range")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x40\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultPlayer is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\0\xBF\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultPlayer is above 20.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\xA1\x41\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultTarget is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\0\xBF\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultTarget is above 20.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\xA1\x41\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultGlobal is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\xBF\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs timeMultGlobal is above 1.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\x90\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs maxTime is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\0\xBF\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs maxTime is above 120.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\xF1\x42\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs minTime is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\0\0\0\xBF\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs minTime is above 120.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\0\0\xF1\x42\0\0\0\0\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs targetPercentBetweenActors is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\xBF\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs targetPercentBetweenActors is above 100.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\xC9\x42\0\0\0\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs nearTargetDistance is below zero")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\0\0\xBFMNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DATAs nearTargetDistance is above 2000.0f")
    {
      const std::string_view data = "CAMS\x87\0\0\0\0\0\0\0\x4A\x08\0\x01\x11\x6E\x25\0\x28\0\0\0EDID\x0F\0F02bLeftFlyCam\0MODL\x1E\0Cameras\\MissileFlyByCam01.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x2C\0\x01\0\0\0\x01\0\0\0\x01\0\0\0\x27\0\0\0\0\0\x80\x3F\xCD\xCC\xCC\x3D\0\0\0\x3F\0\0\x80\x40\xCD\xCC\xCC\x3D\0\0\0\0\0\x10\xFA\x44MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MNAMs")
    {
      const std::string_view data = "CAMS\x9E\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53\x03\0MNAM\x04\0\x01\x53\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MNAM is not four")
    {
      {
        const std::string_view data = "CAMS\x93\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x03\0\x01\x53\x03"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const std::string_view data = "CAMS\x95\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x05\0\x01\x53\x03\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CAMS, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CameraShotRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of MNAM can be read")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\x01\x53"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: MNAM is zero")
    {
      const std::string_view data = "CAMS\x94\0\0\0\0\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0EDID\x19\0KillCam_LeftSideCloseUpA\0MODL\x25\0Cameras\\KillCam_LeftSideCloseUpA.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0DATA\x28\0\0\0\0\0\0\0\0\0\x02\0\0\0\x27\0\0\0\0\0\x80\x3F\0\0\x80\x3F\0\0\x80\x3F\0\0\xC0\x40\xCD\xCC\xCC\x3D\0\0\0\0MNAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CAMS, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CameraShotRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      CameraShotRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x000E7735;
      record.headerRevision = 0x00256811;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000C;
      // -- record data
      record.editorID = "FooDeletedBar";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CAMS\0\0\0\0\x20\0\0\0\x35\x77\x0E\0\x11\x68\x25\0\x28\0\x0C\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("failure: cannot write header data")
    {
      CameraShotRecord record;
      // Set some header data.
      record.headerFlags = 0;
      record.headerFormID = 0x000E7735;
      record.headerRevision = 0x00256811;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000C;

      // Writing should fail due to limited stream storage.
      MWTP::limited_streambuf<15> buffer;
      std::ostream stream(&buffer);
      REQUIRE( stream.good() );

      REQUIRE_FALSE( record.saveToStream(stream) );
    }
  }
}
