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

TEST_CASE("QuestRecord::IndexEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QuestRecord::IndexEntry entry;

    REQUIRE( entry.index == 0 );
    REQUIRE( entry.indexUnknownPart == 0 );
    REQUIRE( entry.theQSDTs.empty() );
  }

  SECTION("equality")
  {
    QuestRecord::IndexEntry a;
    QuestRecord::IndexEntry b;

    SECTION("equal")
    {
      a.index = 1;
      b.index = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("index mismatch")
      {
        a.index = 1;
        b.index = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("indexUnknownPart mismatch")
      {
        a.indexUnknownPart = 1;
        b.indexUnknownPart = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("QSDTs mismatch")
      {
        a.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("hasFinishingQSDT")
  {
    QuestRecord::IndexEntry entry;

    SECTION("empty")
    {
      REQUIRE_FALSE( entry.hasFinishingQSDT() );
    }

    SECTION("no finishers")
    {
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());

      REQUIRE_FALSE( entry.hasFinishingQSDT() );
    }

    SECTION("one finisher")
    {
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.back().isFinisher = true;

      REQUIRE( entry.hasFinishingQSDT() );
    }

    SECTION("multiple finisher")
    {
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.back().isFinisher = true;
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.push_back(QuestRecord::IndexEntry::QSDTRecord());
      entry.theQSDTs.back().isFinisher = true;

      REQUIRE( entry.hasFinishingQSDT() );
    }
  }
}
