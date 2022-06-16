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

#include "../../../../locate_catch.hpp"
#include "../../../../../../sr/base/records/quest/QOBJEntry.hpp"

TEST_CASE("QOBJEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    QOBJEntry entry;

    REQUIRE( entry.unknownQOBJ == 0 );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.displayText.isPresent() );
    REQUIRE( entry.theQSTAs.empty() );
  }

  SECTION("equality")
  {
    QOBJEntry a;
    QOBJEntry b;

    SECTION("equal")
    {
      a.unknownQOBJ = 1;
      b.unknownQOBJ = 1;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("QOBJ mismatch")
      {
        a.unknownQOBJ = 1;
        b.unknownQOBJ = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FNAM mismatch")
      {
        a.unknownFNAM = 1;
        b.unknownFNAM = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("displayText mismatch")
      {
        a.displayText = LocalizedString(LocalizedString::Type::Index, 1, "");

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("QSTAs mismatch")
      {
        a.theQSTAs.push_back(QSTAEntry());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    QOBJEntry entry;

    entry.unknownQOBJ = 0x1234;
    entry.unknownFNAM = 0x12345678;
    entry.displayText = LocalizedString(LocalizedString::Type::Index, 1, "");
    entry.theQSTAs.push_back(QSTAEntry());
    entry.theQSTAs.push_back(QSTAEntry());

    entry.clear();

    REQUIRE( entry.unknownQOBJ == 0 );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.displayText.isPresent() );
    REQUIRE( entry.theQSTAs.empty() );
  }

  SECTION("getWriteSize")
  {
    QOBJEntry entry;

    SECTION("size without any extra data")
    {
      REQUIRE( entry.getWriteSize() == 18 );
    }

    SECTION("size adjusts when FNAM is present")
    {
      REQUIRE( entry.getWriteSize() == 18 );

      entry.displayText = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( entry.getWriteSize() == 28 );
    }

    SECTION("size adjusts with length of QSTA entries")
    {
      REQUIRE( entry.getWriteSize() == 18 );

      entry.theQSTAs.push_back(QSTAEntry());
      REQUIRE( entry.getWriteSize() == 32 );

      entry.theQSTAs.push_back(QSTAEntry());
      REQUIRE( entry.getWriteSize() == 46 );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: no extra data")
    {
      QOBJEntry entry;
      entry.unknownQOBJ = 0xABCD;
      entry.unknownFNAM = 0xAABCDEF9;

      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QOBJ\x02\0\xCD\xAB\x46NAM\x04\0\xF9\xDE\xBC\xAA"sv );
    }

    SECTION("default: entry with display text")
    {
      QOBJEntry entry;
      entry.unknownQOBJ = 0x00CD;
      entry.unknownFNAM = 0xAABCDEF9;
      entry.displayText = LocalizedString(LocalizedString::Type::Index, 0x87654321, "");

      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QOBJ\x02\0\xCD\0FNAM\x04\0\xF9\xDE\xBC\xAANNAM\x04\0\x21\x43\x65\x87"sv );
    }

    SECTION("default: entry with two QTSAs")
    {
      QOBJEntry entry;
      entry.unknownQOBJ = 0x00CD;
      entry.unknownFNAM = 0xAABCDEF9;

      QSTAEntry qsta;
      qsta.unknownQSTA = 0x0123456789ABCDEF;
      entry.theQSTAs.push_back(qsta);
      qsta.unknownQSTA = 0x001122334455CDEF;
      entry.theQSTAs.push_back(qsta);

      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "QOBJ\x02\0\xCD\0FNAM\x04\0\xF9\xDE\xBC\xAAQSTA\x08\0\xEF\xCD\xAB\x89\x67\x45\x23\x01QSTA\x08\0\xEF\xCD\x55\x44\x33\x22\x11\x00"sv );
    }
  }
}
