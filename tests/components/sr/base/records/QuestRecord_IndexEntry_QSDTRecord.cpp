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

TEST_CASE("QuestRecord::IndexEntry::QSDTRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QuestRecord::IndexEntry::QSDTRecord entry;

    REQUIRE_FALSE( entry.isFinisher );
    REQUIRE( entry.nextQuestFormID == 0 );
    REQUIRE_FALSE( entry.unknownSCHR.isPresent() );
    REQUIRE( entry.unknownSCTX.empty() );
    REQUIRE_FALSE( entry.hasQNAM );
    REQUIRE( entry.unknownQNAM == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE_FALSE( entry.logEntry.isPresent() );
  }

  SECTION("equality")
  {
    QuestRecord::IndexEntry::QSDTRecord a;
    QuestRecord::IndexEntry::QSDTRecord b;

    SECTION("equal")
    {
      a.nextQuestFormID = 1;
      b.nextQuestFormID = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("isFinisher mismatch")
      {
        a.isFinisher = true;
        b.isFinisher = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("nextQuestFormID mismatch")
      {
        a.nextQuestFormID = 0x12345678;
        b.nextQuestFormID = 0x9ABCDEF0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SCHR mismatch")
      {
        a.unknownSCHR.setPresence(true);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownSCHR.setPresence(false);
        b.unknownSCHR.setPresence(true);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SCTX mismatch")
      {
        a.unknownSCTX = "foo";
        b.unknownSCTX = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("QNAM mismatch")
      {
        a.hasQNAM = true;
        b.hasQNAM = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasQNAM = false;
        b.hasQNAM = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasQNAM = true;
        a.unknownQNAM = 1;
        b.hasQNAM = true;
        b.unknownQNAM = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("logEntry mismatch")
      {
        a.logEntry = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
