/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2022  Dirk Stolle

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

#include "ESMReaderReIndex.hpp"
#include <algorithm>
#include <iostream>
#include "FormIDFunctions.hpp"

namespace SRTP
{

ESMReaderReIndexMod::ESMReaderReIndexMod(const std::vector<std::string>& loadOrder)
: ESMReader(),
  m_CurrentMod(""),
  m_LoadOrder(loadOrder),
  m_MapIsUpToDate(false),
  m_IndexMap(std::map<uint8_t, uint8_t>())
{
}

void ESMReaderReIndexMod::requestIndexMapUpdate(const std::string& currentModFile)
{
  m_MapIsUpToDate = false;
  m_CurrentMod = currentModFile;
}

bool ESMReaderReIndexMod::indexMapsNeedsUpdate() const
{
  return !m_MapIsUpToDate;
}

bool ESMReaderReIndexMod::updateIndexMap(const std::string& currentModFile)
{
  // We can't work without load order, so tell them!
  if (m_LoadOrder.empty())
  {
    std::cerr << "ESMReaderReIndexMod::updateIndexMap: Error: No load order given!\n";
    return false;
  }
  // More than 255 files won't work either.
  if (m_LoadOrder.size() > 255)
  {
    std::cerr << "ESMReaderReIndexMod::updateIndexMap: Error: More than 255 files in load order!\n";
    return false;
  }
  // Same here.
  if (currentHead.dependencies.size() > 255)
  {
    std::cerr << "ESMReaderReIndexMod::updateIndexMap: Error: More than 255 dependencies!\n";
    return false;
  }

  // clear map
  m_IndexMap.clear();
  uint8_t curIndex = 0;
  for (const auto& masterFile: currentHead.dependencies)
  {
    std::vector<std::string>::const_iterator lo_iter = std::find(m_LoadOrder.begin(), m_LoadOrder.end(), masterFile.fileName);
    if (lo_iter == m_LoadOrder.end())
    {
      std::cerr << "ESMReaderReIndexMod::updateIndexMap: Error: Load order does not contain \""
                << masterFile.fileName << "\"!\n";
      return false;
    }
    m_IndexMap[curIndex] = (lo_iter - m_LoadOrder.begin());
    ++curIndex;
  }
  // current file's index
  const std::vector<std::string>::const_iterator lo_iter = std::find(m_LoadOrder.begin(), m_LoadOrder.end(), currentModFile);
  if (lo_iter == m_LoadOrder.end())
  {
    std::cerr << "ESMReaderReIndexMod::updateIndexMap: Error: Load order does not contain \""
              << currentModFile << "\"!\n";
    return false;
  }
  m_IndexMap[curIndex] = (lo_iter - m_LoadOrder.begin());
  m_MapIsUpToDate = true;
  return true;
}

bool ESMReaderReIndexMod::reIndex(uint32_t& formID) const
{
  const std::map<uint8_t, uint8_t>::const_iterator mapIter = m_IndexMap.find(extractModIndex(formID));
  if (mapIter == m_IndexMap.end())
  {
    std::cerr << "ESMReaderReIndexMod::reIndex: Error: Index "
              << static_cast<unsigned int>(extractModIndex(formID))
              << " not found in map!\n";
    return false;
  }
  changeModIndexInSitu(formID, mapIter->second);
  return true;
}

} // namespace
