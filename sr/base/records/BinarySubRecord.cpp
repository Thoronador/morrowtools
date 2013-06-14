/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "BinarySubRecord.h"
#include <iostream>
#include <cstring>
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

BinarySubRecord::BinarySubRecord()
: m_Pointer(NULL), m_Size(0), m_Present(false)
{

}

BinarySubRecord::BinarySubRecord(const BinarySubRecord& op)
: m_Pointer(NULL), m_Size(op.getSize()), m_Present(op.isPresent())
{
  if (m_Size>0)
  {
    m_Pointer = new uint8_t[m_Size];
    memcpy(m_Pointer, op.getPointer(), m_Size);
  }
}

BinarySubRecord& BinarySubRecord::operator=(const BinarySubRecord& other)
{
  if (this==&other) return *this;
  m_Size = other.getSize();
  delete[] m_Pointer;
  if (m_Size>0)
  {
    m_Pointer = new uint8_t[m_Size];
    memcpy(m_Pointer, other.getPointer(), m_Size);
  }
  else
  {
    m_Pointer = NULL;
  }
  m_Present = other.isPresent();
  return *this;
}

bool BinarySubRecord::operator==(const BinarySubRecord& other) const
{
  if (m_Present!=other.isPresent()) return false;
  if (!m_Present) return true;
  if (m_Size!=other.getSize()) return false;
  if (m_Size==0) return true;
  return (memcmp(m_Pointer, other.getPointer(), m_Size)==0);
}

bool BinarySubRecord::operator!=(const BinarySubRecord& other) const
{
  return (!(*this==other));
}

BinarySubRecord::~BinarySubRecord()
{
  delete[] m_Pointer;
  m_Pointer = NULL;
  m_Size = 0;
}

uint16_t BinarySubRecord::getSize() const
{
  return m_Size;
}

const uint8_t* BinarySubRecord::getPointer() const
{
  return m_Pointer;
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
bool BinarySubRecord::saveToStream(std::ofstream& output, const uint32_t subHeader) const
{
  //don't write if there is no subrecord
  if (!m_Present) return output.good();

  //write subrecord's header
  output.write((const char*) &subHeader, 4);
  //subrecord's length
  output.write((const char*) &m_Size, 2);
  //write content
  output.write((const char*) m_Pointer, m_Size);

  return output.good();
}
#endif

bool BinarySubRecord::loadFromStream(std::istream& in_File, const uint32_t subHeader, const bool withHeader)
{
  if (withHeader)
  {
    uint32_t subRecName = 0;
    //read sub header
    in_File.read((char*) &subRecName, 4);
    if (subRecName!=subHeader)
    {
      UnexpectedRecord(subHeader, subRecName);
      return false;
    }
  }
  //subrecord's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  //read subrecord's data
  if (subLength!=m_Size)
  {
    delete[] m_Pointer;
    m_Pointer = new uint8_t[subLength];
    m_Size = subLength;
  }
  memset(m_Pointer, 0, subLength);
  in_File.read((char*) m_Pointer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord "<<IntTo4Char(subHeader)<<"!\n";
    return false;
  }
  m_Present = true;
  return true;
}

} //namespace
