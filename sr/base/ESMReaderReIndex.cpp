/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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
: ESMReader(), m_CurrentMod(""), m_LoadOrder(loadOrder), m_MapIsUpToDate(false),
  m_IndexMap(std::map<uint8_t, uint8_t>())
{
  //empty
}

ESMReaderReIndexMod::~ESMReaderReIndexMod()
{
  //empty
}

void ESMReaderReIndexMod::requestIndexMapUpdate(const std::string& currentModFile)
{
  m_MapIsUpToDate = false;
  m_CurrentMod = currentModFile;
}

bool ESMReaderReIndexMod::indexMapsNeedsUpdate() const
{
  return (!m_MapIsUpToDate);
}

bool ESMReaderReIndexMod::updateIndexMap(const std::string& currentModFile)
{
  //We can't work without load order, so tell them!
  if (m_LoadOrder.empty())
  {
    std::cout << "ESMReaderReIndexMod::updateIndexMap: Error: no load order given!\n";
    return false;
  }
  //More than 255 files won't work either.
  if (m_LoadOrder.size()>255)
  {
    std::cout << "ESMReaderReIndexMod::updateIndexMap: Error: more than 255 files in load order!\n";
    return false;
  }
  //Same here.
  if (currentHead.dependencies.size()>255)
  {
    std::cout << "ESMReaderReIndexMod::updateIndexMap: Error: more than 255 dependencies!\n";
    return false;
  }

  //clear map
  m_IndexMap.clear();
  uint8_t curIndex = 0;
  std::vector<Tes4HeaderRecord::MasterFile>::const_iterator mf_iter = currentHead.dependencies.begin();
  while (mf_iter!=currentHead.dependencies.end())
  {
    std::vector<std::string>::const_iterator lo_iter = std::find(m_LoadOrder.begin(), m_LoadOrder.end(), mf_iter->fileName);
    if (lo_iter==m_LoadOrder.end())
    {
      std::cout << "ESMReaderReIndexMod::updateIndexMap: Error: load order does not contain \""
                << mf_iter->fileName << "\"!\n";
      return false;
    }//if
    m_IndexMap[curIndex] = (lo_iter - m_LoadOrder.begin());
    ++curIndex;
    ++mf_iter;
  }//while
  //current file's index
  std::vector<std::string>::const_iterator lo_iter = std::find(m_LoadOrder.begin(), m_LoadOrder.end(), currentModFile);
  if (lo_iter==m_LoadOrder.end())
  {
    std::cout << "ESMReaderReIndexMod::updateIndexMap: Error: load order does not contain \""
              << currentModFile << "\"!\n";
    return false;
  }//if
  m_IndexMap[curIndex] = (lo_iter - m_LoadOrder.begin());
  m_MapIsUpToDate = true;
  return true;
}

bool ESMReaderReIndexMod::reIndex(uint32_t& formID) const
{
  const std::map<uint8_t, uint8_t>::const_iterator mapIter = m_IndexMap.find(extractModIndex(formID));
  if (mapIter==m_IndexMap.end())
  {
    std::cout << "ESMReaderReIndexMod::reIndex: Error: index "<<(unsigned int) extractModIndex(formID)
              << " not found in map!\n";
    return false;
  }
  changeModIndexInSitu(formID, mapIter->second);
  return true;
}

} //namespace
