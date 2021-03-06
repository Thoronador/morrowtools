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

#include "ContainerRecord.h"
#include <iostream>
#include <cstring>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

const int32_t ContainerFlag_Organic = 1;
const int32_t ContainerFlag_Respawns = 2;

ContainerRecord::ContainerRecord()
: BasicRecord(),
  recordID(""),
  ModelPath(""),
  ContainerName(""),
  Weight(0.0f),
  ContainerFlags(8),
  ScriptName(""),
  Items(std::vector<ItemRecord>())
{ }

bool ContainerRecord::equals(const ContainerRecord& other) const
{
  return ((recordID==other.recordID) and (ModelPath==other.ModelPath)
      and (ContainerName==other.ContainerName) and (Weight==other.Weight)
      and (ContainerFlags==other.ContainerFlags) and (ScriptName==other.ScriptName)
      and (Items==other.Items));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ContainerRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCONT, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +ContainerName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CNDT */ +4 /* 4 bytes for length */ + 4 /* size of container data */
        +4 /* FLAG */ +4 /* 4 bytes for length */ + 4 /* size of flags */
        +Items.size()*(4 /* NPCO */ +4 /* 4 bytes for length */ +36 /* fixed length of 36 bytes */);
  if (!ScriptName.empty())
  {
    Size = Size+ 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptName.length()+1 /* length of script ID +1 byte for NUL termination */ ;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Container:
    NAME = ID
    MODL = NIF Model
    FNAM = Container name
    CNDT = Container data (4 bytes)
        float Weight
    FLAG = Container flags (4 bytes, bit-field)
        (0x0001=Organic;0x0002=Respawns, organic only;0x0008=Default, unknown)
    SCRI = script name (optional)
    NPCO = An item record (36 bytes, 0+ times)
        long	Count	  Number of the item
        char	Name[32]  The ID of the item */

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

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = ContainerName.length()+1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write container's name
  output.write(ContainerName.c_str(), SubLength);

  //write CNDT
  output.write((const char*) &cCNDT, 4);
  SubLength = 4; //fixed at four bytes
  //write CNDT's length
  output.write((const char*) &SubLength, 4);
  //write container data
  output.write((const char*) &Weight, 4);

  //write FLAG
  output.write((const char*) &cFLAG, 4);
  SubLength = 4; //fixed at four bytes
  //write FLAG's length
  output.write((const char*) &SubLength, 4);
  //write container flags
  output.write((const char*) &ContainerFlags, 4);

  //script name - optional, not always present
  if (!ScriptName.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    SubLength = ScriptName.length()+1; /* length of script ID +1 byte for NUL termination */
    //write SCRI's length
    output.write((const char*) &SubLength, 4);
    //write container's script ID
    output.write(ScriptName.c_str(), SubLength);
  }

  //write items
  unsigned int i, len;
  for (i=0; i<Items.size(); ++i)
  {
    //write NPCO
    output.write((const char*) &cNPCO, 4);
    SubLength = 36; //fixed size: 36 bytes
    //write NPCO's length
    output.write((const char*) &SubLength, 4);
    //write item count
    output.write((const char*) &(Items[i].Count), 4);
    //write item ID
    /* The item ID gets truncated here, if it's longer than 31 characters. */
    len = Items.at(i).Item.length()+1;
    if (len>32)
    {
      len=32;
      std::cout << "ContainerRecord::saveToStream: Warning: item name of container \""
                << recordID << "\" got truncated for index "<<i<<".\n";
    }
    output.write(Items.at(i).Item.c_str(), len);
    if (len<32)
    {
      //fill rest with zeroes
      output.write(NULof32, 32-len);
    }
  }//for
  return output.good();
}
#endif

bool ContainerRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Container:
    NAME = ID
    MODL = NIF Model
    FNAM = Container name
    CNDT = Container data (4 bytes)
        float Weight
    FLAG = Container flags (4 bytes, bit-field)
        (0x0001=Organic;0x0002=Respawns, organic only;0x0008=Default, unknown)
    SCRI = script name (optional)
    NPCO = An item record (36 bytes, 0+ times)
        long	Count	  Number of the item
        char	Name[32]  The ID of the item */

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
    std::cout << "Error: Subrecord NAME of CONT is longer than 255 characters.\n";
    return false;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of CONT!\n";
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
    std::cout << "Error: Subrecord MODL of CONT is longer than 255 characters.\n";
    return false;
  }
  //read model
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of CONT!\n";
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
    std::cout << "Error: Subrecord FNAM of CONT is longer than 255 characters.\n";
    return false;
  }
  //read container name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of CONT!\n";
    return false;
  }
  ContainerName = std::string(Buffer);

  //read CNDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cCNDT)
  {
    UnexpectedRecord(cCNDT, SubRecName);
    return false;
  }
  //CNDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record CNDT of CONT has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  //read weight
  in_File.read((char*) &Weight, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CNDT of CONT!\n";
    return false;
  }

  //read FLAG
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  //FLAG's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLAG of CONT has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  //read flags
  in_File.read((char*) &ContainerFlags, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FLAG of CONT!\n";
    return false;
  }

  //read script name (optional)
  ScriptName.clear();
  if (BytesRead<Size)
  {
    //read SCRI
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    //is it a script record or already the items
    if (SubRecName==cSCRI)
    {
      //script found, read it

      //SCRI's length
      in_File.read((char*) &SubLength, 4);
      BytesRead += 4;
      if (SubLength>255)
      {
        std::cout << "Error: Subrecord SCRI of CONT is longer than 255 characters.\n";
        return false;
      }
      //read container's script ID
      memset(Buffer, '\0', 256);
      in_File.read(Buffer, SubLength);
      BytesRead += SubLength;
      if (!in_File.good())
      {
        std::cout << "Error while reading subrecord SCRI of CONT!\n";
        return false;
      }
      ScriptName = std::string(Buffer);
    }//if
    else
    {
      //seek 4 bytes towards beginning to land before next record name
      in_File.seekg(-4, std::ios::cur);
    }//else
  }//if

  //read multiple NPCO sub-records
  Items.clear();
  ItemRecord temp;
  while (BytesRead<Size)
  {
    //read NPCO
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cNPCO)
    {
      UnexpectedRecord(cNPCO, SubRecName);
      return false;
    }
    //NPCO's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=36)
    {
      std::cout <<"Error: sub record NPCO of CONT has invalid length ("
                <<SubLength<<" bytes). Should be 36 bytes.\n";
      return false;
    }
    //read count and name
    // ---- count
    in_File.read((char*) &(temp.Count), 4);
    // ---- item name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, 32);
    BytesRead += 36;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord NPCO of CONT!\n";
      return false;
    }
    temp.Item = std::string(Buffer);
    Items.push_back(temp);
  } //while
  return in_File.good();
}

bool ContainerRecord::isOrganic() const
{
  return ((ContainerFlags & ContainerFlag_Organic)!=0);
}

bool ContainerRecord::doesRespawn() const
{
  return ((ContainerFlags & ContainerFlag_Respawns)!=0);
}

} //namespace
