/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "BSAFolderRecord.h"
#include <iostream>

namespace SRTP
{

BSAFolderRecord::BSAFolderRecord()
: nameHash(0),
  count(0),
  offset(0)
{
}

BSAFolderRecord::~BSAFolderRecord()
{
  //empty
}

bool BSAFolderRecord::loadFromStream(std::ifstream& in_File)
{
  if (!in_File.good())
  {
    std::cout << "BSAFolderRecord::loadFromStream: Error: bad stream given!\n";
    return false;
  }
  //read folder record's stuff
  in_File.read((char*) &nameHash, sizeof(BSAHash));
  if (!in_File.good())
  {
    std::cout << "BSAFolderRecord::loadFromStream: Error: could not read hash!\n";
    return false;
  }
  //rest of it
  in_File.read((char*) &count, 4);
  in_File.read((char*) &offset, 4);
  if (!in_File.good())
  {
    std::cout << "BSAFolderRecord::loadFromStream: Error while reading data!\n";
    return false;
  }
  //all's well
  return true;
}

} //namespace
