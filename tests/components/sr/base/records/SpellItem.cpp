/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include "../../../../../sr/base/records/SpellItem.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("SpellItem")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SpellItem record;

    REQUIRE( record.castingCost == 0 );
    REQUIRE( record.flags == 0 );
    REQUIRE( record.type == 0 );
    REQUIRE( record.chargeTime == 0.0f );
    REQUIRE( record.castingType == 0 );
    REQUIRE( record.delivery == 0 );
    REQUIRE( record.castDuration == 0 );
    REQUIRE( record.range == 0 );
    REQUIRE( record.castingPerkFormID == 0 );
  }

  SECTION("equals")
  {
    SpellItem a;
    SpellItem b;

    SECTION("equal")
    {
      a.castingCost = 3;
      b.castingCost = 3;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("castingCost mismatch")
      {
        a.castingCost = 1;
        b.castingCost = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("flags mismatch")
      {
        a.flags = 1;
        b.flags = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("type mismatch")
      {
        a.type = 1;
        b.type = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("chargeTime mismatch")
      {
        a.chargeTime = 1.0f;
        b.chargeTime = 1.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("castingType mismatch")
      {
        a.castingType = 1;
        b.castingType = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("delivery mismatch")
      {
        a.delivery = 1;
        b.delivery = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("castDuration mismatch")
      {
        a.castDuration = 1.0f;
        b.castDuration = 1.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("range mismatch")
      {
        a.range = 1.0f;
        b.range = 1.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("castingPerkFormID mismatch")
      {
        a.castingPerkFormID = 0x01234567;
        b.castingPerkFormID = 0x0FEDCBA9;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    uint32_t bytesRead = 0;

    SECTION("default: load + save item # 1")
    {
      const std::string_view data = "SPIT\x24\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPIT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.

      SpellItem record;
      REQUIRE( record.loadFromStream(stream, cSPEL, bytesRead) );
      // Check data.
      REQUIRE( record.castingCost == 14 );
      REQUIRE( record.flags == 0 );
      REQUIRE( record.type == 0 );
      REQUIRE( record.chargeTime == 0.0f );
      REQUIRE( record.castingType == 2 );
      REQUIRE( record.delivery == 2 );
      REQUIRE( record.castDuration == 0.0f );
      REQUIRE( record.range == 0.0f );
      REQUIRE( record.castingPerkFormID == 0x000F2CA8 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load + save # 2")
    {
      const std::string_view data = "SPIT\x24\0\x64\0\0\0\x01\0\0\0\0\0\0\0\0\0\xA0\x40\x01\0\0\0\x04\0\0\0\0\0\0\0\0\0\x48\x42\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPIT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SpellItem record;
      REQUIRE( record.loadFromStream(stream, cSPEL, bytesRead) );
      // Check data.
      REQUIRE( record.castingCost == 100 );
      REQUIRE( record.flags == 1 );
      REQUIRE( record.type == 0 );
      REQUIRE( record.chargeTime == 5.0f );
      REQUIRE( record.castingType == 1 );
      REQUIRE( record.delivery == 4 );
      REQUIRE( record.castDuration == 0.0f );
      REQUIRE( record.range == 50.0f );
      REQUIRE( record.castingPerkFormID == 0x0000000 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of SPIT is not 36")
    {
      {
        const std::string_view data = "SPIT\x23\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPIT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellItem record;
        REQUIRE_FALSE( record.loadFromStream(stream, cSPEL, bytesRead) );
      }

      {
        const std::string_view data = "SPIT\x25\0\x0E\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\0\0\0\0\0\0\0\0\xA8\x2C\x0F\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPIT, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellItem record;
        REQUIRE_FALSE( record.loadFromStream(stream, cSPEL, bytesRead) );
      }
    }

    SECTION("corrupt data: stream ends before all of SPIT can be read")
    {
      const std::string_view data = "SPIT\x24\0\x0E\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPIT, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellItem record;
      REQUIRE_FALSE( record.loadFromStream(stream, cSPEL, bytesRead) );
    }
  }

  SECTION("flags query methods")
  {
    SpellItem record;

    SECTION("doesAutoCalc")
    {
      REQUIRE( record.doesAutoCalc() );
      record.flags = SpellItem::cFlagNoAutoCalc;
      REQUIRE_FALSE( record.doesAutoCalc() );
    }

    SECTION("isPCStartSpell")
    {
      REQUIRE_FALSE( record.isPCStartSpell() );
      record.flags = SpellItem::cFlagPCStartSpell;
      REQUIRE( record.isPCStartSpell() );
    }

    SECTION("areaEffectIgnoresLOS")
    {
      REQUIRE_FALSE( record.areaEffectIgnoresLOS() );
      record.flags = SpellItem::cFlagAreaEffectIgnoresLOS;
      REQUIRE( record.areaEffectIgnoresLOS() );
    }

    SECTION("ignoresResistance")
    {
      REQUIRE_FALSE( record.ignoresResistance() );
      record.flags = SpellItem::cFlagIgnoreResistance;
      REQUIRE( record.ignoresResistance() );
    }

    SECTION("disallowsAbsorbAndReflect")
    {
      REQUIRE_FALSE( record.disallowsAbsorbAndReflect() );
      record.flags = SpellItem::cFlagDisallowAbsorbReflect;
      REQUIRE( record.disallowsAbsorbAndReflect() );
    }

    SECTION("noDualCastModifications")
    {
      REQUIRE_FALSE( record.noDualCastModifications() );
      record.flags = SpellItem::cFlagNoDualCastModifications;
      REQUIRE( record.noDualCastModifications() );
    }
  }
}
