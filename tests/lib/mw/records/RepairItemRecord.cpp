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
#include "../../../../lib/mw/records/RepairItemRecord.hpp"

TEST_CASE("MWTP::RepairItemRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    RepairItemRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Uses == 0 );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptName.empty() );
  }

  SECTION("constructor with ID")
  {
    RepairItemRecord record("bla");

    REQUIRE( record.recordID == "bla" );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Uses == 0 );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptName.empty() );
  }

  SECTION("equals")
  {
    RepairItemRecord a;
    RepairItemRecord b;

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

      SECTION("Uses mismatch")
      {
        a.Uses = 15;
        b.Uses = 25;

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

      SECTION("InventoryIcon mismatch")
      {
        a.InventoryIcon = "icon01.dds";
        b.InventoryIcon = "other_icon.dds";

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

  SECTION("less")
  {
    RepairItemRecord a;
    RepairItemRecord b;

    SECTION("equal")
    {
      a.recordID = "Hammer";
      b.recordID = "Hammer";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "HAMMER";
      b.recordID = "hammer";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "hammer";
      b.recordID = "HAMMER";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less")
    {
      a.recordID = "Hammer";
      b.recordID = "Stop";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "hammer";
      b.recordID = "STOP";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "HAMMER";
      b.recordID = "stop";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater")
    {
      a.recordID = "Stop";
      b.recordID = "Hammer time";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "stop";
      b.recordID = "HAMMER TIME";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "STOP";
      b.recordID = "hammer time";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record without script")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RepairItemRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "repair_master_01" );
      REQUIRE( record.ModelPath == "m\\Repair_Master_01.nif" );
      REQUIRE( record.Name == "Schmiedehammer des Meisters" );
      REQUIRE( record.Weight == 2.0f );
      REQUIRE( record.Value == 50 );
      REQUIRE( record.Uses == 10 );
      REQUIRE( record.Quality == 1.3f );
      REQUIRE( record.InventoryIcon == "m\\Tx_repair_M_01.tga" );
      REQUIRE( record.ScriptName.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "REPA\x9D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RepairItemRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "repair_master_01" );
      REQUIRE( record.ModelPath == "m\\Repair_Master_01.nif" );
      REQUIRE( record.Name == "Schmiedehammer des Meisters" );
      REQUIRE( record.Weight == 2.0f );
      REQUIRE( record.Value == 50 );
      REQUIRE( record.Uses == 10 );
      REQUIRE( record.Quality == 1.3f );
      REQUIRE( record.InventoryIcon == "m\\Tx_repair_M_01.tga" );
      REQUIRE( record.ScriptName == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "REPA\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\x01\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x91\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0FAIL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\x01\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x91\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FAIL\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\x01\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x91\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no RIDT")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0FAIL\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RIDT is not 16")
    {
      {
        const auto data = "REPA\x90\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x0F\0\0\0\0\0\0@2\0\0\0\x0A\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REPA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RepairItemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "REPA\x92\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x11\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?\0ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REPA, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RepairItemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before RIDT can be read")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?FAIL\x15\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\x01\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "REPA\x91\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x2A\0\0\0m\\Tx_repair_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "REPA\x9D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "REPA\x9D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "REPA\x9D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0repair_master_01\0MODL\x17\0\0\0m\\Repair_Master_01.nif\0FNAM\x1C\0\0\0Schmiedehammer des Meisters\0RIDT\x10\0\0\0\0\0\0@2\0\0\0\x0A\0\0\0ff\xA6?ITEX\x15\0\0\0m\\Tx_repair_M_01.tga\0SCRI\x05\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REPA, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RepairItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
