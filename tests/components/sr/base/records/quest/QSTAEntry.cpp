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

#include "../../../../locate_catch.hpp"
#include "../../../../../../sr/base/records/quest/QSTAEntry.hpp"

TEST_CASE("QSTAEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QSTAEntry entry;

    REQUIRE( entry.unknownQSTA == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
  }

  SECTION("equality")
  {
    QSTAEntry a;
    QSTAEntry b;

    SECTION("equal")
    {
      a.unknownQSTA = 1;
      b.unknownQSTA = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("QSTA mismatch")
      {
        a.unknownQSTA = 1;
        b.unknownQSTA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    QSTAEntry entry;

    entry.unknownQSTA = 0xF00BA12;
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(CTDAData(), "foo"));
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

    entry.clear();

    REQUIRE( entry.unknownQSTA == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
  }

  SECTION("getWriteSize")
  {
    QSTAEntry entry;

    entry.unknownQSTA = 0xF00BA12;

    SECTION("size without CTDA/CIS2 pairs")
    {
      REQUIRE( entry.getWriteSize() == 14 );
    }

    SECTION("size adjusts with number of CTDA/CIS2 stuff")
    {
      REQUIRE( entry.getWriteSize() == 14 );

      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( entry.getWriteSize() == 52 );

      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( entry.getWriteSize() == 90 );

      entry.unknownCTDA_CIS2s.back().unknownCISx = "foo";

      REQUIRE( entry.getWriteSize() == 100 );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: no CTDA/CIS2")
    {
      QSTAEntry entry;
      entry.unknownQSTA = 0x0123456789ABCDEF;

      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QSTA\x08\0\xEF\xCD\xAB\x89\x67\x45\x23\x01"sv );
    }

    SECTION("default: with CTDA/CIS2")
    {
      QSTAEntry entry;
      entry.unknownQSTA = 0x1122334489ABCDEF;
      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      entry.unknownCTDA_CIS2s.back().unknownCISx = "foo";
      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      entry.unknownCTDA_CIS2s.back().unknownCISx = "bar";

      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      const auto data = "QSTA\x08\0\xEF\xCD\xAB\x89\x44\x33\x22\x11\x43TDA\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CIS2\x04\0foo\0CTDA\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CIS2\x04\0bar\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
