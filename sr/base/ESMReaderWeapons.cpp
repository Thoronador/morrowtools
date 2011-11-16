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

int ESMReaderWeapons::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  int32_t recName;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    if (recName!=cWEAP)
    {
      UnexpectedRecord(cWEAP, recName);
      return -1;
    }
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
  std::cout << "ESMReaderWeapons::readGroup: Error while reading weapon record!"
            << "\nCurrent position is "<<in_File.tellg()<<" bytes.\n";
  return -1;
}

bool ESMReaderWeapons::needGroup(const GroupData& g_data) const
{
  return (g_data.getGroupName()==cWEAP);
}

int ESMReaderWeapons::readNextRecord(std::ifstream& in_File)
{
  #warning Not implemented yet!
  std::cout << "Error: call to unimplemented function ESMReaderWeapons::readNextRecord!\n";
  return -1;
}

} //namespace
