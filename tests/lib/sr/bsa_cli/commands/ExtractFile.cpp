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
#include <fstream>
#include "../../../../../lib/base/FileGuard.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/ExtractFile.hpp"

TEST_CASE("bsa_cli::ExtractFile")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments with nullptr")
  {
    ExtractFile command;
    REQUIRE( command.parseArguments(5, nullptr) != 0 );
  }

  SECTION("parseArguments")
  {
    const auto arguments = std::string("a.out\0extract-file\0foo_exf1.bsa\0dir\\file.dat\0foo.dat"sv);
    std::array<char*, 5> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[19]),
        const_cast<char*>(&arguments.c_str()[32]),
        const_cast<char*>(&arguments.c_str()[45])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "extract-file"s );
    REQUIRE( argv[2] == "foo_exf1.bsa"s );
    REQUIRE( argv[3] == "dir\\file.dat"s );
    REQUIRE( argv[4] == "foo.dat"s );

    ExtractFile command;

    REQUIRE( command.parseArguments(1, argv) != 0 );
    REQUIRE( command.parseArguments(2, argv) != 0 );
    REQUIRE( command.parseArguments(3, argv) != 0 );
    REQUIRE( command.parseArguments(4, argv) != 0 );
    REQUIRE( command.parseArguments(5, argv) != 0 );

    const std::filesystem::path path{"foo_exf1.bsa"};
    const MWTP::FileGuard guard{path};

    // create "BSA" file
    {
      std::ofstream bsa(path, std::ios::trunc | std::ios::out);
      bsa.close();
    }

    REQUIRE( command.parseArguments(5, argv) == 0 );
  }

  SECTION("run: fail with empty file")
  {
    const auto arguments = std::string("a.out\0extract-file\0foo_extf.bsa\0dir\\file.dat\0foo.dat"sv);
    std::array<char*, 5> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[19]),
        const_cast<char*>(&arguments.c_str()[32]),
        const_cast<char*>(&arguments.c_str()[45])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "extract-file"s );
    REQUIRE( argv[2] == "foo_extf.bsa"s );
    REQUIRE( argv[3] == "dir\\file.dat"s );
    REQUIRE( argv[4] == "foo.dat"s );

    ExtractFile command;

    const std::filesystem::path path{"foo_extf.bsa"};
    const MWTP::FileGuard guard{path};

    // create "BSA" file
    {
      std::ofstream bsa(path, std::ios::trunc | std::ios::out);
      bsa.close();
    }
    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(5, argv) == 0 );
    // Run should fail.
    REQUIRE( command.run() != 0 );
  }

  SECTION("helpShort returns non-empty string")
  {
    ExtractFile command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpLong returns non-empty string")
  {
    ExtractFile command;
    REQUIRE_FALSE( command.helpLong("foo").empty() );
  }
}
