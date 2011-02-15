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

bool ActivatorRecord::saveToStream(std::ofstream& output, const std::string& ActivatorID) const
{
  output.write((char*) &cACTI, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +ActivatorID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for MODL's length */
        +ModelPath.length()+1 /*length of mesh plus one for NUL-termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +ItemName.length() +1 /* length of name +1 byte for NUL termination */;
  if (ScriptName!="")
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptName.length()+1 /*length of script ID + one byte for NUL-termination */;
  }
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  //write NAME
  output.write((char*) &cNAME, 4);
  int32_t SubLength = ActivatorID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write NAME/ID
  output.write(ActivatorID.c_str(), SubLength);
  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = ModelPath.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write MODL/ mesh path
  output.write(ModelPath.c_str(), SubLength);
  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = ItemName.length()+1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write FNAM/ item name
  output.write(ItemName.c_str(), SubLength);
  if (ScriptName!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = ScriptName.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write Script ID
    output.write(ScriptName.c_str(), SubLength);
  }//if script ID present
  return output.good();
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

bool Activators::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Activators::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ActivatorListIterator iter = m_Activators.begin();
  const ActivatorListIterator end_iter = m_Activators.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output, iter->first))
    {
      std::cout << "Activators::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Activators::clearAll()
{
  m_Activators.clear();
}

int Activators::readRecordACTI(std::ifstream& in_File, const int32_t FileSize)
{
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
