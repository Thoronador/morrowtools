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

#include "ApparatusRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

ApparatusRecord::ApparatusRecord()
{
  ApparatusID = "";
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
  return ((ApparatusID==other.ApparatusID)
      and (Model==other.Model) and (ItemName==other.ItemName)
      and (Type==other.Type) and (Quality==other.Quality)
      and (Weight==other.Weight) and (Value==other.Value)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName));
}

bool ApparatusRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cAPPA, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +ApparatusID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for MODL's length */
        +Model.length()+1 /*length of mesh plus one for NUL-termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +ItemName.length() +1 /* length of name +1 byte for NUL termination */
        +4 /* AADT */ +4 /* 4 bytes for length */ +16 /* length of AADT */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length() +1 /* length of icon path +1 byte for NUL termination */;
  if (ScriptName!="")
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptName.length()+1 /*length of script ID + one byte for NUL-termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = ApparatusID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write NAME/ID
  output.write(ApparatusID.c_str(), SubLength);
  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = Model.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write MODL/ mesh path
  output.write(Model.c_str(), SubLength);
  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = ItemName.length()+1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write FNAM/ item name
  output.write(ItemName.c_str(), SubLength);
  //write AADT
  output.write((char*) &cAADT, 4);
  SubLength = 16;
  //write AADT's length
  output.write((char*) &SubLength, 4);
  //write AADT/ alchemy apparatus data
  output.write((char*) &Type, 4);
  output.write((char*) &Quality, 4);
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 4);

  //write ITEX
  output.write((char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((char*) &SubLength, 4);
  //write ITEX/ inventory icon
  output.write(InventoryIcon.c_str(), SubLength);
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

bool ApparatusRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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
    std::cout << "Subrecord NAME of APPA is longer than 255 characters!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read apparatus ID
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of APPA!\n";
    return false;
  }
  ApparatusID = std::string(Buffer);

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
    std::cout << "Subrecord MODL of APPA is longer than 255 characters!\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of APPA!\n";
    return false;
  }
  Model = std::string(Buffer);

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
    std::cout << "Subrecord FNAM of APPA is longer than 255 characters!\n";
    return false;
  }
  //read apparatus name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of APPA!\n";
    return false;
  }
  ItemName = std::string(Buffer);

  //read AADT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cAADT)
  {
    UnexpectedRecord(cAADT, SubRecName);
    return false;
  }
  //AADT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record AADT of APPA has invalid length ("<<SubLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  //read apparatus data
  in_File.read((char*) &Type, 4);
  in_File.read((char*) &Quality, 4);
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  BytesRead += 16;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord AADT of APPA!\n";
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
  //read apparatus icon path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of APPA!\n";
    return false;
  }
  InventoryIcon = std::string(Buffer);

  if (BytesRead<Size)
  { //there's no optional part, if whole size of record was already read
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
      std::cout << "Subrecord SCRI of APPA is longer than 255 characters!\n";
      return false;
    }
    //read apparatus script ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SCRI of APPA!\n";
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
