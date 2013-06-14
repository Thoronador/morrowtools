/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013  Thoronador

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

#include "ApparatusRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

ApparatusRecord::ApparatusRecord()
: BasicRecord(),
  recordID(""),
  Model(""),
  ItemName(""),
  Type(-1),
  Quality(0.0f),
  Weight(-1.0f),
  Value(-1),
  InventoryIcon(""),
  ScriptName("")
{}

bool ApparatusRecord::equals(const ApparatusRecord& other) const
{
  return ((recordID==other.recordID)
      and (Model==other.Model) and (ItemName==other.ItemName)
      and (Type==other.Type) and (Quality==other.Quality)
      and (Weight==other.Weight) and (Value==other.Value)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ApparatusRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cAPPA, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for MODL's length */
        +Model.length()+1 /*length of mesh plus one for NUL-termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +ItemName.length() +1 /* length of name +1 byte for NUL termination */
        +4 /* AADT */ +4 /* 4 bytes for length */ +16 /* length of AADT */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length() +1 /* length of icon path +1 byte for NUL termination */;
  if (!ScriptName.empty())
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptName.length()+1 /*length of script ID + one byte for NUL-termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write NAME/ID
  output.write(recordID.c_str(), SubLength);
  //write MODL
  output.write((const char*) &cMODL, 4);
  SubLength = Model.length()+1;
  //write MODL's length
  output.write((const char*) &SubLength, 4);
  //write MODL/ mesh path
  output.write(Model.c_str(), SubLength);
  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = ItemName.length()+1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write FNAM/ item name
  output.write(ItemName.c_str(), SubLength);
  //write AADT
  output.write((const char*) &cAADT, 4);
  SubLength = 16;
  //write AADT's length
  output.write((const char*) &SubLength, 4);
  //write AADT/ alchemy apparatus data
  output.write((const char*) &Type, 4);
  output.write((const char*) &Quality, 4);
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);

  //write ITEX
  output.write((const char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((const char*) &SubLength, 4);
  //write ITEX/ inventory icon
  output.write(InventoryIcon.c_str(), SubLength);
  if (!ScriptName.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    SubLength = ScriptName.length()+1;
    //write SCRI's length
    output.write((const char*) &SubLength, 4);
    //write Script ID
    output.write(ScriptName.c_str(), SubLength);
  }//if script ID present
  return output.good();
}
#endif

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
  recordID = std::string(Buffer);

  Model.clear();
  ItemName.clear();
  bool hasReadAADT = false;
  InventoryIcon.clear();
  ScriptName.clear();

  while (BytesRead<Size)
  {
    //read sub record's name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cMODL:
           if (!Model.empty())
           {
             std::cout << "Error: APPA seems to have more than one MODL subrecord!\n";
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
           if (Model.empty())
           {
             std::cout << "Error: subrecord MODL of APPA is empty!\n";
             return false;
           }
           break;
      case cFNAM:
           if (!ItemName.empty())
           {
             std::cout << "Error: APPA seems to have more than one FNAM subrecord!\n";
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
           if (ItemName.empty())
           {
             std::cout << "Error: subrecord FNAM of APPA is empty!\n";
             return false;
           }
           break;
      case cAADT:
           if (hasReadAADT)
           {
             std::cout << "Error: APPA seems to have more than one AADT subrecord!\n";
             return false;
           }
           //AADT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=16)
           {
             std::cout << "Error: sub record AADT of APPA has invalid length ("
                       << SubLength << " bytes). Should be 16 bytes.\n";
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
           hasReadAADT = true;
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cout << "Error: APPA seems to have more than one ITEX subrecord!\n";
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
           if (InventoryIcon.empty())
           {
             std::cout << "Error: subrecord ITEX of APPA is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptName.empty())
           {
             std::cout << "Error: APPA seems to have more than one SCRI subrecord!\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Subrecord SCRI of APPA is longer than 255 characters!\n";
             return false;
           }
           //read apparatus script ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of APPA!\n";
             return false;
           }
           ScriptName = std::string(Buffer);
           if (ScriptName.empty())
           {
             std::cout << "Error: subrecord SCRI of APPA is empty!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(SubRecName)
                     << "\" found, but only MODL, FNAM, AADT or ITEX are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (Model.empty() or ItemName.empty() or !hasReadAADT or InventoryIcon.empty())
  {
    std::cout << "Error: At least one required subrecord of APPA is missing!\n";
    return false;
  }

  return in_File.good();
}

} //namespace
