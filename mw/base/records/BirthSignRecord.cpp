/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#include "BirthSignRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

BirthSignRecord::BirthSignRecord()
: BasicRecord(),
  recordID(""),
  Name(""),
  Texture(""),
  Description(""),
  SignSpells(std::vector<std::string>())
{}

BirthSignRecord::BirthSignRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  Name(""),
  Texture(""),
  Description(""),
  SignSpells(std::vector<std::string>())
{}

bool BirthSignRecord::equals(const BirthSignRecord& other) const
{
  return ((recordID==other.recordID) and (Name==other.Name)
      and (Texture==other.Texture) and (Description==other.Description)
      and (SignSpells==other.SignSpells));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool BirthSignRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cBSGN, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* TNAM */ +4 /* 4 bytes for length */
        +Texture.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DESC */ +4 /* 4 bytes for length */
        +Description.length()+1 /* length of name +1 byte for NUL termination */
        +SignSpells.size()*(4 /* NPCS */ +4 /* 4 bytes for length */ +32 /* fixed length of 32 bytes - rest is filled with NUL */);
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Birth Sign
	NAME = Sign ID string
	FNAM = Sign name string
	TNAM = Texture filename
	DESC = Description string
	NPCS = Spell/ability (32 bytes), multiple
  */

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = Name.length()+1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write sign's name
  output.write(Name.c_str(), SubLength);

  //write TNAM
  output.write((const char*) &cTNAM, 4);
  SubLength = Texture.length()+1;
  //write TNAM's length
  output.write((const char*) &SubLength, 4);
  //write texture name
  output.write(Texture.c_str(), SubLength);

  //write DESC
  output.write((const char*) &cDESC, 4);
  SubLength = Description.length()+1;
  //write DESC's length
  output.write((const char*) &SubLength, 4);
  //write description text
  output.write(Description.c_str(), SubLength);

  //write spells/abilities
  unsigned int i;
  for (i=0; i<SignSpells.size(); ++i)
  {
    //write NPCS
    output.write((const char*) &cNPCS, 4);
    SubLength = 32; //length is fixed at 32 bytes, rest is NULs
    //write NPCS's length
    output.write((const char*) &SubLength, 4);
    //write spell ID
    const unsigned int len = SignSpells[i].length()+1;
    output.write(SignSpells[i].c_str(), len);
    if (len<32)
    {
      //fill rest up to 32 bytes with null bytes
      output.write(NULof32, 32-len);
    }
  }//for

  return output.good();
}
#endif

bool BirthSignRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Birth Sign
    NAME = Sign ID string
    FNAM = Sign name string
    TNAM = Texture filename
    DESC = Description string
    NPCS = Spell/ability (32 bytes), multiple
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
    std::cout << "Error: Subrecord NAME of BSGN is longer than 255 characters.\n";
    return false;
  }
  //read birth sign ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of BSGN!\n";
    return false;
  }
  recordID = std::string(Buffer);

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
    std::cout << "Error: Subrecord FNAM of BSGN is longer than 255 characters.\n";
    return false;
  }
  //read birth sign name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of BSGN!\n";
    return false;
  }
  Name = std::string(Buffer);

  //read TNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cTNAM)
  {
    UnexpectedRecord(cTNAM, SubRecName);
    return false;
  }
  //TNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord TNAM of BSGN is longer than 255 characters.\n";
    return false;
  }
  //read texture path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord TNAM of BSGN!\n";
    return false;
  }
  Texture = std::string(Buffer);

  //read DESC
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord DESC of BSGN is longer than 255 characters.\n";
    return false;
  }
  //read description
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DESC of BSGN!\n";
    return false;
  }
  Description = std::string(Buffer);

  //read spells
  SignSpells.clear();

  while (BytesRead<Size)
  {
    //read NPCS
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cNPCS)
    {
      UnexpectedRecord(cNPCS, SubRecName);
      return false;
    }
    //NPCS's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: Subrecord NPCS of BSGN is longer than 255 characters.\n";
      return false;
    }
    //read description
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord NPCS of BSGN!\n";
      return false;
    }
    SignSpells.push_back(std::string(Buffer));
  }//while

  return in_File.good();
}

bool operator<(const BirthSignRecord& left, const BirthSignRecord& right)
{
  return (left.recordID.compare(right.recordID)<0);
}

} //namespace
