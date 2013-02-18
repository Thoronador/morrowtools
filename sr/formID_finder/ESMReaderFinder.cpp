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

ESMReaderFinder::ESMReaderFinder()
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

void ESMReaderFinder::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  //empty
}

void ESMReaderFinder::groupFinished(const GroupData& g_data)
{
  //empty
}

int ESMReaderFinder::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
{
  #warning Not completely implemented yet!
  switch (recName)
  {
    case cACTI:
         return Activators::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cALCH:
         return AlchemyPotions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cAMMO:
         return Ammunitions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cAPPA:
         return Apparatuses::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cARMO:
         return Armours::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCONT:
         return Containers::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFACT:
         return Factions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFLOR:
         return Floras::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFURN:
         return Furniture::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cINGR:
         return Ingredients::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cKEYM:
         return Keys::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cNPC_:
         return NPCs::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cPERK:
         return Perks::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cQUST:
         return Quests::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSCRL:
         return Scrolls::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSHOU:
         return Shouts::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSLGM:
         return SoulGems::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSPEL:
         return Spells::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cTACT:
         return TalkingActivators::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cTREE:
         return Trees::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWEAP:
         return Weapons::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWOOP:
         return WordsOfPower::getSingleton().readNextRecord(in_File, localized, table);
         break;
  }//swi
  return -1;
}

} //namespace
