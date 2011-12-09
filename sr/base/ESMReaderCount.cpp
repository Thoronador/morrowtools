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

int ESMReaderCount::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  //update group stats
  ++GroupCounter[g_data.getGroupName()];
  ++totalGroups;
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int lastResult = 0;
  int32_t recName;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    //update record stats
    ++RecordCounter[recName];
    ++totalRecords;
    //...and skip it!
    lastResult = skipRecord(in_File);
  }//while
  if (lastResult>=0)
  {
    return 0;
  }
  std::cout << "ESMReaderCount::readGroup: Error while skipping record!"
            << "\nCurrent position is "<<in_File.tellg()<<" bytes.\n";
  return -1;
}

bool ESMReaderCount::needGroup(const GroupData& g_data) const
{
  return true;
}

int ESMReaderCount::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  //we skip it
  return skipRecord(in_File);
}

void ESMReaderCount::showStats(const bool withPercentage) const
{
  std::map<int32_t, unsigned int>::const_iterator iter;
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
    std::cout << IntTo4Char(iter->first) << ": "<<iter->second;
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