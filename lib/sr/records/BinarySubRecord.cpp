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

#include "BinarySubRecord.hpp"
#include <iostream>
#include <cstring>
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

BinarySubRecord::BinarySubRecord()
: m_Data(nullptr), m_Size(0), m_Present(false)
{
}

BinarySubRecord::BinarySubRecord(const BinarySubRecord& op)
: m_Data(nullptr), m_Size(op.size()), m_Present(op.isPresent())
{
  if (m_Size > 0)
  {
    m_Data = new uint8_t[m_Size];
    memcpy(m_Data, op.data(), m_Size);
  }
}

BinarySubRecord& BinarySubRecord::operator=(const BinarySubRecord& other)
{
  if (this == &other)
  {
    return *this;
  }
  m_Size = other.size();
  delete[] m_Data;
  if (m_Size > 0)
  {
    m_Data = new uint8_t[m_Size];
    memcpy(m_Data, other.data(), m_Size);
  }
  else
  {
    m_Data = nullptr;
  }
  m_Present = other.isPresent();
  return *this;
}

bool BinarySubRecord::operator==(const BinarySubRecord& other) const
{
  if (m_Present != other.isPresent())
    return false;
  if (!m_Present)
    return true;
  if (m_Size != other.size())
    return false;
  if (m_Size == 0)
    return true;
  return memcmp(m_Data, other.data(), m_Size) == 0;
}

bool BinarySubRecord::operator!=(const BinarySubRecord& other) const
{
  return !(*this == other);
}

BinarySubRecord::~BinarySubRecord()
{
  delete[] m_Data;
  m_Data = nullptr;
  m_Size = 0;
}

uint16_t BinarySubRecord::size() const
{
  return m_Size;
}

const uint8_t* BinarySubRecord::data() const
{
  return m_Data;
}

bool BinarySubRecord::isPresent() const
{
  return m_Present;
}

void BinarySubRecord::setPresence(const bool presence_flag)
{
  m_Present = presence_flag;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool BinarySubRecord::saveToStream(std::ostream& output, const uint32_t subHeader) const
{
  // don't write if there is no subrecord
  if (!m_Present)
    return output.good();

  // write subrecord's header
  output.write(reinterpret_cast<const char*>(&subHeader), 4);
  // subrecord's length
  output.write(reinterpret_cast<const char*>(&m_Size), 2);
  // write content
  output.write(reinterpret_cast<const char*>(m_Data), m_Size);

  return output.good();
}
#endif

bool BinarySubRecord::loadFromStream(std::istream& input, const uint32_t subHeader, const bool withHeader)
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    // read sub header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    if (subRecName != subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }
  // sub record's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  // re-allocate data, if necessary
  if (subLength != m_Size)
  {
    delete[] m_Data;
    m_Data = new uint8_t[subLength];
    m_Size = subLength;
  }
  // read sub record's data - but only if there is something to read
  if (subLength != 0)
  {
    memset(m_Data, 0, subLength);
    input.read(reinterpret_cast<char*>(m_Data), subLength);
    if (!input.good())
    {
      std::cerr << "Error while reading sub record " << IntTo4Char(subHeader)
                << "!\n";
      return false;
    }
  }
  m_Present = true;
  return true;
}

} // namespace
