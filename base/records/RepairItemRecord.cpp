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

#include "RepairItemRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

RepairItemRecord::RepairItemRecord()
{
  RepairItemID = ModelPath = Name = "";
  //repair data
  Weight = 0.0f;
  Value = 0;
  Uses = 0;
  Quality = 0.0f;
  //end of repair data
  InventoryIcon = ScriptName = "";
}

RepairItemRecord::RepairItemRecord(const std::string& ID)
{
  RepairItemID = ID;
  ModelPath = Name = "";
  //repair data
  Weight = 0.0f;
  Value = 0;
  Uses = 0;
  Quality = 0.0f;
  //end of repair data
  InventoryIcon = ScriptName = "";
}

RepairItemRecord::~RepairItemRecord()
{
  //empty
}

bool RepairItemRecord::equals(const RepairItemRecord& other) const
{
  return ((RepairItemID==other.RepairItemID) and (ModelPath==other.ModelPath)
      and (Name==other.Name) and (Weight==other.Weight) and (Value==other.Value)
      and (Uses==other.Uses) and (Quality==other.Quality)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName));
}

bool RepairItemRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cREPA, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +RepairItemID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* RIDT */ +4 /* RIDT's length */ +16 /*size of RIDT (always 16 bytes)*/
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of icon path +1 byte for NUL termination */;
  if (ScriptName!="")
  {
    Size = Size +4 /* SCRIM */ +4 /* 4 bytes for length */
          +ScriptName.length()+1; /* length of script ID +1 byte for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Repair Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    RIDT = Repair Data (16 bytes), required
        float	Weight
        long	Value
        long	Uses
        float	Quality
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = RepairItemID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(RepairItemID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  //MODL's length
  SubLength = ModelPath.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  SubLength = Name.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write repair item name
  output.write(Name.c_str(), SubLength);

  //write RIDT
  output.write((char*) &cRIDT, 4);
  //RIDT's length
  SubLength = 16;//length of RIDT is always 16 bytes
  output.write((char*) &SubLength, 4);
  //write repair item data
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 4);
  output.write((char*) &Uses, 4);
  output.write((char*) &Quality, 4);

  //write ITEX
  output.write((char*) &cITEX, 4);
  //ITEX's length
  SubLength = InventoryIcon.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write inventory icon path
  output.write(InventoryIcon.c_str(), SubLength);

  if (ScriptName!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    //SCRI's length
    SubLength = ScriptName.length()+1;//length of string plus one for NUL-termination
    output.write((char*) &SubLength, 4);
    //write script name/ID
    output.write(ScriptName.c_str(), SubLength);
  }//if

  return output.good();
}

bool RepairItemRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Repair Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    RIDT = Repair Data (16 bytes), required
        float	Weight
        long	Value
        long	Uses
        float	Quality
    ITEX = Inventory Icon
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
    std::cout << "Error: subrecord NAME of REPA is longer than 255 characters.\n";
    return false;
  }
  //read repair item ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of REPA.\n";
    return false;
  }
  RepairItemID = std::string(Buffer);

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
    std::cout << "Error: subrecord MODL of REPA is longer than 255 characters.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of REPA.\n";
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
    std::cout << "Error: subrecord FNAM of REPA is longer than 255 characters.\n";
    return false;
  }
  //read name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of REPA.\n";
    return false;
  }
  Name = std::string(Buffer);

  //read RIDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cRIDT)
  {
    UnexpectedRecord(cRIDT, SubRecName);
    return false;
  }
  //RIDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=16)
  {
    std::cout << "Error: sub record RIDT of REPA has invalid length ("
              <<SubLength<< " bytes). Should be 16 bytes.\n";
    return false;
  }//if
  //read item data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Uses, 4);
  in_File.read((char*) &Quality, 4);
  BytesRead += 16;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord RIDT of REPA.\n";
    return false;
  }

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord FNAM of REPA is longer than 255 characters.\n";
    return false;
  }
  //read icon texture path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of REPA.\n";
    return false;
  }
  InventoryIcon = std::string(Buffer);

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
      std::cout << "Error: subrecord SCRI of REPA is longer than 255 characters.\n";
      return false;
    }
    //read script name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SCRI of REPA.\n";
      return false;
    }
    ScriptName = std::string(Buffer);
  }//if
  else
  {
    ScriptName = "";
  }
  return in_File.good();
}

bool operator<(const RepairItemRecord& left, const RepairItemRecord& right)
{
  return (left.RepairItemID.compare(right.RepairItemID)<0);
}

} //namespace
