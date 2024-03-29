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
#include "../../../../lib/mw/records/ScriptRecord.hpp"

TEST_CASE("MWTP::ScriptRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ScriptRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.NumShorts == 0 );
    REQUIRE( record.NumLongs == 0 );
    REQUIRE( record.NumFloats == 0 );
    REQUIRE( record.ScriptData.size() == 0 );
    REQUIRE( record.LocalVars.empty() );
    REQUIRE( record.ScriptData.data() == nullptr );
    REQUIRE( record.ScriptText.empty() );
  }

  SECTION("getLocalVarSize")
  {
    ScriptRecord record;

    SECTION("empty record")
    {
      REQUIRE( record.getLocalVarSize() == 0 );
    }

    SECTION("one variable name only")
    {
      record.LocalVars = { "doOnce" };
      REQUIRE( record.getLocalVarSize() == 7 );
    }

    SECTION("multiple variable names")
    {
      record.LocalVars = { "doOnce", "an_otter", "fishingFish" };
      REQUIRE( record.getLocalVarSize() == 28 );
    }
  }

  SECTION("equals")
  {
    ScriptRecord a;
    ScriptRecord b;

    SECTION("equal")
    {
      a.recordID = "ScriptOne";
      b.recordID = "ScriptOne";

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

      SECTION("NumShorts mismatch")
      {
        a.NumShorts = 15;
        b.NumShorts = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NumLongs mismatch")
      {
        a.NumLongs = 15;
        b.NumLongs = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NumFloats mismatch")
      {
        a.NumFloats = 15;
        b.NumFloats = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("LocalVars content mismatch")
      {
        a.LocalVars = { "foo" };
        b.LocalVars = { "bar" };

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("LocalVars length mismatch")
      {
        a.LocalVars.push_back("once");
        b.LocalVars.clear();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptData mismatch")
      {
        SECTION("size mismatch")
        {
          a.ScriptData.reset();
          b.ScriptData.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

          REQUIRE_FALSE( a.equals(b) );
          REQUIRE_FALSE( b.equals(a) );
        }

        SECTION("content mismatch")
        {
          a.ScriptData.copy_from(reinterpret_cast<const uint8_t*>("bar"), 4);
          b.ScriptData.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

          REQUIRE_FALSE( a.equals(b) );
          REQUIRE_FALSE( b.equals(a) );
        }
      }

      SECTION("ScriptText mismatch")
      {
        a.ScriptText = "bla bla";
        b.ScriptText = "bla again";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record without local vars")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ScriptRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "ZainabKill" );
      REQUIRE( record.NumShorts == 0 );
      REQUIRE( record.NumLongs == 0 );
      REQUIRE( record.NumFloats == 0 );
      REQUIRE( record.ScriptData.size() == 37 );
      REQUIRE( record.getLocalVarSize() == 0 );
      REQUIRE( record.LocalVars.empty() );
      REQUIRE( record.ScriptData.data() != nullptr );
      const auto scdt = std::string_view(reinterpret_cast<const char*>(record.ScriptData.data()), record.ScriptData.size());
      REQUIRE( scdt == "\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01"sv );
      REQUIRE( record.ScriptText == "begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a single local variable")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0zabamundNoLore\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0(\0\0\0\x07\0\0\0SCVR\x07\0\0\0NoLore\0SCDT(\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x10\x42\x31_UrshilakuKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXf\0\0\0begin zabamundNoLore\x0D\x0A\x0D\x0AShort NoLore\x0D\x0A\x0D\x0Aif ( OnDeath == 1 )\x0D\x0A\x09Journal B1_UrshilakuKill 1\x0D\x0A\x65ndif\x0D\x0A\x0D\x0A\x65nd"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ScriptRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "zabamundNoLore" );
      REQUIRE( record.NumShorts == 1 );
      REQUIRE( record.NumLongs == 0 );
      REQUIRE( record.NumFloats == 0 );
      REQUIRE( record.ScriptData.size() == 40 );
      REQUIRE( record.getLocalVarSize() == 7 );
      REQUIRE( record.LocalVars.size() == 1 );
      REQUIRE( record.LocalVars[0] == "NoLore" );
      REQUIRE( record.ScriptData.data() != nullptr );
      const auto scdt = std::string_view(reinterpret_cast<const char*>(record.ScriptData.data()), record.ScriptData.size());
      REQUIRE( scdt == "\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x10\x42\x31_UrshilakuKill\x01\0\xFF\xFF\x09\x01\x01\x01"sv );
      REQUIRE( record.ScriptText == "begin zabamundNoLore\r\n\r\nShort NoLore\r\n\r\nif ( OnDeath == 1 )\r\n\tJournal B1_UrshilakuKill 1\r\nendif\r\n\r\nend"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCHD")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0FAIL4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCHD is not 52")
    {
      {
        const auto data = "SCPT\xC1\0\0\0\0\0\0\0\0\0\0\0SCHD3\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip SCPT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScriptRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "SCPT\xC3\0\0\0\0\0\0\0\0\0\0\0SCHD5\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip SCPT, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ScriptRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before SCHD can be read, script name")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0Zai"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before SCHD can be read, int data")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCVR")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0zabamundNoLore\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0(\0\0\0\x07\0\0\0FAIL\x07\0\0\0NoLore\0SCDT(\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x10\x42\x31_UrshilakuKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXf\0\0\0begin zabamundNoLore\x0D\x0A\x0D\x0AShort NoLore\x0D\x0A\x0D\x0Aif ( OnDeath == 1 )\x0D\x0A\x09Journal B1_UrshilakuKill 1\x0D\x0A\x65ndif\x0D\x0A\x0D\x0A\x65nd"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR does not match value in header")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0zabamundNoLore\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0(\0\0\0\x12\0\0\0SCVR\x07\0\0\0NoLore\0SCDT(\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x10\x42\x31_UrshilakuKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXf\0\0\0begin zabamundNoLore\x0D\x0A\x0D\x0AShort NoLore\x0D\x0A\x0D\x0Aif ( OnDeath == 1 )\x0D\x0A\x09Journal B1_UrshilakuKill 1\x0D\x0A\x65ndif\x0D\x0A\x0D\x0A\x65nd"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR > 64 KB")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0zabamund_64KB_exceed\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0(\0\0\0\x07\0\x01\0SCVR\x07\0\x01\0NoLore\0SCDT(\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x10\x42\x31_UrshilakuKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXf\0\0\0begin zabamundNoLore\x0D\x0A\x0D\x0AShort NoLore\x0D\x0A\x0D\x0Aif ( OnDeath == 1 )\x0D\x0A\x09Journal B1_UrshilakuKill 1\x0D\x0A\x65ndif\x0D\x0A\x0D\x0A\x65nd"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR is beyond stream")
    {
      const auto data = "SCPT\xE9\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0zabamundNoLore\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0(\0\0\0\x07\0\0\0SCVR\x07\0\0\0No"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCDT")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0FAIL%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCDTs")
    {
      const auto data = "SCPT\xEF\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCDT does not match value in header - less")
    {
      const auto data = "SCPT\xC1\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT\x24\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCDT does not match value in header - more")
    {
      const auto data = "SCPT\xC3\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT\x26\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before SCDT can be read")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCTX")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01NONEQ\0\0\0begin ZainabKill\r\n\t\t\r\nif ( OnDeath == 1 )\r\n\tJournal B3_ZainabKill 1\r\nendif\r\n\r\nend"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before SCTX can be read completely")
    {
      const auto data = "SCPT\xC2\0\0\0\0\0\0\0\0\0\0\0SCHD4\0\0\0ZainabKill\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0%\0\0\0\0\0\0\0SCDT%\0\0\0\x06\x01\x01\x09 X\xF0\x10 == 1\xCC\x10\x0D\x42\x33_ZainabKill\x01\0\xFF\xFF\x09\x01\x01\x01SCTXQ\0\0\0begin ZainabKill\r\n"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip SCPT, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ScriptRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
