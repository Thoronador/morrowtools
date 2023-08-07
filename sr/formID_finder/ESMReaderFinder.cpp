/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "ESMReaderFinder.hpp"
#include "../../lib/sr/SR_Constants.hpp"
#include "../../lib/sr/Activators.hpp"
#include "../../lib/sr/AlchemyPotions.hpp"
#include "../../lib/sr/Ammunitions.hpp"
#include "../../lib/sr/Apparatuses.hpp"
#include "../../lib/sr/Armours.hpp"
#include "../../lib/sr/Books.hpp"
#include "../../lib/sr/Containers.hpp"
#include "../../lib/sr/Factions.hpp"
#include "../../lib/sr/Floras.hpp"
#include "../../lib/sr/Furniture.hpp"
#include "../../lib/sr/Ingredients.hpp"
#include "../../lib/sr/Keys.hpp"
#include "../../lib/sr/MiscObjects.hpp"
#include "../../lib/sr/NPCs.hpp"
#include "../../lib/sr/Perks.hpp"
#include "../../lib/sr/Quests.hpp"
#include "../../lib/sr/Scrolls.hpp"
#include "../../lib/sr/Shouts.hpp"
#include "../../lib/sr/SoulGems.hpp"
#include "../../lib/sr/Spells.hpp"
#include "../../lib/sr/TalkingActivators.hpp"
#include "../../lib/sr/Trees.hpp"
#include "../../lib/sr/Weapons.hpp"
#include "../../lib/sr/WordsOfPower.hpp"

