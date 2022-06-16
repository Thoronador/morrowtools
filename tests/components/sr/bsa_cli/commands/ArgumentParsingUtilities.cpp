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

#include "../../../locate_catch.hpp"
#include <array>
#include <fstream>
#include "../../../../../base/FileFunctions.hpp"
#include "../../../../../sr/bsa_cli/commands/ArgumentParsingUtilities.hpp"

TEST_CASE("bsa_cli::ArgumentParsingUtilities")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArgumentsBsaFileNameOnly")
  {
    SECTION("list example")
    {
      const auto arguments = std::string("a.out\0list\0foo.bsa\0fail\0"sv);
      std::array<char*, 4> argArr = {
          const_cast<char*>(&arguments.c_str()[0]),
          const_cast<char*>(&arguments.c_str()[6]),
          const_cast<char*>(&arguments.c_str()[11]),
          const_cast<char*>(&arguments.c_str()[19])
      };
      char ** argv = argArr.data();

      REQUIRE( argv[0] == "a.out"s );
      REQUIRE( argv[1] == "list"s );
      REQUIRE( argv[2] == "foo.bsa"s );
      REQUIRE( argv[3] == "fail"s );

      std::string fileName = "";
      REQUIRE( parseArgumentsBsaFileNameOnly(1, argv, fileName) != 0 );
      REQUIRE( fileName.empty() );
      REQUIRE( parseArgumentsBsaFileNameOnly(2, argv, fileName) != 0 );
      REQUIRE( fileName.empty() );
      REQUIRE( parseArgumentsBsaFileNameOnly(3, argv, fileName) != 0 );
      REQUIRE( fileName.empty() );

      // create temp. "BSA" file
      std::ofstream bsa("foo.bsa", std::ios::trunc | std::ios::out);
      bsa.close();

      REQUIRE( parseArgumentsBsaFileNameOnly(3, argv, fileName) == 0 );
      REQUIRE( fileName == "foo.bsa" );

      fileName.clear();
      REQUIRE( parseArgumentsBsaFileNameOnly(4, argv, fileName) != 0 );
      REQUIRE( fileName == "foo.bsa" );

      // cleanup: delete file
      REQUIRE( deleteFile("foo.bsa") );
    }

    SECTION("null in argument pointers")
    {
      std::array<char*, 4> argArr = {
          nullptr,
          nullptr,
          nullptr,
          nullptr
      };
      char ** argv = argArr.data();

      std::string fileName = "";
      REQUIRE( parseArgumentsBsaFileNameOnly(3, argv, fileName) != 0 );
      REQUIRE( fileName.empty() );
    }

    SECTION("null as argument pointer")
    {
      char ** argv = nullptr;

      std::string fileName = "";
      REQUIRE( parseArgumentsBsaFileNameOnly(3, argv, fileName) != 0 );
      REQUIRE( fileName.empty() );
    }
  }
}
