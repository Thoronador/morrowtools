/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "ESMFileContents.hpp"
#include <iostream>

namespace SRTP
{

ESMFileContents::ESMFileContents()
: m_Groups(std::vector<Group>())
{
}

Group& ESMFileContents::addNewGroup()
{
  Group temp;
  m_Groups.push_back(temp);
  return m_Groups.back();
}

unsigned int ESMFileContents::getNumberOfGroups() const
{
  return m_Groups.size();
}

void ESMFileContents::removeContents()
{
  m_Groups.clear();
}

unsigned int ESMFileContents::purgeEmptyGroups()
{
  unsigned int purged = 0;
  std::vector<Group>::iterator iter = m_Groups.begin();
  while (iter != m_Groups.end())
  {
    purged += iter->purgeEmptySubGroups();
    // top group
    if (iter->isEmpty())
    {
      iter = m_Groups.erase(iter);
      ++purged;
    }
    else
    {
      ++iter;
    }
  }
  return purged;
}

void ESMFileContents::traverseGroups(traverseFunction func) const
{
  if (func == nullptr)
  {
    std::cerr << "ESMFileContents::traverseGroups: ERROR: Pointer to function"
              << " is NULL!\n";
    return;
  }

  for (const auto& group: m_Groups)
  {
    // top-level group
    if (!(*func)(group, nullptr))
      return;
    // now go into sub groups
    if (!traverseSubGroups(group, func))
      return;
  }
  return;
}

bool ESMFileContents::traverseSubGroups(const Group& grp, traverseFunction func) const
{
  SRTP::Group::SubIterator groupIter = grp.getSubBegin();
  while (groupIter != grp.getSubEnd())
  {
    if (!(*func)(*groupIter, &grp))
      return false;
    // subs
    if (!traverseSubGroups(*groupIter, func))
      return false;
    ++groupIter;
  }
  return true;
}

Group * ESMFileContents::determineLatestGroup(const unsigned int level)
{
  // level zero means no group
  if (level == 0)
    return nullptr;
  // level one is the first valid level, meaning the current one
  // If there are no groups, we can't return anything useful.
  if (m_Groups.empty())
    return nullptr;
  if (level == 1)
    return &(m_Groups.back());
  // level is larger than one, so we have to go into sub groups
  return m_Groups.back().determineLatestGroup(level - 1);
}

} // namespace
