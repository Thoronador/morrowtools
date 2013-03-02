/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "GroupData.h"
#include "../SR_Constants.h"

namespace SRTP
{

const uint32_t GroupData::cTopLevelGroup = 0;
const uint32_t GroupData::cWorldspaceChildren = 1;
const uint32_t GroupData::cCellChildren = 6;
const uint32_t GroupData::cTopicChildren = 7;
const uint32_t GroupData::cCellPersistentChildren = 8;
const uint32_t GroupData::cCellTemporaryChildren = 9;
const uint32_t GroupData::cCellVisibleDistantChildren = 10;

GroupData::GroupData()
: m_GroupSize(0), m_GroupLabel(0), m_GroupType(0)
{
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

#ifndef SR_UNSAVEABLE_RECORDS
bool GroupData::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cGRUP, 4);
  //write size
  output.write((const char*) &m_GroupSize, 4);
  //write label
  output.write((const char*) &m_GroupLabel, 4);
  //write type
  output.write((const char*) &m_GroupType, 4);
  //write unknown data
  unsigned int i;
  for (i=0; i<2; ++i)
  {
    output.write((const char*) &(UnknownGroupDataTwo[i]), 4);
  }//for

  return output.good();
}
#endif

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

bool GroupData::labelIsCellID() const
{
  switch (m_GroupType)
  {
    case cTopLevelGroup:
         return false;
    case cCellChildren:
    case cCellPersistentChildren:
    case cCellTemporaryChildren:
    case cCellVisibleDistantChildren:
         return true;
    default:
         return false;
  }//swi
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
