/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2023  Dirk Stolle

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

#include "../locate_catch.hpp"
#include <filesystem>
#include <string_view>
#include "../../../lib/mw/ESMReader.hpp"
#include "../../../lib/mw/MW_Constants.hpp"

TEST_CASE("MWTP::ESMReader")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("peekESMHeader")
  {
    TES3Record record;

    SECTION("default: read an ESM header")
    {
      // write "ESM file"
      {
        const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0rrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0"sv;
        std::ofstream file("FullTes3HeaderOnly.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE( ESMReader::peekESMHeader("FullTes3HeaderOnly.esm", record) );

      REQUIRE( std::filesystem::remove("FullTes3HeaderOnly.esm") );

      // Check record data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.Version == 1.2f );
      REQUIRE( record.FileFlag == 1 );
      REQUIRE( record.companyName == "Bethesda Softworks" );
      REQUIRE( record.description == "Hauptdatei f\xFCr Morrowind" );
      REQUIRE( record.NumRecords == 48289 );
      REQUIRE( record.dependencies.isEmpty() );
    }

    SECTION("failure: file does not exist")
    {
      REQUIRE_FALSE( ESMReader::peekESMHeader("does-not-exist.esm", record) );
    }

    SECTION("failure: file does not start with TES3 header")
    {
      // write "ESM" file
      {
        std::ofstream file("not-valid-tes3-header.esm");
        file.write("FAIL1234", 8);
        file.close();
      }

      REQUIRE_FALSE( ESMReader::peekESMHeader("not-valid-tes3-header.esm", record) );

      REQUIRE( std::filesystem::remove("not-valid-tes3-header.esm") );
    }

    SECTION("failure: file ends before full header data can be read")
    {
      // write "ESM" file
      {
        const std::string_view data = "TES3\x34\x01\0\0\0\0\0\0"sv;
        std::ofstream file("incompleteTes3Header.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE_FALSE( ESMReader::peekESMHeader("incompleteTes3Header.esm", record) );

      REQUIRE( std::filesystem::remove("incompleteTes3Header.esm") );
    }
  }

  SECTION("readESM")
  {
    ESMReader reader;
    TES3Record header;

    SECTION("default: read ESM file")
    {
      // write "ESM file"
      {
        const std::string_view data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0rrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
        std::ofstream file("readESM-tes3-sample.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      // Default implementation skips all records, so it returns zero.
      REQUIRE( reader.readESM("readESM-tes3-sample.esm", header) == 0 );
      REQUIRE( std::filesystem::remove("readESM-tes3-sample.esm") );

      // Check record data.
      // -- header
      REQUIRE( header.getHeaderOne() == 0 );
      REQUIRE( header.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( header.Version == 1.2f );
      REQUIRE( header.FileFlag == 1 );
      REQUIRE( header.companyName == "Bethesda Softworks" );
      REQUIRE( header.description == "Hauptdatei f\xFCr Morrowind" );
      REQUIRE( header.NumRecords == 48289 );
      REQUIRE( header.dependencies.isEmpty() );
    }

    SECTION("failure: file does not exist")
    {
      REQUIRE( reader.readESM("does-not-exist.esm", header) == -1 );
    }

    SECTION("failure: file does not start with TES3 header")
    {
      // write "ESM" file
      {
        std::ofstream file("readESM-not-valid-tes3-header.esm");
        file.write("FAIL1234", 8);
        file.close();
      }

      REQUIRE( reader.readESM("readESM-not-valid-tes3-header.esm", header) == -1 );

      REQUIRE( std::filesystem::remove("readESM-not-valid-tes3-header.esm") );
    }

    SECTION("failure: file ends before full header data can be read")
    {
      // write "ESM" file
      {
        const std::string_view data = "TES3\x34\x01\0\0\x81"sv;
        std::ofstream file("readESM-tes3-incompleteHeader.esm");
        file.write(data.data(), data.size());
        file.close();
      }

      REQUIRE( reader.readESM("readESM-tes3-incompleteHeader.esm", header) == -1 );

      REQUIRE( std::filesystem::remove("readESM-tes3-incompleteHeader.esm") );
    }
  }

  SECTION("skipRecord")
  {
    ESMReader reader;

    SECTION("default: skip a record")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0APPA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip STAT, because header is handled before skipRecord.
      stream.seekg(4);
      REQUIRE( stream.good() );

      REQUIRE( reader.skipRecord(stream) == 0);

      // Check that the stream is really at the right position by reading the
      // next bytes, which should be "APPA".
      uint32_t next_four_bytes = 0;
      stream.read(reinterpret_cast<char*>(&next_four_bytes), 4);
      REQUIRE( stream.good() );
      REQUIRE( next_four_bytes == cAPPA );
    }

    SECTION("corrupt stream: stream end before full record can be skipped")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip STAT, because header is handled before skipRecord.
      stream.seekg(4);
      REQUIRE( stream.good() );

      REQUIRE( reader.skipRecord(stream) == -1 );
    }
  }

  SECTION("processNextRecord")
  {
    ESMReader reader;

    SECTION("default: process a record")
    {
      const auto data = "STAT\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Default implementation skips records, so it returns zero.
      REQUIRE( reader.processNextRecord(stream) == 0 );
    }

    SECTION("failure: unknown record header")
    {
      const auto data = "FAIL\x48\0\0\0\0\0\0\0\0\0\0\0NAME\x19\0\0\0in_redoran_hut_bfloor_02\0MODL\x1F\0\0\0i\\In_redoran_hut_Bfloor_02.NIF\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      REQUIRE( reader.processNextRecord(stream) == -1 );
    }
  }
}
