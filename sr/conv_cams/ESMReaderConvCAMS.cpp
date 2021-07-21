/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "ESMReaderConvCAMS.hpp"
#include "../base/SR_Constants.hpp"
#include "../base/records/CameraShotRecord.hpp"
#include "../base/records/GenericRecord.hpp"
#include "../base/records/NAVIRecord.hpp"

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

int ESMReaderConvCAMS::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
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
  if (recPtr->loadFromStream(in_File, localized, table))
  {
    contents.m_Groups.back().addRecord(recPtr);
    return 1; //success
  }
  delete recPtr;
  recPtr = NULL;
  return -1; //error
}

} //namespace
