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

#include "StringTable.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

namespace SRTP
{

StringTable::StringTable()
{
  //empty
}

StringTable::~StringTable()
{
  //empty
}

bool StringTable::hasString(const uint32_t stringID) const
{
  return (m_Strings.find(stringID)!=m_Strings.end());
}

const std::string& StringTable::getString(const uint32_t stringID) const
{
  const std::map<uint32_t, std::string>::const_iterator iter = m_Strings.find(stringID);
  if (iter!=m_Strings.end())
  {
    return iter->second;
  }
  std::cout << "StringTable: Error there is no string for ID "<<stringID<<"!\n";
  throw 42;
}

void StringTable::tabulaRasa()
{
  m_Strings.clear();
}

StringTable::TableIterator StringTable::getBegin() const
{
  return m_Strings.begin();
}

StringTable::TableIterator StringTable::getEnd() const
{
  return m_Strings.end();
}

uint32_t StringTable::getNumberOfTableEntries() const
{
  return m_Strings.size();
}

bool StringTable::readTable(const std::string& FileName, DataType stringType)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "StringTable: Error: could not open file \""<<FileName<<"\".\n";
    return false;
  }

  uint32_t count, dataSize;
  count = 0;
  dataSize = 0;

  //read number of entries
  input.read((char*) &count, 4);
  //read data size
  input.read((char*)&dataSize, 4);
  if (!input.good())
  {
    std::cout << "StringTable: Error while reading header!\n";
    input.close();
    return false;
  }

  //read directory entries
  std::cout << "Info: Expecting "<<count<<" entries in file.\n";

  std::vector<DirectoryEntry> theDirectory;
  DirectoryEntry temp;
  uint32_t i;
  for (i=0; i<count; ++i)
  {
    input.read((char*) &(temp.stringID), 4);
    input.read((char*) &(temp.offset), 4);
    if (!input.good())
    {
      std::cout << "StringTable: Error while reading directory!\n";
      input.close();
      return false;
    }
    theDirectory.push_back(temp);
  }//for

  const std::ifstream::pos_type offsetBase = input.tellg();

  uint8_t * ptrSpace = NULL;
  uint32_t allocatedSpace = 0;
  uint32_t length = 0;
  for (i=0; i<count; ++i)
  {
    input.seekg(offsetBase+static_cast<std::ifstream::pos_type>(theDirectory[i].offset), std::ios_base::beg);
    if (!input.good())
    {
      std::cout << "StringTable::readTable: Error: could not jump to given offset!\n";
      input.close();
      return false;
    }
    #warning Function does not consider type yet!
    //read strings, pascal style
    // ---- read length
    input.read((char*) &length, 4);
    if (length>65536)
    {
      std::cout << "Error: length ("<<length<<") is greater than 65536!\n";
      delete[] ptrSpace;
      input.close();
      return false;
    }
    //do we need to allocate more space?
    if (length>=allocatedSpace)
    {
      delete[] ptrSpace;
      ptrSpace = new uint8_t[length+1];
      allocatedSpace = length+1;
    }
    //read whole string into buffer
    memset(ptrSpace, 0, length+1);
    input.read((char*) ptrSpace, length);
    if (!input.good())
    {
      std::cout << "StringTable::readTable: Error while reading string!\n";
      input.close();
      delete[] ptrSpace;
      return false;
    }
    //put it into the table
    m_Strings[theDirectory[i].stringID] = std::string((const char*) ptrSpace);
  }

  delete[] ptrSpace;
  allocatedSpace = 0;
  input.close();
  return true;
}

} //namespace