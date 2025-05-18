/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "ESMReaderCount.hpp"
#include <iostream>
#include "../mw/HelperIO.hpp"

namespace SRTP
{

ESMReaderCount::ESMReaderCount()
{
  resetStats();
}

void ESMReaderCount::resetStats()
{
  RecordCounter.clear();
  totalRecords = 0;
  GroupCounter.clear();
  totalGroups = 0;
}

bool ESMReaderCount::needGroup([[maybe_unused]] const GroupData& g_data) const
{
  return true;
}

bool ESMReaderCount::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  // update group stats
  if (g_data.type() == GroupData::cTopLevelGroup)
  {
    // for top level groups, the label contains the name
    ++GroupCounter[g_data.label()];
  }
  else
  {
    // for sub groups we'll track the type
    ++GroupCounter[g_data.type()];
  }
  ++totalGroups;
  return true;
}

bool ESMReaderCount::groupFinished(const GroupData& g_data)
{
  return false;
}

int ESMReaderCount::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  ++RecordCounter[recName];
  ++totalRecords;
  // We skip it.
  return skipRecord(input);
}

void ESMReaderCount::showStats(const bool withPercentage) const
{
  std::map<uint32_t, unsigned int>::const_iterator iter;
  unsigned int sum = 1;
  if (withPercentage)
  {
    sum = 0;
    iter = RecordCounter.begin();
    while (iter!=RecordCounter.end())
    {
      sum += iter->second;
      ++iter;
    }
    if (sum == 0) sum = 1; //avoid div. by zero
  }
  std::cout << "Record statistics:\n";
  iter = RecordCounter.begin();
  while (iter!=RecordCounter.end())
  {
    std::cout << IntTo4Char(iter->first) << ": "<<iter->second;
    if (withPercentage)
    {
      std::cout << " (" << (iter->second*100.0f/static_cast<float>(sum)) << "%)";
    }
    std::cout << "\n";
    ++iter;
  }
  std::cout << "There are " << RecordCounter.size() << " distinct record types.\n\n";

  // ---- groups
  if (withPercentage)
  {
    sum = 0;
    iter = GroupCounter.begin();
    while (iter!=GroupCounter.end())
    {
      sum += iter->second;
      ++iter;
    }//while
    if (sum==0) sum = 1;//avoid div. by zero
  }//if
  std::cout << "Group statistics:\n";
  iter = GroupCounter.begin();
  while (iter != GroupCounter.end())
  {
    std::cout << IntTo4Char(iter->first);
    if (iter->first<256)  std::cout << " (int: " << iter->first << ")";
    std::cout << ": " << iter->second;
    if (withPercentage)
    {
      std::cout << " (" << (iter->second*100.0f/static_cast<float>(sum)) << "%)";
    }
    std::cout << "\n";
    ++iter;
  }
  std::cout << "There are " << GroupCounter.size() << " distinct group types.\n";
}

} // namespace
