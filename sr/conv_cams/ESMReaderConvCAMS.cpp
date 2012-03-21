/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "ESMReaderConvCAMS.h"
#include "../base/SR_Constants.h"
#include "../base/records/CameraShotRecord.h"
#include "../base/records/GenericRecord.h"
#include "../base/records/NAVIRecord.h"

namespace SRTP
{

ESMReaderConvCAMS::ESMReaderConvCAMS()
{
  contents.removeContents();
}

ESMReaderConvCAMS::~ESMReaderConvCAMS()
{
  contents.removeContents();
}

bool ESMReaderConvCAMS::needGroup(const GroupData& g_data) const
{
  //we want all the stuff
  return true;
}

int ESMReaderConvCAMS::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  BasicRecord * recPtr = NULL;
  switch (recName)
  {
    case cCAMS:
         recPtr = new CameraShotRecord;
         break;
    //we have an extra entry for that type, because this one is too large for
    // GenericRecord's size check limit
    case cNAVI:
         recPtr = new NAVIRecord;
         break;
    default:
         //use generic record type for all other types we don't care about
         recPtr = new GenericRecord;
         break;
  }//swi
  if (recPtr->loadFromStream(in_File))
  {
    contents.m_Groups.back().addRecord(recPtr);
    return 1; //success
  }
  delete recPtr;
  recPtr = NULL;
  return -1; //error
}

} //namespace
