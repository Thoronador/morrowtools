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

#include "../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/BodyPartRecord.hpp"

TEST_CASE("MWTP::BodyPartRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    BodyPartRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.RaceID.empty() );
    REQUIRE( record.Part == 0 );
    REQUIRE( record.Vampire == 0 );
    REQUIRE( record.Flags == 0 );
    REQUIRE( record.PartType == 0 );
  }

  SECTION("equals")
  {
    BodyPartRecord a;
    BodyPartRecord b;

    SECTION("equal")
    {
      a.recordID = "Tree";
      a.ModelPath = "tree01.nif";

      b.recordID = "Tree";
      b.ModelPath = "tree01.nif";

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

      SECTION("ModelPath mismatch")
      {
        a.ModelPath = "mesh01.nif";
        b.ModelPath = "mesh02.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("RaceID mismatch")
      {
        a.RaceID = "bar";
        b.RaceID = "foo";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Part mismatch")
      {
        a.Part = 1;
        b.Part = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Vampire mismatch")
      {
        a.Vampire = 1;
        b.Vampire = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Flags mismatch")
      {
        a.Flags = 1;
        b.Flags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PartType mismatch")
      {
        a.PartType = 1;
        b.PartType = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("less")
  {
    BodyPartRecord a;
    BodyPartRecord b;

    SECTION("equal")
    {
      a.recordID = "Tree";
      b.recordID = "Tree";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("less")
    {
      a.recordID = "Lindentree";
      b.recordID = "Tree";

      REQUIRE( a < b );
      REQUIRE_FALSE( b < a );
    }

    SECTION("greater")
    {
      a.recordID = "Tree";
      b.recordID = "Another tree";

      REQUIRE_FALSE( a < b );
      REQUIRE( b < a );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BodyPartRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "A_M_Chitin_pauldron_CL" );
      REQUIRE( record.ModelPath == "a\\A_M_Chitin_pauldron_CL.NIF" );
      REQUIRE( record.RaceID == "Argonian" );
      REQUIRE( record.Part == 13 );
      REQUIRE( record.Vampire == 0 );
      REQUIRE( record.Flags == 0 );
      REQUIRE( record.PartType == 2 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "BODY\x61\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0FAIL\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\x02\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x61\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0FAIL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\x02\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x50\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FAIL\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 512")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\x02\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x49\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no BYDT")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0FAIL\x04\0\0\0\x0D\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BYDT is not four")
    {
      {
        const auto data = "BODY\x60\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x03\0\0\0\x0D\0\0\x02"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BODY, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BodyPartRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "BODY\x62\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x05\0\0\0\x0D\0\0\x02\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BODY, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BodyPartRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before all of BYDT can be read")
    {
      const auto data = "BODY\x61\0\0\0\0\0\0\0\0\0\0\0NAME\x17\0\0\0A_M_Chitin_pauldron_CL\0MODL\x1D\0\0\0a\\A_M_Chitin_pauldron_CL.NIF\0FNAM\x09\0\0\0Argonian\0BYDT\x04\0\0\0\x0D\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BODY, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BodyPartRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
