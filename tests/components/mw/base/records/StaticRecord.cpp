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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/StaticRecord.hpp"

TEST_CASE("MWTP::StaticRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    StaticRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
  }

  SECTION("equals")
  {
    StaticRecord a;
    StaticRecord b;

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
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      StaticRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "in_redoran_hut_bfloor_02" );
      REQUIRE( record.ModelPath == "i\\In_redoran_hut_Bfloor_02.NIF" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "STAT\x48\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0FAIL\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\x02\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x48\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0FAIL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\x02\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x3F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read STAT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      StaticRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
