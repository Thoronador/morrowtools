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

#include "Apparatuses.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

ApparatusRecord::ApparatusRecord()
{
  Model = "";
  ItemName = "";
  Type = -1;
  Quality = 0.0f;
  Weight = -1.0f;
  Value = -1 ;
  InventoryIcon = "";
  ScriptName = "";
}

bool ApparatusRecord::equals(const ApparatusRecord& other) const
{
  return ((Model==other.Model) and (ItemName==other.ItemName)
      and (Type==other.Type) and (Quality==other.Quality)
      and (Weight==other.Weight) and (Value==other.Value)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName));
}

Apparatuses::Apparatuses()
{
  //empty
}

Apparatuses::~Apparatuses()
{
  //empty
}

Apparatuses& Apparatuses::getSingleton()
{
  static Apparatuses Instance;
  return Instance;
}

void Apparatuses::addApparatus(const std::string& ID, const ApparatusRecord& record)
{
  if (ID!="")
  {
    m_Apparatuses[ID] = record;
  }
}

bool Apparatuses::hasApparatus(const std::string& ID) const
{
  return (m_Apparatuses.find(ID)!=m_Apparatuses.end());
}

unsigned int Apparatuses::getNumberOfApparatuses() const
{
  return m_Apparatuses.size();
}

const ApparatusRecord& Apparatuses::getApparatus(const std::string& ID) const
{
  std::map<std::string, ApparatusRecord>::const_iterator iter = m_Apparatuses.find(ID);
  if (iter!=m_Apparatuses.end())
  {
    return iter->second;
  }
  std::cout << "No apparatus with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ApparatusListIterator Apparatuses::getBegin() const
{
  return m_Apparatuses.begin();
}

ApparatusListIterator Apparatuses::getEnd() const
{
  return m_Apparatuses.end();
}

void Apparatuses::clearAll()
{
  m_Apparatuses.clear();
}

int Apparatuses::readRecordAPPA(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Alchemy Apparatus:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    AADT = Alchemy Data (16 bytes), required
        long    Type (0=Mortar and Pestle,1=Albemic,2=Calcinator,3=Retort)
        float	Quality
        float	Weight
        long	Value
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  long int SubRecName, SubLength;
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
    std::cout << "Subrecord NAME of APPA is longer than 255 characters!\n";
    return -1;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read apparatus ID
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of APPA!\n";
    return -1;
  }
  const std::string ApparatusID = std::string(Buffer);

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
    std::cout << "Subrecord MODL of APPA is longer than 255 characters!\n";
    return -1;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of APPA!\n";
    return -1;
  }
  ApparatusRecord temp;
  temp.Model = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return -1;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Subrecord FNAM of APPA is longer than 255 characters!\n";
    return -1;
  }
  //read apparatus name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of APPA!\n";
    return -1;
  }
  temp.ItemName = std::string(Buffer);

  //read AADT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cAADT)
  {
    UnexpectedRecord(cAADT, SubRecName);
    return -1;
  }
  //AADT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record AADT of APPA has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return -1;
  }
  //read apparatus data
  in_File.read((char*) &(temp.Type), 4);
  in_File.read((char*) &(temp.Quality), 4);
  in_File.read((char*) &(temp.Weight), 4);
  in_File.read((char*) &(temp.Value), 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord AADT of APPA!\n";
    return -1;
  }
  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return -1;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  //read apparatus icon path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of APPA!\n";
    return -1;
  }
  temp.InventoryIcon = std::string(Buffer);

  if (in_File.tellg()>=FileSize)
  { //there's no optional part, if end of file is already reached
    //read optional SCRI
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cSCRI)
    {
      //SCRI's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength>255)
      {
        std::cout << "Subrecord SCRI of APPA is longer than 255 characters!\n";
        return -1;
      }
      //read apparatus script ID
      memset(Buffer, '\0', 256);
      in_File.read(Buffer, SubLength);
      if (!in_File.good())
      {
        std::cout << "Error while reading subrecord SCRI of APPA!\n";
        return -1;
      }
      temp.ScriptName = std::string(Buffer);
    }
    else
    {
      //seek four bytes towards beginning to land before next record name
      in_File.seekg(-4, std::ios::cur);
      temp.ScriptName = "";
    }
  }
  //add it to the list, if not present with same data
  if (hasApparatus(ApparatusID))
  {
    if (getApparatus(ApparatusID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if apparatus present
  addApparatus(ApparatusID, temp);
  return 1;
} //readRecordAPPA
