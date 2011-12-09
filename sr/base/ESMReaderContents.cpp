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
#include "records/AssociationTypeRecord.h"
#include "records/BookRecord.h"
#include "records/CraftableObjectRecord.h"
#include "records/EyeRecord.h"
#include "records/FormListRecord.h"
#include "records/GMSTRecord.h"
#include "records/GlobalRecord.h"
#include "records/KeywordRecord.h"
#include "records/MagicEffectRecord.h"
#include "records/MiscObjectRecord.h"
#include "records/OutfitRecord.h"
#include "records/RelationshipRecord.h"
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

int ESMReaderContents::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  BasicRecord * recPtr = NULL;
  switch (recName)
  {
    /*case cACTI:
         recPtr = new ActivatorRecord;
         break;*/
    case cASTP:
         recPtr = new AssociationTypeRecord;
         break;
    case cBOOK:
         recPtr = new BookRecord;
         break;
    case cCOBJ:
         recPtr = new CraftableObjectRecord;
         break;
    case cEYES:
         recPtr = new EyeRecord;
         break;
    case cFLST:
         recPtr = new FormListRecord;
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
    case cRELA:
         recPtr = new RelationshipRecord;
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
    contents.addRecord(recPtr);
    return 1; //success
  }
  else
  {
    delete recPtr;
    recPtr = NULL;
    return -1; //error
  }
  //we should never get to this point
  throw 42;
}

} //namespace
