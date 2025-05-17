/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013, 2025  Dirk Stolle

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

#include "ESMWriterContents.hpp"

#ifndef SR_UNSAVEABLE_RECORDS
namespace SRTP
{

ESMWriterContents::ESMWriterContents(ESMFileContents& storage)
: contents(storage)
{
}

uint32_t ESMWriterContents::getTotalNumberOfRecords() const
{
  uint32_t result = 0;
  for (const auto& group: contents.m_Groups)
  {
    result += group.getNumberOfRecordsIncludingSubGroups();
  }
  return result;
}

uint32_t ESMWriterContents::getTotalNumberOfGroups() const
{
  uint32_t result = contents.getNumberOfGroups();
  for (const auto& group: contents.m_Groups)
  {
    result += group.getNumberOfGroupsIncludingSubGroups();
  }
  return result;
}

bool ESMWriterContents::writeGroups(std::ofstream& output) const
{
  for (const auto& group: contents.m_Groups)
  {
    if (!group.saveToStream(output))
    {
      return false;
    }
  }
  return true;
}

} // namespace
#endif //SR_UNSAVEABLE_RECORDS
