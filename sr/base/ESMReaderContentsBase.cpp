/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2022  Dirk Stolle

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

#include "ESMReaderContentsBase.hpp"
#include <iostream>
#include <stdexcept>

namespace SRTP
{

ESMReaderContentsBase::ESMReaderContentsBase()
: contents(ESMFileContents()),
  m_InternalGroupLevel(0), m_InternalGroup(nullptr)
{
}

ESMReaderContentsBase::~ESMReaderContentsBase()
{
  contents.removeContents();
  m_InternalGroupLevel = 0;
  m_InternalGroup = nullptr;
}

bool ESMReaderContentsBase::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  /* add a new group to the file content representation and set its GroupData
     (i.e. header) to the stuff that was read from the file stream */
  if (0 == m_InternalGroupLevel)
  {
    Group& newGroup = contents.addNewGroup();
    newGroup.headerData = g_data;
    ++m_InternalGroupLevel;
    m_InternalGroup = contents.determineLatestGroup(m_InternalGroupLevel);
  }
  else
  {
    if (m_InternalGroup == nullptr)
    {
      std::cerr << "ESMReaderContentsBase::nextGroupStarted: Error: Got NULL pointer for internal group!\n";
      // We've screwed up somehow, nice job!
      throw std::runtime_error("ESMReaderContentsBase::nextGroupStarted: Error: Got NULL pointer for internal group!");
      return false;
    }
    // subgroup or next group on that level
    if (sub)
    {
      // create a subgroup of the current group
      m_InternalGroup->addSubGroup(g_data);
      ++m_InternalGroupLevel;
      m_InternalGroup = contents.determineLatestGroup(m_InternalGroupLevel);
    }
    // next group on same level
    else
    {
      Group * parent = contents.determineLatestGroup(m_InternalGroupLevel - 1);
      if (parent == nullptr)
      {
        std::cerr << "ESMReaderContentsBase::nextGroupStarted: Error: Got NULL pointer for parent group!\n";
        throw std::runtime_error("ESMReaderContentsBase::nextGroupStarted: Error: Got NULL pointer for parent group!");
        return false;
      }
      parent->addSubGroup(g_data);
      m_InternalGroup = contents.determineLatestGroup(m_InternalGroupLevel);
    }
  }
  return true;
}

bool ESMReaderContentsBase::groupFinished(const GroupData& g_data)
{
  if (m_InternalGroupLevel > 0)
  {
    --m_InternalGroupLevel;
    m_InternalGroup = contents.determineLatestGroup(m_InternalGroupLevel);
  }
  else
  {
    std::cerr << "ESMReaderContentsBase::groupFinished: Error: Level is already at zero!\n";
    throw std::logic_error("ESMReaderContentsBase::groupFinished: Error: Level is already at zero!");
    return false;
  }
  return true;
}

} // namespace
