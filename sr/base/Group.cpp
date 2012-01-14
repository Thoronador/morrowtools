/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "Group.h"
#include <iostream>

namespace SRTP
{

Group::Group()
{
  //empty
}

Group::~Group()
{
  //empty
}

bool Group::saveToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Group::saveToStream: Error: bad stream!\n";
    return false;
  }
  if (!headerData.saveToStream(output))
  {
    std::cout << "Group::saveToStream: Error: could not write group header!\n";
    return false;
  }
  if (!contents.saveToStream(output))
  {
    std::cout << "Group::saveToStream: Error: could not write group content to stream!\n";
    return false;
  }
  return true;
}

void Group::updateGroupSize()
{
  //basic size for every group is 24 bytes, rest is records
  headerData.setGroupSize(24 + contents.getContentSize());
}

Group * Group::determineLatestGroup(const unsigned int level)
{
  //level zero means no group
  if (level==0) return NULL;
  //level one is the first valid level, meaning the current group
  if (level==1) return this;
  //level is larger than one, so we have to go into sub groups
  //If there are no groups, we can't return anything useful.
  if (contents.getNumberOfSubGroups()==0) return NULL;
  return contents.getSubGroupAtIndexNC(contents.getNumberOfSubGroups()-1)->determineLatestGroup(level-1);
}

} //namespace
