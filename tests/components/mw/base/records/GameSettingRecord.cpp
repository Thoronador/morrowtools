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
#include "../../../../../mw/base/records/GameSettingRecord.hpp"

TEST_CASE("MWTP::GameSettingRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    GameSettingRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Type == GMSTType::Integer );
    REQUIRE( record.iVal == 0 );
    REQUIRE( record.fVal == 0.0f );
    REQUIRE( record.sVal.empty() );
  }

  SECTION("equals")
  {
    GameSettingRecord a;
    GameSettingRecord b;

    SECTION("equal")
    {
      a.recordID = "VarOne";
      a.Type = GMSTType::Integer;
      a.iVal = 5;

      b.recordID = "VarOne";
      b.Type = GMSTType::Integer;
      b.iVal = 5;

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
        a.Type = GMSTType::Integer;
        b.Type = GMSTType::Float;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Type = GMSTType::Integer;
        b.Type = GMSTType::String;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Type = GMSTType::Float;
        b.Type = GMSTType::String;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("iVal mismatch")
      {
        a.Type = GMSTType::Integer;
        a.iVal = 15;

        b.Type = GMSTType::Integer;
        b.iVal = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Float or string values don't care about integers.
        a.Type = GMSTType::Float;
        b.Type = GMSTType::Float;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GMSTType::String;
        b.Type = GMSTType::String;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }

      SECTION("fVal mismatch")
      {
        a.Type = GMSTType::Float;
        a.fVal = 1.5f;

        b.Type = GMSTType::Float;
        b.fVal = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Integer or string values don't care about floats.
        a.Type = GMSTType::Integer;
        b.Type = GMSTType::Integer;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GMSTType::String;
        b.Type = GMSTType::String;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }

      SECTION("sVal mismatch")
      {
        a.Type = GMSTType::String;
        a.sVal = "foo";

        b.Type = GMSTType::String;
        b.sVal = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        // Integer or float values don't care about longs.
        a.Type = GMSTType::Integer;
        b.Type = GMSTType::Integer;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );

        a.Type = GMSTType::Float;
        b.Type = GMSTType::Float;

        REQUIRE( a.equals(b) );
        REQUIRE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record with integer value")
    {
      const auto data = "GMST\x22\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0iGreetDurationINTV\x04\0\0\0\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GameSettingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "iGreetDuration" );
      REQUIRE( record.Type == GMSTType::Integer );
      REQUIRE( record.iVal == 4 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with float value")
    {
      const auto data = "GMST\x21\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0fSneakUseDistFLTV\x04\0\0\0\0\0\xFA\x43"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GameSettingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "fSneakUseDist" );
      REQUIRE( record.Type == GMSTType::Float );
      REQUIRE( record.fVal == 500.0f );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with string value")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x08\0\0\0sAcrobatSTRV\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GameSettingRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "sAcrobat" );
      REQUIRE( record.Type == GMSTType::String );
      REQUIRE( record.sVal == "Acrobat" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "GMST\x1F\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0FAIL\x08\0\0\0sAcrobatSTRV\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x08\x02\0\0sAcrobatSTRV\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x28\0\0\0sAcrobatSTRV\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is zero")
    {
      const auto data = "GMST\x17\0\0\0\0\0\0\0\0\0\0\0NAME\0\0\0\0STRV\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: first character of NAME is not s or f or i")
    {
      const auto data = "GMST\x21\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0kSneakUseDistFLTV\x04\0\0\0\0\0\xFA\x43"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no INTV")
    {
      const auto data = "GMST\x22\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0iGreetDurationFAIL\x04\0\0\0\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INTV is not four")
    {
      {
        const auto data = "GMST\x21\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0iGreetDurationINTV\x03\0\0\0\x04\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GMST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GameSettingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "GMST\x23\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0iGreetDurationINTV\x05\0\0\0\x04\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GMST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GameSettingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INTV can be read")
    {
      const auto data = "GMST\x22\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0iGreetDurationINTV\x04\0\0\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FLTV")
    {
      const auto data = "GMST\x21\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0fSneakUseDistFAIL\x04\0\0\0\0\0\xFA\x43"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FLTV is not four")
    {
      {
        const auto data = "GMST\x20\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0fSneakUseDistFLTV\x03\0\0\0\0\0\xFA"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GMST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GameSettingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "GMST\x22\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0fSneakUseDistFLTV\x05\0\0\0\0\0\xFA\x43\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read GMST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        GameSettingRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FLTV can be read")
    {
      const auto data = "GMST\x21\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0fSneakUseDistFLTV\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no STRV")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x08\0\0\0sAcrobatFAIL\x07\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of STRV > 512")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x08\0\0\0sAcrobatSTRV\x07\x02\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of STRV is beyond stream")
    {
      const auto data = "GMST\x1F\0\0\0\0\0\0\0\0\0\0\0NAME\x08\0\0\0sAcrobatSTRV\x17\0\0\0Acrobat"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read GMST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GameSettingRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
