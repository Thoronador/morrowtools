/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2024  Dirk Stolle

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

#include "Group.hpp"
#include <iostream>

namespace SRTP
{

Group::Group()
: headerData(GroupData()),
  m_SubGroups(std::vector<Group>()),
  m_Records(std::vector<std::shared_ptr<BasicRecord> >()),
  m_Index(std::map<uint32_t, decltype(m_Records)::size_type>())
{
}

Group::~Group()
{
  removeContents();
}

void Group::addRecord(BasicRecord* rec)
{
  if (rec != nullptr)
  {
    m_Index[rec->headerFormID] = m_Records.size();
    std::shared_ptr<BasicRecord> s_ptr(rec);
    m_Records.push_back(s_ptr);
  }
}

bool Group::hasRecord(const uint32_t formID, const bool useIndex) const
{
  if (useIndex)
  {
    return (m_Index.find(formID) != m_Index.end());
  }
  for (decltype(m_Records)::size_type i = 0; i < m_Records.size(); ++i)
  {
    if (m_Records[i]->headerFormID == formID)
      return true;
  }
  return false;
}

const BasicRecord& Group::getRecord(const uint32_t formID, const bool useIndex) const
{
  if (useIndex)
  {
    const std::map<uint32_t, decltype(m_Records)::size_type>::const_iterator iter = m_Index.find(formID);
    if (iter != m_Index.end())
    {
      if (iter->second < m_Records.size())
        return *m_Records[iter->second];
    }
  }
  else
  {
    for (decltype(m_Records)::size_type i = 0; i < m_Records.size(); ++i)
    {
      if (m_Records[i]->headerFormID == formID)
        return *m_Records[i];
    }
  }

  // record was not found
  throw "Record with requested form ID not found!\n";
}

unsigned int Group::getNumberOfRecords() const
{
  return m_Records.size();
}

unsigned int Group::getNumberOfRecordsIncludingSubGroups() const
{
  unsigned int result = m_Records.size();
  SubIterator iterSub = m_SubGroups.begin();
  while (iterSub != m_SubGroups.end())
  {
    result += iterSub->getNumberOfRecordsIncludingSubGroups();
    ++iterSub;
  }
  return result;
}

void Group::removeRecords()
{
  // clear record index
  m_Index.clear();
  // delete all records properly
  // -- std::shared pointer class should handle proper deallocation, when needed
  m_Records.clear();
}

void Group::rebuildIndex()
{
  m_Index.clear();
  for (decltype(m_Records)::size_type i = 0; i < m_Records.size(); ++i)
  {
    m_Index[m_Records[i]->headerFormID] = i;
  }
}

Group::ConstRecIterator Group::getRecBegin() const
{
  return m_Records.begin();
}

Group::ConstRecIterator Group::getRecEnd() const
{
  return m_Records.end();
}

Group::RecIterator Group::getRecBegin()
{
  return m_Records.begin();
}

Group::RecIterator Group::getRecEnd()
{
  return m_Records.end();
}

Group& Group::addSubGroup(const GroupData& g_data)
{
  Group temp;
  temp.headerData = g_data;
  m_SubGroups.push_back(temp);
  return m_SubGroups.back();
}

bool Group::hasSubGroup(const GroupData& g_data) const
{
  std::vector<Group>::const_iterator iter = m_SubGroups.begin();
  while (iter!=m_SubGroups.end())
  {
    if (iter->headerData == g_data) return true;
    ++iter;
  }//while
  return false;
}

const Group& Group::getSubGroup(const GroupData& g_data) const
{
  std::vector<Group>::const_iterator iter = m_SubGroups.begin();
  while (iter != m_SubGroups.end())
  {
    if (iter->headerData == g_data)
    {
      return *iter;
    }
    ++iter;
  }
  throw "Group::getSubGroup: Error: Group not found!\n";
}

const Group& Group::getSubGroupAtIndex(const unsigned int idx) const
{
  if (idx < m_SubGroups.size())
  {
    return m_SubGroups[idx];
  }
  throw "Group::getSubGroupAtIndex: Error: invalid index!\n";
}

Group* Group::getSubGroupAtIndexNC(const unsigned int idx)
{
  if (idx < m_SubGroups.size())
  {
    return &(m_SubGroups[idx]);
  }
  throw "Group::getSubGroupAtIndexNC: Error: invalid index!\n";
}

unsigned int Group::getNumberOfSubGroups() const
{
  return m_SubGroups.size();
}

unsigned int Group::getNumberOfGroupsIncludingSubGroups() const
{
  unsigned int result = m_SubGroups.size();
  SubIterator iter = m_SubGroups.begin();
  while (iter != m_SubGroups.end())
  {
    result += iter->getNumberOfGroupsIncludingSubGroups();
    ++iter;
  }
  return result;
}

Group::SubIterator Group::getSubBegin() const
{
  return m_SubGroups.begin();
}

Group::SubIterator Group::getSubEnd() const
{
  return m_SubGroups.end();
}

bool Group::deleteSubGroup(const GroupData& g_data)
{
  std::vector<Group>::iterator iter = m_SubGroups.begin();
  while (iter != m_SubGroups.end())
  {
    if (iter->headerData == g_data)
    {
      m_SubGroups.erase(iter);
      return true;
    }
    ++iter;
  }
  return false;
}

void Group::removeSubGroups()
{
  m_SubGroups.clear();
}

void Group::removeContents()
{
  //delete all records
  removeRecords();
  //delete sub groups
  removeSubGroups();
}

bool Group::isEmpty() const
{
  if (!m_Records.empty())
  {
    return false;
  }
  SubIterator iter = getSubBegin();
  while (iter != getSubEnd())
  {
    if (!iter->isEmpty())
    {
      return false;
    }
    ++iter;
  }
  return true;
}

unsigned int Group::purgeEmptySubGroups()
{
  unsigned int purged = 0;
  std::vector<Group>::iterator iter = m_SubGroups.begin();
  while (iter != m_SubGroups.end())
  {
    purged += iter->purgeEmptySubGroups();
    if (iter->isEmpty())
    {
      iter = m_SubGroups.erase(iter);
      ++purged;
    }
    else
    {
      ++iter;
    }
  }
  return purged;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t Group::getContentSize() const
{
  uint32_t total = 0;
  // add record sizes
  const auto rec_count = m_Records.size();
  for (decltype(m_Records)::size_type i = 0; i < rec_count; ++i)
  {
    total += m_Records[i]->getTotalWrittenSize();
  }
  // add sub group sizes
  const unsigned int sub_count = m_SubGroups.size();
  for (decltype(m_Records)::size_type i = 0; i < sub_count; ++i)
  {
    total = total +24 /* size of group header */ + m_SubGroups.at(i).getContentSize();
  }
  return total;
}

bool Group::saveToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cerr << "Group::saveToStream: Error: Bad stream!\n";
    return false;
  }
  if (!headerData.saveToStream(output))
  {
    std::cerr << "Group::saveToStream: Error: Could not write group header!\n";
    return false;
  }

  // write sub groups
  for (const auto& group: m_SubGroups)
  {
    if (!group.saveToStream(output))
    {
      return false;
    }
  }
  // write records
  for (const auto& record: m_Records)
  {
    if (!(record->saveToStream(output)))
    {
      return false;
    }
  }
  return true;
}

void Group::updateGroupSize()
{
  // basic size for every group is 24 bytes, rest is records
  headerData.setSize(24 + getContentSize());
}
#endif

Group * Group::determineLatestGroup(const unsigned int level)
{
  // level zero means no group
  if (level == 0)
    return nullptr;
  // level one is the first valid level, meaning the current group
  if (level == 1)
    return this;
  // Level is larger than one, so we have to go into sub groups.
  // If there are no groups, we can't return anything useful.
  if (getNumberOfSubGroups() == 0)
  {
    return nullptr;
  }
  return m_SubGroups.back().determineLatestGroup(level - 1);
}

} //namespace
