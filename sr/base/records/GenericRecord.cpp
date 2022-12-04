/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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
#include <cstring>
#include <iostream>

namespace SRTP
{

GenericRecord::GenericRecord()
: BasicRecord(),
  Header(0),
  m_Size(0),
  m_Data(nullptr)
{
}

GenericRecord::GenericRecord(const GenericRecord& other)
: Header(other.Header),
  m_Size(other.size()),
  m_Data(nullptr)
{
  copyBasicMembers(other);
  if (m_Size != 0)
  {
    m_Data = new unsigned char[m_Size];
    memcpy(m_Data, other.data(), m_Size);
  }
}

GenericRecord& GenericRecord::operator=(const GenericRecord& other)
{
  if (this == &other)
    return *this;
  copyBasicMembers(other);
  delete[] m_Data;
  Header = other.Header;
  m_Size = other.size();
  if (m_Size != 0)
  {
    m_Data = new unsigned char[m_Size];
    memcpy(m_Data, other.data(), m_Size);
  }
  else
  {
    m_Data = nullptr;
  }
  return *this;
}

GenericRecord::~GenericRecord()
{
  delete[] m_Data;
  m_Size = 0;
}

#ifndef SR_NO_RECORD_EQUALITY
bool GenericRecord::equals(const GenericRecord& other) const
{
  if ((m_Size == other.m_Size) && (Header == other.Header))
  {
    if (m_Size != 0)
    {
      return memcmp(m_Data, other.m_Data, m_Size) == 0;
    }
    return true;
  }
  return false;
}
#endif

uint32_t GenericRecord::getRecordType() const
{
  // No record should have zero here, so we use this to avoid trouble.
  return 0;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t GenericRecord::getWriteSize() const
{
  return m_Size;
}

bool GenericRecord::saveToStream(std::ostream& output) const
{
  // record header
  output.write(reinterpret_cast<const char*>(&Header), 4);
  // record size and unknown values
  if (!saveSizeAndUnknownValues(output, m_Size))
    return false;
  // write the real data
  output.write(reinterpret_cast<const char*>(m_Data), m_Size);
  return output.good();
}
#endif

bool GenericRecord::loadFromStream(std::istream& in_File,
                                   [[maybe_unused]] const bool localized,
                                   [[maybe_unused]] const StringTable& table)
{
  uint32_t Size;
  if (!loadSizeAndUnknownValues(in_File, Size))
    return false;
  // prevent excessive memory usage
  if (Size > 256 * 1024)
  {
    std::cerr << "GenericRecord::loadFromStream: Error: Record size is larger than 256 KB, aborting.\n";
    return false;
  }
  // allocate new memory
  unsigned char * tempPtr = new unsigned char[Size];
  memset(tempPtr, '\0', Size);
  in_File.read(reinterpret_cast<char*>(tempPtr), Size);
  if (!in_File.good())
  {
    std::cerr << "GenericRecord::loadFromStream: Error while reading data!\n";
    delete[] tempPtr;
    return false;
  }
  // set data pointer and size according to the read values
  delete[] m_Data;
  m_Data = tempPtr;
  m_Size = Size;
  return true;
}

const unsigned char* GenericRecord::data() const
{
  return m_Data;
}

uint32_t GenericRecord::size() const
{
  return m_Size;
}

} // namespace
