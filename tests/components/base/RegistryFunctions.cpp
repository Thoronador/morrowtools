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

#include "../locate_catch.hpp"
#include "../../../base/RegistryFunctions.hpp"
#include "../../../base/UtilityFunctions.hpp"

TEST_CASE("RegistryFunctions")
{
  SECTION("getRegistryStringValueHKLM")
  {
    #if defined(_WIN32) || defined(_WIN64)
    SECTION("get existing key")
    {
      std::string data;

      REQUIRE( MWTP::getRegistryStringValueHKLM(data,
               "Software\\Microsoft\\Windows\\CurrentVersion\\Installer", "InstallerLocation") );
      const auto lc_data = lowerCase(data);
      const bool with_slash = lc_data == "c:\\windows\\system32\\";
      const bool no_slash = lc_data == "c:\\windows\\system32";
      REQUIRE( (with_slash || no_slash) );
    }

    SECTION("get non-existing key")
    {
      std::string data;

      REQUIRE_FALSE( MWTP::getRegistryStringValueHKLM(data, "foo\\bar\\baz", "quux") );
      REQUIRE( data == "" );
    }
    #else
    // No Windows registry exists on this platform.
    SECTION("fail on non-Windows platform")
    {
      std::string data;

      REQUIRE_FALSE( MWTP::getRegistryStringValueHKLM(data,
                     "Software\\Microsoft\\Windows\\CurrentVersion\\Installer",
                     "InstallerLocation") );
      REQUIRE( data == "");
    }
    #endif
  }
}
