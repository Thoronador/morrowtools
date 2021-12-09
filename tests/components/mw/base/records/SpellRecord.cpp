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
#include "../../../../../mw/base/records/SpellRecord.hpp"

TEST_CASE("MWTP::SpellRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    SpellRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Type == SpellType::Spell );
    REQUIRE( record.Cost == 0 );
    REQUIRE( record.Flags == 0 );
    REQUIRE( record.Enchs.empty() );
  }

  SECTION("equals")
  {
    SpellRecord a;
    SpellRecord b;

    SECTION("equal")
    {
      a.recordID = "SpellOne";

      b.recordID = "SpellOne";

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
        a.Name = "Alice's spell";
        b.Name = "Bob's spell";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Type mismatch")
      {
        a.Type = SpellType::Spell;
        b.Type = SpellType::Curse;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Cost mismatch")
      {
        a.Cost = 5;
        b.Cost = 10;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Flags mismatch")
      {
        a.Flags = 0;
        b.Flags = 3;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Enchantments mismatch")
      {
        a.Enchs.push_back(EnchantmentData());
        b.Enchs.clear();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record with a single spell effect")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SpellRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "fireball" );
      REQUIRE( record.Name == "Feuerball" );
      REQUIRE( record.Type == SpellType::Spell );
      REQUIRE( record.Cost == 5 );
      REQUIRE( record.Flags == 1 );
      REQUIRE( record.Enchs.size() == 1 );
      REQUIRE( record.Enchs[0].EffectID == 0x000E );
      REQUIRE( record.Enchs[0].SkillID == 0xFF );
      REQUIRE( record.Enchs[0].AttributeID == 0xFF );
      REQUIRE( record.Enchs[0].Range == RangeType::Target );
      REQUIRE( record.Enchs[0].Area == 5 );
      REQUIRE( record.Enchs[0].Duration == 1 );
      REQUIRE( record.Enchs[0].MagnitudeMin == 2 );
      REQUIRE( record.Enchs[0].MagnitudeMax == 20 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with multiple effects")
    {
      const auto data = "SPEL\xC1\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0hand of vivec\0FNAM\x0F\0\0\0Hand von Vivec\0SPDT\x0C\0\0\0\0\0\0\0\xBC\0\0\0\0\0\0\0ENAM\x18\0\0\0\x17\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x10\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0F\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      SpellRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "hand of vivec" );
      REQUIRE( record.Name == "Hand von Vivec" );
      REQUIRE( record.Type == SpellType::Spell );
      REQUIRE( record.Cost == 188 );
      REQUIRE( record.Flags == 0 );
      REQUIRE( record.Enchs.size() == 4 );

      REQUIRE( record.Enchs[0].EffectID == 0x0017 );
      REQUIRE( record.Enchs[0].SkillID == 0xFF );
      REQUIRE( record.Enchs[0].AttributeID == 0xFF );
      REQUIRE( record.Enchs[0].Range == RangeType::Touch );
      REQUIRE( record.Enchs[0].Area == 0 );
      REQUIRE( record.Enchs[0].Duration == 3 );
      REQUIRE( record.Enchs[0].MagnitudeMin == 50 );
      REQUIRE( record.Enchs[0].MagnitudeMax == 50 );

      REQUIRE( record.Enchs[1].EffectID == 0x000E );
      REQUIRE( record.Enchs[1].SkillID == 0xFF );
      REQUIRE( record.Enchs[1].AttributeID == 0xFF );
      REQUIRE( record.Enchs[1].Range == RangeType::Touch );
      REQUIRE( record.Enchs[1].Area == 0 );
      REQUIRE( record.Enchs[1].Duration == 3 );
      REQUIRE( record.Enchs[1].MagnitudeMin == 50 );
      REQUIRE( record.Enchs[1].MagnitudeMax == 50 );

      REQUIRE( record.Enchs[2].EffectID == 0x0010 );
      REQUIRE( record.Enchs[2].SkillID == 0xFF );
      REQUIRE( record.Enchs[2].AttributeID == 0xFF );
      REQUIRE( record.Enchs[2].Range == RangeType::Touch );
      REQUIRE( record.Enchs[2].Area == 0 );
      REQUIRE( record.Enchs[2].Duration == 3 );
      REQUIRE( record.Enchs[2].MagnitudeMin == 50 );
      REQUIRE( record.Enchs[2].MagnitudeMax == 50 );

      REQUIRE( record.Enchs[3].EffectID == 0x000F );
      REQUIRE( record.Enchs[3].SkillID == 0xFF );
      REQUIRE( record.Enchs[3].AttributeID == 0xFF );
      REQUIRE( record.Enchs[3].Range == RangeType::Touch );
      REQUIRE( record.Enchs[3].Area == 0 );
      REQUIRE( record.Enchs[3].Duration == 3 );
      REQUIRE( record.Enchs[3].MagnitudeMin == 50 );
      REQUIRE( record.Enchs[3].MagnitudeMax == 50 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "SPEL\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0FAIL\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\x01\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x5F\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FAIL\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\x01\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x5F\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SPDT")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0FAIL\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SPDT is not 12")
    {
      {
        const auto data = "SPEL\x56\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0B\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "SPEL\x58\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0D\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read SPEL, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        SpellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before SPDT can be read")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ENAM")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0FAIL\x18\0\0\0\x0E\0\xFF\xFF\x02\0\0\0\x05\0\0\0\x01\0\0\0\x02\0\0\0\x14\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before all of ENAM can be read")
    {
      const auto data = "SPEL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0fireball\0FNAM\x0A\0\0\0Feuerball\0SPDT\x0C\0\0\0\0\0\0\0\x05\0\0\0\x01\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: more than eight ENAM entries")
    {
      const auto data = "SPEL\x61\x01\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0hand of vivec\0FNAM\x0F\0\0\0Hand von Vivec\0SPDT\x0C\0\0\0\0\0\0\0\xBC\0\0\0\0\0\0\0ENAM\x18\0\0\0\x17\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0E\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x10\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0F\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x10\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0F\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x10\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0F\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0ENAM\x18\0\0\0\x0F\0\xFF\xFF\x01\0\0\0\0\0\0\0\x03\0\0\0\x32\0\0\0\x32\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read SPEL, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      SpellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("fun with flags")
  {
    SpellRecord record;

    SECTION("autoCalculateCosts")
    {
      REQUIRE_FALSE( record.autoCalculateCosts() );

      record.Flags = 1;
      REQUIRE( record.autoCalculateCosts() );

      record.Flags = 2;
      REQUIRE_FALSE( record.autoCalculateCosts() );

      record.Flags = 65535;
      REQUIRE( record.autoCalculateCosts() );
    }

    SECTION("isPCStartSpell")
    {
      REQUIRE_FALSE( record.isPCStartSpell() );

      record.Flags = 2;
      REQUIRE( record.isPCStartSpell() );

      record.Flags = 8;
      REQUIRE_FALSE( record.isPCStartSpell() );

      record.Flags = 65535;
      REQUIRE( record.isPCStartSpell() );
    }

    SECTION("alwaysSucceeds")
    {
      REQUIRE_FALSE( record.alwaysSucceeds() );

      record.Flags = 4;
      REQUIRE( record.alwaysSucceeds() );

      record.Flags = 8;
      REQUIRE_FALSE( record.alwaysSucceeds() );

      record.Flags = 65535;
      REQUIRE( record.alwaysSucceeds() );
    }
  }
}
