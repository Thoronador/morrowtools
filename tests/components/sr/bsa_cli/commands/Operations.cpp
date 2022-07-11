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
#include "../../../../../sr/bsa_cli/commands/Operations.hpp"

TEST_CASE("bsa_cli::Operations")
{
  using namespace SRTP;

  SECTION("allOperations")
  {
    const auto all = allOperations();

    REQUIRE( std::find(all.begin(), all.end(), Operation::CheckHashes) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::Commands) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::Directories) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::DirectoryMetadata) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::ExtractAll) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::ExtractDirectory) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::ExtractFile) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::FileMetadata) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::Help) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::Info) != all.end() );
    REQUIRE( std::find(all.begin(), all.end(), Operation::List) != all.end() );
  }

  SECTION("operationToString")
  {
    const auto all = allOperations();
    for (const auto op: all)
    {
      REQUIRE_FALSE( operationToString(op).empty() );
    }
  }

  SECTION("operationToString - invalid enumeration")
  {
    const Operation op = static_cast<Operation>(static_cast<int>(Operation::List) + 42);

    REQUIRE( operationToString(op).empty() );
  }

  SECTION("operationToString - parseOperation - round trip")
  {
    const auto all = allOperations();
    for (const auto op: all)
    {
      REQUIRE( parseOperation(operationToString(op)) == op );
    }
  }

  SECTION("parseOperation")
  {
    REQUIRE( parseOperation("") == std::nullopt );
    REQUIRE( parseOperation("check-hashes") == Operation::CheckHashes );
    REQUIRE( parseOperation("commands") == Operation::Commands );
    REQUIRE( parseOperation("directories") == Operation::Directories );
    REQUIRE( parseOperation("directory-metadata") == Operation::DirectoryMetadata );
    REQUIRE( parseOperation("extract") == Operation::ExtractAll );
    REQUIRE( parseOperation("extract-all") == Operation::ExtractAll );
    REQUIRE( parseOperation("extract-directory") == Operation::ExtractDirectory );
    REQUIRE( parseOperation("extract-file") == Operation::ExtractFile );
    REQUIRE( parseOperation("file-metadata") == Operation::FileMetadata );
    REQUIRE( parseOperation("help") == Operation::Help );
    REQUIRE( parseOperation("list") == Operation::List );
    REQUIRE( parseOperation("info") == Operation::Info );

    REQUIRE( parseOperation("ChEcK-HaShEs") == std::nullopt );
    REQUIRE( parseOperation("CoMMaNdS") == std::nullopt );
    REQUIRE( parseOperation("DiReCtOrY-MeTaDaTa") == std::nullopt );
    REQUIRE( parseOperation("eXtRacT") == std::nullopt );
    REQUIRE( parseOperation("eXtRacT-AlL") == std::nullopt );
    REQUIRE( parseOperation("EXTRACT-DiReCtoRy") == std::nullopt );
    REQUIRE( parseOperation("EXTRACT-file") == std::nullopt );
    REQUIRE( parseOperation("FiLe-MeTaDaTa") == std::nullopt );
    REQUIRE( parseOperation("DiReCtOrIeS") == std::nullopt );
    REQUIRE( parseOperation("hELp") == std::nullopt );
    REQUIRE( parseOperation("LiSt") == std::nullopt );
    REQUIRE( parseOperation("InFo") == std::nullopt );
  }
}
