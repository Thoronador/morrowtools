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
#include "../../../../mw/base/Enchantment.hpp"
#include <sstream>
#include <string_view>

TEST_CASE("MWTP::EnchantmentData")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    EnchantmentData data;

    REQUIRE( data.EffectID == 0 );
    REQUIRE( data.SkillID == 0 );
    REQUIRE( data.AttributeID == 0 );
    REQUIRE( data.Range == RangeType::Self );
    REQUIRE( data.Area == 0 );
    REQUIRE( data.Duration == 0 );
    REQUIRE( data.MagnitudeMin == 0 );
    REQUIRE( data.MagnitudeMax == 0 );
  }

  SECTION("equality operator")
  {
    EnchantmentData a;
    EnchantmentData b;

    SECTION("equal")
    {
      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("EffectID mismatch")
      {
        a.EffectID = 1;
        b.EffectID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SkillID mismatch")
      {
        a.SkillID = 12;
        b.SkillID = 34;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("AttributeID mismatch")
      {
        a.AttributeID = 1;
        b.AttributeID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Range mismatch")
      {
        a.Range = RangeType::Self;
        b.Range = RangeType::Touch;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Area mismatch")
      {
        a.Area = 1;
        b.Area = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Duration mismatch")
      {
        a.Duration = 1;
        b.Duration = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("MagnitudeMin mismatch")
      {
        a.MagnitudeMin = 1;
        b.MagnitudeMin = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("MagnitudeMax mismatch")
      {
        a.MagnitudeMax = 1;
        b.MagnitudeMax = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("default: load data")
    {
      const auto data = "ENAM\x18\0\0\0\x53\0\x19\xFF\x01\0\0\0\0\0\0\0\xA0\x05\0\0\x05\0\0\0\x05\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ENAM, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      EnchantmentData enchantment;
      REQUIRE( enchantment.loadFromStream(stream) );
      // Check data.
      REQUIRE( enchantment.EffectID == 0x0053 );
      REQUIRE( enchantment.SkillID == 0x19 );
      REQUIRE( enchantment.AttributeID == 0xFF );
      REQUIRE( enchantment.Range == RangeType::Touch );
      REQUIRE( enchantment.Area == 0 );
      REQUIRE( enchantment.Duration == 1440 );
      REQUIRE( enchantment.MagnitudeMin == 5 );
      REQUIRE( enchantment.MagnitudeMax == 5 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( enchantment.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of ENAM is not 24")
    {
      {
        const auto data = "ENAM\x17\0\0\0\x53\0\x19\xFF\x01\0\0\0\0\0\0\0\xA0\x05\0\0\x05\0\0\0\x05\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ENAM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        EnchantmentData enchantment;
        REQUIRE_FALSE( enchantment.loadFromStream(stream) );
      }

      {
        const auto data = "ENAM\x19\0\0\0\x53\0\x19\xFF\x01\0\0\0\0\0\0\0\xA0\x05\0\0\x05\0\0\0\x05\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read ENAM, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        EnchantmentData enchantment;
        REQUIRE_FALSE( enchantment.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before all data can be read")
    {
      const auto data = "ENAM\x18\0\0\0\x53\0\x19\xFF\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read APPA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      EnchantmentData enchantment;
      REQUIRE_FALSE( enchantment.loadFromStream(stream) );
    }
  }
}
