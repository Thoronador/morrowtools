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
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/SkillRecord.hpp"

TEST_CASE("MWTP::SkillRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SkillRecord record;

    REQUIRE( record.getHeaderOne() == 0 );
    REQUIRE( record.getHeaderFlags() == 0 );
    REQUIRE( record.SkillIndex == 0 );
    REQUIRE( record.Attribute == 0 );
    REQUIRE( record.Specialization == 0 );
    REQUIRE( record.UseValue[0] == 0.0f );
    REQUIRE( record.UseValue[1] == 0.0f );
    REQUIRE( record.UseValue[2] == 0.0f );
    REQUIRE( record.UseValue[3] == 0.0f );
    REQUIRE( record.Description.empty() );
  }

  SECTION("equals")
  {
    SkillRecord a;
    SkillRecord b;

    SECTION("equal")
    {
      a.SkillIndex = SkillDestruction;
      b.SkillIndex = SkillDestruction;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("SkillIndex mismatch")
      {
        a.SkillIndex = SkillDestruction;
        b.SkillIndex = SkillMysticism;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Attribute mismatch")
      {
        a.Attribute = 3;
        b.Attribute = 4;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Specialization mismatch")
      {
        a.Specialization = cSpecializationMagic;
        b.Specialization = cSpecializationStealth;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UseValue[0] mismatch")
      {
        a.UseValue[0] = 0.1f;
        b.UseValue[0] = 0.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UseValue[1] mismatch")
      {
        a.UseValue[1] = 1.1f;
        b.UseValue[1] = 0.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UseValue[2] mismatch")
      {
        a.UseValue[2] = 2.1f;
        b.UseValue[2] = 0.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UseValue[3] mismatch")
      {
        a.UseValue[3] = 0.1f;
        b.UseValue[3] = 0.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Description mismatch")
      {
        a.Description = "bar";
        b.Description = "foo";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SkillRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.SkillIndex == SkillSneak );
      REQUIRE( record.Attribute == 3 );
      REQUIRE( record.Specialization == cSpecializationStealth );
      REQUIRE( record.UseValue[0] == 0.25f );
      REQUIRE( record.UseValue[1] == 2.0f );
      REQUIRE( record.UseValue[2] == 1.0f );
      REQUIRE( record.UseValue[3] == 1.0f );
      REQUIRE( record.Description == "Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe." );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no INDX")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0FAIL\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INDX is not four")
    {
      {
        const auto data = "SKIL\xBA\0\0\0\0\0\0\0\0\0\0\0INDX\x03\0\0\0\x13\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SKIL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SkillRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "SKIL\xBC\0\0\0\0\0\0\0\0\0\0\0INDX\x05\0\0\0\x13\0\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SKIL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SkillRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INDX can be read")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SKDT")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0FAIL\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SKDT is not 24")
    {
      {
        const auto data = "SKIL\xBA\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x17\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SKIL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SkillRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "SKIL\xBC\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x19\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?\0DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SKIL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SkillRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before SKDT can be read")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DESC")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?FAIL\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC > 512")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\x02\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC is beyond stream")
    {
      const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x8F\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SKIL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SkillRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
