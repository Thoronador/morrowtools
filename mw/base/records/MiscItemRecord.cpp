/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2013  Thoronador

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

#include "MiscItemRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

MiscItemRecord::MiscItemRecord()
{
  recordID = ModelPath = MiscItemName = "";
  //miscellaneous item data
  Weight = 0.0f;
  Value = -1;
  OtherStuff = 0;
  //end of miscellaneous item data
  InventoryIcon = ScriptID = "";
}

MiscItemRecord::~MiscItemRecord()
{
  //empty
}

bool MiscItemRecord::equals(const MiscItemRecord& other) const
{
  return ((recordID==other.recordID) and (ModelPath==other.ModelPath)
      and (MiscItemName==other.MiscItemName) and (Weight==other.Weight)
      and (Value==other.Value) and (OtherStuff==other.OtherStuff)
      and (InventoryIcon==other.InventoryIcon) and (ScriptID==other.ScriptID));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool MiscItemRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMISC, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* MCDT */ +4 /* 4 bytes for length */ + 12 /* size of misc. data */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of icon path +1 byte for NUL termination */;
  if (!MiscItemName.empty())
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
          +MiscItemName.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!ScriptID.empty())
  {
    Size = Size+ 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */ ;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Misc. Items:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name (not present in one case)
    MCDT = Misc Data, 12 bytes, required
        float Weight
        long  Value
        long  Unknown
    ITEX = Inventory icon filename
    SCRI = script ID string (optional)
  */

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  SubLength = ModelPath.length()+1;
  //write MODL's length
  output.write((const char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  if (!MiscItemName.empty())
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    SubLength = MiscItemName.length()+1;
    //write FNAM's length
    output.write((const char*) &SubLength, 4);
    //write misc. items's name
    output.write(MiscItemName.c_str(), SubLength);
  }

  //write MCDT
  output.write((const char*) &cMCDT, 4);
  SubLength = 12; /* fixed length of 12 bytes */
  //write MCDT's length
  output.write((const char*) &SubLength, 4);
  //write misc. data
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &OtherStuff, 4);

  //write ITEX
  output.write((const char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((const char*) &SubLength, 4);
  //write inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((const char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool MiscItemRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Misc. Items:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name (not present in one case (Tribunal))
    MCDT = Misc Data, 12 bytes, required
        float Weight
        long  Value
        long  Unknown
    ITEX = Inventory icon filename
    SCRI = script ID string (optional)
  */

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead = 8;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of MISC is longer than 255 characters.\n";
    return false;
  }
  //read Misc Item ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of MISC!\n";
    return false;
  }
  recordID = std::string(Buffer);

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
    std::cout << "Error: Subrecord MODL of MISC is longer than 255 characters.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of MISC!\n";
    return false;
  }
  ModelPath = std::string(Buffer);

  //read FNAM (may not be present sometimes)
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cFNAM)
  {
    //FNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: Subrecord FNAM of MISC is longer than 255 characters.\n";
      return false;
    }
    //read item's name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FNAM of MISC!\n";
      return false;
    }
    MiscItemName = std::string(Buffer);

    //read MCDT
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    MiscItemName.clear();
  }

  //read MCDT
  //sub record name was already read above
  if (SubRecName!=cMCDT)
  {
    UnexpectedRecord(cMCDT, SubRecName);
    return false;
  }
  //MCDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout << "Error: Subrecord MODL of MISC has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read misc. data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &OtherStuff, 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MCDT of MISC!\n";
    return false;
  }

  InventoryIcon.clear();
  ScriptID.clear();
  bool hasITEX = false;
  bool hasSCRI = false;
  while (BytesRead<Size)
  {
    //read next subrecord header
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           if (hasITEX)
           {
             std::cout << "Error: record MISC seems to have two ITEX subrecords.\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord ITEX of MISC is longer than 255 characters.\n";
             return false;
           }
           //read inventory icon
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of MISC!\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           hasITEX = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cout << "Error: record MISC seems to have two SCRI subrecords.\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord SCRI of MISC is longer than 255 characters.\n";
             return false;
           }
           //read script ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of MISC!\n";
             return false;
           }
           ScriptID = std::string(Buffer);
           hasSCRI = true;
           break;
      default:
           std::cout << "Error while reading MISC: expected record name ITEX "
                     << "or SCRI was not found. Instead, \""
                     << IntTo4Char(SubRecName)<<"\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
