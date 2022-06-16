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

#include "../../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/CTDAData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("CTDAData")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    CTDAData ctda;

    for(const auto byte: ctda.content)
    {
      REQUIRE( byte == 0 );
    }
  }

  SECTION("equality operator")
  {
    CTDAData a;
    CTDAData b;

    SECTION("equal")
    {
      for (unsigned int i = 0; i < 32; ++i)
      {
        a.content[i] = 2 * i + 1;
        b.content[i] = 2 * i + 1;
      }

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      a.content[31] = 42;
      b.content[31] = 12;

      REQUIRE_FALSE( a == b );
      REQUIRE_FALSE( b == a );
    }
  }

  SECTION("clear")
  {
    CTDAData ctda;
    for (unsigned int i = 0; i < 32; ++i)
    {
      ctda.content[i] = 3 * i + 1;
      REQUIRE( ctda.content[i] != 0 );
    }
    ctda.clear();
    for (const uint8_t byte: ctda.content)
    {
      REQUIRE( byte == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const std::string_view data = "CTDA\x20\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CTDA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CTDAData ctda;
      dummy = 0;
      REQUIRE( ctda.loadFromStream(stream, dummy) );
      REQUIRE( dummy == 34 );
      // Check data.
      REQUIRE( ctda.content[0] == 2 );
      REQUIRE( ctda.content[1] == 0 );
      REQUIRE( ctda.content[2] == 0 );
      REQUIRE( ctda.content[3] == 0 );
      REQUIRE( ctda.content[4] == 0 );
      REQUIRE( ctda.content[5] == 0 );
      REQUIRE( ctda.content[6] == 0x80 );
      REQUIRE( ctda.content[7] == 0x3F );
      REQUIRE( ctda.content[8] == 0x1B );
      REQUIRE( ctda.content[9] == 0 );
      REQUIRE( ctda.content[10] == 0 );
      REQUIRE( ctda.content[11] == 0 );
      REQUIRE( ctda.content[12] == 0x0E );
      REQUIRE( ctda.content[13] == 0 );
      REQUIRE( ctda.content[14] == 0 );
      REQUIRE( ctda.content[15] == 0 );
      REQUIRE( ctda.content[16] == 0 );
      REQUIRE( ctda.content[17] == 0 );
      REQUIRE( ctda.content[18] == 0 );
      REQUIRE( ctda.content[19] == 0 );
      REQUIRE( ctda.content[20] == 0x02 );
      REQUIRE( ctda.content[21] == 0 );
      REQUIRE( ctda.content[22] == 0 );
      REQUIRE( ctda.content[23] == 0 );
      REQUIRE( ctda.content[24] == 0x14 );
      REQUIRE( ctda.content[25] == 0 );
      REQUIRE( ctda.content[26] == 0 );
      REQUIRE( ctda.content[27] == 0 );
      REQUIRE( ctda.content[28] == 0xFF );
      REQUIRE( ctda.content[29] == 0xFF );
      REQUIRE( ctda.content[30] == 0xFF );
      REQUIRE( ctda.content[31] == 0xFF );
    }

    SECTION("corrupt data: length of CTDA is greater than 32")
    {
      const std::string_view data = "CTDA\x21\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CTDA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CTDAData ctda;
      REQUIRE_FALSE( ctda.loadFromStream(stream, dummy) );
    }

    SECTION("corrupt data: length of CTDA is less than 32")
    {
      const std::string_view data = "CTDA\x1F\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CTDA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CTDAData ctda;
      REQUIRE_FALSE( ctda.loadFromStream(stream, dummy) );
    }

    SECTION("corrupt data: stream ends before all data can be read")
    {
      const std::string_view data = "CTDA\x20\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read CTDA, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CTDAData ctda;
      REQUIRE_FALSE( ctda.loadFromStream(stream, dummy) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;

    SECTION("save record")
    {
      std::ostringstream stream;
      REQUIRE( stream.good() );

      CTDAData ctda;
      // Fill data.
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
      REQUIRE( ctda.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "CTDA\x20\0\x02\0\0\0\0\0\x80\x3F\x1B\0\0\0\x0E\0\0\0\0\0\0\0\x02\0\0\0\x14\0\0\0\xFF\xFF\xFF\xFF"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
