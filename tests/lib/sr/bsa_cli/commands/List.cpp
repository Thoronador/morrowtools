/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2022, 2024  Dirk Stolle

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
#include <fstream>
#include "../../../../../lib/base/FileGuard.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/List.hpp"

TEST_CASE("bsa_cli::List")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments")
  {
    const auto arguments = std::string("a.out\0list\0foo_list.bsa\0fail\0"sv);
    std::array<char*, 4> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11]),
        const_cast<char*>(&arguments.c_str()[24])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "list"s );
    REQUIRE( argv[2] == "foo_list.bsa"s );
    REQUIRE( argv[3] == "fail"s );

    List command;

    REQUIRE( command.parseArguments(1, argv) != 0 );
    REQUIRE( command.parseArguments(2, argv) != 0 );
    REQUIRE( command.parseArguments(3, argv) != 0 );

    const std::filesystem::path path{"foo_list.bsa"};
    MWTP::FileGuard guard{path};

    // create "BSA" file
    {
      std::ofstream bsa(path, std::ios::trunc | std::ios::out);
      bsa.close();
    }

    REQUIRE( command.parseArguments(3, argv) == 0 );
  }

  SECTION("run: fail with empty file")
  {
    const auto arguments = std::string("a.out\0list\0foo_list_run.bsa\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "list"s );
    REQUIRE( argv[2] == "foo_list_run.bsa"s );

    List command;

    const std::filesystem::path path{"foo_list_run.bsa"};
    MWTP::FileGuard guard{path};

    // create "BSA" file
    {
      std::ofstream bsa(path, std::ios::trunc | std::ios::out);
      bsa.close();
    }
    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(3, argv) == 0 );
    // Run should fail.
    REQUIRE( command.run() != 0 );
  }

  SECTION("helpShort returns non-empty string")
  {
    List command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpLong returns non-empty string")
  {
    List command;
    REQUIRE_FALSE( command.helpLong("foo").empty() );
  }
}
