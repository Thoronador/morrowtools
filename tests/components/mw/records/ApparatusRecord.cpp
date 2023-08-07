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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/ApparatusRecord.hpp"

TEST_CASE("MWTP::ApparatusRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ApparatusRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.type == ApparatusRecord::Type::MortarAndPestle );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    ApparatusRecord a;
    ApparatusRecord b;

    SECTION("equal")
    {
      a.recordID = "ItemOne";
      a.ModelPath = "item01.nif";

      b.recordID = "ItemOne";
      b.ModelPath = "item01.nif";

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
        a.Name = "Alice's item";
        b.Name = "Bob's item";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("type mismatch")
      {
        a.type = ApparatusRecord::Type::Alembic;
        b.type = ApparatusRecord::Type::Calcinator;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Quality mismatch")
      {
        a.Quality = 0.5f;
        b.Quality = 1.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Weight mismatch")
      {
        a.Weight = 1.5f;
        b.Weight = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Value mismatch")
      {
        a.Value = 15;
        b.Value = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("InventoryIcon mismatch")
      {
        a.InventoryIcon = "icon01.dds";
        b.InventoryIcon = "other_icon.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptID mismatch")
      {
        a.ScriptID = "ThisScript";
        b.ScriptID = "ThatScript";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without script")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ApparatusRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "apparatus_sm_retort_01" );
      REQUIRE( record.ModelPath == "m\\Apparatus_S_Retort_01.nif" );
      REQUIRE( record.Name == "Retorte des Erzmeisters " );
      REQUIRE( record.type == ApparatusRecord::Type::Retort );
      REQUIRE( record.Quality == 2.0f );
      REQUIRE( record.Weight == 2.0f );
      REQUIRE( record.Value == 1000 );
      REQUIRE( record.InventoryIcon == "m\\Tx_retort_05.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "APPA\xAA\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ApparatusRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "apparatus_m_calcinator_01" );
      REQUIRE( record.ModelPath == "m\\App_M_Calcinator_01.nif" );
      REQUIRE( record.Name == "Kalzinierofen des Meisters" );
      REQUIRE( record.type == ApparatusRecord::Type::Calcinator );
      REQUIRE( record.Quality == 1.2f );
      REQUIRE( record.Weight == 13.0f );
      REQUIRE( record.Value == 240 );
      REQUIRE( record.InventoryIcon == "m\\Tx_calcinator_03.tga" );
      REQUIRE( record.ScriptID == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "APPA\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0FAIL\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\x01\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x9F\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0FAIL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple MODL")
    {
      const auto data = "APPA\xBB\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\x01\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x9C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty MODL")
    {
      const auto data = "APPA\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x01\0\0\0\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FAIL\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAM")
    {
      const auto data = "APPA\xB8\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\x01\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x9F\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty FNAM")
    {
      const auto data = "APPA\x7F\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x01\0\0\0\0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no AADT")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0FAIL\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: missing AADT")
    {
      const auto data = "APPA\x7F\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple AADT")
    {
      const auto data = "APPA\xAF\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AADT is not 16")
    {
      {
        const auto data = "APPA\x96\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x0F\0\0\0\x03\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "APPA\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x11\0\0\0\x03\0\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read APPA, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ApparatusRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before AADT can be read")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0FAIL\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEX")
    {
      const auto data = "APPA\xB2\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0ITEX\x13\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x13\x01\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "APPA\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x1F\0\0\0m\\Tx_retort_05.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "APPA\x85\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0apparatus_sm_retort_01\0MODL\x1C\0\0\0m\\Apparatus_S_Retort_01.nif\0FNAM\x19\0\0\0Retorte des Erzmeisters \0AADT\x10\0\0\0\x03\0\0\0\0\0\0@\0\0\0@\xE8\x03\0\0ITEX\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "APPA\xAA\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRI")
    {
      const auto data = "APPA\xB6\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0SCRI\x04\0\0\0foo\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "APPA\xAA\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "APPA\xAA\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0SCRI\x14\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "APPA\xA7\0\0\0\0\0\0\0\0\0\0\0NAME\x1A\0\0\0apparatus_m_calcinator_01\0MODL\x1A\0\0\0m\\App_M_Calcinator_01.nif\0FNAM\x1B\0\0\0Kalzinierofen des Meisters\0AADT\x10\0\0\0\x02\0\0\0\x9A\x99\x99?\0\0PA\xF0\0\0\0ITEX\x17\0\0\0m\\Tx_calcinator_03.tga\0SCRI\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ApparatusRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
