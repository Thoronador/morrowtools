/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2021  Thoronador

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

#include "Database.hpp"
#include <iostream>
#include "FormIDFunctions.hpp"

namespace SRTP
{

Database::Database()
: m_Records(std::map<uint32_t, BasicRecord*>())
{
}

Database::~Database()
{
  deleteAllRecords();
}

Database& Database::get()
{
  static Database Instance;
  return Instance;
}

void Database::addRecord(BasicRecord* record)
{
  if (record == nullptr)
    return;
  std::map<uint32_t, BasicRecord*>::iterator iter = m_Records.find(record->headerFormID);
  if (iter == m_Records.end())
  {
    // just insert it
    m_Records[record->headerFormID] = record;
  }
  else
  {
    if (record == iter->second)
      return; // avoid replacing with itself
    // save old record first (for deletion)
    BasicRecord * temp = iter->second;
    // replace it by new record
    iter->second = record;
    delete temp;
  }
}

bool Database::deleteRecord(const uint32_t formID)
{
  std::map<uint32_t, BasicRecord*>::iterator iter = m_Records.find(formID);
  if (iter != m_Records.end())
  {
    // save old record first (for deletion)
    BasicRecord * temp = iter->second;
    m_Records.erase(iter);
    delete temp;
    return true;
  }
  // nothing found, nothing deleted
  return false;
}

bool Database::hasRecord(const uint32_t formID) const
{
  return m_Records.find(formID) != m_Records.end();
}

const BasicRecord& Database::getRecord(const uint32_t formID) const
{
  const auto iter = m_Records.find(formID);
  if (iter != m_Records.end())
  {
    return *(iter->second);
  }
  std::cout << "Database::getRecord: ERROR: No record with ID \""
            << SRTP::getFormIDAsString(formID)
            << "\" found. Exception will be thrown.\n";
  std::cout.flush();
  throw 42;
}

void Database::deleteAllRecords()
{
  for (auto iter = m_Records.begin(); iter != m_Records.end(); )
  {
    BasicRecord* temp = iter->second;
    iter = m_Records.erase(iter);
    delete temp;
  }
}

unsigned int Database::getNumberOfRecords() const
{
  return m_Records.size();
}

Database::Iterator Database::getFirst() const
{
  return m_Records.begin();
}

Database::Iterator Database::getEnd() const
{
  return m_Records.end();
}

} // namespace
