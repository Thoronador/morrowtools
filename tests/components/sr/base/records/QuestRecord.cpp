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
    REQUIRE_FALSE( record.unknownENAM.has_value() );
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
        a.unknownENAM = 0;
        b.unknownENAM.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownENAM.reset();
        b.unknownENAM = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownENAM = 1;
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

      record.unknownENAM = 1;
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
      REQUIRE_FALSE( record.unknownENAM.has_value() );
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

    SECTION("default: load record with INDX, QSDT, SCHR, SCTX, QNAM")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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
      REQUIRE( record.headerFormID == 0x00024151 );
      REQUIRE( record.headerRevision == 0x00445F02 );
      REQUIRE( record.headerVersion == 30 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "DarkSideContractDialogue" );
      REQUIRE( record.unknownVMAD.isPresent() );
      const auto VMAD = std::string_view(reinterpret_cast<const char*>(record.unknownVMAD.data()), record.unknownVMAD.size());
      REQUIRE( VMAD == "\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0"sv );
      REQUIRE_FALSE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::None );
      REQUIRE( record.unknownDNAM[0] == 0x11 );
      REQUIRE( record.unknownDNAM[1] == 0x01 );
      REQUIRE( record.unknownDNAM[2] == 0x46 );
      REQUIRE( record.unknownDNAM[3] == 0 );
      REQUIRE( record.unknownDNAM[4] == 0 );
      REQUIRE( record.unknownDNAM[5] == 0 );
      REQUIRE( record.unknownDNAM[6] == 0 );
      REQUIRE( record.unknownDNAM[7] == 0 );
      REQUIRE( record.unknownDNAM[8] == 0x04 );
      REQUIRE( record.unknownDNAM[9] == 0 );
      REQUIRE( record.unknownDNAM[10] == 0 );
      REQUIRE( record.unknownDNAM[11] == 0 );
      REQUIRE_FALSE( record.unknownENAM.has_value() );
      REQUIRE( record.unknownQTGLs.empty() );
      REQUIRE( record.unknownCTDA_CIS2s.empty() );
      REQUIRE( record.filter == "Faction\\Dark Brotherhood\\" );
      REQUIRE( record.indices.size() == 3 );

      const auto& idx_0 = record.indices[0];
      REQUIRE( idx_0.index == 0 );
      REQUIRE( idx_0.indexUnknownPart == 0x4100 );
      REQUIRE( idx_0.theQSDTs.size() == 1 );
      const auto& idx_0_qsdt_0 = idx_0.theQSDTs[0];
      REQUIRE_FALSE( idx_0_qsdt_0.isFinisher );
      REQUIRE( idx_0_qsdt_0.nextQuestFormID == 0 );
      REQUIRE( idx_0_qsdt_0.unknownSCHR.isPresent() );
      const auto SCHR_0_0 = std::string_view(reinterpret_cast<const char*>(idx_0_qsdt_0.unknownSCHR.data()), idx_0_qsdt_0.unknownSCHR.size());
      REQUIRE( SCHR_0_0 == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv );
      REQUIRE( idx_0_qsdt_0.unknownSCTX == ";Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogue"sv );
      REQUIRE( idx_0_qsdt_0.unknownQNAM.has_value() );
      REQUIRE( idx_0_qsdt_0.unknownQNAM.value() == 0x00024151 );
      REQUIRE( idx_0_qsdt_0.unknownCTDA_CIS2s.empty() );
      REQUIRE_FALSE( idx_0_qsdt_0.logEntry.isPresent() );

      const auto& idx_1 = record.indices[1];
      REQUIRE( idx_1.index == 1 );
      REQUIRE( idx_1.indexUnknownPart == 0x4102 );
      REQUIRE( idx_1.theQSDTs.size() == 1 );
      const auto& idx_1_qsdt_0 = idx_1.theQSDTs[0];
      REQUIRE_FALSE( idx_1_qsdt_0.isFinisher );
      REQUIRE( idx_1_qsdt_0.nextQuestFormID == 0 );
      REQUIRE( idx_1_qsdt_0.unknownSCHR.isPresent() );
      const auto SCHR_1_0 = std::string_view(reinterpret_cast<const char*>(idx_1_qsdt_0.unknownSCHR.data()), idx_1_qsdt_0.unknownSCHR.size());
      REQUIRE( SCHR_1_0 == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv );
      REQUIRE( idx_1_qsdt_0.unknownSCTX.empty() );
      REQUIRE( idx_1_qsdt_0.unknownQNAM.has_value() );
      REQUIRE( idx_1_qsdt_0.unknownQNAM.value() == 0x00024151 );
      REQUIRE( idx_1_qsdt_0.unknownCTDA_CIS2s.empty() );
      REQUIRE_FALSE( idx_1_qsdt_0.logEntry.isPresent() );

      const auto& idx_2 = record.indices[2];
      REQUIRE( idx_2.index == 0x00FF );
      REQUIRE( idx_2.indexUnknownPart == 0x6C04 );
      REQUIRE( idx_2.theQSDTs.size() == 1 );
      const auto& idx_2_qsdt_0 = idx_2.theQSDTs[0];
      REQUIRE_FALSE( idx_2_qsdt_0.isFinisher );
      REQUIRE( idx_2_qsdt_0.nextQuestFormID == 0 );
      REQUIRE( idx_2_qsdt_0.unknownSCHR.isPresent() );
      const auto SCHR_2_0 = std::string_view(reinterpret_cast<const char*>(idx_2_qsdt_0.unknownSCHR.data()), idx_2_qsdt_0.unknownSCHR.size());
      REQUIRE( SCHR_2_0 == "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0"sv );
      REQUIRE( idx_2_qsdt_0.unknownSCTX.empty() );
      REQUIRE( idx_2_qsdt_0.unknownQNAM.has_value() );
      REQUIRE( idx_2_qsdt_0.unknownQNAM.value() == 0x00024151 );
      REQUIRE( idx_2_qsdt_0.unknownCTDA_CIS2s.empty() );
      REQUIRE_FALSE( idx_2_qsdt_0.logEntry.isPresent() );


      REQUIRE( record.theQOBJs.empty() );
      REQUIRE( record.unknownANAM == 0x00000002 );
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

    SECTION("corrupt data: multiple FLTRs")
    {
      const auto data = "QUST\x06\x01\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\0\0ANAM\x04\0\0\0\0\0"sv;
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

    SECTION("corrupt data: length of NEXT is not zero")
    {
      const auto data = "QUST\xF9\0\0\0\0\0\0\0\xB3\x3E\x01\0\x1B\x69\x55\0\x28\0\x0F\0EDID\x10\0DialogueGeneric\0VMADJ\0\x05\0\x02\0\x01\0\x15\0DialogueGenericScript\0\x02\0\x04\0Gold\x01\x01\0\0\xFF\xFF\x0F\0\0\0\x0E\0RoomRentalCost\x01\x01\0\0\xFF\xFF\x98\xCC\x09\0FULL\x04\0\xD7\x0A\0\0DNAM\x0C\0\x11\0\0\0\0\0\0\0\0\0\0\0QTGL\x04\0\x98\xCC\x09\0FLTR\x09\0Generic\\\0CTDA\x20\0\0\0\0\0\0\0\0\0\xBC\x02TR\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFF\x43TDA\x20\0\0\0\0\0\0\0\x80?\xAA\x01TR\xA5\xB4\x03\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\xFFNEXT\x02\0\0\0ANAM\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of INDX is not four")
    {
      {
        const auto data = "QUST\xE3\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x03\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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
        const auto data = "QUST\xE5\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x05\0\0\0\0\x41\0QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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

    SECTION("corrupt data: stream ends before all of INDX can be read")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: QSDT without previous INDX subrecord")
    {
      const auto data = "QUST\xDA\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of QSDT is not one")
    {
      {
        const auto data = "QUST\xE3\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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
        const auto data = "QUST\xE5\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x02\0\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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

    SECTION("corrupt data: stream ends before all of QSDT can be read")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SCHRs")
    {
      const auto data = "QUST\xFE\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SCHR can be read")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SCTXs")
    {
      const auto data = "QUST\x46\x02\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueSCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SCTX > 512")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\x02\x02;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SCTX is beyond stream")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\xE4\x01;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of SCTX can be read")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple QNAMs")
    {
      const auto data = "QUST\xEE\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0QA\x02\0QNAM\x04\0QA\x02\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Read QUST, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      QuestRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of QNAM is not four")
    {
      {
        const auto data = "QUST\xE3\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x03\0QA\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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
        const auto data = "QUST\xE5\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x05\0QA\x02\0\0INDX\x04\0\x01\0\x02\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0INDX\x04\0\xFF\0\x04lQSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0QNAM\x04\0QA\x02\0ANAM\x04\0\x02\0\0\0"sv;
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

    SECTION("corrupt data: stream ends before all of QNAM can be read")
    {
      const auto data = "QUST\xE4\x01\0\0\0\0\0\0\x51\x41\x02\0\x02\x5F\x44\0\x1E\0\x02\0EDID\x19\0DarkSideContractDialogue\0VMAD|\0\x04\0\x01\0\x02\0$\0QF_DarkSideContractDialogue_00024151\0\0\0\x18\0DarkSideContractDialogue\0\x02\0\x14\0DarkBrotherhoodQuest\x01\x01\\\xEA\x01\0\xFF\xFF\0\0\x04\0DB02\x01\x01Q\xEA\x01\0\xFF\xFF\0\0DNAM\x0C\0\x11\x01\x46\0\0\0\0\0\x04\0\0\0FLTR\x1A\0Faction\\Dark Brotherhood\\\0NEXT\0\0INDX\x04\0\0\0\0\x41QSDT\x01\0\0SCHR\x14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x01\0SCTX\\\0;Set DarkBrotherhood.FirstSet to 1\r\n;SetStage DB02a 20\r\n;Startquest DarkSideContractDialogueQNAM\x04\0Q"sv;
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
