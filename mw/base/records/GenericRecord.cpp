/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#include "GenericRecord.hpp"
#include <iostream>
#include <cstring>

namespace MWTP
{

GenericRecord::GenericRecord()
: BasicRecord(),
  Header(0),
  m_DataSize(0),
  m_Data(NULL)
{
}

GenericRecord::GenericRecord(const GenericRecord& op)
: BasicRecord(op),
  Header(op.Header),
  m_DataSize(op.m_DataSize),
  m_Data(NULL)
{
  if (m_DataSize>0)
  {
    m_Data = new uint8_t[m_DataSize];
    memcpy(m_Data, op.getDataPointer(), m_DataSize);
  }
  else
  {
    m_Data = NULL;
  }
}

GenericRecord& GenericRecord::operator=(const GenericRecord& other)
{
  if (this==&other) return *this;
  m_DataSize = other.getDataSize();
  delete[] m_Data;
  if (m_DataSize>0)
  {
    m_Data = new uint8_t[m_DataSize];
    memcpy(m_Data, other.getDataPointer(), m_DataSize);
  }
  else
  {
    m_Data = NULL;
  }
  Header = other.Header;
  return *this;
}

GenericRecord::~GenericRecord()
{
  delete[] m_Data;
  m_DataSize = 0;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool GenericRecord::saveToStream(std::ostream& output) const
{
  //record header
  output.write((const char*) &Header, 4);
  //header flags
  output.write((const char*) &m_DataSize, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);
  //write the real data
  output.write((const char*) m_Data, m_DataSize);

  return output.good();
}
#endif

bool GenericRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  //prevent excessive memory usage
  if (Size>256*1024)
  {
    std::cout << "GenericRecord::loadFromStream: Error: record size is larger than 256 KB, aborting.\n";
    return false;
  }
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  //allocate new memory
  uint8_t * tempPtr = NULL;
  tempPtr = new uint8_t[Size];
  memset(tempPtr, '\0', Size);
  in_File.read((char*) tempPtr, Size);
  if (!in_File.good())
  {
    std::cout << "GenericRecord::loadFromStream: Error while reading data!\n";
    delete[] tempPtr;
    return false;
  }

  //set data pointer and size according to the read values
  m_Data = tempPtr;
  m_DataSize = Size;
  return true;
}

const uint8_t* GenericRecord::getDataPointer() const
{
  return m_Data;
}

uint32_t GenericRecord::getDataSize() const
{
  return m_DataSize;
}

} //namespace
