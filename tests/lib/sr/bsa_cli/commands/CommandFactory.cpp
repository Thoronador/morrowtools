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
#include "../../../../../apps/sr/bsa_cli/commands/CommandFactory.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/CheckHashes.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/Commands.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/DirectoryMetadata.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/ExtractAll.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/ExtractDirectory.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/ExtractFile.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/FileMetadata.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/Directories.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/Help.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/Info.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/List.hpp"
#include "../../../../../apps/sr/bsa_cli/commands/Operations.hpp"

TEST_CASE("bsa_cli::CommandFactory")
{
  using namespace SRTP;
  using namespace SRTP::bsa_cli;
  using namespace SRTP::bsa_cli::CommandFactory;

  SECTION("createCommand")
  {
    SECTION("all operations return a non-null pointer")
    {
      const auto allOps = allOperations();
      for (const auto op: allOps)
      {
        const auto ptr = createCommand(op);
        REQUIRE_FALSE( ptr == nullptr );
      }
    }

    SECTION("invalid operation returns a null pointer")
    {
      const Operation invalid = static_cast<Operation>(static_cast<int>(Operation::List) + 50);
      const auto ptr = createCommand(invalid);
      REQUIRE( ptr == nullptr );
    }

    SECTION("check-hashes")
    {
      const auto ptr = createCommand(Operation::CheckHashes);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<CheckHashes*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("commands")
    {
      const auto ptr = createCommand(Operation::Commands);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<Commands*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("directories")
    {
      const auto ptr = createCommand(Operation::Directories);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<Directories*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("directory-metadata")
    {
      const auto ptr = createCommand(Operation::DirectoryMetadata);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<DirectoryMetadata*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("extract-all")
    {
      const auto ptr = createCommand(Operation::ExtractAll);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<ExtractAll*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("extract-directory")
    {
      const auto ptr = createCommand(Operation::ExtractDirectory);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<ExtractDirectory*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("extract-file")
    {
      const auto ptr = createCommand(Operation::ExtractFile);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<ExtractFile*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("file-metadata")
    {
      const auto ptr = createCommand(Operation::FileMetadata);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<FileMetadata*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("help")
    {
      const auto ptr = createCommand(Operation::Help);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<Help*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<List*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("info")
    {
      const auto ptr = createCommand(Operation::Info);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<Info*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<List*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }

    SECTION("list")
    {
      const auto ptr = createCommand(Operation::List);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<List*>(ptr.get());
      REQUIRE_FALSE( typed == nullptr );
      const auto wrongType = dynamic_cast<Info*>(ptr.get());
      REQUIRE( wrongType == nullptr );
    }
  }
}
