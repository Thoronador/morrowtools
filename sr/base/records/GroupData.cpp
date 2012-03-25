/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

const uint32_t GroupData::cTopLevelGroup = 0;
const uint32_t GroupData::cCellChildren = 6;
const uint32_t GroupData::cCellPersistentChildren = 8;

GroupData::GroupData()
{
  m_GroupSize = 0;
  m_GroupLabel = 0;
  m_GroupType = 0;
  unsigned int i;
  for (i=0; i<2; ++i)
  {
    UnknownGroupDataTwo[i]=0;
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
  //write label
  output.write((char*) &m_GroupLabel, 4);
  //write type
  output.write((char*) &m_GroupType, 4);
  //write unknown data
  unsigned int i;
  for (i=0; i<2; ++i)
  {
    output.write((char*) &(UnknownGroupDataTwo[i]), 4);
  }//for

  return output.good();
}

bool GroupData::loadFromStream(std::ifstream& in_File)
{
  //read size
  in_File.read((char*) &m_GroupSize, 4);
  //read label
  in_File.read((char*) &m_GroupLabel, 4);
  //read type
  in_File.read((char*) &m_GroupType, 4);
  //read unknown data
  unsigned int i;
  for (i=0; i<2; ++i)
  {
    in_File.read((char*) &(UnknownGroupDataTwo[i]), 4);
  }//for

  return in_File.good();
}

uint32_t GroupData::getGroupLabel() const
{
  return m_GroupLabel;
}

uint32_t GroupData::getGroupSize() const
{
  return m_GroupSize;
}

uint32_t GroupData::getGroupType() const
{
  return m_GroupType;
}

void GroupData::setGroupLabel(const uint32_t newLabel)
{
  m_GroupLabel = newLabel;
}

void GroupData::setGroupSize(const uint32_t newSize)
{
  m_GroupSize = newSize;
}

void GroupData::setGroupType(const uint32_t newType)
{
  m_GroupType = newType;
}

bool GroupData::operator==(const GroupData& other) const
{
  return ((m_GroupSize==other.getGroupSize()) and (m_GroupLabel==other.getGroupLabel())
      and (m_GroupType==other.getGroupType()) and (UnknownGroupDataTwo[0]==other.UnknownGroupDataTwo[0])
      and (UnknownGroupDataTwo[1]==other.UnknownGroupDataTwo[1]));
}

} //namespace
