/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2021  Thoronador

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

#include "LocalizedString.hpp"
#include <cstring>
#include <iostream>
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

LocalizedString::LocalizedString()
: m_Type(Type::None), m_Index(0), m_String("")
{
}

LocalizedString::LocalizedString(const Type t, const uint32_t idx, const std::string& str)
: m_Type(t), m_Index(idx), m_String(str)
{
}

bool LocalizedString::isPresent() const noexcept
{
  return m_Type != Type::None;
}

uint32_t LocalizedString::getIndex() const
{
  if (m_Type == Type::Index)
  {
    return m_Index;
  }
  throw WrongStringType();
}

const std::string& LocalizedString::getString() const
{
  if (m_Type == Type::None)
  {
    throw WrongStringType();
  }
  return m_String;
}

bool LocalizedString::loadFromStream(std::istream& in_Stream, const uint32_t subHeader,
                        const bool withHeader, uint32_t& bytesRead,
                        const bool localized, const StringTable& table,
                        char* buffer)
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    // read header
    in_Stream.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    if (subRecName != subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  } //if with header

  // subrecord's length
  uint16_t subLength = 0;
  in_Stream.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (localized)
  {
    if (subLength != 4)
    {
      std::cerr << "Error: Sub record " << IntTo4Char(subHeader)
                << " has invalid length (" << subLength
                << " bytes). Should be four bytes.\n";
      return false;
    }
    // read value
    in_Stream.read(reinterpret_cast<char*>(&m_Index), 4);
    bytesRead += 4;
    if (!in_Stream.good())
    {
      std::cerr << "LocalizedString::loadFromStream: Error while reading subrecord "
                << IntTo4Char(subHeader) << "!\n";
      return false;
    }
    // treat index zero as empty string, some subrecords allow zero as index
    if (m_Index == 0)
    {
      m_String.clear();
      m_Type = Type::Index;
      return true;
    }
    if (!table.hasString(m_Index))
    {
      std::cerr << "LocalizedString::loadFromStream: Table has no entry for index " << m_Index << "!\n";
      return false;
    }
    m_String = table.getString(m_Index);
    m_Type = Type::Index;
  }
  else
  {
    // unlocalized (plain string)
    if (subLength > 511)
    {
      std::cerr << "Error: Subrecord " << IntTo4Char(subHeader)
                << " is longer than 511 characters!\n";
      return false;
    }
    // read string
    memset(buffer, 0, 512);
    in_Stream.read(buffer, subLength);
    bytesRead += subLength;
    if (!in_Stream.good())
    {
      std::cerr << "Error while reading subrecord " << IntTo4Char(subHeader) << "!\n";
      return false;
    }
    m_String = std::string(buffer);
    m_Type = Type::String;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LocalizedString::getWriteSize() const noexcept
{
  switch (m_Type)
  {
    case Type::Index:
         return 10;
         break;
    case Type::String:
         return (4 /* HEAD */ + 2 /* 2 bytes for length */
                 + m_String.length() + 1 /* length of string +1 byte for NUL-termination */ );
         break;
    default: // Type::None
         return 0;
         break;
  }
}

bool LocalizedString::saveToStream(std::ostream& output, const uint32_t subHeader) const
{
  uint16_t subLength = 0;
  switch (m_Type)
  {
    case Type::Index:
         // write header
         output.write(reinterpret_cast<const char*>(&subHeader), 4);
         // subrecord's length
         subLength = 4;
         output.write(reinterpret_cast<const char*>(&subLength), 2);
         // write index
         output.write(reinterpret_cast<const char*>(&m_Index), 4);
         break;
    case Type::String:
         // write header
         output.write(reinterpret_cast<const char*>(&subHeader), 4);
         // subrecord's length
         subLength = m_String.length()+1;
         output.write(reinterpret_cast<const char*>(&subLength), 2);
         // write string
         output.write(m_String.c_str(), subLength);
         break;
    default: // Type::None
         return true;
  }
  return output.good();
}
#endif

void LocalizedString::reset()
{
  m_Type = Type::None;
  m_Index = 0;
  m_String.clear();
}

bool LocalizedString::operator==(const LocalizedString& other) const
{
  if (m_Type != other.getType())
    return false;
  switch(m_Type)
  {
    case Type::Index:
         return m_Index == other.m_Index;
    case Type::String:
         return m_String == other.m_String;
    default: // Type::None
         return true;
  }
}

} // namespace
