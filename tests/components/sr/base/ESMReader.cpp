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
#include <filesystem>
#include <string_view>
#include "../../../../sr/base/ESMReader.hpp"
#include "../../../../sr/base/SR_Constants.hpp"
#include "TestFactionsReader.hpp"

TEST_CASE("ESMReader")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("peekESMHeader")
  {
    StringTable dummy_table;
    TestFactionsReader reader;
    Tes4HeaderRecord record;

    SECTION("default: read an ESM header")
    {
      // write "ESM file"
      {
        const std::string_view data = "TES4\xF6\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x2B\0\0\0HEDR\x0C\0\x9A\x99\xD9\x3F\x69\x05\0\0\xDF\x09\x00\xFF\x43NAM\x09\0bnesmith\0MAST\x0B\0Skyrim.esm\0DATA\x08\0\0\0\0\0\0\0\0\0ONAM\x9C\0\xFE\x48\x07\x00\xA4\xFE\x10\x00\x9B\xF4\x0D\x00\x5E\x6D\x0A\x00\x04\x8A\x0E\x00\xA0\xFE\x10\x00\x71\x6D\x0A\x00\x65\xBA\x09\x00\xA7\xFE\x10\x00\x46\xE9\x0F\x00\x27\x4D\x10\x00\x60\x6D\x0A\x00\xAC\x0E\x0C\x00\x72\x6D\x0A\x00\x74\xBA\x09\x00\x50\xE9\x0F\x00\x60\xBD\x02\x00\xA3\xFE\x10\x00\xD9\x0E\x0C\x00\xA6\xFE\x10\x00\xE9\x6C\x10\x00\x79\x19\x08\x00\x61\x6D\x0A\x00\xA2\xFE\x10\x00\x73\x6D\x0A\x00\xEF\x89\x0E\x00\x48\xE9\x0F\x00\x6C\xBA\x09\x00\x2D\x4F\x0C\x00\xD2\x2E\x10\x00\xEA\x6C\x10\x00\x2F\xEF\x09\x00\xA5\xFE\x10\x00\x4B\xE9\x0F\x00\x5F\x6D\x0A\x00\xEC\x89\x0E\x00\x70\x6D\x0A\x00\xA1\xFE\x10\x00\xD2\x9C\x10\0INTV\x04\0\x21\0\0\0INCC\x04\0\x17\0\0\0"sv;
        std::ofstream file("FullHeaderOnly.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE( ESMReader::peekESMHeader("FullHeaderOnly.esm", record) );

      REQUIRE( std::filesystem::remove("FullHeaderOnly.esm") );

      // Check record data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000081 );
      REQUIRE( record.headerFormID == 0 );
      REQUIRE( record.headerRevision == 0 );
      REQUIRE( record.headerVersion == 43 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.version == 1.7f );
      REQUIRE( record.numRecordsAndGroups == 1385 );
      REQUIRE( record.nextObjectID == 0xFF0009DF );
      REQUIRE( record.authorName == "bnesmith" );
      REQUIRE( record.dependencies.size() == 1 );
      REQUIRE( record.dependencies[0].fileName == "Skyrim.esm" );
      REQUIRE( record.dependencies[0].data == 0 );
      REQUIRE( record.unknownONAM.size() == 39 );
      REQUIRE( record.unknownONAM[0] == 0x000748FE );
      REQUIRE( record.unknownONAM[1] == 0x0010FEA4 );
      REQUIRE( record.unknownONAM[2] == 0x000DF49B );
      REQUIRE( record.unknownONAM[3] == 0x000A6D5E );
      // Note: There are more, but not all are checked here, only the last.
      REQUIRE( record.unknownONAM[38] == 0x00109CD2 );
      REQUIRE( record.unknownIntValue == 33 );
      REQUIRE( record.unknownINCC.has_value() );
      REQUIRE( record.unknownINCC.value() == 23 );
    }

    SECTION("failure: file does not exist")
    {
      REQUIRE_FALSE( ESMReader::peekESMHeader("does-not-exist.esm", record) );
    }

    SECTION("failure: file does not start with TES4 header")
    {
      // write "ESM" file
      {
        std::ofstream file("not-valid-tes4-header.esm");
        file.write("FAIL1234", 8);
        file.close();
      }

      REQUIRE_FALSE( ESMReader::peekESMHeader("not-valid-tes4-header.esm", record) );

      REQUIRE( std::filesystem::remove("not-valid-tes4-header.esm") );
    }

    SECTION("failure: file ends before full header data can be read")
    {
      // write "ESM" file
      {
        const std::string_view data = "TES4\xF6\0\0\0\x81"sv;
        std::ofstream file("incompleteHeader.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE_FALSE( ESMReader::peekESMHeader("incompleteHeader.esm", record) );

      REQUIRE( std::filesystem::remove("incompleteHeader.esm") );
    }
  }

  SECTION("readESM")
  {
    StringTable dummy_table;
    TestFactionsReader reader;
    Tes4HeaderRecord header;

    SECTION("default: read ESM file without string tables (i. e. non-localized)")
    {
      // write "ESM file"
      {
        const std::string_view data = "TES4\x2C\0\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x02\0\0\0\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::ofstream file("readESM-sample.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      // Clear persistent data in factions singleton.
      Factions::get().clear();

      Tes4HeaderRecord record;
      REQUIRE( reader.readESM("readESM-sample.esm", record) > 0 );
      REQUIRE( std::filesystem::remove("readESM-sample.esm") );

      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000001 );
      REQUIRE( record.headerFormID == 0 );
      REQUIRE( record.headerRevision == 0 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.version == 0.94f );
      REQUIRE( record.numRecordsAndGroups == 2 );
      REQUIRE( record.nextObjectID == 0x00000F92 );
      REQUIRE( record.authorName == "mcarofano" );
      REQUIRE( record.dependencies.empty() );
      REQUIRE( record.unknownONAM.empty() );
      REQUIRE( record.unknownIntValue == 0x000126C5 );
      REQUIRE_FALSE( record.unknownINCC.has_value() );

      REQUIRE( Factions::get().getNumberOfRecords() == 1 );
    }

    SECTION("failure: file does not exist")
    {
      REQUIRE( reader.readESM("does-not-exist.esm", header) == -1 );
    }

    SECTION("failure: file does not start with TES4 header")
    {
      // write "ESM" file
      {
        std::ofstream file("readESM-not-valid-tes4-header.esm");
        file.write("FAIL1234", 8);
        file.close();
      }

      REQUIRE( reader.readESM("readESM-not-valid-tes4-header.esm", header) == -1 );

      REQUIRE( std::filesystem::remove("readESM-not-valid-tes4-header.esm") );
    }

    SECTION("failure: file ends before full header data can be read")
    {
      // write "ESM" file
      {
        const std::string_view data = "TES4\xF6\0\0\0\x81"sv;
        std::ofstream file("readESM-incompleteHeader.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE( reader.readESM("readESM-incompleteHeader.esm", header) == -1 );

      REQUIRE( std::filesystem::remove("readESM-incompleteHeader.esm") );
    }
  }

  SECTION("processGroup")
  {
    StringTable dummy_table;
    TestFactionsReader reader;

    SECTION("default: process a whole group")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, true, true, dummy_table) == 1 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("default: process a whole group, but without header")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, false, true, dummy_table) == 1 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("default: group is not needed and therefore skipped")
    {
      const auto data = "GRUP\x81\0\0\0ABCD\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, true, true, dummy_table) == 0 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("corrupt stream: stream ends before group header (GRUP) can be read")
    {
      const auto data = "GRU"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, true, true, dummy_table) == -1 );
    }

    SECTION("corrupt stream: group header is not GRUP")
    {
      const auto data = "FAIL\0\0\0\0ABCD\0\0\0\0\x16\x6E\x32\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, true, true, dummy_table) == -1 );
    }

    SECTION("corrupt stream: stream ends before all group header data can be read")
    {
      const auto data = "GRUP\x81\0\0\0FACT"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processGroup(stream, true, true, dummy_table) == -1 );
    }

    SECTION("faulty reader: nextGroupStarted fails")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      TestFactionsReaderGroupStartFails failReader;

      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( failReader.processGroup(stream, true, true, dummy_table) == -1 );
    }

    SECTION("faulty reader: groupFinished fails")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      TestFactionsReaderGroupFinishFails failReader;

      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( failReader.processGroup(stream, true, true, dummy_table) == -1 );
    }
  }

  SECTION("readGroup")
  {
    StringTable dummy_table;
    TestFactionsReader reader;

    GroupData group;
    group.setSize(129);
    group.setLabel(cFACT);
    group.setType(0);

    SECTION("default: read a whole group")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.readGroup(stream, group, true, dummy_table) == 1 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("default: read group with sub-group")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      GroupData groupWithSub;
      groupWithSub.setSize(153);
      groupWithSub.setLabel(cFACT);
      groupWithSub.setType(0);

      REQUIRE( Factions::get().getNumberOfRecords() == 0 );
      REQUIRE( reader.readGroup(stream, groupWithSub, true, dummy_table) == 1 );
      REQUIRE( Factions::get().getNumberOfRecords() == 1 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("default: read group with sub-group that just gets skipped")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "GRUP\x81\0\0\0ABCD\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      GroupData groupWithSub;
      groupWithSub.setSize(153);
      groupWithSub.setLabel(cFACT);
      groupWithSub.setType(0);

      REQUIRE( Factions::get().getNumberOfRecords() == 0 );
      REQUIRE( reader.readGroup(stream, groupWithSub, true, dummy_table) == 0 );
      REQUIRE( Factions::get().getNumberOfRecords() == 0 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("fail: stream ends before all of the group data can be read")
    {
      // Clear persistent data in factions singleton.
      Factions::get().clear();

      const auto data = "FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.readGroup(stream, group, true, dummy_table) == -1 );
    }
  }

  SECTION("skipGroup")
  {
    SECTION("default: skip a group")
    {
      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      uint32_t dummy;
      // read GRUP, because header is handled before everything else.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      GroupData group;
      REQUIRE( group.loadFromStream(stream) );

      REQUIRE( ESMReader::skipGroup(stream, group) == 0 );

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("corrupt data: group size is too small")
    {
      const auto data = "GRUP\x11\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      uint32_t dummy;
      // read GRUP, because header is handled before everything else.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      GroupData group;
      REQUIRE( group.loadFromStream(stream) );

      REQUIRE( ESMReader::skipGroup(stream, group) == -1 );

      // Check that the stream position did not change by reading the
      // next bytes, which should be "FACT".
      dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cFACT );
    }

    SECTION("corrupt stream: stream end before full group can be skipped")
    {
      const auto data = "GRUP\x81\0\0\0FACT\0\0\0\0\x16\x6E\x32\0\0\0\0\0FACT\x51"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      uint32_t dummy;
      // read GRUP, because header is handled before everything else.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      GroupData group;
      REQUIRE( group.loadFromStream(stream) );

      REQUIRE( ESMReader::skipGroup(stream, group) == -1 );
    }
  }

  SECTION("skipRecord")
  {
    SECTION("default: skip a record")
    {
      const auto data = "FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08ExclusionFaction\0DATA\x04\0\0\0\0\0CRVA\x14\0\x01\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0VENV\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0XXXX"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      uint32_t dummy;
      // read FACT, because header is handled before skipRecord.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      REQUIRE( ESMReader::skipRecord(stream) == 0);

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "XXXX".
      dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );
      REQUIRE( dummy == cXXXX );
    }

    SECTION("corrupt stream: stream end before full record can be skipped")
    {
      const auto data = "FACT\x51\0\0\0\0\0\0\0\x44\x09\0\x01\x16\x6E\x32\0\x28\0\x01\0EDID\x15\0CR08Ex"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      uint32_t dummy;
      // read FACT, because header is handled before skipRecord.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      REQUIRE( ESMReader::skipRecord(stream) == -1 );
    }
  }
}
