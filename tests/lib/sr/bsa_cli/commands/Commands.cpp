/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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
#include <array>
#include "../../../../../apps/sr/bsa_cli/commands/Commands.hpp"

TEST_CASE("bsa_cli::Commands")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments")
  {
    const auto arguments = std::string("a.out\0commands\0foo_list.bsa\0fail\0"sv);
    std::array<char*, 4> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[15]),
        const_cast<char*>(&arguments.c_str()[28])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "commands"s );
    REQUIRE( argv[2] == "foo_list.bsa"s );
    REQUIRE( argv[3] == "fail"s );

    Commands command;

    REQUIRE( command.parseArguments(2, nullptr) != 0 );

    REQUIRE( command.parseArguments(2, argv) == 0 );
    REQUIRE( command.parseArguments(3, argv) != 0 );
    REQUIRE( command.parseArguments(4, argv) != 0 );
  }

  SECTION("run")
  {
    Commands command;
    // Run should succeed.
    REQUIRE( command.run() == 0 );
  }

  SECTION("helpShort returns non-empty string")
  {
    Commands command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpLong returns non-empty string")
  {
    Commands command;
    REQUIRE_FALSE( command.helpLong("foo").empty() );
  }
}
