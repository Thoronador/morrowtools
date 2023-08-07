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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/EffectBlock.hpp"

TEST_CASE("EffectBlock")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    EffectBlock block;

    REQUIRE( block.effectFormID == 0 );
    REQUIRE( block.magnitude == 0 );
    REQUIRE( block.areaOfEffect == 0 );
    REQUIRE( block.duration == 0 );
    REQUIRE( block.unknownCTDA_CIS2s.empty() );
  }

  SECTION("equality operator")
  {
    EffectBlock a;
    EffectBlock b;

    SECTION("equal")
    {
      a.effectFormID = 0x12345678;
      a.magnitude = 1.25f;
      a.areaOfEffect = 42;
      a.duration = 2;

      b.effectFormID = 0x12345678;
      b.magnitude = 1.25f;
      b.areaOfEffect = 42;
      b.duration = 2;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("effect id mismatch")
      {
        a.effectFormID = 0x12345678;
        b.effectFormID = 0x90ABCDEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("magnitude mismatch")
      {
        a.magnitude = 1.25f;
        b.magnitude = 2.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("areaOfEffect mismatch")
      {
        a.areaOfEffect = 1;
        b.areaOfEffect = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("duration mismatch")
      {
        a.duration = 1;
        b.duration = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
        b.unknownCTDA_CIS2s.clear();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("getWriteSize")
  {
    EffectBlock block;

    SECTION("base size for minimal block")
    {
      REQUIRE( block.getWriteSize() == 28 );
    }

    SECTION("size adjusts with length of CTDA / CIS2")
    {
      REQUIRE( block.getWriteSize() == 28 );
      block.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( block.getWriteSize() == 28 + 38 );
      block.unknownCTDA_CIS2s.back().unknownCISx = "foo";
      REQUIRE( block.getWriteSize() == 28 + 38 + 10 );
      block.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( block.getWriteSize() == 28 + 38 + 10 + 38 );
      block.unknownCTDA_CIS2s.back().unknownCISx = "foobar";
      REQUIRE( block.getWriteSize() == 28 + 38 + 10 + 38 + 13 );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("save minimal block")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      EffectBlock block;
      // Fill data.
      block.effectFormID = 0x00345678;
      block.magnitude = 1.0f;
      block.areaOfEffect = 42;
      block.duration = 0x11223344;
      // -- no CTDA stuff

      // Writing should succeed.
      REQUIRE( block.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "EFID\x04\0\x78\x56\x34\0EFIT\x0C\0\0\0\x80\x3F\x2A\0\0\0\x44\x33\x22\x11"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save block with CTDA/CIS2")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      EffectBlock block;
      // Fill data.
      block.effectFormID = 0x00345678;
      block.magnitude = 1.0f;
      block.areaOfEffect = 42;
      block.duration = 0x11223344;
      // CTDA stuff
      CTDAData ctda;
      ctda.content = { 2, 0, 0, 0, 0, 0, 0x80, 0x3F, 0x1B, 0, 0, 0, 0x0E, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0x14, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF };
      block.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(ctda, "foobar"));

      // Writing should succeed.
      REQUIRE( block.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "EFID\x04\0\x78\x56\x34\0EFIT\x0C\0\0\0\x80\x3F\x2A\0\0\0\x44\x33\x22\x11\x43TDA\x20\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF\x43IS2\x07\0foobar\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
