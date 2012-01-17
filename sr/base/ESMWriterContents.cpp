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

#include "ESMWriterContents.h"

namespace SRTP
{

ESMWriterContents::ESMWriterContents()
{
  contents.removeContents();
}

ESMWriterContents::~ESMWriterContents()
{
  contents.removeContents();
}

uint32_t ESMWriterContents::getTotalNumberOfRecords() const
{
  uint32_t result = 0;
  unsigned int i;
  for (i=0; i<contents.m_Groups.size(); ++i)
  {
    result += contents.m_Groups[i].getNumberOfRecords();
  }//for
  return result;
}

uint32_t ESMWriterContents::getTotalNumberOfGroups() const
{
  return contents.getNumberOfGroups();
}

bool ESMWriterContents::writeGroups(std::ofstream& output) const
{
  const unsigned int groupCount = contents.getNumberOfGroups();
  unsigned int i;
  for (i=0; i<groupCount; ++i)
  {
    if (!contents.m_Groups[i].saveToStream(output))
    {
      return false;
    }
  }//for
  return true;
}

} //namespace
