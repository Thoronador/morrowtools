/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "ESMFileContents.h"

namespace SRTP
{

ESMFileContents::ESMFileContents()
{
  m_Groups.clear();
}

ESMFileContents::~ESMFileContents()
{
  //empty
  //removeContents();
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
  while (iter!=m_Groups.end())
  {
    purged += iter->purgeEmptySubGroups();
    //top group
    if (iter->isEmpty())
    {
      iter = m_Groups.erase(iter);
      ++purged;
    }
    else
    {
      ++iter;
    }
  }//while
  return purged;
}

void ESMFileContents::traverseGroups(traverseFunction func) const
{
  if (func==NULL)
  {
    std::cout << "ESMFileContents::traverseGroups: ERROR: pointer to function"
              << " is NULL!\n";
    return;
  }

  const unsigned int grp_count = m_Groups.size();
  unsigned int i;
  for (i=0; i<grp_count; ++i)
  {
    //top group
    if (!(*func)(m_Groups[i], NULL)) return;
    //now go into sub groups
    if (!traverseSubGroups(m_Groups[i], func)) return;
  }//for
  return;
}

bool ESMFileContents::traverseSubGroups(const Group& grp, traverseFunction func) const
{
  SRTP::Group::SubIterator groupIter = grp.getSubBegin();
  while (groupIter!= grp.getSubEnd())
  {
    if (!(*func)(*groupIter, &grp)) return false;
    //subs
    if (!traverseSubGroups(*groupIter, func)) return false;
    ++groupIter;
  }//while
  return true;
}

Group * ESMFileContents::determineLatestGroup(const unsigned int level)
{
  //level zero means no group
  if (level==0) return NULL;
  //level one is the first valid level, meaning the current one
  //If there are no groups, we can't return anything useful.
  if (m_Groups.empty()) return NULL;
  if (level==1) return &(m_Groups.back());
  //level is larger than one, so we have to go into sub groups
  return m_Groups.back().determineLatestGroup(level-1);
}

} //namespace
