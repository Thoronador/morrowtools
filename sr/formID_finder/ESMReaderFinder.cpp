/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMReaderFinder.h"
#include "../base/SR_Constants.h"
#include "../base/AlchemyPotions.h"
#include "../base/Ammunitions.h"
#include "../base/Apparatuses.h"
#include "../base/Armours.h"
#include "../base/Books.h"
#include "../base/Ingredients.h"
#include "../base/Keys.h"
#include "../base/MiscObjects.h"
#include "../base/NPCs.h"
#include "../base/Perks.h"
#include "../base/Scrolls.h"
#include "../base/Shouts.h"
#include "../base/SoulGems.h"
#include "../base/Spells.h"
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
  switch (g_data.getGroupName())
  {
    case cALCH:
    case cAMMO:
    case cAPPA:
    case cARMO:
    case cBOOK:
    case cINGR:
    case cKEYM:
    case cMISC:
    case cNPC_:
    case cPERK:
    case cSCRL:
    case cSHOU:
    case cSLGM:
    case cSPEL:
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

int ESMReaderFinder::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  #warning Not completely implemented yet!
  switch (recName)
  {
    case cALCH:
         return AlchemyPotions::getSingleton().readNextRecord(in_File);
         break;
    case cAMMO:
         return Ammunitions::getSingleton().readNextRecord(in_File);
         break;
    case cAPPA:
         return Apparatuses::getSingleton().readNextRecord(in_File);
         break;
    case cARMO:
         return Armours::getSingleton().readNextRecord(in_File);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File);
         break;
    case cINGR:
         return Ingredients::getSingleton().readNextRecord(in_File);
         break;
    case cKEYM:
         return Keys::getSingleton().readNextRecord(in_File);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File);
         break;
    case cNPC_:
         return NPCs::getSingleton().readNextRecord(in_File);
         break;
    case cPERK:
         return Perks::getSingleton().readNextRecord(in_File);
         break;
    case cSCRL:
         return Scrolls::getSingleton().readNextRecord(in_File);
         break;
    case cSHOU:
         return Shouts::getSingleton().readNextRecord(in_File);
         break;
    case cSLGM:
         return SoulGems::getSingleton().readNextRecord(in_File);
         break;
    case cSPEL:
         return Spells::getSingleton().readNextRecord(in_File);
         break;
    case cWEAP:
         return Weapons::getSingleton().readNextRecord(in_File);
         break;
    case cWOOP:
         return WordsOfPower::getSingleton().readNextRecord(in_File);
         break;
  }//swi
  return -1;
}

} //namespace
