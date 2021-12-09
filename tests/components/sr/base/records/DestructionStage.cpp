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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/DestructionData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("DestructionStage")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    DestructionStage stage;

    REQUIRE_FALSE( stage.unknownDSTD.isPresent() );
    REQUIRE( stage.replacementModel.empty() );
    REQUIRE_FALSE( stage.unknownDMDT.isPresent() );
  }

  SECTION("reset")
  {
    DestructionStage stage;
    // Set some bogus values.
    stage.unknownDSTD.setPresence(true);
    stage.replacementModel = "foo/bar/replacement.nif";
    stage.unknownDMDT.setPresence(true);
    // Trigger reset.
    stage.reset();
    // Check that values are reset.
    REQUIRE_FALSE( stage.unknownDSTD.isPresent() );
    REQUIRE( stage.replacementModel.empty() );
    REQUIRE_FALSE( stage.unknownDMDT.isPresent() );
  }

  SECTION("isReset")
  {
    DestructionStage stage;

    SECTION("default-constructed")
    {
      REQUIRE( stage.isReset() );
    }

    SECTION("DSTD")
    {
      stage.unknownDSTD.setPresence(true);
      REQUIRE_FALSE( stage.isReset() );
      stage.reset();
      REQUIRE( stage.isReset() );
    }

    SECTION("model path")
    {
      stage.replacementModel = "foo/bar/replacement.nif";
      REQUIRE_FALSE( stage.isReset() );
      stage.reset();
      REQUIRE( stage.isReset() );
    }

    SECTION("DMDT")
    {
      stage.unknownDMDT.setPresence(true);
      REQUIRE_FALSE( stage.isReset() );
      stage.reset();
      REQUIRE( stage.isReset() );
    }
  }

  SECTION("equals")
  {
    DestructionStage a;
    DestructionStage b;

    SECTION("equal")
    {
      a.unknownDSTD.setPresence(true);
      a.replacementModel = "foo/bar/replacement.nif";
      b.unknownDSTD.setPresence(true);
      b.replacementModel = "foo/bar/replacement.nif";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("DSTD mismatch ")
      {
        a.unknownDSTD.setPresence(true);
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownDSTD.setPresence(false);
        b.unknownDSTD.setPresence(true);
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("replacement model mismatch")
      {
        a.replacementModel = "foo/bar/replacement.nif";
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.replacementModel = "foo/foo/replacement.nif";
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("DMDT mismatch ")
      {
        a.unknownDMDT.setPresence(true);
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownDMDT.setPresence(false);
        b.unknownDMDT.setPresence(true);
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("getWriteSize")
  {
    DestructionStage stage;

    SECTION("size adjusts with length of DSTD")
    {
      REQUIRE( stage.getWriteSize() == 6 );

      // Reading data from a stream is currently the only way to get a binary
      // sub record with data, so we do that here.
      {
        const std::string_view data = "DSTD\x04\0\x08\x5D\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDSTD.loadFromStream(stream, cDSTD, true) );
        REQUIRE( stage.getWriteSize() == 16 );
      }

      {
        const std::string_view data = "DSTD\x14\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDSTD.loadFromStream(stream, cDSTD, true) );
        REQUIRE( stage.getWriteSize() == 32 );
      }
    }

    SECTION("size adjusts with length of replacement model path")
    {
      stage.replacementModel = "foo/bar.nif"; // 11 characters
      REQUIRE( stage.getWriteSize() == 24 );

      stage.replacementModel = "foo/another/bar.nif"; // 19 characters
      REQUIRE( stage.getWriteSize() == 32 );
    }

    SECTION("size adjusts with length of DMDT")
    {
      REQUIRE( stage.getWriteSize() == 6 );

      // Reading data from a stream is currently the only way to get a binary
      // sub record with data, so we do that here.
      {
        const std::string_view data = "DMDT\x04\0\x08\x5D\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDMDT.loadFromStream(stream, cDMDT, true) );
        REQUIRE( stage.getWriteSize() == 16 );
      }

      {
        const std::string_view data = "DMDT\x14\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0\x08\x5D\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDMDT.loadFromStream(stream, cDMDT, true) );
        REQUIRE( stage.getWriteSize() == 32 );
      }
    }

    SECTION("empty stage has size six (for end markers)")
    {
      REQUIRE( stage.getWriteSize() == 6 );
    }
  }

  SECTION("saveToStream")
  {
    DestructionStage stage;

    SECTION("save record full stage")
    {
      // Reading data from a stream is currently the only way to get a binary
      // sub record with data, so we do that here.
      {
        const std::string_view data = "DSTD\x04\0\x08\x5D\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDSTD.loadFromStream(stream, cDSTD, true) );
      }
      stage.replacementModel = "foo\\bar.nif";
      {
        const std::string_view data = "DMDT\x14\0\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stage.unknownDMDT.loadFromStream(stream, cDMDT, true) );
      }

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( stage.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "DSTD\x04\0\x08\x5D\x0C\0DMDL\x0C\0foo\\bar.nif\0DMDT\x14\0\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0DSTF\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save empty stage")
    {
      std::ostringstream stream;
      // Writing should succeed.
      REQUIRE( stage.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "DSTF\0\0"sv );
    }
  }
}
