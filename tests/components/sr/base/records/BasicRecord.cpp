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
#include "../../../../../sr/base/records/ActionRecord.hpp"

TEST_CASE("BasicRecord")
{
  using namespace SRTP;

  SECTION("getTotalWrittenSize")
  {
    // The BasicRecord class is abstract and cannot be instantiated, so we use
    // ActionRecord, a derived class of BasicRecord instead.
    ActionRecord record;

    record.editorID = "foo";
    REQUIRE( record.getWriteSize() == 10 );
    REQUIRE( record.getTotalWrittenSize() == 34 );

    record.editorID = "foobarfoobarbaz";
    REQUIRE( record.getWriteSize() == 22 );
    REQUIRE( record.getTotalWrittenSize() == 46 );
  }

  SECTION("flag queries")
  {
    // The BasicRecord class is abstract and cannot be instantiated, so we use
    // ActionRecord, a derived class of BasicRecord instead.
    ActionRecord record;

    SECTION("isCompressed")
    {
      REQUIRE_FALSE( record.isCompressed() );
      record.headerFlags = 0xFF000000;
      REQUIRE_FALSE( record.isCompressed() );
      record.headerFlags = BasicRecord::cCompressionFlag;
      REQUIRE( record.isCompressed() );
      record.headerFlags = 0xFFFFFFFF;
      REQUIRE( record.isCompressed() );
    }

    SECTION("isDeleted")
    {
      REQUIRE_FALSE( record.isDeleted() );
      record.headerFlags = 0xFF000000;
      REQUIRE_FALSE( record.isDeleted() );
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.isDeleted() );
      record.headerFlags = 0xFFFFFFFF;
      REQUIRE( record.isDeleted() );
    }

    SECTION("isIgnored")
    {
      REQUIRE_FALSE( record.isIgnored() );
      record.headerFlags = 0xFF000000;
      REQUIRE_FALSE( record.isIgnored() );
      record.headerFlags = BasicRecord::cIgnoredFlag;
      REQUIRE( record.isIgnored() );
      record.headerFlags = 0xFFFFFFFF;
      REQUIRE( record.isIgnored() );
    }
  }
}
