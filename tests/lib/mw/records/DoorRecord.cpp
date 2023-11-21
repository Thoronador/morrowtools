/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2023  Dirk Stolle

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
#include "../../../../lib/mw/records/DoorRecord.hpp"

TEST_CASE("MWTP::DoorRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    DoorRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Script.empty() );
    REQUIRE( record.SoundOpen.empty() );
    REQUIRE( record.SoundClose.empty() );
  }

  SECTION("equals")
  {
    DoorRecord a;
    DoorRecord b;

    SECTION("equal")
    {
      a.recordID = "Tree";
      a.ModelPath = "tree01.nif";

      b.recordID = "Tree";
      b.ModelPath = "tree01.nif";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ModelPath mismatch")
      {
        a.ModelPath = "mesh01.nif";
        b.ModelPath = "mesh02.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Name mismatch")
      {
        a.Name = "foo";
        b.Name = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Script mismatch")
      {
        a.Script = "foo";
        b.Script = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SoundOpen mismatch")
      {
        a.SoundOpen = "foo";
        b.SoundOpen = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SoundClose mismatch")
      {
        a.SoundClose = "foo";
        b.SoundClose = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DoorRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0x00000400 );
      // -- record data
      REQUIRE( record.recordID == "in_velothismall_ndoor_01_jeanne" );
      REQUIRE( record.Name == "Holzt\xFCr" );
      REQUIRE( record.ModelPath == "d\\In_velothismall_ndoor_01.NIF" );
      REQUIRE( record.Script == "jeannedoorScript" );
      REQUIRE( record.SoundOpen == "Door Latched One Open" );
      REQUIRE( record.SoundClose == "Door Latched One Close" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "DOOR\xB5\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0FAIL \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAMEs")
    {
      const auto data = "DOOR\xD9\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \x03\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME\xB5\0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0FAIL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple MODLs")
    {
      const auto data = "DOOR\xD8\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\x02\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\xB5\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FAIL\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const auto data = "DOOR\xC1\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\x01\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\xB5\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0FAIL\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRIs")
    {
      const auto data = "DOOR\xCA\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\x01\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\xB5\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SNAM")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0FAIL\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const auto data = "DOOR\xCF\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM > 256")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\x01\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\xB5\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ANAM")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0FAIL\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ANAMs")
    {
      const auto data = "DOOR\xD0\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\0\0\0Door Latched One Close\0ANAM\x17\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM > 256")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x17\x01\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM is beyond stream")
    {
      const auto data = "DOOR\xB5\0\0\0\0\0\0\0\0\x04\0\0NAME \0\0\0in_velothismall_ndoor_01_jeanne\0MODL\x1F\0\0\0d\\In_velothismall_ndoor_01.NIF\0FNAM\x08\0\0\0Holzt\xFCr\0SCRI\x11\0\0\0jeannedoorScript\0SNAM\x16\0\0\0Door Latched One Open\0ANAM\x19\0\0\0Door Latched One Close\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DOOR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DoorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
