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
#include "../../../../../mw/base/records/MiscItemRecord.hpp"

TEST_CASE("MWTP::MiscItemRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    MiscItemRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.OtherStuff == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    MiscItemRecord a;
    MiscItemRecord b;

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

      SECTION("OtherStuff mismatch")
      {
        a.OtherStuff = 15;
        b.OtherStuff = 25;

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
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MiscItemRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "misc_skooma_vial" );
      REQUIRE( record.ModelPath == "n\\Potion_Skooma_01.nif" );
      REQUIRE( record.Name == "Leere Phiole" );
      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 1 );
      REQUIRE( record.OtherStuff == 0 );
      REQUIRE( record.InventoryIcon == "n\\Tx_skooma_01.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "MISC\x88\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MiscItemRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "misc_skooma_vial" );
      REQUIRE( record.ModelPath == "n\\Potion_Skooma_01.nif" );
      REQUIRE( record.Name == "Leere Phiole" );
      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 1 );
      REQUIRE( record.OtherStuff == 0 );
      REQUIRE( record.InventoryIcon == "n\\Tx_skooma_01.tga" );
      REQUIRE( record.ScriptID == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "MISC\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0FAIL\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\x01\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x91\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0FAIL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\x01\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x7C\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FAIL\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\x01\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x9F\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MCDT")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0FAIL\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MCDT is not 12")
    {
      {
        const auto data = "MISC\x7B\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0B\0\0\0\0\0\x80?\x01\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscItemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "MISC\x7D\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0D\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MISC, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MiscItemRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before MCDT can be read")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0FAIL\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\x01\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "MISC\x7C\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x1F\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEX subrecords")
    {
      const auto data = "MISC\x97\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "MISC\x88\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "MISC\x88\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "MISC\x88\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0SCRI\x0F\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MISC, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRI subrecords")
    {
      const auto data = "MISC\x94\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0misc_skooma_vial\0MODL\x17\0\0\0n\\Potion_Skooma_01.nif\0FNAM\x0D\0\0\0Leere Phiole\0MCDT\x0C\0\0\0\0\0\x80?\x01\0\0\0\0\0\0\0ITEX\x13\0\0\0n\\Tx_skooma_01.tga\0SCRI\x04\0\0\0foo\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MiscItemRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
