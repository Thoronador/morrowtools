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
#include "../../../../../sr/base/records/QuestRecord.hpp"

TEST_CASE("QuestRecord::QOBJEntry::QSTAEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QuestRecord::QOBJEntry::QSTAEntry entry;

    REQUIRE( entry.unknownQSTA == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
  }

  SECTION("equality")
  {
    QuestRecord::QOBJEntry::QSTAEntry a;
    QuestRecord::QOBJEntry::QSTAEntry b;

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
    QuestRecord::QOBJEntry::QSTAEntry entry;

    entry.unknownQSTA = 0xF00BA12;
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(CTDAData(), "foo"));
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
    entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

    entry.clear();

    REQUIRE( entry.unknownQSTA == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
  }
}
