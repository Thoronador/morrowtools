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
#include <ctime>
#include <fstream>
#include "../../../../base/DirectoryFunctions.hpp"
#include "../../../../sr/base/TableUtilities.hpp"

TEST_CASE("TableUtilities")
{
  using namespace SRTP;

  const std::string delim = std::string(1, MWTP::pathDelimiter);
  const std::string currentFile = std::string(__FILE__);
  const std::string test_directory = currentFile.substr(0, currentFile.size() - std::string("TableUtilities.cpp").size())
          .append("test_files").append(delim);

  SECTION("getAssociatedTableFiles")
  {
    const auto glcDirectory = test_directory + "getLanguageComponent" + delim;

    SECTION("default: matching string files exist")
    {
      std::vector<std::string> files;

      REQUIRE( getAssociatedTableFiles(glcDirectory + "test.esm", files) == 0 );

      REQUIRE( files.size() == 3 );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.strings") != files.end() );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.dlstrings") != files.end() );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.ilstrings") != files.end() );
    }

    SECTION("failure: data directory does not exist")
    {
      std::vector<std::string> files;

      REQUIRE( getAssociatedTableFiles(glcDirectory + "does-not-exist" + delim
                                        + "what.esm", files) != 0 );
    }

    SECTION("failure: only two string files exist")
    {
      std::vector<std::string> files;

      REQUIRE( getAssociatedTableFiles(glcDirectory + "only_two.esm", files) != 0 );
    }

    SECTION("failure: too much string files exist")
    {
      std::vector<std::string> files;

      REQUIRE( getAssociatedTableFiles(glcDirectory + "four.esm", files) != 0 );
    }
  }


  SECTION("getLanguageComponent")
  {
    const auto glcDirectory = test_directory + "getLanguageComponent" + delim;

    SECTION("default: matching string files exist")
    {
      std::string component;
      std::vector<std::string> files;

      REQUIRE( getLanguageComponent(glcDirectory,
                   "test", component, files) == 0 );

      REQUIRE( component == "German" );
      REQUIRE( files.size() == 3 );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.strings") != files.end() );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.dlstrings") != files.end() );
      REQUIRE( std::find(files.begin(), files.end(), glcDirectory + "Strings" + delim + "test_German.ilstrings") != files.end() );
    }

    SECTION("failure: data directory does not exist")
    {
      std::string component;
      std::vector<std::string> files;

      REQUIRE( getLanguageComponent(glcDirectory + "does-not-exist" + delim,
                   "Skyfoo", component, files) != 0 );
    }

    SECTION("failure: only two string files exist")
    {
      std::string component;
      std::vector<std::string> files;

      REQUIRE( getLanguageComponent(glcDirectory,
                   "only_two", component, files) != 0 );
    }

    SECTION("failure: too much string files exist")
    {
      std::string component;
      std::vector<std::string> files;

      REQUIRE( getLanguageComponent(glcDirectory,
                   "four", component, files) != 0 );
    }
  }
}
