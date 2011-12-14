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

#include "ESMReaderAll.h"

#include <iostream>
#include "../../mw/base/HelperIO.h"
#include "SR_Constants.h"
#include "Actions.h"
#include "Activators.h"
#include "AssociationTypes.h"
#include "Books.h"
#include "CraftableObjects.h"
#include "EquipmentSlots.h"
#include "Eyes.h"
#include "FormLists.h"
#include "GameSettings.h"
#include "Globals.h"
#include "Keywords.h"
#include "LandscapeTextures.h"
#include "MiscObjects.h"
#include "Outfits.h"
#include "Perks.h"
#include "Relationships.h"
#include "Scrolls.h"
#include "Shouts.h"
#include "Sounds.h"
#include "Spells.h"
#include "Statics.h"
#include "TextureSets.h"
#include "Weapons.h"
#include "WordsOfPower.h"
#include "records/GenericRecord.h"

namespace SRTP
{

ESMReaderAll::ESMReaderAll()
{
  //empty
}

ESMReaderAll::~ESMReaderAll()
{
  //empty
}

bool ESMReaderAll::needGroup(const GroupData& g_data) const
{
  return true;
}

int ESMReaderAll::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  int result;
  GenericRecord * genRec = NULL;
  switch (recName)
  {
    case cAACT:
         return Actions::getSingleton().readNextRecord(in_File);
         break;
    case cACTI:
         return Activators::getSingleton().readNextRecord(in_File);
         break;
    case cASTP:
         return AssociationTypes::getSingleton().readNextRecord(in_File);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File);
         break;
    case cCOBJ:
         return CraftableObjects::getSingleton().readNextRecord(in_File);
         break;
    case cEQUP:
         return EquipmentSlots::getSingleton().readNextRecord(in_File);
         break;
    case cEYES:
         return Eyes::getSingleton().readNextRecord(in_File);
         break;
    case cFLST:
         return FormLists::getSingleton().readNextRecord(in_File);
         break;
    case cGMST:
         return GameSettings::getSingleton().readNextRecord(in_File);
         break;
    case cGLOB:
         return Globals::getSingleton().readNextRecord(in_File);
         break;
    case cKYWD:
         return Keywords::getSingleton().readNextRecord(in_File);
         break;
    case cLTEX:
         return LandscapeTextures::getSingleton().readNextRecord(in_File);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File);
         break;
    case cOTFT:
         return Outfits::getSingleton().readNextRecord(in_File);
         break;
    case cPERK:
         return Perks::getSingleton().readNextRecord(in_File);
         break;
    case cRELA:
         return Relationships::getSingleton().readNextRecord(in_File);
         break;
    case cSCRL:
         return Scrolls::getSingleton().readNextRecord(in_File);
         break;
    case cSHOU:
         return Shouts::getSingleton().readNextRecord(in_File);
         break;
    case cSOUN:
         return Sounds::getSingleton().readNextRecord(in_File);
         break;
    case cSPEL:
         return Spells::getSingleton().readNextRecord(in_File);
         break;
    case cSTAT:
         return Statics::getSingleton().readNextRecord(in_File);
         break;
    case cTXST:
         return TextureSets::getSingleton().readNextRecord(in_File);
         break;
    case cWEAP:
         return Weapons::getSingleton().readNextRecord(in_File);
         break;
    case cWOOP:
         return WordsOfPower::getSingleton().readNextRecord(in_File);
         break;
    default:
         //this branch should not be neccessary once the reader class is finished
         if (encounters.find(recName)==encounters.end())
         {
           std::cout << "Warning: unknown record type \""<< IntTo4Char(recName)
                     <<"\" encountered!\n    File position is "
                     <<(unsigned int) in_File.tellg()<<".\n";
           encounters.insert(recName);
         }
         genRec = new GenericRecord;
         genRec->Header = recName;
         if (genRec->loadFromStream(in_File))
         {
           result = 0;
         }
         else
         {
           result = -1;
         }
         delete genRec;
         genRec = NULL;
         return result;
         break;
  }//swi
  //we should never get to this point
  throw 42;
}

} //namespace
