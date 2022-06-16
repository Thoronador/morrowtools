/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include "../../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/GlobalRecord.hpp"

TEST_CASE("MWTP::GlobalRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    GlobalRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Type == GlobalType::Short );
    REQUIRE( record.shortVal == 0 );
    REQUIRE( record.longVal == 0 );
    REQUIRE( record.floatVal == 0.0f );
  }

  SECTION("constructor with ID")
  {
    GlobalRecord record("bla");

    REQUIRE( record.recordID == "bla" );
    REQUIRE( record.Type == GlobalType::Short );
    REQUIRE( record.shortVal == 0 );
    REQUIRE( record.longVal == 0 );
    REQUIRE( record.floatVal == 0.0f );
  }

  SECTION("equals")
  {
    GlobalRecord a;
    GlobalRecord b;

    SECTION("equal")
    {
      a.recordID = "VarOne";
      a.Type = GlobalType::Short;
      a.shortVal = 5;

      b.recordID = "VarOne";
      b.Type = GlobalType::Short;
      b.shortVal = 5;

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

      SECTION("Type mismatch")
      {
        a.Type = GlobalType::Short;
        b.Type = GlobalType::Long;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Type = GlobalType::Short;
        b.Type = GlobalType::Float;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Type = GlobalType::Long;
        b.Type = GlobalType::Float;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("shortVal mismatch")
      {
        a.Type = GlobalType::Short;
        a.shortVal = 15;

        b.Type = GlobalType::Short;
        b.shortVal = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Long or float values don't care about shorts.
        a.Type = GlobalType::Long;
        b.Type = GlobalType::Long;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GlobalType::Float;
        b.Type = GlobalType::Float;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }

      SECTION("longVal mismatch")
      {
        a.Type = GlobalType::Long;
        a.longVal = 15;

        b.Type = GlobalType::Long;
        b.longVal = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Short or float values don't care about longs.
        a.Type = GlobalType::Short;
        b.Type = GlobalType::Short;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GlobalType::Float;
        b.Type = GlobalType::Float;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }

      SECTION("floatVal mismatch")
      {
        a.Type = GlobalType::Float;
        a.floatVal = 1.5f;

        b.Type = GlobalType::Float;
        b.floatVal = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Short or long values don't care about floats.
        a.Type = GlobalType::Short;
        b.Type = GlobalType::Short;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GlobalType::Long;
        b.Type = GlobalType::Long;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }
    }
  }

  SECTION("less")
  {
    GlobalRecord a;
    GlobalRecord b;

    SECTION("equal")
    {
      a.recordID = "World";
      b.recordID = "World";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "WORLD";
      b.recordID = "world";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "world";
      b.recordID = "WORLD";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less")
    {
      a.recordID = "Earth";
      b.recordID = "World";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "earth";
      b.recordID = "WORLD";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "EARTH";
      b.recordID = "world";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater")
    {
      a.recordID = "foo";
      b.recordID = "bar";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "foo";
      b.recordID = "BAR";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "FOO";
      b.recordID = "bar";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record with 16 bit short")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GlobalRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "NPCVoiceDistance" );
      REQUIRE( record.Type == GlobalType::Short );
      REQUIRE( record.shortVal == 750 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with 32 bit long")
    {
      const auto data = "GLOB\x24\0\0\0\0\0\0\0\0\0\0\0NAME\x07\0\0\0PCGold\0FNAM\x01\0\0\0lFLTV\x04\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GlobalRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "PCGold" );
      REQUIRE( record.Type == GlobalType::Long );
      REQUIRE( record.longVal == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with float")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0WerewolfClawMult\0FNAM\x01\0\0\0fFLTV\x04\0\0\0\0\0\xC8\x41"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GlobalRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "WerewolfClawMult" );
      REQUIRE( record.Type == GlobalType::Float );
      REQUIRE( record.floatVal == 25.0f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\x01\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x31\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FAIL\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is not one")
    {
      {
        const auto data = "GLOB\x2D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\0\0\0\0FLTV\x04\0\0\0\0\x80;D"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GLOB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GlobalRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "GLOB\x2F\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x02\0\0\0s\0FLTV\x04\0\0\0\0\x80;D"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GLOB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GlobalRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FNAM can be read")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: FNAM is not s or f or l")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0kFLTV\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FLTV")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFAIL\x04\0\0\0\0\x80;D"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FLTV is not four")
    {
      {
        const auto data = "GLOB\x2D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x03\0\0\0\0\x80;"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GLOB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GlobalRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "GLOB\x2F\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x05\0\0\0\0\x80;D\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GLOB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GlobalRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FLTV can be read")
    {
      const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GLOB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GlobalRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
