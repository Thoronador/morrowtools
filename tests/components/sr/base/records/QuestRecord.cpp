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
#include <string_view>
#include <sstream>
#include "../../../../../sr/base/records/QuestRecord.hpp"

TEST_CASE("QuestRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    QuestRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    REQUIRE_FALSE( record.name.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownDNAM[i] == 0 );
    }
    REQUIRE_FALSE( record.hasENAM );
    REQUIRE( record.unknownENAM == 0 );
    REQUIRE( record.unknownQTGLs.empty() );
    REQUIRE( record.unknownCTDA_CIS2s.empty() );
    REQUIRE( record.filter.empty() );
    REQUIRE( record.indices.empty() );
    REQUIRE( record.theQOBJs.empty() );
    REQUIRE( record.unknownANAM == 0 );
    REQUIRE( record.aliases.empty() );
  }

  SECTION("equals")
  {
    QuestRecord a;
    QuestRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("VMAD mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownVMAD.setPresence(false);
        b.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM[11] = 12;
        b.unknownDNAM[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ENAM mismatch")
      {
        a.hasENAM = true;
        b.hasENAM = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasENAM = false;
        b.hasENAM = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasENAM = true;
        a.unknownENAM = 1;
        b.hasENAM = true;
        b.unknownENAM = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("QTGLs mismatch")
      {
        a.unknownQTGLs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownQTGLs.push_back(0x01234567);
        b.unknownQTGLs.push_back(0x09ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("filter mismatch")
      {
        a.filter = "foo";
        b.filter = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("indices mismatch")
      {
        a.indices.push_back(IndexEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("QOBJs mismatch")
      {
        a.theQOBJs.push_back(QOBJEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ANAM mismatch")
      {
        a.unknownANAM = 0x01234567;
        b.unknownANAM = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("aliases mismatch")
      {
        a.aliases.push_back(AliasEntry());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    QuestRecord record;

    REQUIRE( record.getRecordType() == cQUST );
  }

  SECTION("getWriteSize")
  {
    QuestRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 44 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 44 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 68 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 51 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 54 );
    }

    SECTION("size adjusts when ENAM is present")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.hasENAM = true;
      REQUIRE( record.getWriteSize() == 54 );
    }

    SECTION("size adjusts with length of QTGLs")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.unknownQTGLs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 54 );

      record.unknownQTGLs.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 64 );
    }

    SECTION("size adjusts with length of filter")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.filter = "foo";
      REQUIRE( record.getWriteSize() == 54 );

      record.filter = "foobarbaz";
      REQUIRE( record.getWriteSize() == 60 );
    }

    SECTION("size adjusts with CTDA/CIS2")
    {
      REQUIRE( record.getWriteSize() == 44 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 82 );

      record.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( record.getWriteSize() == 120 );
    }

    // TODO: Add tests for indices, aliases and QOBJs as soon as they are
    //       calculated properly in getWriteSize().
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00000AD7, "foo");

    SECTION("default: load record with VMAD, FULL, FLTR")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      QuestRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00013EB3 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000F );
      // -- record data
      REQUIRE( record.editorID == "DialogueGeneric" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0"sv );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00000AD7 );
      // \x11\0\0\0\0\0\0\0\0\0\0\0
      REQUIRE( record.unknownDNAM[0] == 0x11 );
      REQUIRE( record.unknownDNAM[1] == 0 );
      REQUIRE( record.unknownDNAM[2] == 0 );
      REQUIRE( record.unknownDNAM[3] == 0 );
      REQUIRE( record.unknownDNAM[4] == 0 );
      REQUIRE( record.unknownDNAM[5] == 0 );
      REQUIRE( record.unknownDNAM[6] == 0 );
      REQUIRE( record.unknownDNAM[7] == 0 );
      REQUIRE( record.unknownDNAM[8] == 0 );
      REQUIRE( record.unknownDNAM[9] == 0 );
      REQUIRE( record.unknownDNAM[10] == 0 );
      REQUIRE( record.unknownDNAM[11] == 0 );
      REQUIRE_FALSE( record.hasENAM );
      REQUIRE( record.unknownENAM == 0 );
      REQUIRE( record.unknownQTGLs.size() == 1 );
      REQUIRE( record.unknownQTGLs[0] == 0x0009CC98 );
      REQUIRE( record.unknownCTDA_CIS2s.size() == 2 );
      const auto& CTDA_0 = record.unknownCTDA_CIS2s[0].unknownCTDA.content;
      REQUIRE( std::string_view(reinterpret_cast<const char*>(CTDA_0.data()), CTDA_0.size()) == "\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF"sv );
      REQUIRE( record.unknownCTDA_CIS2s[0].unknownCISx.empty() );
      const auto& CTDA_1 = record.unknownCTDA_CIS2s[1].unknownCTDA.content;
      REQUIRE( std::string_view(reinterpret_cast<const char*>(CTDA_1.data()), CTDA_1.size()) == "\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF"sv );
      REQUIRE( record.unknownCTDA_CIS2s[1].unknownCISx.empty() );
      REQUIRE( record.filter == "Generic\\" );
      REQUIRE( record.indices.empty() );
      REQUIRE( record.theQOBJs.empty() );
      REQUIRE( record.unknownANAM == 0 );
      REQUIRE( record.aliases.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0FAIL\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\x02GialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\xF7\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple VMADs")
    {
      const auto data = "QUST\x47\x01\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "QUST\x01\x01\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DNAM")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0FAIL\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DNAM")
    {
      const auto data = "QUST\xE5\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "QUST\x09\x01\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not 12")
    {
      {
        const auto data = "QUST\xF6\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0B\0\x11\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Read QUST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        QuestRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "QUST\xF8\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0D\0\x11\0\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Read QUST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        QuestRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DNAM can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of QTGL can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98"sv;
      // const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of CTDA can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FLTR > 512")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\x02Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of FLTR is beyond stream")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\xF7\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FLTR can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Gen"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ANAM")
    {
      const auto data = "QUST\xED\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ANAMs")
    {
      const auto data = "QUST\x01\x01\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ANAM is not four")
    {
      {
        const auto data = "QUST\xF6\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x03\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Read QUST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        QuestRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "QUST\xF8\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x05\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Read QUST, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        QuestRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ANAM can be read")
    {
      const auto data = "QUST\xF7\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
