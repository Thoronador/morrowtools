/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include <array>
#include <fstream>
#include "../../../../../base/FileFunctions.hpp"
#include "../../../../../sr/bsa_cli/commands/Help.hpp"

TEST_CASE("bsa_cli::Help")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments: default")
  {
    const auto arguments = std::string("a.out\0help\0list\0fail\0"sv);
    std::array<char*, 4> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11]),
        const_cast<char*>(&arguments.c_str()[16])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "help"s );
    REQUIRE( argv[2] == "list"s );
    REQUIRE( argv[3] == "fail"s );

    Help command;

    REQUIRE( command.parseArguments(1, argv) != 0 );
    REQUIRE( command.parseArguments(2, argv) != 0 );
    REQUIRE( command.parseArguments(3, argv) == 0 );
    REQUIRE( command.parseArguments(3, nullptr) != 0 );
    REQUIRE( command.parseArguments(4, argv) != 0 );
  }

  SECTION("parseArguments: unknown command")
  {
    const auto arguments = std::string("a.out\0help\0what-ever\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "help"s );
    REQUIRE( argv[2] == "what-ever"s );

    Help command;
    REQUIRE( command.parseArguments(3, argv) != 0 );
  }

  SECTION("run: help for list command")
  {
    const auto arguments = std::string("a.out\0help\0list\0"sv);
    std::array<char*, 4> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "help"s );
    REQUIRE( argv[2] == "list"s );

    Help command;
    // parse arguments to get command name
    REQUIRE( command.parseArguments(3, argv) == 0 );
    // Run should succeed.
    REQUIRE( command.run() == 0 );
  }

  SECTION("helpShort returns non-empty string")
  {
    Help command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpFull returns non-empty string")
  {
    Help command;
    REQUIRE_FALSE( command.helpFull("foo").empty() );
  }
}
