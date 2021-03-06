/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "ESMReaderFinder.h"
#include "../base/SR_Constants.h"
#include "../base/Activators.h"
#include "../base/AlchemyPotions.h"
#include "../base/Ammunitions.h"
#include "../base/Apparatuses.h"
#include "../base/Armours.h"
#include "../base/Books.h"
#include "../base/Containers.h"
#include "../base/Factions.h"
#include "../base/Floras.h"
#include "../base/Furniture.h"
#include "../base/Ingredients.h"
#include "../base/Keys.h"
#include "../base/MiscObjects.h"
#include "../base/NPCs.h"
#include "../base/Perks.h"
#include "../base/Quests.h"
#include "../base/Scrolls.h"
#include "../base/Shouts.h"
#include "../base/SoulGems.h"
#include "../base/Spells.h"
#include "../base/TalkingActivators.h"
#include "../base/Trees.h"
#include "../base/Weapons.h"
#include "../base/WordsOfPower.h"

namespace SRTP
{

ESMReaderFinder::ESMReaderFinder(const std::vector<std::string>& loadOrder)
: ESMReaderReIndexMod(loadOrder)
{
  //empty
}

ESMReaderFinder::~ESMReaderFinder()
{
  //empty
}

bool ESMReaderFinder::needGroup(const GroupData& g_data) const
{
  switch (g_data.getGroupLabel())
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
  }//swi
  return false;
}

bool ESMReaderFinder::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  if (indexMapsNeedsUpdate())
  {
    updateIndexMap(m_CurrentMod);
  }
  return true;
}

bool ESMReaderFinder::groupFinished(const GroupData& g_data)
{
  return true;
}

int ESMReaderFinder::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
{
  BasicRecord * recPtr = NULL;
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
  }//swi
  //load record
  if (!recPtr->loadFromStream(in_File, localized, table))
  {
    delete recPtr;
    return -1;
  }
  //re-index record's form ID
  if (!reIndex(recPtr->headerFormID))
  {
    delete recPtr;
    return -1;
  }
  //add record
  switch (recName)
  {
    case cACTI:
         Activators::getSingleton().addRecord(*static_cast<ActivatorRecord*>(recPtr));
         break;
    case cALCH:
         AlchemyPotions::getSingleton().addRecord(*static_cast<AlchemyPotionRecord*>(recPtr));
         break;
    case cAMMO:
         Ammunitions::getSingleton().addRecord(*static_cast<AmmunitionRecord*>(recPtr));
         break;
    case cAPPA:
         Apparatuses::getSingleton().addRecord(*static_cast<ApparatusRecord*>(recPtr));
         break;
    case cARMO:
         Armours::getSingleton().addRecord(*static_cast<ArmourRecord*>(recPtr));
         break;
    case cBOOK:
         Books::getSingleton().addRecord(*static_cast<BookRecord*>(recPtr));
         break;
    case cCONT:
         Containers::getSingleton().addRecord(*static_cast<ContainerRecord*>(recPtr));
         break;
    case cFACT:
         Factions::getSingleton().addRecord(*static_cast<FactionRecord*>(recPtr));
         break;
    case cFLOR:
         Floras::getSingleton().addRecord(*static_cast<FloraRecord*>(recPtr));
         break;
    case cFURN:
         Furniture::getSingleton().addRecord(*static_cast<FurnitureRecord*>(recPtr));
         break;
    case cINGR:
         Ingredients::getSingleton().addRecord(*static_cast<IngredientRecord*>(recPtr));
         break;
    case cKEYM:
         Keys::getSingleton().addRecord(*static_cast<KeyRecord*>(recPtr));
         break;
    case cMISC:
         MiscObjects::getSingleton().addRecord(*static_cast<MiscObjectRecord*>(recPtr));
         break;
    case cNPC_:
         NPCs::getSingleton().addRecord(*static_cast<NPCRecord*>(recPtr));
         break;
    case cPERK:
         Perks::getSingleton().addRecord(*static_cast<PerkRecord*>(recPtr));
         break;
    case cQUST:
         Quests::getSingleton().addRecord(*static_cast<QuestRecord*>(recPtr));
         break;
    case cSCRL:
         Scrolls::getSingleton().addRecord(*static_cast<ScrollRecord*>(recPtr));
         break;
    case cSHOU:
         Shouts::getSingleton().addRecord(*static_cast<ShoutRecord*>(recPtr));
         break;
    case cSLGM:
         SoulGems::getSingleton().addRecord(*static_cast<SoulGemRecord*>(recPtr));
         break;
    case cSPEL:
         Spells::getSingleton().addRecord(*static_cast<SpellRecord*>(recPtr));
         break;
    case cTACT:
          TalkingActivators::getSingleton().addRecord(*static_cast<TalkingActivatorRecord*>(recPtr));
         break;
    case cTREE:
         Trees::getSingleton().addRecord(*static_cast<TreeRecord*>(recPtr));
         break;
    case cWEAP:
         Weapons::getSingleton().addRecord(*static_cast<WeaponRecord*>(recPtr));
         break;
    case cWOOP:
         WordsOfPower::getSingleton().addRecord(*static_cast<WordOfPowerRecord*>(recPtr));
         break;
    default:
         std::cout << "ESMReaderFinder::readNextRecord: cannot add unknown record type!\n";
         delete recPtr;
         return -1;
  }//swi
  delete recPtr;
  return 1;
}

} //namespace
