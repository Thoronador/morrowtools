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

#include "StringTableCompound.h"
#include <iostream>

namespace SRTP
{

StringTableCompound::StringTableCompound()
{
  m_DLStrings.tabulaRasa();
  m_ILStrings.tabulaRasa();
  m_strings.tabulaRasa();
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
  }//swi
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
         std::cout << "StringTableCompound::getString: Error: string with ID "
                   << stringID<<" was not found!\n";
         throw 42;
         break;
  }//swi
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
         std::cout << "StringTableCompound::getStringTable: Error: ttNone is "
                   << "not allowed as parameter value!\n";
         throw 42;
         break;
  }//swi
}

bool StringTableCompound::readCompound(const std::string& BaseFileName)
{
  if (!m_DLStrings.readTable(BaseFileName+".DLStrings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::readCompound: Error while reading 1st table!\n";
    return false;
  }
  if (!m_ILStrings.readTable(BaseFileName+".ILStrings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::readCompound: Error while reading 2nd table!\n";
    return false;
  }
  if (!m_strings.readTable(BaseFileName+".strings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::readCompound: Error while reading 3rd table!\n";
    return false;
  }
  return true;
}

bool StringTableCompound::writeCompound(const std::string& BaseFileName) const
{
  if (!m_DLStrings.writeTable(BaseFileName+".DLStrings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::writeCompound: Error while writing 1st table!\n";
    return false;
  }
  if (!m_ILStrings.writeTable(BaseFileName+".ILStrings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::writeCompound: Error while writing 2nd table!\n";
    return false;
  }
  if (!m_strings.writeTable(BaseFileName+".strings", StringTable::sdUnknown))
  {
    std::cout << "StringTableCompound::writeCompound: Error while writing 3rd table!\n";
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
