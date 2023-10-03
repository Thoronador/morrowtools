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
#include "../../../../lib/mw/records/RaceRecord.hpp"
#include "../../../../lib/mw/SkillConstants.hpp"

TEST_CASE("MWTP::SkillBonus")
{
  using namespace MWTP;

  SECTION("equality operator")
  {
    SECTION("equal")
    {
      SkillBonus a;
      SkillBonus b;

      a.SkillID = SkillRestoration;
      a.Bonus = 5;

      b.SkillID = SkillRestoration;
      b.Bonus = 5;

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("skill mismatch")
      {
        SkillBonus a;
        SkillBonus b;

        a.SkillID = SkillAlteration;
        a.Bonus = 5;

        b.SkillID = SkillRestoration;
        b.Bonus = 5;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("bonus amount mismatch")
      {
        SkillBonus a;
        SkillBonus b;

        a.SkillID = SkillAlteration;
        a.Bonus = 5;

        b.SkillID = SkillAlteration;
        b.Bonus = 15;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }
}

TEST_CASE("MWTP::RaceRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    RaceRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Boni.empty() );
    REQUIRE( record.Strength[0] == 0 );
    REQUIRE( record.Strength[1] == 0 );
    REQUIRE( record.Intelligence[0] == 0 );
    REQUIRE( record.Intelligence[1] == 0 );
    REQUIRE( record.Willpower[0] == 0 );
    REQUIRE( record.Willpower[1] == 0 );
    REQUIRE( record.Agility[0] == 0 );
    REQUIRE( record.Agility[1] == 0 );
    REQUIRE( record.Speed[0] == 0 );
    REQUIRE( record.Speed[1] == 0 );
    REQUIRE( record.Endurance[0] == 0 );
    REQUIRE( record.Endurance[1] == 0 );
    REQUIRE( record.Personality[0] == 0 );
    REQUIRE( record.Personality[1] == 0 );
    REQUIRE( record.Luck[0] == 0 );
    REQUIRE( record.Luck[1] == 0 );
    REQUIRE( record.Weight[0] == 0.0f );
    REQUIRE( record.Weight[1] == 0.0f );
    REQUIRE( record.Height[0] == 1.0f );
    REQUIRE( record.Height[1] == 1.0f );
    REQUIRE( record.Flags == 0 );
    REQUIRE( record.Powers.empty() );
    REQUIRE( record.Description.empty() );
  }

  SECTION("constructor with ID")
  {
    RaceRecord record("bla");

    REQUIRE( record.recordID == "bla" );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Boni.empty() );
    REQUIRE( record.Strength[0] == 0 );
    REQUIRE( record.Strength[1] == 0 );
    REQUIRE( record.Intelligence[0] == 0 );
    REQUIRE( record.Intelligence[1] == 0 );
    REQUIRE( record.Willpower[0] == 0 );
    REQUIRE( record.Willpower[1] == 0 );
    REQUIRE( record.Agility[0] == 0 );
    REQUIRE( record.Agility[1] == 0 );
    REQUIRE( record.Speed[0] == 0 );
    REQUIRE( record.Speed[1] == 0 );
    REQUIRE( record.Endurance[0] == 0 );
    REQUIRE( record.Endurance[1] == 0 );
    REQUIRE( record.Personality[0] == 0 );
    REQUIRE( record.Personality[1] == 0 );
    REQUIRE( record.Luck[0] == 0 );
    REQUIRE( record.Luck[1] == 0 );
    REQUIRE( record.Weight[0] == 0.0f );
    REQUIRE( record.Weight[1] == 0.0f );
    REQUIRE( record.Height[0] == 1.0f );
    REQUIRE( record.Height[1] == 1.0f );
    REQUIRE( record.Flags == 0 );
    REQUIRE( record.Powers.empty() );
    REQUIRE( record.Description.empty() );
  }

  SECTION("equals")
  {
    RaceRecord a;
    RaceRecord b;

    SECTION("equal")
    {
      a.recordID = "ExampleId";
      a.Name = "Foo";

      b.recordID = "ExampleId";
      b.Name = "Foo";

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

      SECTION("Boni mismatch")
      {
        a.Boni.clear();
        b.Boni.push_back(SkillBonus{1, 5});

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Strength mismatch")
      {
        a.Strength[0] = 20;
        b.Strength[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Strength[0] = 20;
        b.Strength[0] = 20;
        a.Strength[1] = 20;
        b.Strength[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Intelligence mismatch")
      {
        a.Intelligence[0] = 20;
        b.Intelligence[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Intelligence[0] = 20;
        b.Intelligence[0] = 20;
        a.Intelligence[1] = 20;
        b.Intelligence[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Willpower mismatch")
      {
        a.Willpower[0] = 20;
        b.Willpower[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Willpower[0] = 20;
        b.Willpower[0] = 20;
        a.Willpower[1] = 20;
        b.Willpower[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Agility mismatch")
      {
        a.Agility[0] = 20;
        b.Agility[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Agility[0] = 20;
        b.Agility[0] = 20;
        a.Agility[1] = 20;
        b.Agility[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Speed mismatch")
      {
        a.Speed[0] = 20;
        b.Speed[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Speed[0] = 20;
        b.Speed[0] = 20;
        a.Speed[1] = 20;
        b.Speed[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Endurance mismatch")
      {
        a.Endurance[0] = 20;
        b.Endurance[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Endurance[0] = 20;
        b.Endurance[0] = 20;
        a.Endurance[1] = 20;
        b.Endurance[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Personality mismatch")
      {
        a.Personality[0] = 20;
        b.Personality[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Personality[0] = 20;
        b.Personality[0] = 20;
        a.Personality[1] = 20;
        b.Personality[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Luck mismatch")
      {
        a.Luck[0] = 20;
        b.Luck[0] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Luck[0] = 20;
        b.Luck[0] = 20;
        a.Luck[1] = 20;
        b.Luck[1] = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Weight mismatch")
      {
        a.Weight[0] = 1.0f;
        b.Weight[0] = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Weight[0] = 1.0f;
        b.Weight[0] = 1.0f;
        a.Weight[1] = 1.0f;
        b.Weight[1] = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Height mismatch")
      {
        a.Height[0] = 1.0f;
        b.Height[0] = 1.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Height[0] = 1.0f;
        b.Height[0] = 1.0f;
        a.Height[1] = 1.0f;
        b.Height[1] = 1.5f;

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

      SECTION("Powers mismatch")
      {
        a.Powers.clear();
        b.Powers.push_back("foo");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Description mismatch")
      {
        a.Description = "blah blah";
        b.Description = "more blah blah";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("less")
  {
    RaceRecord a;
    RaceRecord b;

    SECTION("equal")
    {
      a.recordID = "Altmer";
      b.recordID = "Altmer";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "ALTMER";
      b.recordID = "altmer";

      REQUIRE_FALSE( a < b );
      REQUIRE_FALSE( b < a );

      a.recordID = "altmer";
      b.recordID = "ALTMER";

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
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      RaceRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "Wood Elf" );
      REQUIRE( record.Name == "Waldelf" );
      REQUIRE( record.Boni.size() == 7 );
      REQUIRE( record.Boni[0].SkillID == SkillAcrobatics );
      REQUIRE( record.Boni[0].Bonus == 5 );
      REQUIRE( record.Boni[1].SkillID == SkillAlchemy );
      REQUIRE( record.Boni[1].Bonus == 5 );
      REQUIRE( record.Boni[2].SkillID == SkillLightArmor );
      REQUIRE( record.Boni[2].Bonus == 10 );
      REQUIRE( record.Boni[3].SkillID == SkillMarksman );
      REQUIRE( record.Boni[3].Bonus == 15 );
      REQUIRE( record.Boni[4].SkillID == SkillSneak );
      REQUIRE( record.Boni[4].Bonus == 10 );
      REQUIRE( record.Boni[5].SkillID == -1 );
      REQUIRE( record.Boni[5].Bonus == 0 );
      REQUIRE( record.Boni[6].SkillID == -1 );
      REQUIRE( record.Boni[6].Bonus == 0 );
      REQUIRE( record.Strength[0] == 30 );
      REQUIRE( record.Strength[1] == 30 );
      REQUIRE( record.Intelligence[0] == 40 );
      REQUIRE( record.Intelligence[1] == 40 );
      REQUIRE( record.Willpower[0] == 30 );
      REQUIRE( record.Willpower[1] == 30 );
      REQUIRE( record.Agility[0] == 50 );
      REQUIRE( record.Agility[1] == 50 );
      REQUIRE( record.Speed[0] == 50 );
      REQUIRE( record.Speed[1] == 50 );
      REQUIRE( record.Endurance[0] == 30 );
      REQUIRE( record.Endurance[1] == 30 );
      REQUIRE( record.Personality[0] == 40 );
      REQUIRE( record.Personality[1] == 40 );
      REQUIRE( record.Luck[0] == 40 );
      REQUIRE( record.Luck[1] == 40 );
      REQUIRE( record.Height[0] == 0.9f );
      REQUIRE( record.Height[1] == 1.0f );
      REQUIRE( record.Weight[0] == 0.95f );
      REQUIRE( record.Weight[1] == 0.9f );
      REQUIRE( record.Flags == 0x00000001UL );
      REQUIRE( record.Powers.size() == 2 );
      REQUIRE( record.Powers[0] == "resist disease_75" );
      REQUIRE( record.Powers[1] == "beast tongue" );
      REQUIRE( record.Description == "Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht." );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "RACE\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x46\x41IL\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\x01\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\xF9\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FAIL\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\x01\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x58\0\0\0Waldelf\0RA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no RADT")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0FAIL\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RADT is not 140")
    {
      {
        const auto data = "RACE\xC6\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8B\0\0\0\x14\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "RACE\xC8\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8D\0\0\0\x14\0\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before RADT can be read")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NPCS")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0FAIL \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NPCS is not 32")
    {
      {
        const auto data = "RACE\xC6\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS\x1F\0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS\x21\0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip RACE, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        RaceRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NPCS can be read")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DESC")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0FAIL\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC > 1024")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x04\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht.Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht.Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before DESC can be read")
    {
      const auto data = "RACE\xC7\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man "sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple DESCs")
    {
      const auto data = "RACE\xD2\x02\0\0\0\0\0\0\0\0\0\0\x4E\x41\x4D\x45\x09\0\0\0Wood Elf\0FNAM\x08\0\0\0Waldelf\0RADT\x8C\0\0\0\x14\0\0\0\x05\0\0\0\x10\0\0\0\x05\0\0\0\x15\0\0\0\x0A\0\0\0\x17\0\0\0\x0F\0\0\0\x13\0\0\0\x0A\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0\x1E\0\0\0\x1E\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x32\0\0\0\x1E\0\0\0\x1E\0\0\0(\0\0\0(\0\0\0(\0\0\0(\0\0\0fff?\0\0\x80?33s?fff?\x01\0\0\0NPCS \0\0\0resist disease_75\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0NPCS \0\0\0beast tongue\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0DESC\xBA\x01\0\0Unter Waldelfen versteht man die verschiedenen wilden Elfenclans aus den westlichen Gebieten Valenwalds. Diese mit den Hoch- und Dunkelelfen verwandten Hochelfen sind beh\xE4nde, k\xF6rperlich flink und geistig rege. Wegen ihrer angeborenen Neugier und Geschicklichkeit eignen sie sich besonders gut als Sp\xE4her, Agenten und Diebe. Vor allem aber sind Waldelfen sehr geschickt im Umgang mit dem Bogen. Bessere Sch\xFCtzen gibt es in ganz Tamriel nicht.DESC\x03\0\0\0Foo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip RACE, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      RaceRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("fun with flags")
  {
    RaceRecord record;

    SECTION("isPlayable")
    {
      REQUIRE_FALSE( record.isPlayable() );

      record.Flags = 0x00000001;
      REQUIRE( record.isPlayable() );

      record.Flags = 0x00000002;
      REQUIRE_FALSE( record.isPlayable() );

      record.Flags = 0x0000FFFF;
      REQUIRE( record.isPlayable() );
    }

    SECTION("isBeastRace")
    {
      REQUIRE_FALSE( record.isBeastRace() );

      record.Flags = 0x00000001;
      REQUIRE_FALSE( record.isBeastRace() );

      record.Flags = 0x00000002;
      REQUIRE( record.isBeastRace() );

      record.Flags = 0x0000FFFF;
      REQUIRE( record.isBeastRace() );
    }
  }
}
