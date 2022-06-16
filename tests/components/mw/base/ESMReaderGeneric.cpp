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

#include "../../locate_catch.hpp"
#include <filesystem>
#include <string_view>
#include "../../../../mw/base/ESMReaderGeneric.hpp"
#include "../../../../mw/base/MW_Constants.hpp"
#include "../../../../mw/base/records/StaticRecord.hpp"

TEST_CASE("MWTP::ESMReaderGeneric")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SECTION("non-null vector")
    {
      ESMReaderGeneric::VectorType vec;
      ESMReaderGeneric reader(&vec);
    }

    SECTION("throws when vector is null")
    {
      REQUIRE_THROWS( ESMReaderGeneric(nullptr) );
    }
  }

  SECTION("deallocateRecordsInVector")
  {
    ESMReaderGeneric::VectorType vec;
    vec.push_back(new StaticRecord);
    vec.push_back(nullptr);
    vec.push_back(new StaticRecord);
    ESMReaderGeneric reader(&vec);

    REQUIRE( vec.size() == 3 );
    reader.deallocateRecordsInVector();
    REQUIRE( vec.size() == 0 );
  }

  SECTION("processNextRecord")
  {
    ESMReaderGeneric::VectorType vec;
    ESMReaderGeneric reader(&vec);

    SECTION("default: process a record")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( vec.size() == 0 );
      REQUIRE( reader.processNextRecord(stream) == 1 );
      REQUIRE( vec.size() == 1 );
      reader.deallocateRecordsInVector();
      REQUIRE( vec.size() == 0 );
    }

    SECTION("failure: incomplete record")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processNextRecord(stream) == -1 );
      REQUIRE( vec.size() == 0 );
    }

    SECTION("failure: unknown record header")
    {
      const auto data = "FAIL\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processNextRecord(stream) == -1 );
      REQUIRE( vec.size() == 0 );
    }
  }
}
