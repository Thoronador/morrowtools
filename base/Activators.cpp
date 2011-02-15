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

#include "Activators.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

ActivatorRecord::ActivatorRecord()
{
  ModelPath = "";
  ItemName = "";
  ScriptName = "";
}

bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return ((ModelPath==other.ModelPath) and (ItemName==other.ItemName)
         and (ScriptName==other.ScriptName));
}

Activators::Activators()
{
  //empty
}

Activators::~Activators()
{
  //empty
}

Activators& Activators::getSingleton()
{
  static Activators Instance;
  return Instance;
}

void Activators::addActivator(const std::string& ID, const ActivatorRecord& record)
{
  if (ID!="")
  {
    m_Activators[ID] = record;
  }
}

bool Activators::hasActivator(const std::string& ID) const
{
  return (m_Activators.find(ID)!=m_Activators.end());
}

unsigned int Activators::getNumberOfActivators() const
{
  return m_Activators.size();
}

const ActivatorRecord& Activators::getActivator(const std::string& ID) const
{
  std::map<std::string, ActivatorRecord>::const_iterator iter = m_Activators.find(ID);
  if (iter!=m_Activators.end())
  {
    return iter->second;
  }
  std::cout << "No activator with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

ActivatorListIterator Activators::getBegin() const
{
  return m_Activators.begin();
}

ActivatorListIterator Activators::getEnd() const
{
  return m_Activators.end();
}

void Activators::clearAll()
{
  m_Activators.clear();
}

int Activators::readRecordACTI(std::ifstream& in_File, const int32_t FileSize)
{
  #warning Replace with proper code!
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

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
    std::cout << "Subrecord NAME of ACTI is longer than 255 characters!\n";
    return -1;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read activator ID
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of ACTI!\n";
    return -1;
  }
  const std::string ActivatorID = std::string(Buffer);
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
    std::cout << "Subrecord MODL of ACTI is longer than 255 characters!\n";
    return -1;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of ACTI!\n";
    return -1;
  }
  ActivatorRecord temp;
  temp.ModelPath = std::string(Buffer);
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
    std::cout << "Subrecord FNAM of ACTI is longer than 255 characters!\n";
    return -1;
  }
  //read item name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of ACTI!\n";
    return -1;
  }

  if (in_File.tellg()<FileSize)
  {
    //read optional SCRI
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cSCRI)
    {
      //SCRI's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength>255)
      {
        std::cout << "Subrecord SCRI of ACTI is longer than 255 characters!\n";
        return -1;
      }
      //read script ID
      memset(Buffer, '\0', 256);
      in_File.read(Buffer, SubLength);
      if (!in_File.good())
      {
        std::cout << "Error while reading subrecord SCRI of ACTI!\n";
        return -1;
      }
      temp.ScriptName = std::string(Buffer);
    }
    else
    {
      //seek four bytes towards beginning to land before the name of the next record
      in_File.seekg(-4, std::ios::cur);
      temp.ScriptName = "";
    }
  }//if
  //add it to the list, if not present with same data
  if (hasActivator(ActivatorID))
  {
    if (getActivator(ActivatorID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if activator present
  addActivator(ActivatorID, temp);
  return 1;
} //readRecordACTI
