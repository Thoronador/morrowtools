/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
#include "../../../../../mw/base/records/LightRecord.hpp"

TEST_CASE("MWTP::LightRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    LightRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Time == 0 );
    REQUIRE( record.Radius == 0 );
    REQUIRE( record.Red == 0 );
    REQUIRE( record.Green == 0 );
    REQUIRE( record.Blue == 0 );
    REQUIRE( record.Zero == 0 );
    REQUIRE( record.LightFlags == 0 );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.SoundID.empty() );
  }

  SECTION("equals")
  {
    LightRecord a;
    LightRecord b;

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

      SECTION("InventoryIcon mismatch")
      {
        a.InventoryIcon = "icon01.dds";
        b.InventoryIcon = "other_icon.dds";

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

      SECTION("Time mismatch")
      {
        a.Time = 15;
        b.Time = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Radius mismatch")
      {
        a.Radius = 15;
        b.Radius = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Red mismatch")
      {
        a.Red = 15;
        b.Red = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Green mismatch")
      {
        a.Green = 15;
        b.Green = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Blue mismatch")
      {
        a.Blue = 15;
        b.Blue = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Zero mismatch")
      {
        a.Zero = 15;
        b.Zero = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("LightFlags mismatch")
      {
        a.LightFlags = 5;
        b.LightFlags = 10;

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

      SECTION("SoundID mismatch")
      {
        a.SoundID = "ThisSound";
        b.SoundID = "ThatSound";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without name, icon, script, sound")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      LightRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "light_de_lantern_13_300" );
      REQUIRE( record.ModelPath == "l\\Light_De_Lantern_13.NIF" );
      REQUIRE( record.Name.empty() );
      REQUIRE( record.InventoryIcon.empty() );
      REQUIRE( record.Weight == 0.0f );
      REQUIRE( record.Value == 0 );
      REQUIRE( record.Time == 0xFFFFFFFF );
      REQUIRE( record.Radius == 300 );
      REQUIRE( record.Red == 0xFF );
      REQUIRE( record.Green == 0x0F );
      REQUIRE( record.Blue == 0x05 );
      REQUIRE( record.Zero == 0 );
      REQUIRE( record.LightFlags == 0x00000001 );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.SoundID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with name, icon and sound, but without script")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      LightRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0x00000400 );
      // -- record data
      REQUIRE( record.recordID == "torch_infinite_time_unique" );
      REQUIRE( record.ModelPath == "l\\light_torch10.nif" );
      REQUIRE( record.Name == "Fackel" );
      REQUIRE( record.InventoryIcon == "l\\light_torch10.tga" );
      REQUIRE( record.Weight == 3.0f );
      REQUIRE( record.Value == 5 );
      REQUIRE( record.Time == 300 );
      REQUIRE( record.Radius == 400 );
      REQUIRE( record.Red == 0xEF );
      REQUIRE( record.Green == 0x9D );
      REQUIRE( record.Blue == 0x2E );
      REQUIRE( record.Zero == 0 );
      REQUIRE( record.LightFlags == 0x0000003B );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.SoundID == "Fire 40" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with script ID")
    {
      const auto data = "LIGH\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      LightRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "light_de_lantern_13_300" );
      REQUIRE( record.ModelPath == "l\\Light_De_Lantern_13.NIF" );
      REQUIRE( record.Name.empty() );
      REQUIRE( record.InventoryIcon.empty() );
      REQUIRE( record.Weight == 0.0f );
      REQUIRE( record.Value == 0 );
      REQUIRE( record.Time == 0xFFFFFFFF );
      REQUIRE( record.Radius == 300 );
      REQUIRE( record.Red == 0xFF );
      REQUIRE( record.Green == 0x0F );
      REQUIRE( record.Blue == 0x05 );
      REQUIRE( record.Zero == 0 );
      REQUIRE( record.LightFlags == 0x00000001 );
      REQUIRE( record.ScriptID == "foo" );
      REQUIRE( record.SoundID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "LIGH\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0FAIL\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\x01\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x6F\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0FAIL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\x01\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x6F\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FAIL\x07\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\x01\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x9F\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0FAIL\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x14\x01\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x9F\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no LHDT")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0FAIL\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of LHDT is not 24")
    {
      {
        const auto data = "LIGH\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x17\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read LIGH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        LightRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "LIGH\x63\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x19\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read LIGH, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        LightRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before LHDT can be read")
    {
      const auto data = "LIGH\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "LIGH\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "LIGH\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "LIGH\x6E\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0light_de_lantern_13_300\0MODL\x1A\0\0\0l\\Light_De_Lantern_13.NIF\0LHDT\x18\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF,\x01\0\0\xFF\x0F\x05\0\x01\0\0\0SCRI\x14\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SNAM")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0FAIL\x08\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM > 256")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x08\x01\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM is beyond stream")
    {
      const auto data = "LIGH\x9A\0\0\0\0\0\0\0\0\x04\0\0NAME\x1B\0\0\0torch_infinite_time_unique\0MODL\x14\0\0\0l\\light_torch10.nif\0FNAM\x07\0\0\0Fackel\0ITEX\x14\0\0\0l\\light_torch10.tga\0LHDT\x18\0\0\0\0\0@@\x05\0\0\0,\x01\0\0\x90\x01\0\0\xEF\x9D.\0;\0\0\0SNAM\x18\0\0\0Fire 40\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LIGH, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LightRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
