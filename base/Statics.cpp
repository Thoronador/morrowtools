/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011 Thoronador

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

#include "Statics.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

bool StaticRecord::equals(const StaticRecord& other) const
{
  return (Mesh==other.Mesh);
}

Statics::Statics()
{
  //empty
}

Statics::~Statics()
{
  //empty
}

Statics& Statics::getSingleton()
{
  static Statics Instance;
  return Instance;
}

void Statics::addStatic(const std::string& ID, const StaticRecord& record)
{
  if (ID!="")
  {
    m_Statics[ID] = record;
  }
}

bool Statics::hasStatic(const std::string& ID) const
{
  return (m_Statics.find(ID)!=m_Statics.end());
}

unsigned int Statics::getNumberOfStatics() const
{
  return m_Statics.size();
}

const StaticRecord& Statics::getStatic(const std::string& ID) const
{
  std::map<std::string, StaticRecord>::const_iterator iter = m_Statics.find(ID);
  if (iter!=m_Statics.end())
  {
    return iter->second;
  }
  std::cout << "No static with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

StaticListIterator Statics::getBegin() const
{
  return m_Statics.begin();
}

StaticListIterator Statics::getEnd() const
{
  return m_Statics.end();
}

void Statics::clearAll()
{
  m_Statics.clear();
}

int Statics::readRecordSTAT(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;
  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return -1;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of STAT is longer than 255 characters.\n";
    return -1;
  }
  //read NAME
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of STAT.\n";
    return -1;
  }
  const std::string StaticID = std::string(Buffer);
  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return -1;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord MODL of STAT is longer than 255 characters.\n";
    return -1;
  }
  //read MODL
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of STAT.\n";
    return -1;
  }
  StaticRecord temp;
  temp.Mesh = std::string(Buffer);

  //now add static to the list
  if ( hasStatic(StaticID))
  {
    if ( getStatic(StaticID).equals(temp))
    {
      //same static already present - add nothing and return zero
      return 0;
    }
  }
  addStatic(StaticID, temp);
  return 1;
} //readRecordSTAT
