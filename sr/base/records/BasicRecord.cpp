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

#include "BasicRecord.h"
#include <iostream>

namespace SRTP
{

BasicRecord::BasicRecord()
{
  Unknown[3] = Unknown[2] = Unknown[1] = Unknown[0] = 0;
}

BasicRecord::~BasicRecord()
{
  //empty
  //Purpose is to have a virtual destructor to get rid of compiler warnings.
}

bool BasicRecord::loadSizeAndUnknownValues(std::ifstream& in_File, uint32_t& sizeStorage)
{
  in_File.read((char*) &sizeStorage, 4);
  //unknown values
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    in_File.read((char*) &(Unknown[i]), 4);
  }//for
  if (!in_File.good())
  {
    std::cout << "BasicRecord::loadSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

bool BasicRecord::saveSizeAndUnknownValues(std::ofstream& output, const uint32_t theSize) const
{
  //record size
  output.write((char*) &theSize, 4);
  //unknown values
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    output.write((char*) &(Unknown[i]), 4);
  }//for
  if (!output.good())
  {
    std::cout << "BasicRecord::saveSizeAndUnknownValues: Error while reading "
              << "record size and unknown header data.\n";
    return false;
  }
  return true;
}

} //namespace
