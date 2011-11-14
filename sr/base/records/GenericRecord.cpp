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

#include "GenericRecord.h"
#include <cstring>
#include <iostream>

namespace SRTP
{

GenericRecord::GenericRecord()
{
  m_DataSize = 0;
  m_Data = NULL;
  Header = 0;
  Unknown[3] = Unknown[2] = Unknown[1] = Unknown[0] = 0;
}

GenericRecord::GenericRecord(const GenericRecord& other)
{
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
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    Unknown[i] = other.Unknown[i];
  }//for
}

GenericRecord& GenericRecord::operator=(const GenericRecord& other)
{
  if (this==&other) return *this;
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
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    Unknown[i] = other.Unknown[i];
  }//for
  return *this;
}

GenericRecord::~GenericRecord()
{
  delete[] m_Data;
  m_DataSize = 0;
}

bool GenericRecord::saveToStream(std::ofstream& output) const
{
  //record header
  output.write((char*) &Header, 4);
  //record size
  output.write((char*) &m_DataSize, 4);
  //unknown values
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    output.write((char*) &(Unknown[i]), 4);
  }//for
  //write the real data
  output.write((char*) m_Data, m_DataSize);
  return output.good();
}

bool GenericRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  //prevent excessive memory usage
  if (Size>256*1024)
  {
    std::cout << "GenericRecord::loadFromStream: Error: record size is larger than 256 KB, aborting.\n";
    return false;
  }
  //unknown values
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    in_File.read((char*) &(Unknown[i]), 4);
  }//for
  if (!in_File.good())
  {
    std::cout << "GenericRecord::loadFromStream: Error while reading record size"
              << " and unknown header data.\n";
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
