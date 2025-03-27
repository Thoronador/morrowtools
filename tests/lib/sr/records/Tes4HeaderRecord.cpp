/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2025  Dirk Stolle

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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/sr/records/TES4HeaderRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("Tes4HeaderRecord")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    Tes4HeaderRecord record;

    REQUIRE( record.version == 0.0f );
    REQUIRE( record.numRecordsAndGroups == 0 );
    REQUIRE( record.nextObjectID == 0 );
    REQUIRE( record.authorName.empty() );
    REQUIRE( record.summary.empty() );
    REQUIRE( record.dependencies.empty() );
    REQUIRE( record.unknownONAM.empty() );
    REQUIRE( record.unknownIntValue == 0 );
    REQUIRE_FALSE( record.unknownINCC.has_value() );
  }

  SECTION("equals")
  {
    Tes4HeaderRecord a;
    Tes4HeaderRecord b;

    SECTION("equal")
    {
      a.version = 0.94f;
      a.numRecordsAndGroups = 920184;
      a.nextObjectID = 3986;
      a.authorName = "mcarofano";
      a.summary = "foo bar";
      a.dependencies = {};
      a.unknownONAM = { 1, 2, 3, 4 };
      a.unknownINCC = 0xDEADBEEF;

      b.version = 0.94f;
      b.numRecordsAndGroups = 920184;
      b.nextObjectID = 3986;
      b.authorName = "mcarofano";
      b.summary = "foo bar";
      b.dependencies = {};
      b.unknownONAM = { 1, 2, 3, 4 };
      b.unknownINCC = 0xDEADBEEF;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("version mismatch")
      {
        a.version = 1.7f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("record number mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920185;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("next object ID mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 4444;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("author name mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "bnesmith";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("summary mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar baz";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("dependencies mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        auto master = Tes4HeaderRecord::MasterFile();
        master.fileName = "Skyrim.esm";
        master.data = 0;
        b.dependencies.push_back(master);
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ONAM mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 42 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xDEADBEEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("INCC mismatch")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = 0xAFFEAFFE;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("INCC mismatch: present and not present")
      {
        a.version = 0.94f;
        a.numRecordsAndGroups = 920184;
        a.nextObjectID = 3986;
        a.authorName = "mcarofano";
        a.summary = "foo bar";
        a.dependencies = {};
        a.unknownONAM = { 1, 2, 3, 4 };
        a.unknownINCC = 0xDEADBEEF;

        b.version = 0.94f;
        b.numRecordsAndGroups = 920184;
        b.nextObjectID = 3986;
        b.authorName = "mcarofano";
        b.summary = "foo bar";
        b.dependencies = {};
        b.unknownONAM = { 1, 2, 3, 4 };
        b.unknownINCC = {};

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    Tes4HeaderRecord record;

    REQUIRE( record.getRecordType() == cTES4 );
  }

  SECTION("isMasterFile")
  {
    Tes4HeaderRecord record;

    REQUIRE_FALSE( record.isMasterFile() );

    record.headerFlags |= Tes4HeaderRecord::cMasterFlag;
    REQUIRE( record.isMasterFile() );

    record.headerFlags = 0x22 | Tes4HeaderRecord::cMasterFlag;
    REQUIRE( record.isMasterFile() );

    record.headerFlags = 0xFFFFFFFF & !Tes4HeaderRecord::cMasterFlag;
    REQUIRE_FALSE( record.isMasterFile() );
  }

  SECTION("isLocalized")
  {
    Tes4HeaderRecord record;

    REQUIRE_FALSE( record.isLocalized() );

    record.headerFlags |= Tes4HeaderRecord::cLocalizedFlag;
    REQUIRE( record.isLocalized() );

    record.headerFlags = 0x22 | Tes4HeaderRecord::cLocalizedFlag;
    REQUIRE( record.isLocalized() );

    record.headerFlags = 0xFFFFFFFF & !Tes4HeaderRecord::cLocalizedFlag;
    REQUIRE_FALSE( record.isLocalized() );
  }

  SECTION("getWriteSize")
  {
    Tes4HeaderRecord record;

    SECTION("near minimal data")
    {
      record.authorName = "foo bar";
      REQUIRE( record.getWriteSize() == 42 );
    }

    SECTION("size adjusts with length of author name")
    {
      record.authorName = "foo bar";
      REQUIRE( record.getWriteSize() == 42 );

      record.authorName = "foo bar baz";
      REQUIRE( record.getWriteSize() == 46 );
    }

    SECTION("size adjusts with length of summary")
    {
      record.authorName = "foo bar";
      record.summary = "bla bla bla";
      REQUIRE( record.getWriteSize() == 60 );

      record.authorName = "foo bar";
      record.summary = "bla bla bla bla bla";
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts with length of dependencies")
    {
      record.authorName = "foo bar";
      auto master = Tes4HeaderRecord::MasterFile();
      master.fileName = "Skyrim.esm";
      master.data = 0;
      record.dependencies.push_back(master);
      REQUIRE( record.getWriteSize() == 73 );

      master.fileName = "WhateverItNeeds.esm";
      record.dependencies.push_back(master);
      REQUIRE( record.getWriteSize() == 113 );
    }

    SECTION("size adjusts with length of ONAM")
    {
      record.authorName = "foo bar";
      record.unknownONAM = { 1, 2, 3, 4 };
      REQUIRE( record.getWriteSize() == 64 );

      record.authorName = "foo bar";
      record.unknownONAM = { 1, 2, 3, 4, 5 };
      REQUIRE( record.getWriteSize() == 68 );
    }

    SECTION("size adjusts when INCC is present")
    {
      record.authorName = "foo bar";
      record.unknownINCC = {};
      REQUIRE( record.getWriteSize() == 42 );

      record.unknownINCC = 0xDEADBEEF;
      REQUIRE( record.getWriteSize() == 52 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    StringTable dummy_table;

    SECTION("default: load Skyrim header")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      Tes4HeaderRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000081 );
      REQUIRE( record.headerFormID == 0 );
      REQUIRE( record.headerRevision == 0 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.version == 0.94f );
      REQUIRE( record.numRecordsAndGroups == 920184 );
      REQUIRE( record.nextObjectID == 0x00000F92 );
      REQUIRE( record.authorName == "mcarofano" );
      REQUIRE( record.dependencies.empty() );
      REQUIRE( record.unknownONAM.empty() );
      REQUIRE( record.unknownIntValue == 0x000126C5 );
      REQUIRE_FALSE( record.unknownINCC.has_value() );
    }

    SECTION("default: load + save Update header")
    {
      const std::string_view data = "TES4\xF6\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x2B\0\0\0HEDR\x0C\0\x9A\x99\xD9\x3F\x69\x05\0\0\xDF\x09\x00\xFF\x43NAM\x09\0bnesmith\0MAST\x0B\0Skyrim.esm\0DATA\x08\0\0\0\0\0\0\0\0\0ONAM\x9C\0\xFE\x48\x07\x00\xA4\xFE\x10\x00\x9B\xF4\x0D\x00\x5E\x6D\x0A\x00\x04\x8A\x0E\x00\xA0\xFE\x10\x00\x71\x6D\x0A\x00\x65\xBA\x09\x00\xA7\xFE\x10\x00\x46\xE9\x0F\x00\x27\x4D\x10\x00\x60\x6D\x0A\x00\xAC\x0E\x0C\x00\x72\x6D\x0A\x00\x74\xBA\x09\x00\x50\xE9\x0F\x00\x60\xBD\x02\x00\xA3\xFE\x10\x00\xD9\x0E\x0C\x00\xA6\xFE\x10\x00\xE9\x6C\x10\x00\x79\x19\x08\x00\x61\x6D\x0A\x00\xA2\xFE\x10\x00\x73\x6D\x0A\x00\xEF\x89\x0E\x00\x48\xE9\x0F\x00\x6C\xBA\x09\x00\x2D\x4F\x0C\x00\xD2\x2E\x10\x00\xEA\x6C\x10\x00\x2F\xEF\x09\x00\xA5\xFE\x10\x00\x4B\xE9\x0F\x00\x5F\x6D\x0A\x00\xEC\x89\x0E\x00\x70\x6D\x0A\x00\xA1\xFE\x10\x00\xD2\x9C\x10\0INTV\x04\0\x21\0\0\0INCC\x04\0\x17\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      Tes4HeaderRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
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

      // Save it.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before full header can be read")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no HEDR")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0FAIL\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before full HEDR can be read")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of HEDR != 12")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0D\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no CNAM")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0FAIL\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CNAM is beyond stream")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x15\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ONAM")
    {
      const std::string_view data = "TES4\xF6\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x2B\0\0\0HEDR\x0C\0\x9A\x99\xD9\x3F\x69\x05\0\0\xDF\x09\x00\xFF\x43NAM\x09\0bnesmith\0MAST\x0B\0Skyrim.esm\0DATA\x08\0\0\0\0\0\0\0\0\0ONAM\x9C\0\xFE\x48\x07\x00\xA4\xFE\x10\x00\x9B\xF4\x0D\x00\x5E\x6D\x0A\x00\x04\x8A\x0E\x00\xA0\xFE\x10\x00\x71\x6D\x0A\x00\x65\xBA\x09\x00\xA7\xFE\x10\x00\x46\xE9\x0F\x00\x27\x4D\x10\x00\x60\x6D\x0A\x00\xAC\x0E\x0C\x00\x72\x6D\x0A\x00\x74\xBA\x09\x00\x50\xE9\x0F\x00\x60\xBD\x02\x00\xA3\xFE\x10\x00\xD9\x0E\x0C\x00\xA6\xFE\x10\x00\xE9\x6C\x10\x00\x79\x19\x08\x00\x61\x6D\x0A\x00\xA2\xFE\x10\x00\x73\x6D\x0A\x00\xEF\x89\x0E\x00\x48\xE9\x0F\x00\x6C\xBA\x09\x00\x2D\x4F\x0C\x00\xD2\x2E\x10\x00\xEA\x6C\x10\x00\x2F\xEF\x09\x00\xA5\xFE\x10\x00\x4B\xE9\x0F\x00\x5F\x6D\x0A\x00\xEC\x89\x0E\x00\x70\x6D\x0A\x00\xA1\xFE\x10\x00\xD2\x9C\x10\0ONAM\x04\0\x21\0\0\0ONAM\x04\0\x17\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ONAM is not a multiple of four")
    {
      const std::string_view data = "TES4\x6B\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x2B\0\0\0HEDR\x0C\0\x9A\x99\xD9\x3F\x69\x05\0\0\xDF\x09\x00\xFF\x43NAM\x09\0bnesmith\0MAST\x0B\0Skyrim.esm\0DATA\x08\0\0\0\0\0\0\0\0\0ONAM\x07\0\xFE\x48\x07\x00\xA4\xFE\x10INCC\x04\0\x17\0\0\0INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no INTV")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0FAIL\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing INTV")
    {
      const std::string_view data = "TES4\x22\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple INTV")
    {
      const std::string_view data = "TES4\x36\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00INTV\x04\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of INTV is not four")
    {
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0FAIL\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x05\0\xC5\x26\x01\x00"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TES4, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      Tes4HeaderRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: save record of Skyrim.esm")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      Tes4HeaderRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000081;
      record.headerFormID = 0;
      record.headerRevision = 0;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0000;
      // -- record data
      record.version = 0.94f;
      record.numRecordsAndGroups = 920184;
      record.nextObjectID = 0x00000F92;
      record.authorName = "mcarofano";
      record.dependencies.clear();
      record.unknownONAM.clear();
      record.unknownIntValue = 0x000126C5;
      record.unknownINCC = {};

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "TES4\x2C\0\0\0\x81\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0HEDR\x0C\0\xD7\xA3\x70\x3F\x78\x0A\x0E\x00\x92\x0F\0\0CNAM\x0A\0mcarofano\0INTV\x04\0\xC5\x26\x01\x00"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      Tes4HeaderRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000081 | BasicRecord::cDeletedFlag;
      record.headerFormID = 0;
      record.headerRevision = 0;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x0000;
      // -- record data
      record.authorName = "This should not even be written.";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "TES4\0\0\0\0\xA1\0\0\0\0\0\0\0\0\0\0\0\x28\0\0\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
