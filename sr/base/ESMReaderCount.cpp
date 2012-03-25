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

#include "ESMReaderCount.h"
#include <iostream>
#include "../../mw/base/HelperIO.h"

namespace SRTP
{

ESMReaderCount::ESMReaderCount()
{
  resetStats();
}

ESMReaderCount::~ESMReaderCount()
{
  //empty
}

void ESMReaderCount::resetStats()
{
  RecordCounter.clear();
  totalRecords = 0;
  GroupCounter.clear();
  totalGroups = 0;
}

bool ESMReaderCount::needGroup(const GroupData& g_data) const
{
  return true;
}

void ESMReaderCount::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  //update group stats
  if (g_data.getGroupType()==GroupData::cTopLevelGroup)
  {
    //for top level groups, the label contains the name
    ++GroupCounter[g_data.getGroupLabel()];
  }
  else
  {
    //for sub groups we'll track the type
    ++GroupCounter[g_data.getGroupType()];
  }
  ++totalGroups;
}

void ESMReaderCount::groupFinished(const GroupData& g_data)
{
  //nothing to do here
}

int ESMReaderCount::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  ++RecordCounter[recName];
  ++totalRecords;
  //we skip it
  return skipRecord(in_File);
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
    }//while
    if (sum==0) sum = 1;//avoid div. by zero
  }//if
  std::cout << "Record statistics:\n";
  iter = RecordCounter.begin();
  while (iter!=RecordCounter.end())
  {
    std::cout << IntTo4Char(iter->first) << ": "<<iter->second;
    if (withPercentage)
    {
      std::cout << " ("<<(iter->second*100.0f/static_cast<float>(sum))<<"%)";
    }
    std::cout <<"\n";
    ++iter;
  }//while
  std::cout << "There are "<<RecordCounter.size()<<" distinct record types.\n\n";

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
  while (iter!=GroupCounter.end())
  {
    std::cout << IntTo4Char(iter->first);
    if (iter->first<256)  std::cout << " (int: "<<iter->first<<")";
    std::cout << ": "<<iter->second;
    if (withPercentage)
    {
      std::cout << " ("<<(iter->second*100.0f/static_cast<float>(sum))<<"%)";
    }
    std::cout <<"\n";
    ++iter;
  }//while
  std::cout << "There are "<<GroupCounter.size()<<" distinct group types.\n";
}

} //namespace
