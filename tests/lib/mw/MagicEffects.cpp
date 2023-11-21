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
#include "../../../lib/mw/MagicEffects.hpp"

TEST_CASE("MWTP::MagicEffects")
{
  using namespace MWTP;

  SECTION("get (Singleton)")
  {
    auto& one = MagicEffects::get();
    auto& two = MagicEffects::get();

    REQUIRE( &one == &two );
  }

  SECTION("addEffect / hasEffect / getEffect")
  {
    auto& mgr = MagicEffects::get();
    mgr.clear();

    MagicEffectRecord recordOne;
    recordOne.Index = EffectIndex::FireDamage;
    recordOne.SpellSchool = MagicSchool::Destruction;

    MagicEffectRecord recordTwo;
    recordTwo.Index = EffectIndex::RestoreHealth;
    recordOne.SpellSchool = MagicSchool::Restoration;

    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::FireDamage) );
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::RestoreHealth) );
    mgr.addEffect(recordOne);
    REQUIRE( mgr.getNumberOfEffects() == 1 );
    REQUIRE( mgr.hasEffect(EffectIndex::FireDamage) );
    REQUIRE( mgr.getEffect(EffectIndex::FireDamage).equals(recordOne) );
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::RestoreHealth) );
    mgr.addEffect(recordTwo);
    REQUIRE( mgr.getNumberOfEffects() == 2 );
    REQUIRE( mgr.hasEffect(EffectIndex::FireDamage) );
    REQUIRE( mgr.hasEffect(EffectIndex::RestoreHealth) );
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::SummonSkeletalMinion) );
    REQUIRE( mgr.getEffect(EffectIndex::RestoreHealth).equals(recordTwo) );
  }

  SECTION("getEffect throws when index is not present")
  {
    auto& mgr = MagicEffects::get();
    mgr.clear();

    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::SummonScamp) );
    REQUIRE_THROWS( mgr.getEffect(EffectIndex::SummonScamp) );
  }

  SECTION("clear")
  {
    auto& mgr = MagicEffects::get();
    mgr.clear();

    MagicEffectRecord recordOne;
    recordOne.Index = EffectIndex::FireDamage;
    recordOne.SpellSchool = MagicSchool::Destruction;

    MagicEffectRecord recordTwo;
    recordTwo.Index = EffectIndex::RestoreHealth;
    recordOne.SpellSchool = MagicSchool::Restoration;

    MagicEffectRecord recordThree;
    recordThree.Index = EffectIndex::ResistShock;
    recordThree.SpellSchool = MagicSchool::Alteration;

    mgr.addEffect(recordOne);
    mgr.addEffect(recordTwo);
    mgr.addEffect(recordThree);
    REQUIRE( mgr.hasEffect(EffectIndex::FireDamage) );
    REQUIRE( mgr.hasEffect(EffectIndex::RestoreHealth) );
    REQUIRE( mgr.hasEffect(EffectIndex::ResistShock) );

    mgr.clear();
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::FireDamage) );
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::RestoreHealth) );
    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::ResistShock) );
  }

  SECTION("getNumberOfEffects")
  {
    auto& mgr = MagicEffects::get();
    mgr.clear();

    MagicEffectRecord recordOne;
    recordOne.Index = EffectIndex::FireDamage;
    recordOne.SpellSchool = MagicSchool::Destruction;

    MagicEffectRecord recordTwo;
    recordTwo.Index = EffectIndex::RestoreHealth;
    recordOne.SpellSchool = MagicSchool::Restoration;

    MagicEffectRecord recordThree;
    recordThree.Index = EffectIndex::ResistShock;
    recordThree.SpellSchool = MagicSchool::Alteration;

    REQUIRE( mgr.getNumberOfEffects() == 0 );
    mgr.addEffect(recordOne);
    REQUIRE( mgr.getNumberOfEffects() == 1 );
    mgr.addEffect(recordTwo);
    REQUIRE( mgr.getNumberOfEffects() == 2 );
    mgr.addEffect(recordThree);
    REQUIRE( mgr.getNumberOfEffects() == 3 );


    MagicEffectRecord recordFour;
    recordFour.Index = EffectIndex::Telekinesis;
    recordFour.SpellSchool = MagicSchool::Mysticism;

    MagicEffectRecord recordFive;
    recordFive.Index = EffectIndex::SummonAncestralGhost;
    recordFive.SpellSchool = MagicSchool::Conjuration;

    mgr.addEffect(recordFour);
    mgr.addEffect(recordFive);
    REQUIRE( mgr.getNumberOfEffects() == 5 );
  }

  SECTION("begin / end")
  {
    auto& mgr = MagicEffects::get();
    mgr.clear();

    MagicEffectRecord recordOne;
    recordOne.Index = EffectIndex::FireDamage;
    recordOne.SpellSchool = MagicSchool::Destruction;

    MagicEffectRecord recordTwo;
    recordTwo.Index = EffectIndex::RestoreHealth;
    recordOne.SpellSchool = MagicSchool::Restoration;

    MagicEffectRecord recordThree;
    recordThree.Index = EffectIndex::ResistShock;
    recordThree.SpellSchool = MagicSchool::Alteration;

    REQUIRE( mgr.begin() == mgr.end() );

    mgr.addEffect(recordOne);
    mgr.addEffect(recordTwo);
    mgr.addEffect(recordThree);

    REQUIRE_FALSE( mgr.begin() == mgr.end() );

    auto iter = mgr.begin();
    REQUIRE( iter->first == EffectIndex::FireDamage );
    ++iter;
    REQUIRE( iter->first == EffectIndex::RestoreHealth );
    ++iter;
    REQUIRE( iter->first == EffectIndex::ResistShock );
    ++iter;
    REQUIRE( iter == mgr.end() );

    mgr.clear();
    REQUIRE( mgr.begin() == mgr.end() );
  }

  SECTION("readNextRecord + saveToStream: basic stuff")
  {
    using namespace std::string_view_literals;
    const auto data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = MagicEffects::get();
    mgr.clear();

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasEffect(EffectIndex::RemoveCurse) );
    REQUIRE( mgr.getEffect(EffectIndex::RemoveCurse).EffectIcon == "s\\Tx_S_remcurse.tga" );
    REQUIRE( mgr.getNumberOfEffects() == 1 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: reading the same record twice still yields only one record")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches.MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = MagicEffects::get();
    mgr.clear();

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  0 );

    REQUIRE( mgr.hasEffect(EffectIndex::RemoveCurse) );
    REQUIRE( mgr.getEffect(EffectIndex::RemoveCurse).EffectIcon == "s\\Tx_S_remcurse.tga" );
    REQUIRE( mgr.getNumberOfEffects() == 1 );
  }

  SECTION("readNextRecord + saveToStream: multiple records")
  {
    using namespace std::string_view_literals;
    const auto data = "MGEF\x12\x02\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0Y\0\0\0MEDT$\0\0\0\x04\0\0\0\0\0\0@\0\x06\0\0\xFF\0\0\0\xDF\0\0\0\xFF\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_Ab_Skill.tga\0PTEX\x15\0\0\0vfx_myst_flare01.tga\0BSND\x11\0\0\0Restoration Bolt\0CSND\x11\0\0\0Restoration Cast\0HSND\x10\0\0\0Restoration Hit\0ASND\x11\0\0\0Restoration Area\0CVFX\x12\0\0\0VFX_MysticismCast\0BVFX\x12\0\0\0VFX_MysticismBolt\0HVFX\x11\0\0\0VFX_MysticismHit\0AVFX\x12\0\0\0VFX_MysticismArea\0DESC\xCF\0\0\0Verringert zeitweilig eine Fertigkeit des Opfers und steigert die des Zaubernden. Nach Ablauf des Effekts verliert der Zaubernde die zus\xE4tzlichen Fertigkeitspunkte, w\xE4hrend das Opfer sie wieder hinzugewinnt.MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0d\0\0\0MEDT$\0\0\0\x05\0\0\0\0\0pA\0\0\0\0\x82\0\0\0\xA4\0\0\0\xFD\0\0\0\0\0\x80?\0\0\x80?\0\0HBITEX\x14\0\0\0s\\Tx_S_remcurse.tga\0PTEX\x12\0\0\0vfx_bluecloud.tga\0CVFX\x14\0\0\0VFX_RestorationCast\0BVFX\x10\0\0\0VFX_RestoreBolt\0HVFX\x13\0\0\0VFX_RestorationHit\0AVFX\x14\0\0\0VFX_RestorationArea\0DESC@\0\0\0Befreit das Ziel des Zaubers von den Auswirkungen eines Fluches."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = MagicEffects::get();
    mgr.clear();

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasEffect(EffectIndex::AbsorbSkill) );
    REQUIRE( mgr.getEffect(EffectIndex::AbsorbSkill).EffectIcon == "s\\Tx_S_Ab_Skill.tga" );
    REQUIRE( mgr.getNumberOfEffects() == 1 );

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasEffect(EffectIndex::RemoveCurse) );
    REQUIRE( mgr.getEffect(EffectIndex::RemoveCurse).EffectIcon == "s\\Tx_S_remcurse.tga" );
    REQUIRE( mgr.getNumberOfEffects() == 2 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: failure")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "MGEF\x21\x01\0\0\0\0\0\0\0\0\0\0FAIL"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = MagicEffects::get();
    mgr.clear();

    // Skip MGEF, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) == -1 );

    REQUIRE_FALSE( mgr.hasEffect(EffectIndex::RemoveCurse) );
    REQUIRE( mgr.getNumberOfEffects() == 0 );
  }

  SECTION("saveAllToStream: failure with bad stream")
  {
    std::stringstream stream;
    stream.str(std::string("bla"));

    uint32_t dummy = 0;
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE_FALSE( stream.good() );
    // Writing stuff should fail.
    auto& mgr = MagicEffects::get();
    REQUIRE_FALSE( mgr.saveAllToStream(stream) );
  }

  SECTION("isSkillRelatedEffect")
  {
    // checks from WaterBreathing to DrainFatigue
    for (uint32_t i = 0; i < 21; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(idx) );
    }
    REQUIRE( MagicEffects::isSkillRelatedEffect(EffectIndex::DrainSkill) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::DamageAttribute) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::DamageHealth) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::DamageMagicka) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::DamageFatigue) );
    REQUIRE( MagicEffects::isSkillRelatedEffect(EffectIndex::DamageSkill) );
    // Poison to RestoreFatigue
    for (uint32_t i = 27; i < 78; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(idx) );
    }
    REQUIRE( MagicEffects::isSkillRelatedEffect(EffectIndex::RestoreSkill) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifyAttribute) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifyHealth) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifySpellpoints) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifyFatigue) );
    REQUIRE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifySkill) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::FortifyMagickaMultiplier) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::AbsorbAttribute) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::AbsorbHealth) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::AbsorbSpellPoints) );
    REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(EffectIndex::AbsorbFatigue) );
    REQUIRE( MagicEffects::isSkillRelatedEffect(EffectIndex::AbsorbSkill) );
    // ResistFire to StuntedMagicka
    for (uint32_t i = 90; i <= 136; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isSkillRelatedEffect(idx) );
    }
  }

  SECTION("isAttributeRelatedEffect")
  {
    // checks from WaterBreathing to FrostDamage
    for (uint32_t i = 0; i < 17; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(idx) );
    }
    REQUIRE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DrainAttribute) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DrainHealth) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DrainSpellpoints) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DrainFatigue) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DrainSkill) );
    REQUIRE( MagicEffects::isAttributeRelatedEffect(EffectIndex::DamageAttribute) );
    // checks from DamageHealth to CureParalyzation
    for (uint32_t i = 23; i < 74; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(idx) );
    }
    REQUIRE( MagicEffects::isAttributeRelatedEffect(EffectIndex::RestoreAttribute) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::RestoreHealth) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::RestoreSpellPoints) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::RestoreFatigue) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::RestoreSkill) );
    REQUIRE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifyAttribute) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifyHealth) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifySpellpoints) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifyFatigue) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifySkill) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifyFatigue) );
    REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(EffectIndex::FortifyMagickaMultiplier) );
    REQUIRE( MagicEffects::isAttributeRelatedEffect(EffectIndex::AbsorbAttribute) );
    // checks from AbsorbHealth to CureParalyzation
    for (uint32_t i = 86; i <= 136; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      REQUIRE_FALSE( MagicEffects::isAttributeRelatedEffect(idx) );
    }
  }

  SECTION("getSettingNameForEffect: known effects")
  {
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WaterBreathing) == "sEffectWaterBreathing" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SwiftSwim) == "sEffectSwiftSwim" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WaterWalking) == "sEffectWaterWalking" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Shield) == "sEffectShield" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FireShield) == "sEffectFireShield" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::LightningShield) == "sEffectLightningShield" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FrostShield) == "sEffectFrostShield" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Burden) == "sEffectBurden" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Feather) == "sEffectFeather" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Jump) == "sEffectJump" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Levitate) == "sEffectLevitate" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SlowFall) == "sEffectSlowFall" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Lock) == "sEffectLock" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Open) == "sEffectOpen" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FireDamage) == "sEffectFireDamage" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ShockDamage) == "sEffectShockDamage" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FrostDamage) == "sEffectFrostDamage" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DrainAttribute) == "sEffectDrainAttribute" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DrainHealth) == "sEffectDrainHealth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DrainSpellpoints) == "sEffectDrainSpellpoints" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DrainFatigue) == "sEffectDrainFatigue" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DrainSkill) == "sEffectDrainSkill" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DamageAttribute) == "sEffectDamageAttribute" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DamageHealth) == "sEffectDamageHealth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DamageMagicka) == "sEffectDamageMagicka" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DamageFatigue) == "sEffectDamageFatigue" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DamageSkill) == "sEffectDamageSkill" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Poison) == "sEffectPoison" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToFire) == "sEffectWeaknessToFire" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToFrost) == "sEffectWeaknessToFrost" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToShock) == "sEffectWeaknessToShock" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToMagicka) == "sEffectWeaknessToMagicka" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToCommonDisease) == "sEffectWeaknessToCommonDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToBlightDisease) == "sEffectWeaknessToBlightDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToCorprusDisease) == "sEffectWeaknessToCorprusDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToPoison) == "sEffectWeaknessToPoison" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::WeaknessToNormalWeapons) == "sEffectWeaknessToNormalWeapons" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DisintegrateWeapon) == "sEffectDisintegrateWeapon" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DisintegrateArmor) == "sEffectDisintegrateArmor" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Invisibility) == "sEffectInvisibility" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Chameleon) == "sEffectChameleon" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Light) == "sEffectLight" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Sanctuary) == "sEffectSanctuary" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::NightEye) == "sEffectNightEye" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Charm) == "sEffectCharm" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Paralyze) == "sEffectParalyze" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Silence) == "sEffectSilence" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Blind) == "sEffectBlind" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Sound) == "sEffectSound" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CalmHumanoid) == "sEffectCalmHumanoid" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CalmCreature) == "sEffectCalmCreature" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FrenzyHumanoid) == "sEffectFrenzyHumanoid" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FrenzyCreature) == "sEffectFrenzyCreature" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DemoralizeHumanoid) == "sEffectDemoralizeHumanoid" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DemoralizeCreature) == "sEffectDemoralizeCreature" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RallyHumanoid) == "sEffectRallyHumanoid" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RallyCreature) == "sEffectRallyCreature" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Dispel) == "sEffectDispel" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Soultrap) == "sEffectSoultrap" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Telekinesis) == "sEffectTelekinesis" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Mark) == "sEffectMark" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Recall) == "sEffectRecall" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DivineIntervention) == "sEffectDivineIntervention" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AlmsiviIntervention) == "sEffectAlmsiviIntervention" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DetectAnimal) == "sEffectDetectAnimal" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DetectEnchantment) == "sEffectDetectEnchantment" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::DetectKey) == "sEffectDetectKey" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SpellAbsorption) == "sEffectSpellAbsorption" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Reflect) == "sEffectReflect" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CureCommonDisease) == "sEffectCureCommonDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CureBlightDisease) == "sEffectCureBlightDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CureCorprusDisease) == "sEffectCureCorprusDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CurePoison) == "sEffectCurePoison" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CureParalyzation) == "sEffectCureParalyzation" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RestoreAttribute) == "sEffectRestoreAttribute" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RestoreHealth) == "sEffectRestoreHealth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RestoreSpellPoints) == "sEffectRestoreSpellPoints" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RestoreFatigue) == "sEffectRestoreFatigue" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RestoreSkill) == "sEffectRestoreSkill" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifyAttribute) == "sEffectFortifyAttribute" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifyHealth) == "sEffectFortifyHealth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifySpellpoints) == "sEffectFortifySpellpoints" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifyFatigue) == "sEffectFortifyFatigue" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifySkill) == "sEffectFortifySkill" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifyMagickaMultiplier) == "sEffectFortifyMagickaMultiplier" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AbsorbAttribute) == "sEffectAbsorbAttribute" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AbsorbHealth) == "sEffectAbsorbHealth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AbsorbSpellPoints) == "sEffectAbsorbSpellPoints" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AbsorbFatigue) == "sEffectAbsorbFatigue" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::AbsorbSkill) == "sEffectAbsorbSkill" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistFire) == "sEffectResistFire" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistFrost) == "sEffectResistFrost" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistShock) == "sEffectResistShock" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistMagicka) == "sEffectResistMagicka" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistCommonDisease) == "sEffectResistCommonDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistBlightDisease) == "sEffectResistBlightDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistCorprusDisease) == "sEffectResistCorprusDisease" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistPoison) == "sEffectResistPoison" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistNormalWeapons) == "sEffectResistNormalWeapons" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ResistParalysis) == "sEffectResistParalysis" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::RemoveCurse) == "sEffectRemoveCurse" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::TurnUndead) == "sEffectTurnUndead" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonScamp) == "sEffectSummonScamp" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonClannfear) == "sEffectSummonClannfear" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonDaedroth) == "sEffectSummonDaedroth" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonDremora) == "sEffectSummonDremora" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonAncestralGhost) == "sEffectSummonAncestralGhost" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonSkeletalMinion) == "sEffectSummonSkeletalMinion" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonLeastBonewalker) == "sEffectSummonLeastBonewalker" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonGreaterBonewalker) == "sEffectSummonGreaterBonewalker" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonBonelord) == "sEffectSummonBonelord" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonWingedTwilight) == "sEffectSummonWingedTwilight" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonHunger) == "sEffectSummonHunger" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonGoldenSaint) == "sEffectSummonGoldensaint" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonFlameAtronach) == "sEffectSummonFlameAtronach" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonFrostAtronach) == "sEffectSummonFrostAtronach" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonStormAtronach) == "sEffectSummonStormAtronach" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::FortifyAttackBonus) == "sEffectFortifyAttackBonus" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CommandCreatures) == "sEffectCommandCreatures" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::CommandHumanoids) == "sEffectCommandHumanoids" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundDagger) == "sEffectBoundDagger" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundLongsword) == "sEffectBoundLongsword" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundMace) == "sEffectBoundMace" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundBattleAxe) == "sEffectBoundBattleAxe" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundSpear) == "sEffectBoundSpear" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundLongbow) == "sEffectBoundLongbow" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::ExtraSpell) == "sEffectExtraSpell" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundCuirass) == "sEffectBoundCuirass" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundHelm) == "sEffectBoundHelm" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundBoots) == "sEffectBoundBoots" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundShield) == "sEffectBoundShield" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::BoundGloves) == "sEffectBoundGloves" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Corprus) == "sEffectCorpus" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::Vampirism) == "sEffectVampirism" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SummonCenturionSphere) == "sEffectSummonCenturionSphere" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::SunDamage) == "sEffectSunDamage" );
    REQUIRE( MagicEffects::getSettingNameForEffect(EffectIndex::StuntedMagicka) == "sEffectStuntedMagicka" );
  }

  SECTION("getSettingNameForEffect: throw on unknown index")
  {
    const auto index = static_cast<EffectIndex>(150);
    REQUIRE_THROWS( MagicEffects::getSettingNameForEffect(index) );
  }
}
