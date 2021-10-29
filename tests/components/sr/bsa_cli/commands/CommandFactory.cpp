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
#include "../../../../../sr/bsa_cli/commands/CommandFactory.hpp"
#include "../../../../../sr/bsa_cli/commands/Commands.hpp"
#include "../../../../../sr/bsa_cli/commands/ExtractFile.hpp"
#include "../../../../../sr/bsa_cli/commands/Help.hpp"
#include "../../../../../sr/bsa_cli/commands/Info.hpp"
#include "../../../../../sr/bsa_cli/commands/List.hpp"
#include "../../../../../sr/bsa_cli/commands/Operations.hpp"

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

    SECTION("commands")
    {
      const auto ptr = createCommand(Operation::Commands);

      REQUIRE_FALSE( ptr == nullptr );
      const auto typed = dynamic_cast<Commands*>(ptr.get());
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
