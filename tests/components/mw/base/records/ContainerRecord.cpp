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
#include "../../../../../mw/base/records/ContainerRecord.hpp"

TEST_CASE("MWTP::ContainerRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ContainerRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Flags == 8 );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.Items.empty() );
  }

  SECTION("equals")
  {
    ContainerRecord a;
    ContainerRecord b;

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
        a.Name = "Alice's container";
        b.Name = "Bob's container";

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

      SECTION("Flags mismatch")
      {
        a.Flags = 15;
        b.Flags = 25;

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

      SECTION("Items mismatch")
      {
        a.Items.push_back(ItemRecord());
        b.Items.clear();

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
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "flora_gold_kanet_02_uni" );
      REQUIRE( record.ModelPath == "o\\Flora_gold_kanet_02.NIF" );
      REQUIRE( record.Name == "Goldkanet" );
      REQUIRE( record.Weight == 0.0f );
      REQUIRE( record.Flags == 0x0000000B );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Items.size() == 1 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "ingred_gold_kanet_unique" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load container without contents")
    {
      const auto data = "CONT\x65\0\0\0\0\0\0\0\0\x04\0\0NAME\x14\0\0\0com_chest_02_jzhirr\0MODL\x1B\0\0\0o\\Contain_Com_Chest_02.NIF\0FNAM\x06\0\0\0Kiste\0CNDT\x04\0\0\0\0\0HCFLAG\x04\0\0\0\x08\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0x00000400 );
      // -- record data
      REQUIRE( record.recordID == "com_chest_02_jzhirr" );
      REQUIRE( record.ModelPath == "o\\Contain_Com_Chest_02.NIF" );
      REQUIRE( record.Name == "Kiste" );
      REQUIRE( record.Weight == 200.0f );
      REQUIRE( record.Flags == 0x00000008 );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Items.size() == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "CONT\x07\x01\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0SCRI\x0C\0\0\0Sound_Flies\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ContainerRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "contain_corpse_Arenim00" );
      REQUIRE( record.ModelPath == "o\\Contain_corpse10.NIF" );
      REQUIRE( record.Name == "Agustas Skelett" );
      REQUIRE( record.Weight == 120.0f );
      REQUIRE( record.Flags == 0x00000008 );
      REQUIRE( record.ScriptID == "Sound_Flies" );
      REQUIRE( record.Items.size() == 3 );
      REQUIRE( record.Items[0].Count == 1 );
      REQUIRE( record.Items[0].Item == "amulet_Agustas_unique" );
      REQUIRE( record.Items[1].Count == 1 );
      REQUIRE( record.Items[1].Item == "ingred_bonemeal_01" );
      REQUIRE( record.Items[2].Count == 5 );
      REQUIRE( record.Items[2].Item == "Gold_001" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "CONT\x98\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0FAIL\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\x01\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\xA8\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0FAIL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\x01\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x9A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FAIL\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\x01\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x9A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no CNDT")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0FAIL\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNDT is not four")
    {
      {
        const auto data = "CONT\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x03\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CONT\x99\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x05\0\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before CNDT can be read")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FLAG")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FAIL\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FLAG is not four")
    {
      {
        const auto data = "CONT\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x03\0\0\0\x0B\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CONT\x99\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x05\0\0\0\x0B\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before FLAG can be read")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NPCO")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0FAIL$\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPCO is not 36")
    {
      {
        const auto data = "CONT\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO\x23\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CONT\x99\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO\x25\0\0\0\x01\0\0\0ingred_gold_kanet_unique\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read CONT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ContainerRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NPCO can be read completely")
    {
      const auto data = "CONT\x98\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0flora_gold_kanet_02_uni\0MODL\x1A\0\0\0o\\Flora_gold_kanet_02.NIF\0FNAM\x0A\0\0\0Goldkanet\0CNDT\x04\0\0\0\0\0\0\0FLAG\x04\0\0\0\x0B\0\0\0NPCO$\0\0\0\x01\0\0\0ing"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "CONT\x07\x01\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0FAIL\x0C\0\0\0Sound_Flies\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRIs")
    {
      const auto data = "CONT\x1B\x01\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0SCRI\x0C\0\0\0Sound_Flies\0SCRI\x0C\0\0\0Sound_Flies\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "CONT\x07\x01\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0SCRI\x0C\x01\0\0Sound_Flies\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "CONT\x07\x01\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0SCRI\xFC\0\0\0Sound_Flies\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: SCRI is empty")
    {
      const auto data = "CONT\xFC\0\0\0\0\0\0\0\0\0\0\0NAME\x18\0\0\0contain_corpse_Arenim00\0MODL\x17\0\0\0o\\Contain_corpse10.NIF\0FNAM\x10\0\0\0Agustas Skelett\0CNDT\x04\0\0\0\0\0\xF0\x42\x46LAG\x04\0\0\0\x08\0\0\0SCRI\x0C\0\0\0\0NPCO$\0\0\0\x01\0\0\0amulet_Agustas_unique\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x01\0\0\0ingred_bonemeal_01\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCO$\0\0\0\x05\0\0\0Gold_001\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CONT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ContainerRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("fun with flags")
  {
    ContainerRecord record;

    SECTION("isOrganic")
    {
      REQUIRE_FALSE( record.isOrganic() );

      record.Flags = 0x00000001;
      REQUIRE( record.isOrganic() );

      record.Flags = 0x00000002;
      REQUIRE_FALSE( record.isOrganic() );

      record.Flags = 0x0000FFFF;
      REQUIRE( record.isOrganic() );
    }

    SECTION("doesRespawn")
    {
      REQUIRE_FALSE( record.doesRespawn() );

      record.Flags = 0x00000001;
      REQUIRE_FALSE( record.doesRespawn() );

      record.Flags = 0x00000002;
      REQUIRE( record.doesRespawn() );

      record.Flags = 0x0000FFFF;
      REQUIRE( record.doesRespawn() );
    }
  }
}
