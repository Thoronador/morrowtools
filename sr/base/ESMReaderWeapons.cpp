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

#include "ESMReaderWeapons.h"
#include <iostream>
#include "../../mw/base/HelperIO.h"
#include "SR_Constants.h"
#include "Weapons.h"

namespace SRTP
{

ESMReaderWeapons::ESMReaderWeapons()
{
  //empty
}

ESMReaderWeapons::~ESMReaderWeapons()
{
  //empty
}

int ESMReaderWeapons::readNextGroup(std::ifstream& in_File)
{
  int32_t recordHeader = 0;
  //read "GRUP"
  in_File.read((char*) &recordHeader, 4);
  if (!in_File.good())
  {
    std::cout << "ESMReaderWeapons::readNextGroup: Error: could not read group header!\n";
    return -1;
  }
  if (recordHeader!=cGRUP)
  {
    UnexpectedRecord(cGRUP, recordHeader);
    return -1;
  }
  //now read the group data header
  GroupData gd;
  if (!gd.loadFromStream(in_File))
  {
    std::cout << "ESMReaderWeapons::readNextGroup: Error: could not read group data!\n";
    return -1;
  }
  //is it the weapons group?
  if (gd.getGroupName()!=cWEAP)
  {
    //we skip the group, it's not a WEAP group
    return skipGroup(in_File, gd);
  }
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(gd.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    lastResult = Weapons::getSingleton().readRecordWEAP(in_File);
    if (lastResult>0)
    {
      recordsRead += lastResult;
    }
  }//while
  if (lastResult>=0)
  {
    if (recordsRead>0) return 1;
    return 0;
  }
  return -1;
}

int ESMReaderWeapons::readNextRecord(std::ifstream& in_File)
{
  #warning Not implemented yet!
  std::cout << "Error: call to unimplemented function ESMReaderWeapons::readNextRecord!\n";
  return -1;
}

} //namespace
