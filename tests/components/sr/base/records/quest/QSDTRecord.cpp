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

#include <catch.hpp>
#include "../../../../../../sr/base/records/quest/QSDTRecord.hpp"
#include <sstream>
#include <string_view>

TEST_CASE("QSDTRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QSDTRecord entry;

    REQUIRE_FALSE( entry.isFinisher );
    REQUIRE( entry.nextQuestFormID == 0 );
    REQUIRE_FALSE( entry.unknownSCHR.isPresent() );
    REQUIRE( entry.unknownSCTX.empty() );
    REQUIRE_FALSE( entry.unknownQNAM.has_value() );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE_FALSE( entry.logEntry.isPresent() );
  }

  SECTION("equality")
  {
    QSDTRecord a;
    QSDTRecord b;

    SECTION("equal")
    {
      a.nextQuestFormID = 1;
      b.nextQuestFormID = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("isFinisher mismatch")
      {
        a.isFinisher = true;
        b.isFinisher = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("nextQuestFormID mismatch")
      {
        a.nextQuestFormID = 0x12345678;
        b.nextQuestFormID = 0x9ABCDEF0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SCHR mismatch")
      {
        a.unknownSCHR.setPresence(true);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownSCHR.setPresence(false);
        b.unknownSCHR.setPresence(true);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SCTX mismatch")
      {
        a.unknownSCTX = "foo";
        b.unknownSCTX = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("QNAM mismatch")
      {
        a.unknownQNAM = 0;
        b.unknownQNAM.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownQNAM.reset();
        b.unknownQNAM = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownQNAM = 1;
        b.unknownQNAM = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("logEntry mismatch")
      {
        a.logEntry = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("getWriteSize")
  {
    QSDTRecord record;

    SECTION("size adjusts when nextQuestFormID is present")
    {
      REQUIRE( record.getWriteSize() == 7 );

      record.nextQuestFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 17 );
    }

    SECTION("size adjusts with length of SCHR")
    {
      using namespace std::string_view_literals;

      REQUIRE( record.getWriteSize() == 7 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "SCHR\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownSCHR.loadFromStream(stream, cSCHR, true) );
      }
      REQUIRE( record.getWriteSize() == 31 );

      {
        const std::string_view data = "SCHR\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownSCHR.loadFromStream(stream, cSCHR, true) );
      }
      REQUIRE( record.getWriteSize() == 14 );
    }

    SECTION("size adjusts with length of SCTX")
    {
      REQUIRE( record.getWriteSize() == 7 );

      record.unknownSCTX = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 16 );

      record.unknownSCTX = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 28 );
    }

    SECTION("size adjusts when QNAM is present")
    {
      REQUIRE( record.getWriteSize() == 7 );

      record.unknownQNAM = 0;
      REQUIRE( record.getWriteSize() == 17 );
    }

    SECTION("size adjusts with length of CTDA/CIS2")
    {
      REQUIRE( record.getWriteSize() == 7 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 45 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 83 );
    }

    SECTION("size adjusts when logEntry is present")
    {
      REQUIRE( record.getWriteSize() == 7 );

      record.logEntry = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 17 );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    QSDTRecord record;
    record.isFinisher = false;

    SECTION("record with nextQuestFormID")
    {
      record.nextQuestFormID = 0x00112233;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QSDT\x01\0\0NAM0\x04\0\x33\x22\x11\0"sv );
    }

    SECTION("record with CTDA/CIS2")
    {
      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      record.unknownCTDA_CIS2s.back().unknownCISx = "abc";

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QSDT\x01\0\0CTDA\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CIS2\x04\0abc\0"sv );
    }
  }
}
