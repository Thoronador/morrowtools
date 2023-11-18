/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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
#include "../../../../lib/mw/records/BirthSignRecord.hpp"

TEST_CASE("MWTP::BirthSignRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    BirthSignRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Texture.empty() );
    REQUIRE( record.Description.empty() );
    REQUIRE( record.SignSpells.empty() );
  }

  SECTION("equals")
  {
    BirthSignRecord a;
    BirthSignRecord b;

    SECTION("equal")
    {
      a.recordID = "Foo";
      a.Name = "Blob";

      b.recordID = "Foo";
      b.Name = "Blob";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Name mismatch")
      {
        a.Name = "foo";
        b.Name = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Texture mismatch")
      {
        a.Texture = "foo.tga";
        b.Texture = "bar.tga";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Description mismatch")
      {
        a.Description = "foo";
        b.Description = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SignSpells mismatch")
      {
        a.SignSpells.clear();
        b.SignSpells = { "foo", "bar", "baz", "blah" };

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("less")
  {
    BirthSignRecord a;
    BirthSignRecord b;

    SECTION("equal")
    {
      a.recordID = "Sign";
      b.recordID = "Sign";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "SIGN";
      b.recordID = "sign";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "sign";
      b.recordID = "SIGN";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less")
    {
      a.recordID = "Bar";
      b.recordID = "Foo";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "bar";
      b.recordID = "FOO";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "BAR";
      b.recordID = "foo";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater")
    {
      a.recordID = "Foo";
      b.recordID = "Bar";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "foo";
      b.recordID = "BAR";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );

      a.recordID = "FOO";
      b.recordID = "bar";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BirthSignRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "Lady's Favor" );
      REQUIRE( record.Name == "Die F\xFCrstin" );
      REQUIRE( record.Texture == "Birthsigns\\Tx_birth_lady.tga" );
      REQUIRE( record.Description == "Sternbild der F\xFCrstin mit dem Hauptaspekt Masser." );
      REQUIRE( record.SignSpells.size() == 2 );
      REQUIRE( record.SignSpells[0] == "lady's favor" );
      REQUIRE( record.SignSpells[1] == "lady's grace" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0FAIL\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAMEs")
    {
      const auto data = "BSGN\xED\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 255")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\x01\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Fav"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FAIL\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const auto data = "BSGN\xEC\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\x01\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCr"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no TNAM")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0FAIL\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple TNAMs")
    {
      const auto data = "BSGN\xFD\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TNAM > 255")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\x01\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TNAM is beyond stream")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DESC")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0FAIL2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple DESCs")
    {
      const auto data = "BSGN\xE3\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0DESC\x03\0\0\0Gh\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC > 256")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\x01\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC is beyond stream")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NPCS")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0FAIL \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPCS > 256")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \x01\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's grace\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before NPCS is complete")
    {
      const auto data = "BSGN\xD8\0\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x0D\0\0\0Lady's Favor\0FNAM\x0C\0\0\0Die F\xFCrstin\0TNAM\x1D\0\0\0Birthsigns\\Tx_birth_lady.tga\0DESC2\0\0\0Sternbild der F\xFCrstin mit dem Hauptaspekt Masser.\0NPCS \0\0\0lady's favor\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0lady's gra"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip BSGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BirthSignRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
