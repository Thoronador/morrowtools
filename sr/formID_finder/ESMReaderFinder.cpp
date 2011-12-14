/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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
#include "../base/Ammunitions.h"
#include "../base/Books.h"
#include "../base/MiscObjects.h"
#include "../base/Perks.h"
#include "../base/Scrolls.h"
#include "../base/Shouts.h"
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
    //case cALCH:
    case cAMMO:
    //case cAPPA:
    //case cARMO:
    case cBOOK:
    //case cINGR:
    //case cKEYM:
    case cMISC:
    case cPERK:
    case cSCRL:
    case cSHOU:
    case cSPEL:
    case cWEAP:
    case cWOOP:
         return true;
         break;
  }//swi
  return false;
}

int ESMReaderFinder::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  #warning Not completely implemented yet!
  switch (recName)
  {
    case cAMMO:
         return Ammunitions::getSingleton().readNextRecord(in_File);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File);
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
