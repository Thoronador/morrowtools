/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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
#include "../../../../../mw/base/records/StaticRecord.hpp"

TEST_CASE("MWTP::BasicRecord")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    StaticRecord record;

    REQUIRE( record.getHeaderOne() == 0 );
    REQUIRE( record.getHeaderFlags() == 0 );
  }

  SECTION("getHeaderOne / setHeaderOne")
  {
    StaticRecord record;

    record.setHeaderOne(123);
    REQUIRE( record.getHeaderOne() == 123 );
    record.setHeaderOne(456);
    REQUIRE( record.getHeaderOne() == 456 );
  }

  SECTION("getHeaderFlags / setHeaderFlags")
  {
    StaticRecord record;

    record.setHeaderFlags(123);
    REQUIRE( record.getHeaderFlags() == 123 );
    record.setHeaderFlags(456);
    REQUIRE( record.getHeaderFlags() == 456 );
  }

  SECTION("isBlocked")
  {
    StaticRecord record;

    REQUIRE_FALSE( record.isBlocked() );
    record.setHeaderFlags(8192);
    REQUIRE( record.isBlocked() );
    record.setHeaderFlags(4);
    REQUIRE_FALSE( record.isBlocked() );
    record.setHeaderFlags(65535);
    REQUIRE( record.isBlocked() );
  }

  SECTION("isPersistent")
  {
    StaticRecord record;

    REQUIRE_FALSE( record.isPersistent() );
    record.setHeaderFlags(1024);
    REQUIRE( record.isPersistent() );
    record.setHeaderFlags(4);
    REQUIRE_FALSE( record.isPersistent() );
    record.setHeaderFlags(65535);
    REQUIRE( record.isPersistent() );
  }
}
