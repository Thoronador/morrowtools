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
#include <array>
#include <fstream>
#include "../../../../../base/FileFunctions.hpp"
#include "../../../../../sr/bsa_cli/commands/ExtractFolder.hpp"

TEST_CASE("bsa_cli::ExtractFolder")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments with nullptr")
  {
    ExtractFolder command;
    REQUIRE( command.parseArguments(5, nullptr) != 0 );
  }

  SECTION("parseArguments")
  {
    const auto arguments = std::string("a.out\0extract-folder\0foo_extract_folder.bsa\0dir\\file.dat\0foo.dat"sv);
    std::array<char*, 5> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[21]),
        const_cast<char*>(&arguments.c_str()[44]),
        const_cast<char*>(&arguments.c_str()[57])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "extract-folder"s );
    REQUIRE( argv[2] == "foo_extract_folder.bsa"s );
    REQUIRE( argv[3] == "dir\\file.dat"s );
    REQUIRE( argv[4] == "foo.dat"s );

    ExtractFolder command;

    REQUIRE( command.parseArguments(1, argv) != 0 );
    REQUIRE( command.parseArguments(2, argv) != 0 );
    REQUIRE( command.parseArguments(3, argv) != 0 );
    REQUIRE( command.parseArguments(4, argv) != 0 );
    REQUIRE( command.parseArguments(5, argv) != 0 );

    // create "BSA" file
    std::ofstream bsa("foo_extract_folder.bsa", std::ios::trunc | std::ios::out);
    bsa.close();

    REQUIRE( command.parseArguments(5, argv) == 0 );

    // cleanup: delete file
    REQUIRE( deleteFile("foo_extract_folder.bsa") );
  }

  SECTION("run: fail with empty file")
  {
    const auto arguments = std::string("a.out\0extract-folder\0foo_extract_folder.bsa\0dir\\file.dat\0foo.dat"sv);
    std::array<char*, 5> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[21]),
        const_cast<char*>(&arguments.c_str()[44]),
        const_cast<char*>(&arguments.c_str()[57])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "extract-folder"s );
    REQUIRE( argv[2] == "foo_extract_folder.bsa"s );
    REQUIRE( argv[3] == "dir\\file.dat"s );
    REQUIRE( argv[4] == "foo.dat"s );

    ExtractFolder command;

    // create "BSA" file
    std::ofstream bsa("foo_extract_folder.bsa", std::ios::trunc | std::ios::out);
    bsa.close();
    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(5, argv) == 0 );
    // Run should fail.
    REQUIRE( command.run() != 0 );
    // cleanup: delete file
    REQUIRE( deleteFile("foo_extract_folder.bsa") );
  }

  SECTION("helpShort returns non-empty string")
  {
    ExtractFolder command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpFull returns non-empty string")
  {
    ExtractFolder command;
    REQUIRE_FALSE( command.helpFull("foo").empty() );
  }
}
