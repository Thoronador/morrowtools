/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/RegionRecord.hpp"

TEST_CASE("MWTP::RegionRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    RegionRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.RegionName.empty() );
    REQUIRE( record.Clear == 0 );
    REQUIRE( record.Cloudy == 0 );
    REQUIRE( record.Foggy == 0 );
    REQUIRE( record.Overcast == 0 );
    REQUIRE( record.Rain == 0 );
    REQUIRE( record.Thunder == 0 );
    REQUIRE( record.Ash == 0 );
    REQUIRE( record.Blight == 0 );
    REQUIRE( record.Snow == 0 );
    REQUIRE( record.Blizzard == 0 );
    REQUIRE( record.SleepCreature.empty() );
    REQUIRE( record.Red == 0 );
    REQUIRE( record.Green == 0 );
    REQUIRE( record.Blue == 0 );
    REQUIRE( record.Zero == 0 );
    REQUIRE( record.SoundChances.empty() );
  }

  SECTION("constructor with ID")
  {
    RegionRecord record("bla");

    REQUIRE( record.recordID == "bla" );
    REQUIRE( record.RegionName.empty() );
    REQUIRE( record.Clear == 0 );
    REQUIRE( record.Cloudy == 0 );
    REQUIRE( record.Foggy == 0 );
    REQUIRE( record.Overcast == 0 );
    REQUIRE( record.Rain == 0 );
    REQUIRE( record.Thunder == 0 );
    REQUIRE( record.Ash == 0 );
    REQUIRE( record.Blight == 0 );
    REQUIRE( record.Snow == 0 );
    REQUIRE( record.Blizzard == 0 );
    REQUIRE( record.SleepCreature.empty() );
    REQUIRE( record.Red == 0 );
    REQUIRE( record.Green == 0 );
    REQUIRE( record.Blue == 0 );
    REQUIRE( record.Zero == 0 );
    REQUIRE( record.SoundChances.empty() );
  }

  SECTION("equals")
  {
    RegionRecord a;
    RegionRecord b;

    SECTION("equal")
    {
      a.recordID = "RegionOne";
      a.RegionName = "foo's bar";

      b.recordID = "RegionOne";
      b.RegionName = "foo's bar";

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

      SECTION("RegionName mismatch")
      {
        a.RegionName = "One";
        b.RegionName = "Another";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Clear mismatch")
      {
        a.Clear = 15;
        b.Clear = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Cloudy mismatch")
      {
        a.Cloudy = 15;
        b.Cloudy = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Foggy mismatch")
      {
        a.Foggy = 15;
        b.Foggy = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Overcast mismatch")
      {
        a.Overcast = 15;
        b.Overcast = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Rain mismatch")
      {
        a.Rain = 15;
        b.Rain = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Thunder mismatch")
      {
        a.Thunder = 15;
        b.Thunder = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Ash mismatch")
      {
        a.Ash = 15;
        b.Ash = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Blight mismatch")
      {
        a.Blight = 15;
        b.Blight = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Snow mismatch")
      {
        a.Snow = 15;
        b.Snow = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Blizzard mismatch")
      {
        a.Blizzard = 15;
        b.Blizzard = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SleepCreature mismatch")
      {
        a.SleepCreature = "ex_one";
        b.SleepCreature = "ex_two";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Red mismatch")
      {
        a.Red = 15;
        b.Red = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Green mismatch")
      {
        a.Green = 15;
        b.Green = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Blue mismatch")
      {
        a.Blue = 15;
        b.Blue = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Zero mismatch")
      {
        a.Zero = 0;
        b.Zero = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SoundChances mismatch")
      {
        a.SoundChances.clear();
        SoundChanceRecord sc;
        sc.Sound = "foo";
        sc.Chance = 100;
        b.SoundChances.push_back(sc);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("less")
  {
    RegionRecord a;
    RegionRecord b;

    SECTION("equal")
    {
      a.recordID = "Region";
      b.recordID = "Region";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "REGION";
      b.recordID = "region";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "region";
      b.recordID = "REGION";

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
    SECTION("default: load record Morrowind-style record")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      const auto data_clean = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x04"sv;
      REQUIRE( data.size() == data_clean.size() );
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RegionRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "Grazelands Region" );
      REQUIRE( record.RegionName == "Weidenl\xE4nder" );
      REQUIRE( record.Clear == 30 );
      REQUIRE( record.Cloudy == 40 );
      REQUIRE( record.Foggy == 5 );
      REQUIRE( record.Overcast == 5 );
      REQUIRE( record.Rain == 10 );
      REQUIRE( record.Thunder == 10 );
      REQUIRE( record.Ash == 0 );
      REQUIRE( record.Blight == 0 );
      REQUIRE( record.Snow == 0 );
      REQUIRE( record.Blizzard == 0 );
      REQUIRE( record.SleepCreature == "ex_grazelands_sleep" );
      REQUIRE( record.Red == 255 );
      REQUIRE( record.Green == 177 );
      REQUIRE( record.Blue == 32 );
      REQUIRE( record.Zero == 0 );
      REQUIRE( record.SoundChances.size() == 5 );
      REQUIRE( record.SoundChances[0].Sound == "wind calm1" );
      REQUIRE( record.SoundChances[0].Chance == 4 );
      REQUIRE( record.SoundChances[1].Sound == "wind calm2" );
      REQUIRE( record.SoundChances[1].Chance == 4 );
      REQUIRE( record.SoundChances[2].Sound == "wind calm3" );
      REQUIRE( record.SoundChances[2].Chance == 4 );
      REQUIRE( record.SoundChances[3].Sound == "wind calm4" );
      REQUIRE( record.SoundChances[3].Chance == 4 );
      REQUIRE( record.SoundChances[4].Sound == "wind calm5" );
      REQUIRE( record.SoundChances[4].Chance == 4 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      // Morrowind's implementation writes some garbage data after the sound
      // names, so we check against a cleaned version of the original data.
      REQUIRE( streamOut.str() == data_clean );
    }

    SECTION("default: load Bloodmoon-style record")
    {
      const auto data = "REGN\x4A\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0Thirsk Region\0FNAM\x0E\0\0\0Thirsk-Region\0WEAT\x0A\0\0\0\x14\x19#\x14\0\0\0\0\0\0CNAM\x04\0\0\0\xFE\x93\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RegionRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "Thirsk Region" );
      REQUIRE( record.RegionName == "Thirsk-Region" );
      REQUIRE( record.Clear == 20 );
      REQUIRE( record.Cloudy == 25 );
      REQUIRE( record.Foggy == 35 );
      REQUIRE( record.Overcast == 20 );
      REQUIRE( record.Rain == 0 );
      REQUIRE( record.Thunder == 0 );
      REQUIRE( record.Ash == 0 );
      REQUIRE( record.Blight == 0 );
      REQUIRE( record.Snow == 0 );
      REQUIRE( record.Blizzard == 0 );
      REQUIRE( record.SleepCreature.empty() );
      REQUIRE( record.Red == 254 );
      REQUIRE( record.Green == 147 );
      REQUIRE( record.Blue == 1 );
      REQUIRE( record.Zero == 0 );
      REQUIRE( record.SoundChances.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut, true) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0FAIL\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\x01\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Graze"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FAIL\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\x01\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "REGN\xE2\0\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\xED\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no WEAT")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0FAIL\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of WEAT is not eight")
    {
      {
        const auto data = "REGN\x33\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x07\0\0\0\x1E(\x05\x05\x0A\x0A\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "REGN\x35\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x09\0\0\0\x1E(\x05\x05\x0A\x0A\0\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: length of WEAT is not ten")
    {
      {
        const auto data = "REGN\x49\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0Thirsk Region\0FNAM\x0E\0\0\0Thirsk-Region\0WEAT\x09\0\0\0\x14\x19#\x14\0\0\0\0\0CNAM\x04\0\0\0\xFE\x93\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "REGN\x4B\0\0\0\0\0\0\0\0\0\0\0NAME\x0E\0\0\0Thirsk Region\0FNAM\x0E\0\0\0Thirsk-Region\0WEAT\x0B\0\0\0\x14\x19#\x14\0\0\0\0\0\0\0CNAM\x04\0\0\0\xFE\x93\x01\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before WEAT can be read")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no BNAM")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0FAIL\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM > 256")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\x01\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM is beyond stream")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_graze"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no CNAM")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0FAIL\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM is not four")
    {
      {
        const auto data = "REGN\x33\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x03\0\0\0\xFF\xB1 SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "REGN\x35\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x05\0\0\0\xFF\xB1 \0\0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before CNAM can be read")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SNAM")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0FAIL!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM is not 33")
    {
      {
        const auto data = "REGN\x33\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM\x20\0\0\0wind calm5\0\0\0\0\0\0\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM\x22\0\0\0wind calm5\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip REGN, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RegionRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: length of SNAM is beyond stream")
    {
      const auto data = "REGN\x34\x01\0\0\0\0\0\0\0\0\0\0NAME\x12\0\0\0Grazelands Region\0FNAM\x0D\0\0\0Weidenl\xE4nder\0WEAT\x08\0\0\0\x1E(\x05\x05\x0A\x0A\0\0BNAM\x14\0\0\0ex_grazelands_sleep\0CNAM\x04\0\0\0\xFF\xB1 \0SNAM!\0\0\0wind calm1\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm2\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm3\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind calm4\0\0\0\0\0\0\xFC\xD6\xE8\0\x98\x09m\0\xA0\xF3U\0`m\xE8\0\x04SNAM!\0\0\0wind ca"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip REGN, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RegionRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
