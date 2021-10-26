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
#include "../../../../../sr/bsa_cli/commands/Operations.hpp"

TEST_CASE("bsa_cli::Operations")
{
  using namespace SRTP;

  SECTION("allOperations")
  {
    const auto all = allOperations();

    REQUIRE( std::find(all.begin(), all.end(), Operation::Commands) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::Info) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::List) != all.end() );
  }

  SECTION("operationToString")
  {
    const auto all = allOperations();
    for (const auto op: all)
    {
      REQUIRE_FALSE( operationToString(op).empty() );
    }
  }

  SECTION("operationToString - parseOperation - roundtrip")
  {
    const auto all = allOperations();
    for (const auto op: all)
    {
      REQUIRE( parseOperation(operationToString(op)) == op );
    }
  }

  SECTION("parseOperation")
  {
    REQUIRE( parseOperation("") == std::nullopt );
    REQUIRE( parseOperation("commands") == Operation::Commands );
    REQUIRE( parseOperation("list") == Operation::List );
    REQUIRE( parseOperation("info") == Operation::Info );

    REQUIRE( parseOperation("CoMMaNdS") == std::nullopt );
    REQUIRE( parseOperation("LiSt") == std::nullopt );
    REQUIRE( parseOperation("InFo") == std::nullopt );
  }
}