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
#include "../../../../lib/mw/records/ActivatorRecord.hpp"

TEST_CASE("MWTP::ActivatorRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ActivatorRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.ItemName.empty() );
    REQUIRE( record.ScriptName.empty() );
  }

  SECTION("equals")
  {
    ActivatorRecord a;
    ActivatorRecord b;

    SECTION("equal")
    {
      a.recordID = "SpeakingTree";
      a.ModelPath = "tree01.nif";

      b.recordID = "SpeakingTree";
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

      SECTION("ItemName mismatch")
      {
        a.ItemName = "Item";
        b.ItemName = "AnotherItem";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptName mismatch")
      {
        a.ScriptName = "ThisScript";
        b.ScriptName = "ThatScript";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record without script")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActivatorRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "active_akula_shield" );
      REQUIRE( record.ModelPath == "i\\active_akula_shield.NIF" );
      REQUIRE( record.ItemName == "Akula-Schild" );
      REQUIRE( record.ScriptName.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "ACTI\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0furn_banner_councilclub\0MODL\x1C\0\0\0f\\furn_banner_tavern_01.NIF\0FNAM\x0C\0\0\0Ratstaverne\0SCRI\x0E\0\0\0OutsideBanner\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ActivatorRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "furn_banner_councilclub" );
      REQUIRE( record.ModelPath == "f\\furn_banner_tavern_01.NIF" );
      REQUIRE( record.ItemName == "Ratstaverne" );
      REQUIRE( record.ScriptName == "OutsideBanner" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "ACTI\x53\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0FAIL\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\x01\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x5F\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0FAIL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\x01\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x5F\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FAIL\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\x01\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x1F\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "ACTI\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0furn_banner_councilclub\0MODL\x1C\0\0\0f\\furn_banner_tavern_01.NIF\0FNAM\x0C\0\0\0Ratstaverne\0FAIL\x0E\0\0\0OutsideBanner\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "ACTI\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0furn_banner_councilclub\0MODL\x1C\0\0\0f\\furn_banner_tavern_01.NIF\0FNAM\x0C\0\0\0Ratstaverne\0SCRI\x0E\x01\0\0OutsideBanner\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "ACTI\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0furn_banner_councilclub\0MODL\x1C\0\0\0f\\furn_banner_tavern_01.NIF\0FNAM\x0C\0\0\0Ratstaverne\0SCRI\x1E\0\0\0OutsideBanner\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ACTI, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ActivatorRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
