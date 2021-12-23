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
#include "../../../../../mw/base/records/TES3Record.hpp"

TEST_CASE("MWTP::TES3Record")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    TES3Record record;

    REQUIRE( record.Version == 1.2f );
    REQUIRE( record.FileFlag == 0 );
    REQUIRE( record.companyName.empty() );
    REQUIRE( record.description.empty() );
    REQUIRE( record.NumRecords == 0 );
    REQUIRE( record.dependencies.isEmpty() );
  }

  SECTION("equals")
  {
    TES3Record a;
    TES3Record b;

    SECTION("equal")
    {
      a.companyName = "ACME";

      b.companyName = "ACME";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("Version mismatch")
      {
        a.Version = 1.5f;
        b.Version = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FileFlag mismatch")
      {
        a.FileFlag = 15;
        b.FileFlag = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("companyName mismatch")
      {
        a.companyName = "foo";
        b.companyName = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("description mismatch")
      {
        a.description = "Alice's plugin";
        b.description = "Bob's plugin";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NumRecords mismatch")
      {
        a.NumRecords = 155;
        b.NumRecords = 255;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("dependencies mismatch")
      {
        a.dependencies.push_back(DepFile("foo.esm"));
        b.dependencies.clear();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without dependencies")
    {
      const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TES3Record record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
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

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with dependency")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\0\0\0Morrowind.esm\0DATA\x08\0\0\0\x08{\xCE\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TES3Record record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.Version == 1.3f );
      REQUIRE( record.FileFlag == 1 );
      REQUIRE( record.companyName == "Bethesda Softworks" );
      REQUIRE( record.description == "The main data file for Tribunal.\r\n(requires Morrowind.esm to run)" );
      REQUIRE( record.NumRecords == 14789 );
      REQUIRE( record.dependencies.getSize() == 1 );
      REQUIRE( record.dependencies.at(0).name == "Morrowind.esm" );
      REQUIRE( record.dependencies.at(0).size == 80640776 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "TES3\x5A\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no HEDR")
    {
      const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0FAIL,\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0rrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of HEDR is not 300")
    {
      {
        const auto data = "TES3\x33\x01\0\0\0\0\0\0\0\0\0\0HEDR\x2B\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0rrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TES3, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TES3Record record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "TES3\x35\x01\0\0\0\0\0\0\0\0\0\0HEDR\x2D\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0rrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1\xBC\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TES3, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TES3Record record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before HEDR flags can be read")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before HEDR's company name can be read")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Be"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before HEDR's description can be read")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before HEDR's record count can be read")
    {
      const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0Hauptdatei f\xFCr Morrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xA1"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MAST")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0FAIL\x0E\0\0\0Morrowind.esm\0DATA\x08\0\0\0\x08{\xCE\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MAST > 256")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\x01\0\0Morrowind.esm\0DATA\x08\0\0\0\x08{\xCE\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MAST is beyond stream")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x2E\0\0\0Morrowind.esm\0DATA\x08\0\0\0\x08{\xCE\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\0\0\0Morrowind.esm\0FAIL\x08\0\0\0\x08{\xCE\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not eight")
    {
      {
        const auto data = "TES3\x59\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\0\0\0Morrowind.esm\0DATA\x07\0\0\0\x08{\xCE\x04\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TES3, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TES3Record record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "TES3\x5B\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\0\0\0Morrowind.esm\0DATA\x09\0\0\0\x08{\xCE\x04\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read TES3, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TES3Record record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before all of DATA can be read")
    {
      const auto data = "TES3\x5A\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0ff\xA6?\x01\0\0\0Bethesda Softworks\0\0\0\0\0\0\0\0\0\0\0\0\0\0The main data file for Tribunal.\x0D\x0A(requires Morrowind.esm to run)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xC5\x39\0\0MAST\x0E\0\0\0Morrowind.esm\0DATA\x08\0\0\0\x08{"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read TES3, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TES3Record record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("long company name is cut off automatically")
    {
      TES3Record record;
      record.Version = 1.2f;
      record.FileFlag = 1;
      record.companyName = "ACME, Very Long Name Division # 123456789";
      record.description = "Hauptdatei f\xFCr Morrowind";
      record.NumRecords = 5;
      record.dependencies.clear();

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0ACME, Very Long Name Division #\0Hauptdatei f\xFCr Morrowind\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x05\0\0\0"sv;
      REQUIRE( streamOut.str() == data );
    }

    SECTION("long description is cut off automatically")
    {
      TES3Record record;
      record.Version = 1.2f;
      record.FileFlag = 1;
      record.companyName = "ACME";
      record.description = std::string("Description2345678901234567890") // 30
                          .append("123456789012345678901234567890") // 60
                          .append("123456789012345678901234567890") // 90
                          .append("123456789012345678901234567890") // 120
                          .append("123456789012345678901234567890") // 150
                          .append("123456789012345678901234567890") // 180
                          .append("123456789012345678901234567890") // 210
                          .append("123456789012345678901234567890") // 240
                          .append("123456789012345678901234567890"); // 270
      record.NumRecords = 5;
      record.dependencies.clear();

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      const auto data = "TES3\x34\x01\0\0\0\0\0\0\0\0\0\0HEDR,\x01\0\0\x9A\x99\x99?\x01\0\0\0ACME\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0Description2345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345\0\x05\0\0\0"sv;
      REQUIRE( streamOut.str() == data );
    }
  }
}
