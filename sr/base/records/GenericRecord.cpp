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

#include "GenericRecord.h"
#include <cstring>
#include <iostream>

namespace SRTP
{

GenericRecord::GenericRecord()
: BasicRecord(),
  Header(0),
  m_DataSize(0),
  m_Data(NULL)
{

}

GenericRecord::GenericRecord(const GenericRecord& other)
: Header(other.Header),
  m_DataSize(other.getDataSize())
{
  copyBasicMembers(other);
  if (m_DataSize!=0)
  {
    m_Data = new unsigned char[m_DataSize];
    memcpy(m_Data, other.getDataPointer(), m_DataSize);
  }
  else
  {
    m_Data = NULL;
  }
}

GenericRecord& GenericRecord::operator=(const GenericRecord& other)
{
  if (this==&other) return *this;
  copyBasicMembers(other);
  delete[] m_Data;
  Header = other.Header;
  m_DataSize = other.getDataSize();
  if (m_DataSize!=0)
  {
    m_Data = new unsigned char[m_DataSize];
    memcpy(m_Data, other.getDataPointer(), m_DataSize);
  }
  else
  {
    m_Data = NULL;
  }
  return *this;
}

GenericRecord::~GenericRecord()
{
  delete[] m_Data;
  m_DataSize = 0;
}

#ifndef SR_NO_RECORD_EQUALITY
bool GenericRecord::equals(const GenericRecord& other) const
{
  if ((m_DataSize==other.m_DataSize) and (Header==other.Header))
  {
    if (m_DataSize!=0)
    {
      return (memcmp(m_Data, other.m_Data, m_DataSize)==0);
    }
    return true;
  }//if
  return false;
}
#endif

bool GenericRecord::isGenericRecord() const
{
  return true;
}

uint32_t GenericRecord::getRecordType() const
{
  //no record should have zero here, so we use this to avoid trouble
  return 0;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t GenericRecord::getWriteSize() const
{
  return m_DataSize;
}

bool GenericRecord::saveToStream(std::ofstream& output) const
{
  //record header
  output.write((const char*) &Header, 4);
  //record size and unknown values
  if (!saveSizeAndUnknownValues(output, m_DataSize)) return false;
  //write the real data
  output.write((const char*) m_Data, m_DataSize);
  return output.good();
}
#endif

bool GenericRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t Size;
  if (!loadSizeAndUnknownValues(in_File, Size)) return false;
  //prevent excessive memory usage
  if (Size>256*1024)
  {
    std::cout << "GenericRecord::loadFromStream: Error: record size is larger than 256 KB, aborting.\n";
    return false;
  }
  //allocate new memory
  unsigned char * tempPtr = NULL;
  tempPtr = new unsigned char[Size];
  memset(tempPtr, '\0', Size);
  in_File.read((char*) tempPtr, Size);
  if (!in_File.good())
  {
    std::cout << "GenericRecord::loadFromStream: Error while reading data!\n";
    delete[] tempPtr;
    return false;
  }
  //set data pointer and size according to the read values
  delete[] m_Data;
  m_Data = tempPtr;
  m_DataSize = Size;
  return true;
}

const unsigned char* GenericRecord::getDataPointer() const
{
  return m_Data;
}

uint32_t GenericRecord::getDataSize() const
{
  return m_DataSize;
}

} //namespace
