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
#include "../../../../../mw/base/records/LockpickRecord.hpp"

TEST_CASE("MWTP::LockpickRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    LockpickRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Quality == 0.0f );
    REQUIRE( record.Uses == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
  }

  SECTION("equals")
  {
    LockpickRecord a;
    LockpickRecord b;

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

      SECTION("Name mismatch")
      {
        a.Name = "Alice's item";
        b.Name = "Bob's item";

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
        a.InventoryIcon = "icon01.tga";
        b.InventoryIcon = "other_icon.tga";

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
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      LockpickRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "skeleton_key" );
      REQUIRE( record.ModelPath == "m\\Skeleton_key.NIF" );
      REQUIRE( record.Name == "Der Schlosswandler" );
      REQUIRE( record.Weight == 0.5f );
      REQUIRE( record.Value == 1000 );
      REQUIRE( record.Quality == 5.0f );
      REQUIRE( record.Uses == 50 );
      REQUIRE( record.InventoryIcon == "m\\tx_skeleton_key.tga" );
      REQUIRE( record.ScriptID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with a script")
    {
      const auto data = "LOCK\x8D\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0SCRI\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      LockpickRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "skeleton_key" );
      REQUIRE( record.ModelPath == "m\\Skeleton_key.NIF" );
      REQUIRE( record.Name == "Der Schlosswandler" );
      REQUIRE( record.Weight == 0.5f );
      REQUIRE( record.Value == 1000 );
      REQUIRE( record.Quality == 5.0f );
      REQUIRE( record.Uses == 50 );
      REQUIRE( record.InventoryIcon == "m\\tx_skeleton_key.tga" );
      REQUIRE( record.ScriptID == "foo" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "LOCK\x81\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0FAIL\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\x01\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x8F\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0FAIL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\x01\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x9F\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FAIL\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\x01\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x83\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no LKDT")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0FAIL\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of LKDT is not 16")
    {
      {
        const auto data = "LOCK\x80\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x0F\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read LOCK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        LockpickRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "LOCK\x82\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x11\0\0\0\0\0\0?\xE8\x03\0\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read LOCK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        LockpickRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before LKDT can be read")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0FAIL\x16\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\x01\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "LOCK\x81\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x1F\0\0\0m\\tx_skeleton_key.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "LOCK\x8D\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0FAIL\x04\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "LOCK\x8D\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0SCRI\x04\x01\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "LOCK\x8D\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0skeleton_key\0MODL\x13\0\0\0m\\Skeleton_key.NIF\0FNAM\x13\0\0\0Der Schlosswandler\0LKDT\x10\0\0\0\0\0\0?\xE8\x03\0\0\0\0\xA0@2\0\0\0ITEX\x16\0\0\0m\\tx_skeleton_key.tga\0SCRI\x0F\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read LOCK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      LockpickRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
