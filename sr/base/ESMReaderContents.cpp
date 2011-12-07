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

#include "ESMReaderContents.h"
#include <iostream>
#include "SR_Constants.h"
//#include "records/ActivatorRecord.h"
#include "records/BookRecord.h"
#include "records/CraftableObjectRecord.h"
#include "records/EyeRecord.h"
#include "records/GMSTRecord.h"
#include "records/GlobalRecord.h"
#include "records/KeywordRecord.h"
#include "records/MagicEffectRecord.h"
#include "records/MiscObjectRecord.h"
#include "records/OutfitRecord.h"
#include "records/ShoutRecord.h"
#include "records/SoundRecord.h"
#include "records/StaticRecord.h"
#include "records/TextureSetRecord.h"
#include "records/WordOfPowerRecord.h"
#include "records/GenericRecord.h"

namespace SRTP
{

ESMReaderContents::ESMReaderContents()
{
  contents.removeContents();
}

ESMReaderContents::~ESMReaderContents()
{
  contents.removeContents();
}

bool ESMReaderContents::needGroup(const GroupData& g_data) const
{
  return true;
}

int ESMReaderContents::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  int32_t recName;
  BasicRecord * recPtr = NULL;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    switch (recName)
    {
      /*case cACTI:
           recPtr = new ActivatorRecord;
           break;*/
      case cBOOK:
           recPtr = new BookRecord;
           break;
      case cCOBJ:
           recPtr = new CraftableObjectRecord;
           break;
      case cEYES:
           recPtr = new EyeRecord;
           break;
      case cGMST:
           recPtr = new GMSTRecord;
           break;
      case cGLOB:
           recPtr = new GlobalRecord;
           break;
      case cKYWD:
           recPtr = new KeywordRecord;
           break;
      case cMGEF:
           recPtr = new MagicEffectRecord;
           break;
      case cMISC:
           recPtr = new MiscObjectRecord;
           break;
      case cOTFT:
           recPtr = new OutfitRecord;
           break;
      case cSHOU:
           recPtr = new ShoutRecord;
           break;
      case cSOUN:
           recPtr = new SoundRecord;
           break;
      case cSTAT:
           recPtr = new StaticRecord;
           break;
      case cTXST:
           recPtr = new TextureSetRecord;
           break;
      case cWOOP:
           recPtr = new WordOfPowerRecord;
           break;
      default:
           //use generic record type for all other / unimplemented types
           recPtr = new GenericRecord;
           break;
    }//swi
    if (recPtr->loadFromStream(in_File))
    {
      lastResult = 1;
      contents.addRecord(recPtr);
      recordsRead += 1;
    }
    else
    {
      lastResult = -1;
      delete recPtr;
      recPtr = NULL;
    }
  }//while
  if (lastResult>=0)
  {
    if (recordsRead>0) return 1;
    return 0;
  }
  std::cout << "ESMReaderContents::readGroup: Error while reading record!"
            << "\nCurrent position is "<<in_File.tellg()<<" bytes.\n";
  return -1;
}

} //namespace
