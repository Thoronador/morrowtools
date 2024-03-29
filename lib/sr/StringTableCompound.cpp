/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013, 2014  Thoronador

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

#include "StringTableCompound.hpp"
#include <iostream>
#include <stdexcept>

namespace SRTP
{

StringTableCompound::StringTableCompound()
: m_DLStrings(StringTable()),
  m_ILStrings(StringTable()),
  m_strings(StringTable())
{
}

StringTableCompound::~StringTableCompound()
{
  m_DLStrings.tabulaRasa();
  m_ILStrings.tabulaRasa();
  m_strings.tabulaRasa();
}

void StringTableCompound::addString(const uint32_t stringID, const std::string& content, const TableType tt)
{
  switch (tt)
  {
    case ttDLStrings:
         m_DLStrings.addString(stringID, content);
         break;
    case ttILStrings:
         m_ILStrings.addString(stringID, content);
         break;
    case ttstrings:
         m_strings.addString(stringID, content);
         break;
    case ttNone:
         //do nothing here
         break;
  }
}

bool StringTableCompound::hasString(const uint32_t stringID) const
{
  return (locateString(stringID)!=ttNone);
}

StringTableCompound::TableType StringTableCompound::locateString(const uint32_t stringID) const
{
  if (m_DLStrings.hasString(stringID)) return ttDLStrings;
  if (m_ILStrings.hasString(stringID)) return ttILStrings;
  if (m_strings.hasString(stringID)) return ttstrings;
  return ttNone;
}

const std::string& StringTableCompound::getString(const uint32_t stringID) const
{
  const TableType location = locateString(stringID);
  switch (location)
  {
    case ttDLStrings:
         return m_DLStrings.getString(stringID);
         break;
    case ttILStrings:
         return m_ILStrings.getString(stringID);
         break;
    case ttstrings:
         return m_strings.getString(stringID);
         break;
    default: //location==ttNone
         std::cerr << "StringTableCompound::getString: Error: string with ID "
                   << stringID << " was not found!\n";
         throw std::runtime_error("StringTableCompound::getString: Error: string with the given ID was not found!");
         break;
  }
}

const StringTable& StringTableCompound::getStringTable(const TableType tt) const
{
  switch (tt)
  {
    case ttDLStrings:
         return m_DLStrings;
         break;
    case ttILStrings:
         return m_ILStrings;
         break;
    case ttstrings:
         return m_strings;
         break;
    default: //location==ttNone
         std::cerr << "StringTableCompound::getStringTable: Error: ttNone is "
                   << "not allowed as parameter value!\n";
         throw std::invalid_argument("StringTableCompound::getStringTable(): "
              + std::string("Error: ttNone is not allowed as parameter value!"));
         break;
  }
}

bool StringTableCompound::readCompound(const std::string& BaseFileName)
{
  if (!m_DLStrings.readTable(BaseFileName+".DLStrings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::readCompound: Error while reading 1st table!\n";
    return false;
  }
  if (!m_ILStrings.readTable(BaseFileName+".ILStrings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::readCompound: Error while reading 2nd table!\n";
    return false;
  }
  if (!m_strings.readTable(BaseFileName+".strings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::readCompound: Error while reading 3rd table!\n";
    return false;
  }
  return true;
}

bool StringTableCompound::writeCompound(const std::string& BaseFileName) const
{
  if (!m_DLStrings.writeTable(BaseFileName+".DLStrings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::writeCompound: Error while writing 1st table!\n";
    return false;
  }
  if (!m_ILStrings.writeTable(BaseFileName+".ILStrings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::writeCompound: Error while writing 2nd table!\n";
    return false;
  }
  if (!m_strings.writeTable(BaseFileName+".strings", StringTable::sdUnknown))
  {
    std::cerr << "StringTableCompound::writeCompound: Error while writing 3rd table!\n";
    return false;
  }
  return true;
}

uint32_t StringTableCompound::getNumberOfCompoundEntries() const
{
  return (m_DLStrings.getNumberOfTableEntries()
         +m_ILStrings.getNumberOfTableEntries()
         +m_strings.getNumberOfTableEntries());
}

} //namespace
