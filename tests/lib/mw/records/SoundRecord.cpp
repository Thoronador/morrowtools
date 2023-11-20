/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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
#include "../../../../lib/mw/records/SoundRecord.hpp"

TEST_CASE("MWTP::SoundRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SoundRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.FileName.empty() );
    REQUIRE( record.Volume == 0 );
    REQUIRE( record.MinRange == 0 );
    REQUIRE( record.MaxRange == 0 );
  }

  SECTION("equals")
  {
    SoundRecord a;
    SoundRecord b;

    SECTION("equal")
    {
      a.recordID = "Foo";
      a.FileName = "blob.wav";

      b.recordID = "Foo";
      b.FileName = "blob.wav";

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

      SECTION("FileName mismatch")
      {
        a.FileName = "foo.wav";
        b.FileName = "bar.wav";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Volume mismatch")
      {
        a.Volume = 127;
        b.Volume = 80;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MinRange mismatch")
      {
        a.MinRange = 0;
        b.MinRange = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MaxRange mismatch")
      {
        a.MaxRange = 23;
        b.MaxRange = 45;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SoundRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "DefaultLandWater" );
      REQUIRE( record.FileName == "Fx\\FOOT\\splsh.wav" );
      REQUIRE( record.Volume == 255 );
      REQUIRE( record.MinRange == 0 );
      REQUIRE( record.MaxRange == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAMEs")
    {
      const auto data = "SOUN\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0NAME\x02\0\0\0A\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 255")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\x01\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0Def"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FAIL\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const auto data = "SOUN\x4C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0FNAM\x06\0\0\0a.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\x01\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0FAIL\x03\0\0\0\xFF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not three")
    {
      SECTION("too short")
      {
        const auto data = "SOUN\x3D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x02\0\0\0\xFF\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip SOUN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoundRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      SECTION("too long")
      {
        const auto data = "SOUN\x3F\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x04\0\0\0\xFF\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip SOUN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SoundRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read completely")
    {
      const auto data = "SOUN\x3E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0DefaultLandWater\0FNAM\x12\0\0\0Fx\\\x46OOT\\splsh.wav\0DATA\x03\0\0\0\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SOUN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SoundRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
