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
#include "../../../../../sr/base/records/CTDAData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("CTDA_CISx_compound")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    CTDA_CISx_compound<cCIS2> compound;

    for(const auto byte: compound.unknownCTDA.content)
    {
      REQUIRE( byte == 0 );
    }
    REQUIRE( compound.unknownCISx.empty() );
  }

  SECTION("equality operator")
  {
    CTDA_CISx_compound<cCIS2> a;
    CTDA_CISx_compound<cCIS2> b;

    SECTION("equal")
    {
      for (unsigned int i = 0; i < 32; ++i)
      {
        a.unknownCTDA.content[i] = 2 * i + 1;
        b.unknownCTDA.content[i] = 2 * i + 1;
      }

      REQUIRE( a == b );
      REQUIRE( b == a );

      a.unknownCISx = "foo";
      b.unknownCISx = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("CTDAData mismatch")
      {
        a.unknownCTDA.content[31] = 42;
        b.unknownCTDA.content[31] = 12;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CISx mismatch")
      {
        for (unsigned int i = 0; i < 32; ++i)
        {
          a.unknownCTDA.content[i] = 3 * i + 2;
          b.unknownCTDA.content[i] = 3 * i + 2;
        }

        REQUIRE( a == b );

        a.unknownCISx = "foobar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.unknownCISx = "foobar";

        REQUIRE( a == b );

        b.unknownCISx = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownCISx = "bar";
        REQUIRE( a == b );

        a.unknownCISx.clear();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("getWriteSize")
  {
    CTDA_CISx_compound<cCIS2> compound;

    // CTDA only
    REQUIRE( compound.getWriteSize() == 38 );

    // with CIS2
    compound.unknownCISx = "foo";
    REQUIRE( compound.getWriteSize() == 48 );
    compound.unknownCISx = "foobar";
    REQUIRE( compound.getWriteSize() == 51 );
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("save compound without CISx")
    {
      std::ostringstream stream;
      REQUIRE( stream.good() );

      CTDA_CISx_compound<cCIS2> compound;
      // Fill data.
      auto& ctda = compound.unknownCTDA;
      ctda.content[0] = 2;
      ctda.content[1] = 0;
      ctda.content[2] = 0;
      ctda.content[3] = 0;
      ctda.content[4] = 0;
      ctda.content[5] = 0;
      ctda.content[6] = 0x80;
      ctda.content[7] = 0x3F;
      ctda.content[8] = 0x1B;
      ctda.content[9] = 0;
      ctda.content[10] = 0;
      ctda.content[11] = 0;
      ctda.content[12] = 0x0E;
      ctda.content[13] = 0;
      ctda.content[14] = 0;
      ctda.content[15] = 0;
      ctda.content[16] = 0;
      ctda.content[17] = 0;
      ctda.content[18] = 0;
      ctda.content[19] = 0;
      ctda.content[20] = 0x02;
      ctda.content[21] = 0;
      ctda.content[22] = 0;
      ctda.content[23] = 0;
      ctda.content[24] = 0x14;
      ctda.content[25] = 0;
      ctda.content[26] = 0;
      ctda.content[27] = 0;
      ctda.content[28] = 0xFF;
      ctda.content[29] = 0xFF;
      ctda.content[30] = 0xFF;
      ctda.content[31] = 0xFF;

      // Writing should succeed.
      REQUIRE( compound.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CTDA\x20\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save compound with CISx: CIS1")
    {
      std::ostringstream stream;
      REQUIRE( stream.good() );

      CTDA_CIS1_compound compound;
      // Fill data.
      auto& ctda = compound.unknownCTDA;
      ctda.content[0] = 0x09;
      ctda.content[1] = 0x08;
      ctda.content[2] = 0x07;
      ctda.content[3] = 0x06;
      ctda.content[4] = 0x05;
      ctda.content[5] = 0x06;
      ctda.content[6] = 0x07;
      ctda.content[7] = 0x08;
      ctda.content[8] = 0x09;
      ctda.content[9] = 0x0A;
      ctda.content[10] = 0x0B;
      ctda.content[11] = 0x0C;
      ctda.content[12] = 0x0D;
      ctda.content[13] = 0x0E;
      ctda.content[14] = 0x0F;
      ctda.content[15] = 0x10;
      ctda.content[16] = 0;
      ctda.content[17] = 0;
      ctda.content[18] = 0;
      ctda.content[19] = 0;
      ctda.content[20] = 0x02;
      ctda.content[21] = 0;
      ctda.content[22] = 0;
      ctda.content[23] = 0;
      ctda.content[24] = 0x14;
      ctda.content[25] = 0;
      ctda.content[26] = 0;
      ctda.content[27] = 0;
      ctda.content[28] = 0xFF;
      ctda.content[29] = 0xFF;
      ctda.content[30] = 0xFF;
      ctda.content[31] = 0;

      compound.unknownCISx = "foobar";

      // Writing should succeed.
      REQUIRE( compound.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CTDA\x20\0\x09\x08\x07\x06\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\0CIS1\x07\0foobar\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save compound with CISx: CIS2")
    {
      std::ostringstream stream;
      REQUIRE( stream.good() );

      CTDA_CIS2_compound compound;
      // Fill data.
      auto& ctda = compound.unknownCTDA;
      ctda.content[0] = 0x01;
      ctda.content[1] = 0x02;
      ctda.content[2] = 0x03;
      ctda.content[3] = 0x04;
      ctda.content[4] = 0x05;
      ctda.content[5] = 0x06;
      ctda.content[6] = 0x07;
      ctda.content[7] = 0x08;
      ctda.content[8] = 0x09;
      ctda.content[9] = 0x0A;
      ctda.content[10] = 0x0B;
      ctda.content[11] = 0x0C;
      ctda.content[12] = 0x0D;
      ctda.content[13] = 0x0E;
      ctda.content[14] = 0x0F;
      ctda.content[15] = 0x10;
      ctda.content[16] = 0;
      ctda.content[17] = 0;
      ctda.content[18] = 0;
      ctda.content[19] = 0;
      ctda.content[20] = 0x02;
      ctda.content[21] = 0;
      ctda.content[22] = 0;
      ctda.content[23] = 0;
      ctda.content[24] = 0x14;
      ctda.content[25] = 0;
      ctda.content[26] = 0;
      ctda.content[27] = 0;
      ctda.content[28] = 0xFF;
      ctda.content[29] = 0xFF;
      ctda.content[30] = 0xFF;
      ctda.content[31] = 0;

      compound.unknownCISx = "abc";

      // Writing should succeed.
      REQUIRE( compound.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CTDA\x20\0\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\0CIS2\x04\0abc\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
