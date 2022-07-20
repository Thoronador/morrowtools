/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include "../../locate_catch.hpp"
#include <algorithm>
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

    // Four string table files required a file system with case-sensitive names,
    // but that is usually not available on Windows platforms.
    #if !defined(_WIN32)
    SECTION("failure: too much string files exist")
    {
      std::vector<std::string> files;

      REQUIRE( getAssociatedTableFiles(glcDirectory + "four.esm", files) != 0 );
    }
    #endif
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

    // Four string table files required a file system with case-sensitive names,
    // but that is usually not available on Windows platforms.
    #if !defined(_WIN32)
    SECTION("failure: too much string files exist")
    {
      std::string component;
      std::vector<std::string> files;

      REQUIRE( getLanguageComponent(glcDirectory,
                   "four", component, files) != 0 );
    }
    #endif
  }

  SECTION("loadStringTables")
  {
    const auto glcDirectory = test_directory + "getLanguageComponent" + delim;

    SECTION("success: all tables exist")
    {
      StringTable table;
      REQUIRE( loadStringTables(glcDirectory + "TestTables.esm", table, std::nullopt) );
      // Check read strings.
      // -- *.strings
      REQUIRE( table.hasString(0x12345678) );
      REQUIRE( table.getString(0x12345678) == "foo" );
      REQUIRE( table.hasString(0x0000AFFE) );
      REQUIRE( table.getString(0x0000AFFE) == "bar" );
      REQUIRE( table.hasString(42) );
      REQUIRE( table.getString(42) == "foobar" );
      // -- *.dlstrings
      REQUIRE( table.hasString(0x123456AA) );
      REQUIRE( table.getString(0x123456AA) == "xyz" );
      REQUIRE( table.hasString(0xC001AFFE) );
      REQUIRE( table.getString(0xC001AFFE) == "bar" );
      REQUIRE( table.hasString(43) );
      REQUIRE( table.getString(43) == "string" );
      // -- *.ilstrings
      REQUIRE( table.hasString(0x123456BB) );
      REQUIRE( table.getString(0x123456BB) == "abc" );
      REQUIRE( table.hasString(0x000BAFFE) );
      REQUIRE( table.getString(0x000BAFFE) == "bar" );
      REQUIRE( table.hasString(44) );
      REQUIRE( table.getString(44) == "Italia" );
    }

    SECTION("failure: no matching string files exist")
    {
      StringTable table;
      REQUIRE_FALSE( loadStringTables(glcDirectory + "TestNone.esm", table, std::nullopt) );
    }

    SECTION("failure: matching string files exist, but they are empty")
    {
      StringTable table;

      REQUIRE_FALSE( loadStringTables(glcDirectory + "test.esm", table, std::nullopt) );
      REQUIRE( table.getNumberOfTableEntries() == 0 );
    }
  }

  SECTION("getBsaName")
  {
    // well-known ESM file names
    REQUIRE( getBsaName("Skyrim") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("skyrim") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("Dawnguard") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("dawnguard") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("HearthFires") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("hearthfires") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("Dragonborn") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("dragonborn") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("Update") == "Skyrim - Interface.bsa" );
    REQUIRE( getBsaName("update") == "Skyrim - Interface.bsa" );
    // some creation club ESM files
    REQUIRE( getBsaName("ccBGSSSE001-Fish") == "ccBGSSSE001-Fish.bsa" );
    REQUIRE( getBsaName("ccBGSSSE025-AdvDSGS") == "ccBGSSSE025-AdvDSGS.bsa" );
    REQUIRE( getBsaName("ccBGSSSE037-Curios") == "ccBGSSSE037-Curios.bsa" );
    REQUIRE( getBsaName("ccQDRSSE001-SurvivalMode") == "ccQDRSSE001-SurvivalMode.bsa" );
  }
}
