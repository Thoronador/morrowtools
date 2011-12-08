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
#include "Eyes.h"
#include "GameSettings.h"
#include "Globals.h"
#include "Keywords.h"
#include "MiscObjects.h"
#include "Outfits.h"
#include "Relationships.h"
#include "Shouts.h"
#include "Sounds.h"
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

int ESMReaderAll::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  int32_t recName;
  GenericRecord * genRec = NULL;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    //read record based on header
    switch (recName)
    {
      case cAACT:
           lastResult = Actions::getSingleton().readNextRecord(in_File);
           break;
      case cACTI:
           lastResult = Activators::getSingleton().readNextRecord(in_File);
           break;
      case cASTP:
           lastResult = AssociationTypes::getSingleton().readNextRecord(in_File);
           break;
      case cBOOK:
           lastResult = Books::getSingleton().readNextRecord(in_File);
           break;
      case cCOBJ:
           lastResult = CraftableObjects::getSingleton().readNextRecord(in_File);
           break;
      case cEYES:
           lastResult = Eyes::getSingleton().readNextRecord(in_File);
           break;
      case cGMST:
           lastResult = GameSettings::getSingleton().readNextRecord(in_File);
           break;
      case cGLOB:
           lastResult = Globals::getSingleton().readNextRecord(in_File);
           break;
      case cKYWD:
           lastResult = Keywords::getSingleton().readNextRecord(in_File);
           break;
      case cMISC:
           lastResult = MiscObjects::getSingleton().readNextRecord(in_File);
           break;
      case cOTFT:
           lastResult = Outfits::getSingleton().readNextRecord(in_File);
           break;
      case cRELA:
           lastResult = Relationships::getSingleton().readNextRecord(in_File);
           break;
      case cSHOU:
           lastResult = Shouts::getSingleton().readNextRecord(in_File);
           break;
      case cSOUN:
           lastResult = Sounds::getSingleton().readNextRecord(in_File);
           break;
      case cSTAT:
           lastResult = Statics::getSingleton().readNextRecord(in_File);
           break;
      case cTXST:
           lastResult = TextureSets::getSingleton().readNextRecord(in_File);
           break;
      case cWEAP:
           lastResult = Weapons::getSingleton().readNextRecord(in_File);
           break;
      case cWOOP:
           lastResult = WordsOfPower::getSingleton().readNextRecord(in_File);
           break;
      default:
           //this branch should not be neccessary once the reader class is finished
           if (encounters.find(recName)==encounters.end())
           {
             std::cout << "Warning: unknown record type \""<< IntTo4Char(recName)<<"\" encountered!\n"
                       << "    File position is "<<(unsigned int) in_File.tellg()<<".\n";
             encounters.insert(recName);
           }
           genRec = new GenericRecord;
           genRec->Header = recName;
           if (genRec->loadFromStream(in_File))
           {
             lastResult = 0;
           }
           else
           {
             lastResult = -1;
           }
           delete genRec;
           genRec = NULL;
           break;
    }//swi

    if (lastResult>0)
    {
      recordsRead += lastResult;
    }
  }//while
  if (lastResult>=0)
  {
    if (recordsRead>0) return 1;
    return 0;
  }
  std::cout << "ESMReaderAll::readGroup: Error while reading record!"
            << "\nCurrent position is "<<in_File.tellg()<<" bytes.\n";
  return -1;
}

bool ESMReaderAll::needGroup(const GroupData& g_data) const
{
  return true;
}

int ESMReaderAll::readNextRecord(std::ifstream& in_File)
{
  #warning Not implemented yet!
  std::cout << "Error: call to unimplemented function ESMReaderAll::readNextRecord!\n";
  return -1;
}

} //namespace
