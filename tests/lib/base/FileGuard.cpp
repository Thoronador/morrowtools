/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2024  Dirk Stolle

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
#include <filesystem>
#include <fstream>
#include "../../../lib/base/FileGuard.hpp"

TEST_CASE("FileGuard")
{
  using namespace MWTP;
  namespace fs = std::filesystem;

  const auto temp = std::filesystem::temp_directory_path();

  SECTION("no change when file does not exist")
  {
    const auto path = temp / "FileGuard_file_does_not_exist.txt";

    REQUIRE_FALSE( fs::exists(path) );

    {
      FileGuard guard{path};
      // Guard does not create the file.
      REQUIRE_FALSE( fs::exists(path) );
    }

    REQUIRE_FALSE( fs::exists(path) );
  }

  SECTION("existing file gets deleted")
  {
    const auto path = temp / "FileGuard_existing_file.txt";
    // Create file.
    {
      std::ofstream fs(path, std::ios::out | std::ios::trunc);
      fs.close();
    }

    REQUIRE( fs::exists(path) );

    {
      FileGuard guard{path};
      // File still exists, because guard is not out of scope - yet.
      REQUIRE( fs::exists(path) );
    }

    // Guard is out of scope, so file must be gone.
    REQUIRE_FALSE( fs::exists(path) );
  }
}
