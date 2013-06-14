/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#include "ESMWriterContents.h"

#ifndef SR_UNSAVEABLE_RECORDS
namespace SRTP
{

ESMWriterContents::ESMWriterContents()
: contents(ESMFileContents())
{
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
    result += contents.m_Groups[i].getNumberOfRecordsIncludingSubGroups();
  }//for
  return result;
}

uint32_t ESMWriterContents::getTotalNumberOfGroups() const
{
  uint32_t result = contents.getNumberOfGroups();
  unsigned int i;
  for (i=0; i<contents.m_Groups.size(); ++i)
  {
    result += contents.m_Groups[i].getNumberOfGroupsIncludingSubGroups();
  }//for
  return result;
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
#endif //SR_UNSAVEABLE_RECORDS
