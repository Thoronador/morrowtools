/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "BinarySubRecordExtended.hpp"

#include <iostream>
#include <cstring>
#include "../../../mw/base/HelperIO.hpp"
#include "../SR_Constants.hpp"

namespace SRTP
{

BinarySubRecordExtended::BinarySubRecordExtended()
: m_Pointer(NULL), m_Size(0), m_Present(false)
{

}

BinarySubRecordExtended::BinarySubRecordExtended(const BinarySubRecordExtended& op)
: m_Pointer(NULL), m_Size(op.getSize()), m_Present(op.isPresent())
{
  if (m_Size>0)
  {
    m_Pointer = new uint8_t[m_Size];
    memcpy(m_Pointer, op.getPointer(), m_Size);
  }
}

BinarySubRecordExtended& BinarySubRecordExtended::operator=(const BinarySubRecordExtended& other)
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

bool BinarySubRecordExtended::operator==(const BinarySubRecordExtended& other) const
{
  if (m_Present!=other.isPresent()) return false;
  if (!m_Present) return true;
  if (m_Size!=other.getSize()) return false;
  if (m_Size==0) return true;
  return (memcmp(m_Pointer, other.getPointer(), m_Size)==0);
}

bool BinarySubRecordExtended::operator!=(const BinarySubRecordExtended& other) const
{
  return (!(*this==other));
}

BinarySubRecordExtended::~BinarySubRecordExtended()
{
  delete[] m_Pointer;
  m_Pointer = NULL;
  m_Size = 0;
}

uint32_t BinarySubRecordExtended::getSize() const
{
  return m_Size;
}

const uint8_t* BinarySubRecordExtended::getPointer() const
{
  return m_Pointer;
}

bool BinarySubRecordExtended::isPresent() const
{
  return m_Present;
}

void BinarySubRecordExtended::setPresence(const bool presence_flag)
{
  m_Present = presence_flag;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool BinarySubRecordExtended::saveToStream(std::ostream& output, const uint32_t subHeader) const
{
  //don't write if there is no subrecord
  if (!m_Present) return output.good();

  uint16_t subLength;
  if (m_Size>65535)
  {
    //write XXXX's header
    output.write((const char*) &cXXXX, 4);
    //XXXX's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write content
    output.write((const char*) &m_Size, 4);
  }//large version

  //write subrecord's header
  output.write((const char*) &subHeader, 4);
  //subrecord's length
  if (m_Size<65536)
  {
    subLength = m_Size;
  }
  else
  {
    subLength = 0;
  }
  output.write((const char*) &subLength, 2);
  //write content
  output.write((const char*) m_Pointer, m_Size);

  return output.good();
}
#endif

bool BinarySubRecordExtended::loadFromStream(std::istream& in_File, const uint32_t subHeader, const bool withHeader)
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
    std::cerr << "Error while reading subrecord "<<IntTo4Char(subHeader)<<"!\n";
    return false;
  }
  m_Present = true;
  return true;
}

bool BinarySubRecordExtended::loadFromStreamExtended(std::istream& in_File, const uint32_t subHeader, const bool withHeader, const uint32_t realSize)
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
  if (subLength!=0)
  {
    std::cerr << "Error while reading extended subrecord "<<IntTo4Char(subHeader)
              << "sub length is not zero!\n";
    return false;
  }
  //reallocate pointer, if needed
  if (m_Size!=realSize)
  {
    delete[] m_Pointer;
    m_Pointer = new uint8_t[realSize];
    m_Size = realSize;
  }
  memset(m_Pointer, 0, realSize);
  //read subrecord's data
  in_File.read((char*) m_Pointer, realSize);
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord "<<IntTo4Char(subHeader)<<"!\n";
    return false;
  }
  m_Present = true;
  return true;
}

} //namespace
