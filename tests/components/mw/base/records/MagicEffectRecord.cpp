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
#include "../../../../../mw/base/records/MagicEffectRecord.hpp"

TEST_CASE("MWTP::MagicEffectRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    MagicEffectRecord record;

    REQUIRE( record.Index == -1 );
    REQUIRE( record.SpellSchool == MagicSchool::Alteration );
    REQUIRE( record.BaseCost == 0.0f );
    REQUIRE( record.Flags == 0 );
    REQUIRE( record.Red == 0 );
    REQUIRE( record.Green == 0 );
    REQUIRE( record.Blue == 0 );
    REQUIRE( record.SpeedX == 0.0f );
    REQUIRE( record.SizeX == 0.0f );
    REQUIRE( record.SizeCap == 0.0f );
    REQUIRE( record.EffectIcon.empty() );
    REQUIRE( record.ParticleTexture.empty() );
    REQUIRE( record.CastingVisual.empty() );
    REQUIRE( record.BoltVisual.empty() );
    REQUIRE( record.HitVisual.empty() );
    REQUIRE( record.AreaVisual.empty() );
    REQUIRE( record.Description.empty() );
    REQUIRE( record.CastSound.empty() );
    REQUIRE( record.BoltSound.empty() );
    REQUIRE( record.HitSound.empty() );
    REQUIRE( record.AreaSound.empty() );
  }

  SECTION("equals")
  {
    MagicEffectRecord a;
    MagicEffectRecord b;

    SECTION("equal")
    {
      a.Index = 1;
      a.SpellSchool = MagicSchool::Destruction;

      b.Index = 1;
      b.SpellSchool = MagicSchool::Destruction;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("Index mismatch")
      {
        a.Index = 1;
        b.Index = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SpellSchool mismatch")
      {
        a.SpellSchool = MagicSchool::Conjuration;
        b.SpellSchool = MagicSchool::Destruction;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BaseCost mismatch")
      {
        a.BaseCost = 1.5f;
        b.BaseCost = 2.5f;

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

      SECTION("Red mismatch")
      {
        a.Red = 1;
        b.Red = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Green mismatch")
      {
        a.Green = 1;
        b.Green = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Blue mismatch")
      {
        a.Blue = 1;
        b.Blue = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SpeedX mismatch")
      {
        a.SpeedX = 1.5f;
        b.SpeedX = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SizeX mismatch")
      {
        a.SizeX = 1.5f;
        b.SizeX = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SizeCap mismatch")
      {
        a.SizeCap = 1.5f;
        b.SizeCap = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EffectIcon mismatch")
      {
        a.EffectIcon = "foo.tga";
        b.EffectIcon = "bar.tga";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ParticleTexture mismatch")
      {
        a.ParticleTexture = "foo.tga";
        b.ParticleTexture = "bar.tga";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CastingVisual mismatch")
      {
        a.CastingVisual = "VFX_foo";
        b.CastingVisual = "VFX_bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BoltVisual mismatch")
      {
        a.BoltVisual = "VFX_foo";
        b.BoltVisual = "VFX_bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HitVisual mismatch")
      {
        a.HitVisual = "VFX_foo";
        b.HitVisual = "VFX_bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AreaVisual mismatch")
      {
        a.AreaVisual = "VFX_foo";
        b.AreaVisual = "VFX_bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Description mismatch")
      {
        a.Description = "blah goes here";
        b.Description = "blah 2";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CastSound mismatch")
      {
        a.CastSound = "A Sound";
        b.CastSound = "Another Sound";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BoltSound mismatch")
      {
        a.BoltSound = "A Sound";
        b.BoltSound = "Another Sound";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("HitSound mismatch")
      {
        a.HitSound = "A Sound";
        b.HitSound = "Another Sound";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AreaSound mismatch")
      {
        a.AreaSound = "A Sound";
        b.AreaSound = "Another Sound";

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
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MagicEffectRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.Index == EffectRemoveCurse );

      REQUIRE( record.SpellSchool == MagicSchool::Restoration );
      REQUIRE( record.BaseCost == 15.0f );
      REQUIRE( record.Flags == 0 );
      REQUIRE( record.Red == 130 );
      REQUIRE( record.Green == 164 );
      REQUIRE( record.Blue == 253 );
      REQUIRE( record.SpeedX == 1.0f );
      REQUIRE( record.SizeX == 1.0f );
      REQUIRE( record.SizeCap == 50.0f );

      REQUIRE( record.EffectIcon == "s\\Tx_S_remcurse.tga" );
      REQUIRE( record.ParticleTexture == "vfx_bluecloud.tga" );
      REQUIRE( record.BoltSound.empty() );
      REQUIRE( record.CastSound.empty() );
      REQUIRE( record.HitSound.empty() );
      REQUIRE( record.AreaSound.empty() );
      REQUIRE( record.CastingVisual == "VFX_RestorationCast" );
      REQUIRE( record.BoltVisual == "VFX_RestoreBolt" );
      REQUIRE( record.HitVisual == "VFX_RestorationHit" );
      REQUIRE( record.AreaVisual == "VFX_RestorationArea" );
      REQUIRE( record.Description == "Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches." );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with sounds")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      MagicEffectRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.Index == EffectAbsorbSkill );

      REQUIRE( record.SpellSchool == MagicSchool::Mysticism );
      REQUIRE( record.BaseCost == 2.0f );
      REQUIRE( record.Flags == 0x00000600 );
      REQUIRE( record.Red == 255 );
      REQUIRE( record.Green == 223 );
      REQUIRE( record.Blue == 255 );
      REQUIRE( record.SpeedX == 1.0f );
      REQUIRE( record.SizeX == 1.0f );
      REQUIRE( record.SizeCap == 50.0f );

      REQUIRE( record.EffectIcon == "s\\Tx_S_Ab_Skill.tga" );
      REQUIRE( record.ParticleTexture == "vfx_myst_flare01.tga" );
      REQUIRE( record.BoltSound == "Restoration Bolt" );
      REQUIRE( record.CastSound == "Restoration Cast" );
      REQUIRE( record.HitSound == "Restoration Hit" );
      REQUIRE( record.AreaSound == "Restoration Area" );
      REQUIRE( record.CastingVisual == "VFX_MysticismCast" );
      REQUIRE( record.BoltVisual == "VFX_MysticismBolt" );
      REQUIRE( record.HitVisual == "VFX_MysticismHit" );
      REQUIRE( record.AreaVisual == "VFX_MysticismArea" );
      REQUIRE( record.Description == "Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt." );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "MGEF\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no INDX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0FAIL\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INDX is not four")
    {
      {
        const auto data = "MGEF\x20\x01\0\0\0\0\0\0\0\0\0\0INDX\x03\0\0\0d\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MGEF, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MagicEffectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "MGEF\x22\x01\0\0\0\0\0\0\0\0\0\0INDX\x05\0\0\0d\0\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MGEF, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MagicEffectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INDX can be read completely")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MEDT")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0FAIL$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MEDT is not 36")
    {
      {
        const auto data = "MGEF\x20\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT\x23\0\0\0\x05\0\0\0\0\0pA\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MGEF, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MagicEffectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "MGEF\x22\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT\x25\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read MGEF, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        MagicEffectRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before MEDT can be read")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBFAIL\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEX")
    {
      const auto data = "MGEF\x3D\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0ITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\x01\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\xFD\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no PTEX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0FAIL\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple PTEX")
    {
      const auto data = "MGEF\x3B\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PTEX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\x01\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PTEX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\xFD\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no BSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0FAIL\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple BSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0BSND\x11\0\0\0Restoration Bolt\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BSND > 256")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\x01\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BSND is beyond stream")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Rest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no CSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0FAIL\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple CSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0CSND\x11\0\0\0Restoration Cast\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CSND > 256")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\x01\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CSND is beyond stream")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Rest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no HSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0FAIL\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple HSND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0HSND\x11\0\0\0Restoration Hit1\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of HSND > 256")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\x01\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of HSND is beyond stream")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Rest"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ASND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0FAIL\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ASND")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0ASND\x10\0\0\0Restoration Aah\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ASND > 256")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\x01\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ASND is beyond stream")
    {
      const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Re"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no CVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0FAIL\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple CVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0CVFX\x10\0\0\0VFX_RestoreCast\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CVFX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\x01\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CVFX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\xFD\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no BVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0FAIL\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple BVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0BVFX\x13\0\0\0VFX_RestorationBlt\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BVFX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\x01\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BVFX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\xFD\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no HVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0FAIL\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple HVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0HVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of HVFX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\x01\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of HVFX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\xFD\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no AVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0FAIL\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple AVFX")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0AVFX\x13\0\0\0VFX_RestorationAAA\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AVFX > 256")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\x01\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AVFX is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\xAD\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DESC")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0FAIL@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple DESC")
    {
      const auto data = "MGEF\x2C\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches.DESC\x03\0\0\0foo"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC > 1024")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\x04\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DESC is beyond stream")
    {
      const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC\xAD\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read MGEF, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      MagicEffectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
