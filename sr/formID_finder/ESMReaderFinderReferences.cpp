/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#include "ESMReaderFinderReferences.hpp"
#include "../base/Cells.hpp"
#include "../base/WorldSpaces.hpp"
#include "../base/SR_Constants.hpp"
#include "../base/records/CharacterReferenceRecord.hpp"
#include "../base/records/SimplifiedReferenceRecord.hpp"

namespace SRTP
{

ESMReaderFinderReferences::CellRefIDPair::CellRefIDPair(const uint32_t cell, const uint32_t ref)
: cellID(cell), refID(ref)
{
}

ESMReaderFinderReferences::ESMReaderFinderReferences(const std::vector<std::string>& loadOrder)
: ESMReaderReIndexMod(loadOrder),
  refMap(std::map<uint32_t, std::vector<CellRefIDPair> >()),
  m_CellStack(std::vector<uint32_t>())
{
}

bool ESMReaderFinderReferences::needGroup(const GroupData& g_data) const
{
  return
          // We want top level cell and worldspace groups.
          ((g_data.type() == GroupData::cTopLevelGroup)
         && ((g_data.label() == cWRLD) || (g_data.label() == cCELL)))
        // ...and non-top level groups, that are not topic-related
        || (g_data.type() != GroupData::cTopicChildren);
}

bool ESMReaderFinderReferences::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  if (indexMapsNeedsUpdate())
  {
    updateIndexMap(m_CurrentMod);
  }

  if (g_data.labelIsCellID())
  {
    // label is cell form ID in that case
    uint32_t cellFormID = g_data.label();
    if (!reIndex(cellFormID))
    {
      std::cerr << "ESMReaderFinderReferences::nextGroupStarted: Warning: could not adjust mod index for cell!\n";
      return false;
    }
    m_CellStack.push_back(cellFormID);
  }
  return true;
}

bool ESMReaderFinderReferences::groupFinished(const GroupData& g_data)
{
  if (g_data.labelIsCellID())
  {
    // label is cell form ID in that case - remove it from "stack"
    uint32_t cellFormID = g_data.label();
    if (!reIndex(cellFormID))
    {
      std::cerr << "ESMReaderFinderReferences::groupFinished: Warning: could not adjust mod index for cell!\n";
      return false;
    }
    if (m_CellStack.back() != cellFormID)
    {
      std::cerr << "ESMReaderFinderReferences::groupFinished: Warning: label does not match stack content!\n";
      return false;
    }
    m_CellStack.pop_back();
  }
  return true;
}

int ESMReaderFinderReferences::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  SimplifiedReferenceRecord * recPtr;
  switch (recName)
  {
    case cCELL:
         {
           CellRecord recC;
           if (!recC.loadFromStream(input, localized, table))
             return -1;
           if (!reIndex(recC.headerFormID))
             return -1;
           Cells::get().addRecord(recC);
           return 1;
         }
         break;
    case cWRLD:
         {
           WorldSpaceRecord recW;
           if (!recW.loadFromStream(input, localized, table))
             return -1;
           if (!reIndex(recW.headerFormID))
             return -1;
           WorldSpaces::get().addRecord(recW);
           return 1;
         }
         break;
    case cREFR:
    case cACHR:
         recPtr = new SimplifiedReferenceRecord;
         break;
    default:
         return skipRecord(input);
         break;
  }
  if (!recPtr->loadFromStream(input, localized, table))
  {
    delete recPtr;
    std::cerr << "ESMReaderFinderReferences::readNextRecord: Error while reading reference record!\n";
    return -1;
  }
  // re-index record's form ID and ID of object
  if ((!reIndex(recPtr->headerFormID)) || (!reIndex(recPtr->baseObjectFormID)))
  {
    delete recPtr;
    return -1;
  }
  // save form IDs in refMap
  refMap[recPtr->baseObjectFormID].push_back(CellRefIDPair(m_CellStack.back(), recPtr->headerFormID));
  delete recPtr;
  return 1;
}

} // namespace
