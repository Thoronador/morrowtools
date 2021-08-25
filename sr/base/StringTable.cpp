/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013, 2014, 2021  Thoronador

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

#include "StringTable.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include "../../base/UtilityFunctions.hpp"

namespace SRTP
{

StringTable::StringTable()
: m_Strings(std::map<uint32_t, std::string>())
{
}

void StringTable::addString(const uint32_t stringID, const std::string& content)
{
  if (stringID != 0)
  {
    m_Strings[stringID] = content;
  }
}

bool StringTable::hasString(const uint32_t stringID) const
{
  return m_Strings.find(stringID) != m_Strings.end();
}

const std::string& StringTable::getString(const uint32_t stringID) const
{
  const auto iter = m_Strings.find(stringID);
  if (iter != m_Strings.end())
  {
    return iter->second;
  }
  std::cerr << "StringTable: Error there is no string for ID " << stringID << "!\n";
  throw std::runtime_error("StringTable: Error there is no string for the given ID!");
}

bool StringTable::deleteString(const uint32_t stringID)
{
  return m_Strings.erase(stringID) != 0;
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
  // Try to determine the data type via extension, if it is not given.
  if (stringType == sdUnknown)
  {
    const std::string::size_type dotPos = FileName.rfind('.');
    if (dotPos == std::string::npos)
    {
      std::cerr << "Error: Cannot determine string data type!\n";
      return false;
    }
    const std::string ext = lowerCase(FileName.substr(dotPos));
    if (ext == ".strings")
    {
      stringType = sdNULterminated;
    }
    else if ((ext == ".dlstrings") || (ext == ".ilstrings"))
    {
      stringType = sdPascalStyle;
    }
    else
    {
      std::cerr << "Error: Cannot determine string data type, unknown extension \""
                << ext << "\"!\n";
      return false;
    }
  }

  std::ifstream input;
  input.open(FileName, std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cerr << "StringTable: Error: could not open file \"" << FileName
              << "\".\n";
    return false;
  }

  uint32_t count = 0;
  uint32_t dataSize = 0;

  // read number of entries
  input.read((char*) &count, 4);
  // read data size
  input.read((char*)&dataSize, 4);
  if (!input.good())
  {
    std::cerr << "StringTable: Error while reading header!\n";
    input.close();
    return false;
  }

  //read directory entries
  std::vector<DirectoryEntry> theDirectory;
  DirectoryEntry temp;
  for (uint32_t i = 0; i < count; ++i)
  {
    input.read(reinterpret_cast<char*>(&(temp.stringID)), 4);
    input.read(reinterpret_cast<char*>(&(temp.offset)), 4);
    if (!input.good())
    {
      std::cerr << "StringTable: Error while reading directory!\n";
      input.close();
      return false;
    }
    theDirectory.emplace_back(temp);
  }

  const std::ifstream::pos_type offsetBase = input.tellg();

  uint8_t * ptrSpace = nullptr;
  uint32_t allocatedSpace = 0;
  uint32_t length = 0;

  if (stringType == sdNULterminated)
  {
    // pre-allocate memory
    ptrSpace = new uint8_t[65537];
    allocatedSpace = 65537;
  }

  for (uint32_t i = 0; i < count; ++i)
  {
    input.seekg(offsetBase + static_cast<std::ifstream::pos_type>(theDirectory[i].offset), std::ios_base::beg);
    if (!input.good())
    {
      std::cerr << "StringTable::readTable: Error: could not jump to given offset!\n";
      delete[] ptrSpace;
      input.close();
      return false;
    }
    if (stringType == sdPascalStyle)
    {
      // read strings, Pascal style
      // ---- read length
      input.read(reinterpret_cast<char*>(&length), 4);
      if (length > 65536)
      {
        std::cerr << "Error: length (" << length << ") is greater than 65536!\n";
        delete[] ptrSpace;
        input.close();
        return false;
      }
      // Do we need to allocate more space?
      if (length >= allocatedSpace)
      {
        delete[] ptrSpace;
        ptrSpace = new uint8_t[length + 1];
        allocatedSpace = length + 1;
      }
      // read whole string into buffer
      memset(ptrSpace, 0, length + 1);
      input.read(reinterpret_cast<char*>(ptrSpace), length);
      if (!input.good())
      {
        std::cerr << "StringTable::readTable: Error while reading string!\n";
        input.close();
        delete[] ptrSpace;
        return false;
      }
    }
    else
    {
      // read NUL-terminated string data
      length = 0;
      do
      {
        // read next character
        input.read(reinterpret_cast<char*>(&(ptrSpace[length])), 1);
        ++length;
      } while ((ptrSpace[length - 1] != '\0') && (length < allocatedSpace));
      if (length + 1 == allocatedSpace)
      {
        ptrSpace[length] = '\0';
        std::cerr << "Error: string was cut off after reaching allocation limit!\n";
        delete[] ptrSpace;
        return false;
      }
    }
    // put it into the table
    m_Strings[theDirectory[i].stringID] = std::string(reinterpret_cast<const char*>(ptrSpace));
  }

  delete[] ptrSpace;
  allocatedSpace = 0;
  input.close();
  return true;
}

bool StringTable::writeTable(const std::string& FileName, DataType stringType) const
{
  // try to determine the data type via extension, if it is not given
  if (stringType == sdUnknown)
  {
    const std::string::size_type dotPos = FileName.rfind('.');
    if (dotPos == std::string::npos)
    {
      std::cerr << "Error: Cannot determine string data type!\n";
      return false;
    }
    std::string ext = lowerCase(FileName.substr(dotPos));
    if (ext == ".strings")
    {
      stringType = sdNULterminated;
    }
    else if ((ext == ".dlstrings") || (ext == ".ilstrings"))
    {
      stringType = sdPascalStyle;
    }
    else
    {
      std::cerr << "Error: Cannot determine string data type, unknown extension \""
                << ext << "\"!\n";
      return false;
    }
  }

  // prepare directory entries
  std::vector<DirectoryEntry> theDirectory;
  DirectoryEntry temp;
  uint32_t nextAvailableOffset = 0;

  for (const auto& [key, value]: m_Strings)
  {
    temp.stringID = key;
    temp.offset = nextAvailableOffset;
    // strings are NUL-terminated, even in "Pascal" style, so we add +1 at the end
    nextAvailableOffset = nextAvailableOffset + value.length() + 1;
    // add extra four bytes for length in "Pascal" style
    if (sdPascalStyle == stringType)
    {
      nextAvailableOffset += 4;
    }
    theDirectory.emplace_back(temp);
  }

  // prepare values for count and data size
  const uint32_t count = theDirectory.size();
  uint32_t dataSize = 0;

  // ---- data size is offset of last string plus length for that entry
  if (!theDirectory.empty())
  {
    dataSize = theDirectory.back().offset + m_Strings.find(theDirectory.back().stringID)->second.length() + 1;
    if (sdPascalStyle == stringType)
    {
      dataSize += 4;
    }
  }

  // now write the file
  std::ofstream output;
  output.open(FileName, std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cerr << "StringTable::writeTable: Error: could not open file \""
              << FileName << "\".\n";
    return false;
  }

  // write number of entries
  output.write(reinterpret_cast<const char*>(&count), 4);
  // write data size
  output.write(reinterpret_cast<const char*>(&dataSize), 4);
  if (!output.good())
  {
    std::cerr << "StringTable::writeTable: Error while writing header!\n";
    output.close();
    return false;
  }

  // write directory entries
  for (const auto& entry: theDirectory)
  {
    output.write(reinterpret_cast<const char*>(&(entry.stringID)), 4);
    output.write(reinterpret_cast<const char*>(&(entry.offset)), 4);
    if (!output.good())
    {
      std::cerr << "StringTable::writeTable: Error while writing directory entries!\n";
      output.close();
      return false;
    }
  }

  // write the data entries
  uint32_t i;
  for (const auto& key_value: m_Strings)
  {
    // store length in i
    i = key_value.second.length() + 1;
    if (sdPascalStyle == stringType)
    {
      // write length
      output.write(reinterpret_cast<const char*>(&i), 4);
    }
    // write string
    output.write(key_value.second.c_str(), i);
    if (!output.good())
    {
      std::cerr << "StringTable::writeTable: Error while writing string data!\n";
      output.close();
      return false;
    }
  }

  output.close();
  return true;
}

void StringTable::mergeTables(const StringTable& other)
{
  TableIterator iter = other.getBegin();
  const TableIterator endIter = other.getEnd();
  while (iter != endIter)
  {
    m_Strings[iter->first] = iter->second;
    ++iter;
  }
}

} // namespace
