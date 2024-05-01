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
#include "../../../../../apps/sr/bsa_cli/commands/Info.hpp"

void writeBsaHeaderForTest(const std::string& fileName, const uint32_t version, const uint32_t flags)
{
  using namespace std::string_view_literals;
  const auto data0 = "BSA\0"sv;
  const auto offset = "\x24\0\0\0"sv;
  const auto data1 = "\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;

  // write BSA header to file
  std::ofstream file(fileName);
  file.write(data0.data(), data0.size());
  file.write(reinterpret_cast<const char*>(&version), 4);
  file.write(offset.data(), offset.size());
  file.write(reinterpret_cast<const char*>(&flags), 4);
  file.write(data1.data(), data1.size());
  file.close();
}

TEST_CASE("bsa_cli::Info")
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace SRTP::bsa_cli;

  SECTION("parseArguments")
  {
    const auto arguments = std::string("a.out\0info\0foo_info.bsa\0fail\0"sv);
    std::array<char*, 4> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11]),
        const_cast<char*>(&arguments.c_str()[24])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "info"s );
    REQUIRE( argv[2] == "foo_info.bsa"s );
    REQUIRE( argv[3] == "fail"s );

    Info command;

    REQUIRE( command.parseArguments(1, argv) != 0 );
    REQUIRE( command.parseArguments(2, argv) != 0 );
    REQUIRE( command.parseArguments(3, argv) != 0 );

    const std::filesystem::path path{"foo_info.bsa"};
    const MWTP::FileGuard guard{path};

    // create "BSA" file
    {
      std::ofstream bsa(path, std::ios::trunc | std::ios::out);
      bsa.close();
    }

    REQUIRE( command.parseArguments(3, argv) == 0 );
  }

  SECTION("run: standard header, v104, compressed by default")
  {
    const auto arguments = std::string("a.out\0info\0foo_info_run0.bsa\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "info"s );
    REQUIRE( argv[2] == "foo_info_run0.bsa"s );

    const std::filesystem::path path{"foo_info_run0.bsa"};
    const MWTP::FileGuard guard{path};

    Info command;
    writeBsaHeaderForTest("foo_info_run0.bsa", 104, 7);

    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(3, argv) == 0 );
    // Run should succeed.
    REQUIRE( command.run() == 0 );
  }

  SECTION("run: standard header, v105, compressed by default")
  {
    const auto arguments = std::string("a.out\0info\0foo_info_run1.bsa\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "info"s );
    REQUIRE( argv[2] == "foo_info_run1.bsa"s );

    Info command;
    writeBsaHeaderForTest("foo_info_run1.bsa", 105, 7);

    const std::filesystem::path path{"foo_info_run1.bsa"};
    const MWTP::FileGuard guard{path};

    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(3, argv) == 0 );
    // Run should succeed.
    REQUIRE( command.run() == 0 );
  }

  SECTION("run: standard header, v104, not compressed by default")
  {
    const auto arguments = std::string("a.out\0info\0foo_info_run2.bsa\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "info"s );
    REQUIRE( argv[2] == "foo_info_run2.bsa"s );

    const std::filesystem::path path{"foo_info_run2.bsa"};
    const MWTP::FileGuard guard{path};

    Info command;
    writeBsaHeaderForTest("foo_info_run2.bsa", 104, 3);

    // parse arguments to get file name of BSA
    REQUIRE( command.parseArguments(3, argv) == 0 );
    // Run should succeed.
    REQUIRE( command.run() == 0 );
  }

  SECTION("run: fail with empty file")
  {
    const auto arguments = std::string("a.out\0info\0foo_info_run.bsa\0"sv);
    std::array<char*, 3> argArr = {
        const_cast<char*>(&arguments.c_str()[0]),
        const_cast<char*>(&arguments.c_str()[6]),
        const_cast<char*>(&arguments.c_str()[11])
    };
    char ** argv = argArr.data();

    REQUIRE( argv[0] == "a.out"s );
    REQUIRE( argv[1] == "info"s );
    REQUIRE( argv[2] == "foo_info_run.bsa"s );

    Info command;

    const std::filesystem::path path{"foo_info_run.bsa"};
    const MWTP::FileGuard guard{path};

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
    Info command;
    REQUIRE_FALSE( command.helpShort().empty() );
  }

  SECTION("helpLong returns non-empty string")
  {
    Info command;
    REQUIRE_FALSE( command.helpLong("foo").empty() );
  }
}