namespace SRTP
{

ESMReaderFinder::ESMReaderFinder(const std::vector<std::string>& loadOrder)
: ESMReaderReIndexMod(loadOrder)
{
}

bool ESMReaderFinder::needGroup(const GroupData& g_data) const
{
  switch (g_data.label())
  {
    case cACTI:
    case cALCH:
    case cAMMO:
    case cAPPA:
    case cARMO:
    case cBOOK:
    case cCONT:
    case cFACT:
    case cFLOR:
    case cFURN:
    case cINGR:
    case cKEYM:
    case cMISC:
    case cNPC_:
    case cPERK:
    case cQUST:
    case cSCRL:
    case cSHOU:
    case cSLGM:
    case cSPEL:
    case cTACT:
    case cTREE:
    case cWEAP:
    case cWOOP:
         return true;
         break;
  }
  return false;
}

bool ESMReaderFinder::nextGroupStarted([[maybe_unused]] const GroupData& g_data,
                                       [[maybe_unused]] const bool sub)
{
  if (indexMapsNeedsUpdate())
  {
    updateIndexMap(m_CurrentMod);
  }
  return true;
}

bool ESMReaderFinder::groupFinished([[maybe_unused]] const GroupData& g_data)
{
  return true;
}

int ESMReaderFinder::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  BasicRecord * recPtr = nullptr;
  switch (recName)
  {
    case cACTI:
         recPtr = new ActivatorRecord;
         break;
    case cALCH:
         recPtr = new AlchemyPotionRecord;
         break;
    case cAMMO:
         recPtr = new AmmunitionRecord;
         break;
    case cAPPA:
         recPtr = new ApparatusRecord;
         break;
    case cARMO:
         recPtr = new ArmourRecord;
         break;
    case cBOOK:
         recPtr = new BookRecord;
         break;
    case cCONT:
         recPtr = new ContainerRecord;
         break;
    case cFACT:
         recPtr = new FactionRecord;
         break;
    case cFLOR:
         recPtr = new FloraRecord;
         break;
    case cFURN:
         recPtr = new FurnitureRecord;
         break;
    case cINGR:
         recPtr = new IngredientRecord;
         break;
    case cKEYM:
         recPtr = new KeyRecord;
         break;
    case cMISC:
         recPtr = new MiscObjectRecord;
         break;
    case cNPC_:
         recPtr = new NPCRecord;
         break;
    case cPERK:
         recPtr = new PerkRecord;
         break;
    case cQUST:
         recPtr = new QuestRecord;
         break;
    case cSCRL:
         recPtr = new ScrollRecord;
         break;
    case cSHOU:
         recPtr = new ShoutRecord;
         break;
    case cSLGM:
         recPtr = new SoulGemRecord;
         break;
    case cSPEL:
         recPtr = new SpellRecord;
         break;
    case cTACT:
         recPtr = new TalkingActivatorRecord;
         break;
    case cTREE:
         recPtr = new TreeRecord;
         break;
    case cWEAP:
         recPtr = new WeaponRecord;
         break;
    case cWOOP:
         recPtr = new WordOfPowerRecord;
         break;
    default:
         return -1;
  }
  // load record
  if (!recPtr->loadFromStream(input, localized, table))
  {
    delete recPtr;
    return -1;
  }
  // re-index record's form ID
  if (!reIndex(recPtr->headerFormID))
  {
    delete recPtr;
    return -1;
  }
  // add record
  switch (recName)
  {
    case cACTI:
         Activators::get().addRecord(*static_cast<ActivatorRecord*>(recPtr));
         break;
    case cALCH:
         AlchemyPotions::get().addRecord(*static_cast<AlchemyPotionRecord*>(recPtr));
         break;
    case cAMMO:
         Ammunitions::get().addRecord(*static_cast<AmmunitionRecord*>(recPtr));
         break;
    case cAPPA:
         Apparatuses::get().addRecord(*static_cast<ApparatusRecord*>(recPtr));
         break;
    case cARMO:
         Armours::get().addRecord(*static_cast<ArmourRecord*>(recPtr));
         break;
    case cBOOK:
         Books::get().addRecord(*static_cast<BookRecord*>(recPtr));
         break;
    case cCONT:
         Containers::get().addRecord(*static_cast<ContainerRecord*>(recPtr));
         break;
    case cFACT:
         Factions::get().addRecord(*static_cast<FactionRecord*>(recPtr));
         break;
    case cFLOR:
         Floras::get().addRecord(*static_cast<FloraRecord*>(recPtr));
         break;
    case cFURN:
         Furniture::get().addRecord(*static_cast<FurnitureRecord*>(recPtr));
         break;
    case cINGR:
         Ingredients::get().addRecord(*static_cast<IngredientRecord*>(recPtr));
         break;
    case cKEYM:
         Keys::get().addRecord(*static_cast<KeyRecord*>(recPtr));
         break;
    case cMISC:
         MiscObjects::get().addRecord(*static_cast<MiscObjectRecord*>(recPtr));
         break;
    case cNPC_:
         NPCs::get().addRecord(*static_cast<NPCRecord*>(recPtr));
         break;
    case cPERK:
         Perks::get().addRecord(*static_cast<PerkRecord*>(recPtr));
         break;
    case cQUST:
         Quests::get().addRecord(*static_cast<QuestRecord*>(recPtr));
         break;
    case cSCRL:
         Scrolls::get().addRecord(*static_cast<ScrollRecord*>(recPtr));
         break;
    case cSHOU:
         Shouts::get().addRecord(*static_cast<ShoutRecord*>(recPtr));
         break;
    case cSLGM:
         SoulGems::get().addRecord(*static_cast<SoulGemRecord*>(recPtr));
         break;
    case cSPEL:
         Spells::get().addRecord(*static_cast<SpellRecord*>(recPtr));
         break;
    case cTACT:
          TalkingActivators::get().addRecord(*static_cast<TalkingActivatorRecord*>(recPtr));
         break;
    case cTREE:
         Trees::get().addRecord(*static_cast<TreeRecord*>(recPtr));
         break;
    case cWEAP:
         Weapons::get().addRecord(*static_cast<WeaponRecord*>(recPtr));
         break;
    case cWOOP:
         WordsOfPower::get().addRecord(*static_cast<WordOfPowerRecord*>(recPtr));
         break;
    default:
         std::cerr << "ESMReaderFinder::readNextRecord: Cannot add unknown record type!\n";
         delete recPtr;
         return -1;
  }
  delete recPtr;
  return 1;
}

} // namespace
