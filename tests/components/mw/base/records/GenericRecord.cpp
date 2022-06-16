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

#include "../../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/MW_Constants.hpp"
#include "../../../../../mw/base/records/GenericRecord.hpp"

TEST_CASE("MWTP::GenericRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    GenericRecord record;

    REQUIRE( record.Header == 0 );
    REQUIRE( record.getHeaderOne() == 0 );
    REQUIRE( record.getHeaderFlags() == 0 );
    REQUIRE( record.size() == 0 );
    REQUIRE( record.data() == nullptr );
  }

  SECTION("copy constructor")
  {
    SECTION("empty record")
    {
      GenericRecord record;
      GenericRecord copied(record);

      REQUIRE( copied.Header == 0 );
      REQUIRE( copied.getHeaderOne() == 0 );
      REQUIRE( copied.getHeaderFlags() == 0 );
      REQUIRE( copied.size() == 0 );
      REQUIRE( copied.data() == nullptr );
    }

    SECTION("loaded record")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GenericRecord record;
      record.Header = cACTI;
      REQUIRE( record.loadFromStream(stream) );

      // copy
      GenericRecord copied(record);
      REQUIRE( copied.Header == cACTI );
      REQUIRE( copied.getHeaderOne() == 0 );
      REQUIRE( copied.getHeaderFlags() == 0 );
      REQUIRE( copied.size() == 83 );
      REQUIRE_FALSE( copied.data() == nullptr );

      // Data pointers should not match.
      REQUIRE_FALSE( record.data() == nullptr );
      REQUIRE_FALSE( record.data() == copied.data() );
      // But data should be the same.
      const auto content = "NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      const auto copiedContent = std::string_view(reinterpret_cast<const char*>(copied.data()), copied.size());
      const auto originalContent = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( copiedContent == content );
      REQUIRE( originalContent == content );
    }
  }

  SECTION("assignment operator")
  {
    SECTION("empty record")
    {
      GenericRecord record;
      GenericRecord assigned = record;

      REQUIRE( assigned.Header == 0 );
      REQUIRE( assigned.getHeaderOne() == 0 );
      REQUIRE( assigned.getHeaderFlags() == 0 );
      REQUIRE( assigned.size() == 0 );
      REQUIRE( assigned.data() == nullptr );
    }

    SECTION("loaded record")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      uint32_t dummy = 0;
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GenericRecord record;
      record.Header = cACTI;
      REQUIRE( record.loadFromStream(stream) );

      // assignment
      GenericRecord assigned = record;
      REQUIRE( assigned.Header == cACTI );
      REQUIRE( assigned.getHeaderOne() == 0 );
      REQUIRE( assigned.getHeaderFlags() == 0 );
      REQUIRE( assigned.size() == 83 );
      REQUIRE_FALSE( assigned.data() == nullptr );

      // Data pointers should not match.
      REQUIRE_FALSE( record.data() == nullptr );
      REQUIRE_FALSE( record.data() == assigned.data() );
      // But data should be the same.
      const auto content = "NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      const auto assignedContent = std::string_view(reinterpret_cast<const char*>(assigned.data()), assigned.size());
      const auto originalContent = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( assignedContent == content );
      REQUIRE( originalContent == content );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const auto data = "ACTI\x53\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      GenericRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.data() != nullptr );
      REQUIRE( record.size() == 83 );
      const auto content = std::string_view(reinterpret_cast<const char*>(record.data()), record.size());
      REQUIRE( content == "NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv );

      // Writing should succeed.
      record.Header = cACTI;
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "ACTI\x53\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: size of record > 256 KB")
    {
      const auto data = "ACTI\x53\xFF\x04\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of record is beyond stream")
    {
      const auto data = "ACTI\xFF\0\0\0\0\0\0\0\0\0\0\0NAME\x14\0\0\0active_akula_shield\0MODL\x1A\0\0\0i\\active_akula_shield.NIF\0FNAM\x0D\0\0\0Akula-Schild\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ACTI, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      GenericRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
