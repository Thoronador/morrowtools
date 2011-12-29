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

#include "GroupData.h"
#include "../SR_Constants.h"

namespace SRTP
{

GroupData::GroupData()
{
  m_GroupSize = 0;
  m_GroupName = 0;
  unsigned int i;
  for (i=0; i<3; ++i)
  {
    UnknownGroupData[i]=0;
  }//for
}

GroupData::~GroupData()
{
  //empty
}

bool GroupData::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cGRUP, 4);
  //write size
  output.write((char*) &m_GroupSize, 4);
  //write name
  output.write((char*) &m_GroupName, 4);
  //write unknown data
  unsigned int i;
  for (i=0; i<3; ++i)
  {
    output.write((char*) &(UnknownGroupData[i]), 4);
  }//for

  return output.good();
}

bool GroupData::loadFromStream(std::ifstream& in_File)
{
  //read size
  in_File.read((char*) &m_GroupSize, 4);
  //read name
  in_File.read((char*) &m_GroupName, 4);
  //read unknown data
  unsigned int i;
  for (i=0; i<3; ++i)
  {
    in_File.read((char*) &(UnknownGroupData[i]), 4);
  }//for

  return in_File.good();
}

int32_t GroupData::getGroupName() const
{
  return m_GroupName;
}

uint32_t GroupData::getGroupSize() const
{
  return m_GroupSize;
}

void GroupData::setGroupName(const int32_t newName)
{
  m_GroupName = newName;
}

void GroupData::setGroupSize(const uint32_t newSize)
{
  m_GroupSize = newSize;
}

} //namespace
