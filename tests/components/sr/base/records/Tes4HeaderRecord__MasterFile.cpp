/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../../../../sr/base/records/TES4HeaderRecord.hpp"

TEST_CASE("Tes4HeaderRecord::MasterFile")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    Tes4HeaderRecord::MasterFile file;

    REQUIRE( file.fileName.empty() );
    REQUIRE( file.data == 0 );
  }

  SECTION("equality operator")
  {
    Tes4HeaderRecord::MasterFile a;
    Tes4HeaderRecord::MasterFile b;

    SECTION("equal")
    {
      a.fileName = "Update.esm";
      a.data = 0xDEADBEEFDEADBEEF;

      b.fileName = "Update.esm";
      b.data = 0xDEADBEEFDEADBEEF;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("fileName mismatch")
      {
        a.fileName = "Skyrim.esm";
        a.data = 0xDEADBEEFDEADBEEF;

        b.fileName = "Update.esm";
        b.data = 0xDEADBEEFDEADBEEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("data mismatch")
      {
        a.fileName = "Skyrim.esm";
        a.data = 0x00000000DEADBEEF;

        b.fileName = "Skyrim.esm";
        b.data = 0xDEADBEEFDEADBEEF;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}
