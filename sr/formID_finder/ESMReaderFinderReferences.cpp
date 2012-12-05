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

#include "ESMReaderFinderReferences.h"
#include "../base/Cells.h"
#include "../base/WorldSpaces.h"
#include "../base/SR_Constants.h"
#include "../base/records/CharacterReferenceRecord.h"
#include "../base/records/SimplifiedReferenceRecord.h"

namespace SRTP
{

ESMReaderFinderReferences::CellRefIDPair::CellRefIDPair(const uint32_t cell, const uint32_t ref)
{
  cellID = cell;
  refID = ref;
}

ESMReaderFinderReferences::ESMReaderFinderReferences()
{
  refMap.clear();
  m_CellStack.clear();
}

ESMReaderFinderReferences::~ESMReaderFinderReferences()
{
  //empty
}

bool ESMReaderFinderReferences::needGroup(const GroupData& g_data) const
{
  return  (
          //we want top level cell and worldspace groups
          ((g_data.getGroupType()==GroupData::cTopLevelGroup)
         and ((g_data.getGroupLabel()==cWRLD) or (g_data.getGroupLabel()==cCELL)))
        //...and non-top level groups, that are not topic-related
        or (g_data.getGroupType()!=GroupData::cTopicChildren)
        );
}

void ESMReaderFinderReferences::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  if ((g_data.getGroupType()!=GroupData::cTopLevelGroup) and (g_data.getGroupType()!=GroupData::cTopicChildren))
  {
    //label is cell form ID in that case
    m_CellStack.push_back(g_data.getGroupLabel());
  }
}

void ESMReaderFinderReferences::groupFinished(const GroupData& g_data)
{
  if ((g_data.getGroupType()!=GroupData::cTopLevelGroup) and (g_data.getGroupType()!=GroupData::cTopicChildren))
  {
    //label is cell form ID in that case - remove it from "stack"
    if (m_CellStack.back()!=g_data.getGroupLabel())
    {
      std::cout << "ESMReaderFinderReferences::groupFinished: Warning: label does not match stack content!\n";
    }
    m_CellStack.pop_back();
  }
}

int ESMReaderFinderReferences::readNextRecord(std::ifstream& in_File, const uint32_t recName)
{
  #warning Not completely implemented yet!
  SimplifiedReferenceRecord * recPtr;
  switch (recName)
  {
    case cCELL:
         return Cells::getSingleton().readNextRecord(in_File);
         break;
    case cWRLD:
         return WorldSpaces::getSingleton().readNextRecord(in_File);
         break;
    case cREFR:
    case cACHR:
         recPtr = new SimplifiedReferenceRecord;
         break;
    default:
         return skipRecord(in_File);
         break;
  }//swi
  if (!recPtr->loadFromStream(in_File))
  {
    delete recPtr;
    std::cout << "ESMReaderFinderReferences::readNextRecord: Error while reading reference record!\n";
    return -1;
  }
  //save form IDs in refMap
  refMap[recPtr->baseObjectFormID].push_back(CellRefIDPair(m_CellStack.back(), recPtr->headerFormID));
  delete recPtr;
  return 1;
}

} //namespace
