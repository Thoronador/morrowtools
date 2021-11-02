/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2015, 2021  Thoronador

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

#include "AuxFunctions.hpp"
#include "../../base/UtilityFunctions.hpp"
#include "../base/Cells.hpp"
#include "../base/FormIDFunctions.hpp"

bool matchesKeyword(const std::string& haystack, const std::string& keyword, const bool caseMatters)
{
  if (caseMatters)
  {
    return haystack.find(keyword) != std::string::npos;
  }
  return lowerCase(haystack).find(keyword) != std::string::npos;
}

void showRefIDs(const uint32_t baseID, const std::map<uint32_t, std::vector<SRTP::ESMReaderFinderReferences::CellRefIDPair> >& refMap, std::ostream& basic_out)
{
  basic_out << "        references: ";
  const auto iter = refMap.find(baseID);
  if (iter != refMap.end())
  {
    basic_out << iter->second.size() << "\n";
  }
  else
  {
    basic_out << "none\n";
    return;
  }
  bool hasName = false;
  for (const auto& cellRefPair: iter->second)
  {
    hasName = false;
    basic_out << "          ref ID " << SRTP::getFormIDAsStringXX(cellRefPair.refID);
    if (SRTP::Cells::get().hasRecord(cellRefPair.cellID))
    {
      const SRTP::CellRecord& theCell = SRTP::Cells::get().getRecord(cellRefPair.cellID);
      if (theCell.name.isPresent())
      {
        if (!theCell.name.getString().empty())
        {
          basic_out << " in cell \"" << theCell.name.getString() << "\"\n";
          hasName = true;
        }
      }
      else
      {
        // check for coordinates
        if (theCell.gridLocation.presence)
        {
          basic_out << " in exterior cell [" << theCell.gridLocation.locationX
                    << ";" << theCell.gridLocation.locationY << "]\n";
          hasName = true;
        }
      }
    }
    if (!hasName)
    {
      basic_out << " in unnamed cell\n";
    }
  }
}
