/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2014, 2021  Dirk Stolle

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

#include "MagicEffects.hpp"
#include <iostream>
#include <stdexcept>

namespace MWTP
{

MagicEffects::MagicEffects()
: m_Effects(std::map<EffectIndex, MagicEffectRecord>())
{
}

MagicEffects& MagicEffects::get()
{
  static MagicEffects Instance;
  return Instance;
}

void MagicEffects::addEffect(const MagicEffectRecord& effect)
{
  m_Effects[effect.Index] = effect;
}

bool MagicEffects::hasEffect(const EffectIndex index) const
{
  return m_Effects.find(index) != m_Effects.end();
}

unsigned int MagicEffects::getNumberOfEffects() const
{
  return m_Effects.size();
}

const MagicEffectRecord& MagicEffects::getEffect(const EffectIndex index) const
{
  const auto iter = m_Effects.find(index);
  if (iter != m_Effects.end())
  {
    return iter->second;
  }
  // nothing found
  std::cerr << "No magic effect with index " << static_cast<uint32_t>(index) << " found!\n";
  throw std::runtime_error("MagicEffects::getEffect(): No magic effect for given index found!");
}

std::string MagicEffects::getSettingNameForEffect(const EffectIndex index)
{
  switch (index)
  {
    case EffectIndex::AbsorbAttribute:
         return "sEffectAbsorbAttribute";
    case EffectIndex::AbsorbFatigue:
         return "sEffectAbsorbFatigue";
    case EffectIndex::AbsorbHealth:
         return "sEffectAbsorbHealth";
    case EffectIndex::AbsorbSkill:
         return "sEffectAbsorbSkill";
    case EffectIndex::AbsorbSpellPoints:
         return "sEffectAbsorbSpellPoints";
    case EffectIndex::AlmsiviIntervention:
         return "sEffectAlmsiviIntervention";
    case EffectIndex::Blind:
         return "sEffectBlind";
    case EffectIndex::BoundBattleAxe:
         return "sEffectBoundBattleAxe";
    case EffectIndex::BoundBoots:
         return "sEffectBoundBoots";
    case EffectIndex::BoundCuirass:
         return "sEffectBoundCuirass";
    case EffectIndex::BoundDagger:
         return "sEffectBoundDagger";
    case EffectIndex::BoundGloves:
         return "sEffectBoundGloves";
    case EffectIndex::BoundHelm:
         return "sEffectBoundHelm";
    case EffectIndex::BoundLongbow:
         return "sEffectBoundLongbow";
    case EffectIndex::BoundLongsword:
         return "sEffectBoundLongsword";
    case EffectIndex::BoundMace:
         return "sEffectBoundMace";
    case EffectIndex::BoundShield:
         return "sEffectBoundShield";
    case EffectIndex::BoundSpear:
         return "sEffectBoundSpear";
    case EffectIndex::Burden:
         return "sEffectBurden";
    case EffectIndex::CalmCreature:
         return "sEffectCalmCreature";
    case EffectIndex::CalmHumanoid:
         return "sEffectCalmHumanoid";
    case EffectIndex::Chameleon:
         return "sEffectChameleon";
    case EffectIndex::Charm:
         return "sEffectCharm";
    case EffectIndex::CommandCreatures:
         return "sEffectCommandCreatures";
    case EffectIndex::CommandHumanoids:
         return "sEffectCommandHumanoids";
    case EffectIndex::Corprus:
         return "sEffectCorpus";
    case EffectIndex::CureBlightDisease:
         return "sEffectCureBlightDisease";
    case EffectIndex::CureCommonDisease:
         return "sEffectCureCommonDisease";
    case EffectIndex::CureCorprusDisease:
         return "sEffectCureCorprusDisease";
    case EffectIndex::CureParalyzation:
         return "sEffectCureParalyzation";
    case EffectIndex::CurePoison:
         return "sEffectCurePoison";
    case EffectIndex::DamageAttribute:
         return "sEffectDamageAttribute";
    case EffectIndex::DamageFatigue:
         return "sEffectDamageFatigue";
    case EffectIndex::DamageHealth:
         return "sEffectDamageHealth";
    case EffectIndex::DamageMagicka:
         return "sEffectDamageMagicka";
    case EffectIndex::DamageSkill:
         return "sEffectDamageSkill";
    case EffectIndex::DemoralizeCreature:
         return "sEffectDemoralizeCreature";
    case EffectIndex::DemoralizeHumanoid:
         return "sEffectDemoralizeHumanoid";
    case EffectIndex::DetectAnimal:
         return "sEffectDetectAnimal";
    case EffectIndex::DetectEnchantment:
         return "sEffectDetectEnchantment";
    case EffectIndex::DetectKey:
         return "sEffectDetectKey";
    case EffectIndex::DisintegrateWeapon:
         return "sEffectDisintegrateWeapon";
    case EffectIndex::DisintegrateArmor:
         return "sEffectDisintegrateArmor";
    case EffectIndex::Dispel:
         return "sEffectDispel";
    case EffectIndex::DivineIntervention:
         return "sEffectDivineIntervention";
    case EffectIndex::DrainAttribute:
         return "sEffectDrainAttribute";
    case EffectIndex::DrainFatigue:
         return "sEffectDrainFatigue";
    case EffectIndex::DrainHealth:
         return "sEffectDrainHealth";
    case EffectIndex::DrainSkill:
         return "sEffectDrainSkill";
    case EffectIndex::DrainSpellpoints:
         return "sEffectDrainSpellpoints";
    case EffectIndex::ExtraSpell:
         return "sEffectExtraSpell";
    case EffectIndex::Feather:
         return "sEffectFeather";
    case EffectIndex::FireDamage:
         return "sEffectFireDamage";
    case EffectIndex::FireShield:
         return "sEffectFireShield";
    case EffectIndex::FortifyAttackBonus:
         return "sEffectFortifyAttackBonus";
    case EffectIndex::FortifyAttribute:
         return "sEffectFortifyAttribute";
    case EffectIndex::FortifyFatigue:
         return "sEffectFortifyFatigue";
    case EffectIndex::FortifyHealth:
         return "sEffectFortifyHealth";
    case EffectIndex::FortifyMagickaMultiplier:
         return "sEffectFortifyMagickaMultiplier";
    case EffectIndex::FortifySkill:
         return "sEffectFortifySkill";
    case EffectIndex::FortifySpellpoints:
         return "sEffectFortifySpellpoints";
    case EffectIndex::FrenzyCreature:
         return "sEffectFrenzyCreature";
    case EffectIndex::FrenzyHumanoid:
         return "sEffectFrenzyHumanoid";
    case EffectIndex::FrostDamage:
         return "sEffectFrostDamage";
    case EffectIndex::FrostShield:
         return "sEffectFrostShield";
    case EffectIndex::Invisibility:
         return "sEffectInvisibility";
    case EffectIndex::Jump:
         return "sEffectJump";
    case EffectIndex::Levitate:
         return "sEffectLevitate";
    case EffectIndex::Light:
         return "sEffectLight";
    case EffectIndex::LightningShield:
         return "sEffectLightningShield";
    case EffectIndex::Lock:
         return "sEffectLock";
    case EffectIndex::Mark:
         return "sEffectMark";
    case EffectIndex::NightEye:
         return "sEffectNightEye";
    case EffectIndex::Open:
         return "sEffectOpen";
    case EffectIndex::Paralyze:
         return "sEffectParalyze";
    case EffectIndex::Poison:
         return "sEffectPoison";
    case EffectIndex::RallyCreature:
         return "sEffectRallyCreature";
    case EffectIndex::RallyHumanoid:
         return "sEffectRallyHumanoid";
    case EffectIndex::Recall:
         return "sEffectRecall";
    case EffectIndex::Reflect:
         return "sEffectReflect";
    case EffectIndex::RemoveCurse:
         return "sEffectRemoveCurse";
    case EffectIndex::ResistBlightDisease:
         return "sEffectResistBlightDisease";
    case EffectIndex::ResistCommonDisease:
         return "sEffectResistCommonDisease";
    case EffectIndex::ResistCorprusDisease:
         return "sEffectResistCorprusDisease";
    case EffectIndex::ResistFire:
         return "sEffectResistFire";
    case EffectIndex::ResistFrost:
         return "sEffectResistFrost";
    case EffectIndex::ResistMagicka:
         return "sEffectResistMagicka";
    case EffectIndex::ResistNormalWeapons:
         return "sEffectResistNormalWeapons";
    case EffectIndex::ResistParalysis:
         return "sEffectResistParalysis";
    case EffectIndex::ResistPoison:
         return "sEffectResistPoison";
    case EffectIndex::ResistShock:
         return "sEffectResistShock";
    case EffectIndex::RestoreAttribute:
         return "sEffectRestoreAttribute";
    case EffectIndex::RestoreFatigue:
         return "sEffectRestoreFatigue";
    case EffectIndex::RestoreHealth:
         return "sEffectRestoreHealth";
    case EffectIndex::RestoreSkill:
         return "sEffectRestoreSkill";
    case EffectIndex::RestoreSpellPoints:
         return "sEffectRestoreSpellPoints";
    case EffectIndex::Sanctuary:
         return "sEffectSanctuary";
    case EffectIndex::Shield:
         return "sEffectShield";
    case EffectIndex::ShockDamage:
         return "sEffectShockDamage";
    case EffectIndex::Silence:
         return "sEffectSilence";
    case EffectIndex::SlowFall:
         return "sEffectSlowFall";
    case EffectIndex::Soultrap:
         return "sEffectSoultrap";
    case EffectIndex::Sound:
         return "sEffectSound";
    case EffectIndex::SpellAbsorption:
         return "sEffectSpellAbsorption";
    case EffectIndex::StuntedMagicka:
         return "sEffectStuntedMagicka";
    case EffectIndex::SummonAncestralGhost:
         return "sEffectSummonAncestralGhost";
    case EffectIndex::SummonBonelord:
         return "sEffectSummonBonelord";
    case EffectIndex::SummonCenturionSphere:
         return "sEffectSummonCenturionSphere";
    case EffectIndex::SummonClannfear:
         return "sEffectSummonClannfear";
    case EffectIndex::SummonDaedroth:
         return "sEffectSummonDaedroth";
    case EffectIndex::SummonDremora:
         return "sEffectSummonDremora";
    case EffectIndex::SummonFlameAtronach:
         return "sEffectSummonFlameAtronach";
    case EffectIndex::SummonFrostAtronach:
         return "sEffectSummonFrostAtronach";
    case EffectIndex::SummonGoldenSaint:
         return "sEffectSummonGoldensaint";
    case EffectIndex::SummonGreaterBonewalker:
         return "sEffectSummonGreaterBonewalker";
    case EffectIndex::SummonHunger:
         return "sEffectSummonHunger";
    case EffectIndex::SummonLeastBonewalker:
         return "sEffectSummonLeastBonewalker";
    case EffectIndex::SummonScamp:
         return "sEffectSummonScamp";
    case EffectIndex::SummonSkeletalMinion:
         return "sEffectSummonSkeletalMinion";
    case EffectIndex::SummonStormAtronach:
         return "sEffectSummonStormAtronach";
    case EffectIndex::SummonWingedTwilight:
         return "sEffectSummonWingedTwilight";
    case EffectIndex::SunDamage:
         return "sEffectSunDamage";
    case EffectIndex::SwiftSwim:
         return "sEffectSwiftSwim";
    case EffectIndex::Telekinesis:
         return "sEffectTelekinesis";
    case EffectIndex::TurnUndead:
         return "sEffectTurnUndead";
    case EffectIndex::Vampirism:
         return "sEffectVampirism";
    case EffectIndex::WaterBreathing:
         return "sEffectWaterBreathing";
    case EffectIndex::WaterWalking:
         return "sEffectWaterWalking";
    case EffectIndex::WeaknessToBlightDisease:
         return "sEffectWeaknessToBlightDisease";
    case EffectIndex::WeaknessToCommonDisease:
         return "sEffectWeaknessToCommonDisease";
    case EffectIndex::WeaknessToCorprusDisease:
         return "sEffectWeaknessToCorprusDisease";
    case EffectIndex::WeaknessToFire:
         return "sEffectWeaknessToFire";
    case EffectIndex::WeaknessToFrost:
         return "sEffectWeaknessToFrost";
    case EffectIndex::WeaknessToMagicka:
         return "sEffectWeaknessToMagicka";
    case EffectIndex::WeaknessToNormalWeapons:
         return "sEffectWeaknessToNormalWeapons";
    case EffectIndex::WeaknessToPoison:
         return "sEffectWeaknessToPoison";
    case EffectIndex::WeaknessToShock:
         return "sEffectWeaknessToShock";
  }
  std::cerr << "Error: No setting defined for effect " << static_cast<uint32_t>(index) << "!\n";
  throw std::runtime_error("MagicEffects::getSettingNameForEffect(): Error: No setting defined for effect index!\n");
}

bool MagicEffects::isSkillRelatedEffect(const EffectIndex index)
{
  return (index == EffectIndex::DrainSkill)
     || (index == EffectIndex::DamageSkill)
     || (index == EffectIndex::RestoreSkill)
     || (index == EffectIndex::FortifySkill)
     || (index == EffectIndex::AbsorbSkill);
}

bool MagicEffects::isAttributeRelatedEffect(const EffectIndex index)
{
  return (index == EffectIndex::DrainAttribute)
      || (index == EffectIndex::DamageAttribute)
      || (index == EffectIndex::RestoreAttribute)
      || (index == EffectIndex::FortifyAttribute)
      || (index == EffectIndex::AbsorbAttribute);
}

int MagicEffects::readNextRecord(std::istream& input)
{
  MagicEffectRecord tempData;
  if (!tempData.loadFromStream(input))
  {
    std::cerr << "MagicEffects::readNextRecord: Error while reading record.\n";
    return -1;
  }
  if (hasEffect(tempData.Index))
  {
    // Are the contents the same?
    if (getEffect(tempData.Index).equals(tempData))
    {
      // same content, nothing changed here
      return 0;
    }
  }
  // no effect with that index present or different content, so just add it
  addEffect(tempData);
  return 1;
}

MagicEffects::Iterator MagicEffects::begin() const
{
  return m_Effects.begin();
}

MagicEffects::Iterator MagicEffects::end() const
{
  return m_Effects.end();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool MagicEffects::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cerr << "MagicEffects::saveAllToStream: Error: Bad stream.\n";
    return false;
  }
  for (const auto& [idx, effect]: m_Effects)
  {
    if (!effect.saveToStream(output))
    {
      std::cerr << "MagicEffects::saveAllToStream: Error while writing record for index "
                << static_cast<uint32_t>(idx) << ".\n";
      return false;
    }
  }
  return true;
}
#endif // MW_UNSAVEABLE_RECORDS

void MagicEffects::clear()
{
  m_Effects.clear();
}

} // namespace
