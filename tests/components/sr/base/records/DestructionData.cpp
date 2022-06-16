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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/DestructionData.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"

TEST_CASE("DestructionData")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    DestructionData data;

    REQUIRE_FALSE( data.isPresent );
    REQUIRE( data.health == 0 );
    REQUIRE( data.stageCount == 0 );
    REQUIRE( data.unknownTwo == 0 );
    REQUIRE( data.unknownThreeFour == 0 );
    REQUIRE( data.stages.empty() );
  }

  SECTION("clear")
  {
    DestructionData data;

    // Set some bogus values.
    data.isPresent = true;
    data.health = 5;
    data.stageCount = 1;
    data.unknownTwo = 2;
    data.unknownThreeFour = 34;
    // ... and add a stage.
    {
      DestructionStage stage;
      stage.unknownDSTD.setPresence(true);
      stage.replacementModel = "foo/bar/replacement.nif";
      stage.unknownDMDT.setPresence(true);
      data.stages.emplace_back(stage);
    }
    // Trigger reset.
    data.clear();
    // Check that values are reset.
    REQUIRE_FALSE( data.isPresent );
    REQUIRE( data.health == 0 );
    REQUIRE( data.stageCount == 0 );
    REQUIRE( data.unknownTwo == 0 );
    REQUIRE( data.unknownThreeFour == 0 );
    REQUIRE( data.stages.empty() );
  }

  SECTION("equality operator")
  {
    DestructionData a;
    DestructionData b;

    SECTION("equal")
    {
      SECTION("empty")
      {
        REQUIRE( a == b );
        REQUIRE( b == a );
      }

      SECTION("present")
      {
        a.isPresent = true;
        b.isPresent = true;

        REQUIRE( a == b );
        REQUIRE( b == a );
      }

      SECTION("present with data")
      {
        a.isPresent = true;
        a.health = 5;
        a.stageCount = 1;
        a.unknownTwo = 2;
        a.unknownThreeFour = 34;
          DestructionStage stage;
          stage.unknownDSTD.setPresence(true);
          stage.replacementModel = "foo/bar/replacement.nif";
          stage.unknownDMDT.setPresence(true);
        a.stages.push_back(stage);

        b.isPresent = true;
        b.health = 5;
        b.stageCount = 1;
        b.unknownTwo = 2;
        b.unknownThreeFour = 34;
        b.stages.push_back(stage);

        REQUIRE( a == b );
        REQUIRE( b == a );
      }
    }

    SECTION("unequal")
    {
      a.isPresent = true;
      b.isPresent = true;

      SECTION("isPresent mismatch ")
      {
        a.isPresent = true;
        b.isPresent = false;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.isPresent = false;
        b.isPresent = true;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("health mismatch")
      {
        a.health = 5;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.health = 7;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknownTwo mismatch ")
      {
        a.unknownTwo = 2;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.unknownTwo = 4;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("unknownThreeFour mismatch ")
      {
        a.unknownThreeFour = 34;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.unknownThreeFour = 56;
        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("stages mismatch ")
      {
        DestructionStage stage;
        stage.unknownDSTD.setPresence(true);
        stage.replacementModel = "foo/bar/replacement.nif";
        stage.unknownDMDT.setPresence(true);

        a.stages.push_back(stage);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        b.stages.push_back(stage);
        b.stages.push_back(stage);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("getWriteSize")
  {
    DestructionData data;

    SECTION("non-present data has size zero")
    {
      data.isPresent = false;
      REQUIRE( data.getWriteSize() == 0 );
    }

    SECTION("data without stages")
    {
      // Unlikely to occur in real data, but test it anyway.
      data.isPresent = true;
      REQUIRE( data.getWriteSize() == 14 );
    }

    SECTION("size adjusts with length of stages")
    {
      data.isPresent = true;

      DestructionStage stage;
      stage.unknownDSTD.setPresence(true);
      stage.replacementModel = "foo/bar/replacement.nif";
      stage.unknownDMDT.setPresence(true);

      data.stages.push_back(stage);

      REQUIRE( data.getWriteSize() == 62 );

      stage.unknownDSTD.setPresence(false);
      stage.unknownDMDT.setPresence(false);
      stage.replacementModel = "foo/bar.nif";

      data.stages.push_back(stage);

      REQUIRE( data.getWriteSize() == 86 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    char buffer[512];
    uint32_t bytesRead = 0;

    SECTION("data without stages")
    {
      // unlikely to occur in real data, but do it anyway
      const std::string_view data = "DEST\x08\0\xEF\xCD\xAB\0\0\xFF\xFE\xAF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read DEST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DestructionData destData;
      REQUIRE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
      // Check data.
      REQUIRE( destData.health == 0xABCDEF );
      REQUIRE( destData.stageCount == 0 );
      REQUIRE( destData.unknownTwo == 255 );
      REQUIRE( destData.unknownThreeFour == 0xAFFE );
      REQUIRE( destData.stages.empty() );
    }

    SECTION("data with full single stage")
    {
      const std::string_view data = "DEST\x08\0\x56\x34\x12\0\x01\xCC\x0B\0DSTD\x04\0\x08\x5D\x0C\0DMDL\x0C\0foo\\bar.nif\0DMDT\x14\0\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0DSTF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read DEST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DestructionData destData;
      REQUIRE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
      // Check data.
      REQUIRE( destData.health == 0x123456 );
      REQUIRE( destData.stageCount == 1 );
      REQUIRE( destData.unknownTwo == 0xCC );
      REQUIRE( destData.unknownThreeFour == 0x000B );
      REQUIRE( destData.stages.size() == 1 );
      const auto& dstd = destData.stages[0].unknownDSTD;
      REQUIRE( dstd.isPresent() );
      REQUIRE( std::string_view(reinterpret_cast<const char*>(dstd.data()), dstd.size()) == "\x08\x5D\x0C\0"sv );
      REQUIRE( destData.stages[0].replacementModel == "foo\\bar.nif" );
      const auto& dmdt = destData.stages[0].unknownDMDT;
      REQUIRE( dmdt.isPresent() );
      REQUIRE( std::string_view(reinterpret_cast<const char*>(dmdt.data()), dmdt.size()) == "\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( destData.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("data with multiple stages")
    {
      const std::string_view data = "DEST\x08\0\x56\x34\x12\0\x02\xDD\x0C\0DSTD\x04\0\x08\x5D\x0C\0DMDL\x0C\0foo\\bar.nif\0DSTF\0\0DMDL\x0C\0bar\\foo.nif\0DMDT\x14\0\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0DSTF\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read DEST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DestructionData destData;
      REQUIRE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
      // Check data.
      REQUIRE( destData.health == 0x123456 );
      REQUIRE( destData.stageCount == 2 );
      REQUIRE( destData.unknownTwo == 0xDD );
      REQUIRE( destData.unknownThreeFour == 0x000C );
      REQUIRE( destData.stages.size() == 2 );

      const auto& dstd = destData.stages[0].unknownDSTD;
      REQUIRE( dstd.isPresent() );
      REQUIRE( std::string_view(reinterpret_cast<const char*>(dstd.data()), dstd.size()) == "\x08\x5D\x0C\0"sv );
      REQUIRE( destData.stages[0].replacementModel == "foo\\bar.nif" );
      REQUIRE_FALSE( destData.stages[0].unknownDMDT.isPresent() );

      REQUIRE_FALSE( destData.stages[1].unknownDSTD.isPresent() );
      REQUIRE( destData.stages[1].replacementModel == "bar\\foo.nif" );
      const auto& dmdt = destData.stages[1].unknownDMDT;
      REQUIRE( dmdt.isPresent() );
      REQUIRE( std::string_view(reinterpret_cast<const char*>(dmdt.data()), dmdt.size()) == "\x08\x5D\x0C\0\x07\x65\x43\0\x21\x09\x08\0\x07\x65\x0C\0\x43\x21\x0C\0"sv );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( destData.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of DEST is not eight")
    {
      {
        const std::string_view data = "DEST\x07\0\xEF\xCD\xAB\0\0\xFF\xFE\xAF"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read DEST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DestructionData destData;
        REQUIRE_FALSE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
      }

      {
        const std::string_view data = "DEST\x09\0\xEF\xCD\xAB\0\0\xFF\xFE\xAF\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read DEST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DestructionData destData;
        REQUIRE_FALSE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
      }
    }

    SECTION("corrupt data: stream ends before DEST can be read completely")
    {
      const std::string_view data = "DEST\x08\0\xEF\xCD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read DEST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DestructionData destData;
      REQUIRE_FALSE( destData.loadFromStream(stream, cFURN, buffer, bytesRead) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save absent destruction data")
    {
      DestructionData destData;
      // Fill data.
      destData.isPresent = false;
      destData.health = 0xABCDEF;
      destData.stageCount = 0;
      destData.unknownTwo = 255;
      destData.unknownThreeFour = 0xAFFE;
      destData.stages.clear();

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( destData.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str().empty() );
    }

    SECTION("save data without stages")
    {
      DestructionData destData;
      // Fill data.
      destData.isPresent = true;
      destData.health = 0xABCDEF;
      destData.stageCount = 0;
      destData.unknownTwo = 255;
      destData.unknownThreeFour = 0xAFFE;
      destData.stages.clear();

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( destData.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "DEST\x08\0\xEF\xCD\xAB\0\0\xFF\xFE\xAF"sv;
      REQUIRE( stream.str() == data );
    }

    // Cases with stages are already saved in loadToStream tests.
  }
}
