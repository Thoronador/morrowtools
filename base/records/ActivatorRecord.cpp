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

#include "ActivatorRecord.h"
#include <iostream>
#include <cstring>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

ActivatorRecord::ActivatorRecord()
{
  ActivatorID = "";
  ModelPath = "";
  ItemName = "";
  ScriptName = "";
}

bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return ((ActivatorID==other.ActivatorID) and (ModelPath==other.ModelPath)
      and (ItemName==other.ItemName) and (ScriptName==other.ScriptName));
}

bool ActivatorRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cACTI, 4);
  uint32_t Size;
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
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = ActivatorID.length()+1;
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

bool ActivatorRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Subrecord NAME of ACTI is longer than 255 characters!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read activator ID
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of ACTI!\n";
    return false;
  }
  ActivatorID = std::string(Buffer);
  //read MODL
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Subrecord MODL of ACTI is longer than 255 characters!\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of ACTI!\n";
    return false;
  }
  ModelPath = std::string(Buffer);
  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Subrecord FNAM of ACTI is longer than 255 characters!\n";
    return false;
  }
  //read item name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of ACTI!\n";
    return false;
  }

  if (BytesRead<Size)
  {
    //read optional SCRI
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName!=cSCRI)
    {
      UnexpectedRecord(cSCRI, SubRecName);
      return false;
    }
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    if (SubLength>255)
    {
      std::cout << "Subrecord SCRI of ACTI is longer than 255 characters!\n";
      return false;
    }
    //read script ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SCRI of ACTI!\n";
      return false;
    }
    ScriptName = std::string(Buffer);
  }
  else
  {
    ScriptName = "";
  }
  return in_File.good();
}

} //namespace
