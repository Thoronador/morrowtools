/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "GenericRecord.h"
#include <iostream>
#include <cstring>

GenericRecord::GenericRecord()
{
  m_DataSize = 0;
  m_Data = NULL;
  Header = 0;
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
  //header flags
  output.write((char*) &m_DataSize, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);
  //write the real data
  output.write(m_Data, m_DataSize);

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
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  //allocate new memory
  char * tempPtr = NULL;
  tempPtr = new char[Size];
  memset(tempPtr, '\0', Size);
  in_File.read(tempPtr, Size);
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

const char* GenericRecord::getDataPointer() const
{
  return m_Data;
}

uint32_t GenericRecord::getDataSize() const
{
  return m_DataSize;
}
