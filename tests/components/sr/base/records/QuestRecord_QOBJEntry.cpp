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

TEST_CASE("QuestRecord::QOBJEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QuestRecord::QOBJEntry entry;

    REQUIRE( entry.unknownQOBJ == 0 );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.displayText.isPresent() );
    REQUIRE( entry.theQSTAs.empty() );
  }

  SECTION("equality")
  {
    QuestRecord::QOBJEntry a;
    QuestRecord::QOBJEntry b;

    SECTION("equal")
    {
      a.unknownQOBJ = 1;
      b.unknownQOBJ = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("QOBJ mismatch")
      {
        a.unknownQOBJ = 1;
        b.unknownQOBJ = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FNAM mismatch")
      {
        a.unknownFNAM = 1;
        b.unknownFNAM = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("displayText mismatch")
      {
        a.displayText = LocalizedString(LocalizedString::Type::Index, 1, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("QSTAs mismatch")
      {
        a.theQSTAs.push_back(QuestRecord::QOBJEntry::QSTAEntry());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    QuestRecord::QOBJEntry entry;

    entry.unknownQOBJ = 0x1234;
    entry.unknownFNAM = 0x12345678;
    entry.displayText = LocalizedString(LocalizedString::Type::Index, 1, "");
    entry.theQSTAs.push_back(QuestRecord::QOBJEntry::QSTAEntry());
    entry.theQSTAs.push_back(QuestRecord::QOBJEntry::QSTAEntry());

    entry.clear();

    REQUIRE( entry.unknownQOBJ == 0 );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.displayText.isPresent() );
    REQUIRE( entry.theQSTAs.empty() );
  }
}
