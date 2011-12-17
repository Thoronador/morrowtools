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

#include "BSAFileRecord.h"
#include <iostream>

namespace SRTP
{

BSAFileRecord::BSAFileRecord()
{
  nameHash = 0;
  fileSize = 0;
  offset = 0;
  fileName = "";
}

BSAFileRecord::~BSAFileRecord()
{
  //empty
}

bool BSAFileRecord::loadFromStream(std::ifstream& in_File)
{
  if (!in_File.good())
  {
    std::cout << "BSAFileRecord::loadFromStream: Error: bad stream given!\n";
    return false;
  }
  //read file record's stuff
  in_File.read((char*) &nameHash, sizeof(BSAHash));
  if (!in_File.good())
  {
    std::cout << "BSAFileRecord::loadFromStream: Error: could not read hash!\n";
    return false;
  }
  //rest of it
  in_File.read((char*) &fileSize, 4);
  in_File.read((char*) &offset, 4);
  if (!in_File.good())
  {
    std::cout << "BSAFileRecord::loadFromStream: Error while reading data!\n";
    return false;
  }
  fileName.clear();
  //all's well
  return true;
}

bool BSAFileRecord::isCompressionToggled() const
{
  return ((fileSize & (1<<30))!=0);
}

uint32_t BSAFileRecord::getRealFileSize() const
{
  return (fileSize & (~(1<<30)));
}

} //namespace
