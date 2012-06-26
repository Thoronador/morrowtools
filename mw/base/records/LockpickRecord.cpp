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

#include "LockpickRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

LockpickRecord::LockpickRecord()
{
  LockpickID = Name = ModelPath = "";
  //lockpick data
  Weight = 0.0f;
  Value = 0;
  Quality = 0.0f;
  Uses = 0;
  //end of lockpick data
  InventoryIcon = ScriptID = "";
}

bool LockpickRecord::equals(const LockpickRecord& other) const
{
  return ((LockpickID==other.LockpickID) and (Name==other.Name)
      and (ModelPath==other.ModelPath) and (Weight==other.Weight)
      and (Value==other.Value) and (Quality==other.Quality)
      and (Uses==other.Uses) and (InventoryIcon==other.InventoryIcon)
      and (ScriptID==other.ScriptID));
}

bool LockpickRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLOCK, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +LockpickID.length()+1 /* length of mesh path +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of mesh path +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of mesh path +1 byte for NUL termination */
        +4 /* LKDT */ +4 /* 4 bytes for length */ + 16 /* size of lockpick data */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of mesh path +1 byte for NUL termination */;

  if (ScriptID!="")
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Lockpicks:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    LKDT = Lock Data (16 bytes), required
        float Weight
        long  Value
        float Quality
        long  Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = LockpickID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(LockpickID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = ModelPath.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = Name.length()+1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write lockpick's name
  output.write(Name.c_str(), SubLength);

  //write LKDT
  output.write((char*) &cLKDT, 4);
  SubLength = 16; /* length is always 16 bytes */
  //write LKDT's length
  output.write((char*) &SubLength, 4);
  //write lockpick data
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 4);
  output.write((char*) &Quality, 4);
  output.write((char*) &Uses, 4);

  //write ITEX
  output.write((char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((char*) &SubLength, 4);
  //write inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (ScriptID!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  return output.good();
}

bool LockpickRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Lockpicks:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    LKDT = Lock Data (16 bytes), required
        float Weight
        long  Value
        float Quality
        long  Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional)

    Note: Sequence of ITEX and SCRI could also be changed.
  */

  uint32_t SubRecName;
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
    std::cout << "Error: subrecord NAME of LOCK is longer than 255 bytes.\n";
    return false;
  }
  //read lockpick's ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of LOCK.\n";
    return false;
  }
  LockpickID = std::string(Buffer);

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
    std::cout << "Error: subrecord MODL of LOCK is longer than 255 bytes.\n";
    return false;
  }
  //read lockpick's model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of LOCK.\n";
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
    std::cout << "Error: subrecord FNAM of LOCK is longer than 255 bytes.\n";
    return false;
  }
  //read lockpick's real name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of LOCK.\n";
    return false;
  }
  Name = std::string(Buffer);

  //read LKDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cLKDT)
  {
    UnexpectedRecord(cLKDT, SubRecName);
    return false;
  }
  //LKDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record LKDT of LOCK has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  //read lockpick data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Quality, 4);
  in_File.read((char*) &Uses, 4);
  BytesRead += 16;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LKDT of LOCK.\n";
    return false;
  }

  ScriptID = "";
  InventoryIcon = "";
  bool hasITEX = false;
  bool hasSCRI = false;

  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           if (hasITEX)
           {
             std::cout << "Error: record LOCK seems to have two ITEX subrecords.\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ITEX of LOCK is longer than 255 bytes.\n";
             return false;
           }
           //read lockpick's icon texture path
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of LOCK.\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           hasITEX = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cout << "Error: record LOCK seems to have two SCRI subrecords.\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord SCRI of LOCK is longer than 255 bytes.\n";
             return false;
           }
           //read lockpick's script name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of LOCK.\n";
             return false;
           }
           ScriptID = std::string(Buffer);
           hasSCRI = true;
           break;
      default:
           //unexpected record found
           std::cout << "Error: expected record name ITEX or SCRI was not "
                     << "found. Instead, \""<<IntTo4Char(SubRecName)
                     << "\" was found.\n";
           return false;
    }//swi
  }//while

  //at least ITEX should have been present
  if (!hasITEX)
  {
    std::cout << "Error: record LOCK seems to have no ITEX subrecord.\n";
    return false;
  }

  return in_File.good();
}

} //namespace
