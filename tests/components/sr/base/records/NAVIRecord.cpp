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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/NAVIRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("NAVIRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NAVIRecord record;

    REQUIRE( record.unknownNVER == 0 );
    REQUIRE( record.unknownNVMIs.empty() );
    REQUIRE_FALSE( record.unknownNVPP.isPresent() );
    REQUIRE( record.unknownNVSI == 0 );
  }

  SECTION("equals")
  {
    NAVIRecord a;
    NAVIRecord b;

    SECTION("equal")
    {
      a.unknownNVER = 1;
      b.unknownNVER = 1;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("NVER mismatch")
      {
        a.unknownNVER = 1;
        b.unknownNVER = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NVMIs mismatch")
      {
        BinarySubRecord bsr;
        bsr.setPresence(true);

        a.unknownNVMIs.push_back(bsr);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NVPP mismatch")
      {
        a.unknownNVPP.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownNVPP.setPresence(false);
        b.unknownNVPP.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NVSI mismatch")
      {
        a.unknownNVSI = 0x01234567;
        b.unknownNVSI = 0x0FEDCBA9;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    NAVIRecord record;

    REQUIRE( record.getRecordType() == cNAVI );
  }

  SECTION("getWriteSize")
  {
    NAVIRecord record;

    SECTION("size adjusts with length of NVMIs")
    {
      REQUIRE( record.getWriteSize() == 10 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "NVMI\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        BinarySubRecord nvmi;
        REQUIRE( nvmi.loadFromStream(stream, cNVMI, true) );
        record.unknownNVMIs.emplace_back(nvmi);
      }
      REQUIRE( record.getWriteSize() == 34 );

      {
        const std::string_view data = "NVMI\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        BinarySubRecord nvmi;
        REQUIRE( nvmi.loadFromStream(stream, cNVMI, true) );
        record.unknownNVMIs.emplace_back(nvmi);
      }
      REQUIRE( record.getWriteSize() == 41 );
    }

    SECTION("size adjusts with length of NVPP")
    {
      REQUIRE( record.getWriteSize() == 10 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "NVPP\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNVPP.loadFromStream(stream, cNVPP, true) );
      }
      REQUIRE( record.getWriteSize() == 34 );

      {
        const std::string_view data = "NVPP\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNVPP.loadFromStream(stream, cNVPP, true) );
      }
      REQUIRE( record.getWriteSize() == 17 );
    }

    SECTION("size adjusts when NVSI is present")
    {
      REQUIRE( record.getWriteSize() == 10 );

      record.unknownNVSI = 1;
      REQUIRE( record.getWriteSize() == 20 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("special: load deleted record")
    {
      const auto data = "NAVI\0\0\0\0\x20\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NAVI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NAVIRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000020 );
      REQUIRE( record.headerFormID == 0x00012FB4 );
      REQUIRE( record.headerRevision == 0x00386908 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000E );
      // -- record data
      REQUIRE( record.unknownNVER == 0 );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "NAVIxFE\x40\x29\0\0\0\0\0\xB4\x2F\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NAVI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NAVIRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no NVER")
    {
      const auto data = "NAVI\x0A\0\0\0\0\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0FAIL\x04\0\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NAVI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NAVIRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NVER is not four")
    {
      {
        const auto data = "NAVI\x09\0\0\0\0\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0NVER\x03\0\x0C\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NAVI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NAVIRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NAVI\x0B\0\0\0\0\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0NVER\x05\0\x0C\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NAVI, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NAVIRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NVMI can be read")
    {
      const auto data = "NAVI\xFE\x40\x29\0\0\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0NVER\x04\0\x0C\0\0\0NVMI9\0'\x0A\x10\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NAVI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NAVIRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    // Note:
    // More extensible tests are hard, because a real data record is something
    // like 2,5 MB or more. Such an amount of data inside a single string view
    // would be possible but very large and hard to handle.
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      NAVIRecord record;
      // Fill data.
      // -- header
      record.headerFlags = 0x00000020;
      record.headerFormID = 0x00012FB4;
      record.headerRevision = 0x00386908;
      record.headerVersion = 40;
      record.headerUnknown5 = 0x000E;
      // -- record data
      record.unknownNVER = 0xDEADBEEF;

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const auto data = "NAVI\0\0\0\0\x20\0\0\0\xB4\x2F\x01\0\x08\x69\x38\0\x28\0\x0E\0"sv;
      REQUIRE( stream.str() == data );
    }
  }
}
