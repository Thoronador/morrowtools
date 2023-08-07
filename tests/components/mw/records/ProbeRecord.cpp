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
#include "../../../../lib/mw/records/ProbeRecord.hpp"

TEST_CASE("MWTP::ProbeRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ProbeRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.Uses == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptName.empty() );
  }

  SECTION("constructor with ID")
  {
    ProbeRecord record("bla");

    REQUIRE( record.recordID == "bla" );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.Uses == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptName.empty() );
  }

  SECTION("equals")
  {
    ProbeRecord a;
    ProbeRecord b;

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

      SECTION("Quality mismatch")
      {
        a.Quality = 0.5f;
        b.Quality = 1.0f;

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
    ProbeRecord a;
    ProbeRecord b;

    SECTION("equal")
    {
      a.recordID = "Probe";
      b.recordID = "Probe";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "PROBE";
      b.recordID = "probe";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "probe";
      b.recordID = "PROBE";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less")
    {
      a.recordID = "Bar";
      b.recordID = "Foo";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "bar";
      b.recordID = "FOO";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "BAR";
      b.recordID = "foo";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater")
    {
      a.recordID = "Foo";
      b.recordID = "Bar";

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

    SECTION("default: load record without script")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ProbeRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "probe_master" );
      REQUIRE( record.ModelPath == "m\\Probe_Master_01.nif" );
      REQUIRE( record.Name == "Sonde des Meisters" );
      REQUIRE( record.Weight == 0.25f );
      REQUIRE( record.Value == 100 );
      REQUIRE( record.Quality == 1.0f );
      REQUIRE( record.Uses == 25 );
      REQUIRE( record.InventoryIcon == "m\\Tx_probe_M_01.tga" );
      REQUIRE( record.ScriptName.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "PROB\x8E\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ProbeRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "probe_master" );
      REQUIRE( record.ModelPath == "m\\Probe_Master_01.nif" );
      REQUIRE( record.Name == "Sonde des Meisters" );
      REQUIRE( record.Weight == 0.25f );
      REQUIRE( record.Value == 100 );
      REQUIRE( record.Quality == 1.0f );
      REQUIRE( record.Uses == 25 );
      REQUIRE( record.InventoryIcon == "m\\Tx_probe_M_01.tga" );
      REQUIRE( record.ScriptName == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "PROB\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0FAIL\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\x01\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x99\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0FAIL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\x01\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x82\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FAIL\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\x01\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x99\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no PBDT")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0FAIL\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PBDT is not 16")
    {
      {
        const auto data = "PROB\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x0F\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read PROB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ProbeRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "PROB\x83\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x11\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read PROB, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ProbeRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before PBDT can be read")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0FAIL\x14\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\x01\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "PROB\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x24\0\0\0m\\Tx_probe_M_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "PROB\x8E\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "PROB\x8E\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "PROB\x8E\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0probe_master\0MODL\x16\0\0\0m\\Probe_Master_01.nif\0FNAM\x13\0\0\0Sonde des Meisters\0PBDT\x10\0\0\0\0\0\x80>d\0\0\0\0\0\x80?\x19\0\0\0ITEX\x14\0\0\0m\\Tx_probe_M_01.tga\0SCRI\x05\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read PROB, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ProbeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
