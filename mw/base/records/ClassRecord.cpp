/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012  Thoronador

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

#include "ClassRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

ClassRecord::ClassRecord()
: BasicRecord(),
  recordID (""),
  Name(""),
  AttrID1(0),
  AttrID2(0),
  Specialization(0),
  MinorID1(0),
  MajorID1(0),
  MinorID2(0),
  MajorID2(0),
  MinorID3(0),
  MajorID3(0),
  MinorID4(0),
  MajorID4(0),
  MinorID5(0),
  MajorID5(0),
  ClassFlags(0),
  AutoCalcFlags(0),
  Description("")
{}

bool ClassRecord::equals(const ClassRecord& other) const
{
  return ((recordID==other.recordID) and (Name==other.Name)
      and (AttrID1==other.AttrID1) and (AttrID2==other.AttrID2)
      and (Specialization==other.Specialization) and (MinorID1==other.MinorID1)
      and (MajorID1==other.MajorID1) and (MinorID2==other.MinorID2)
      and (MajorID2==other.MajorID2) and (MinorID3==other.MinorID3)
      and (MajorID3==other.MajorID3) and (MinorID4==other.MinorID4)
      and (MajorID4==other.MajorID4) and (MinorID5==other.MinorID5)
      and (MajorID5==other.MajorID5) and (ClassFlags==other.ClassFlags)
      and (AutoCalcFlags==other.AutoCalcFlags) and (Description==other.Description));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ClassRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCLAS, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CLDT */ +4 /* 4 bytes for length */ +60 /* length of CLDT */;
  if (!Description.empty())
  {
    Size = Size +4 /* DESC */ +4 /* 4 bytes for length */
        +Description.length() /* length of description (no NUL termination here) */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Class Definition
	NAME = Class ID string
	FNAM = Class name string
	CLDT = Class Data (60 bytes)
		long AttributeID1
		long AttributeID2
		long Specialization?
			0 = Combat
			1 = Magic
			2 = Stealth
		long MinorID1
		long MajorID1
		long MinorID2
		long MajorID2
		long MinorID3
		long MajorID3
		long MinorID4
		long MajorID4
		long MinorID5
		long MajorID5
		long Flags
			0x0001 = Playable
		long AutoCalcFlags
			0x00001 = Weapon
			0x00002 = Armor
			0x00004 = Clothing
			0x00008 = Books
			0x00010 = Ingrediant
			0x00020 = Picks
			0x00040 = Probes
			0x00080 = Lights
			0x00100 = Apparatus
			0x00200 = Repair
			0x00400 = Misc
			0x00800 = Spells
			0x01000 = Magic Items
			0x02000 = Potions
			0x04000 = Training
			0x08000 = Spellmaking
			0x10000 = Enchanting
			0x20000 = Repair Item
	DESC = Description string (optional for non-playable classes)
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
  //write class name
  output.write(Name.c_str(), SubLength);

  //write CLDT
  output.write((const char*) &cCLDT, 4);
  SubLength = 60; /* CLDT has fixed length of 60 bytes */
  //write CLDT's length
  output.write((const char*) &SubLength, 4);
  //write class data
  output.write((const char*) &AttrID1, 4);
  output.write((const char*) &AttrID2, 4);
  output.write((const char*) &Specialization, 4);
  output.write((const char*) &MinorID1, 4);
  output.write((const char*) &MajorID1, 4);
  output.write((const char*) &MinorID2, 4);
  output.write((const char*) &MajorID2, 4);
  output.write((const char*) &MinorID3, 4);
  output.write((const char*) &MajorID3, 4);
  output.write((const char*) &MinorID4, 4);
  output.write((const char*) &MajorID4, 4);
  output.write((const char*) &MinorID5, 4);
  output.write((const char*) &MajorID5, 4);
  output.write((const char*) &ClassFlags, 4);
  output.write((const char*) &AutoCalcFlags, 4);

  if (!Description.empty())
  {
    //write DESC
    output.write((const char*) &cDESC, 4);
    SubLength = Description.length();
    //write DESC's length
    output.write((const char*) &SubLength, 4);
    //write class description
    output.write(Description.c_str(), SubLength);
  }
  return output.good();
}
#endif

bool ClassRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Class Definition
	NAME = Class ID string
	FNAM = Class name string
	CLDT = Class Data (60 bytes)
		long AttributeID1
		long AttributeID2
		long Specialization?
			0 = Combat
			1 = Magic
			2 = Stealth
		long MinorID1
		long MajorID1
		long MinorID2
		long MajorID2
		long MinorID3
		long MajorID3
		long MinorID4
		long MajorID4
		long MinorID5
		long MajorID5
		long Flags
			0x0001 = Playable
		long AutoCalcFlags
			0x00001 = Weapon
			0x00002 = Armor
			0x00004 = Clothing
			0x00008 = Books
			0x00010 = Ingrediant
			0x00020 = Picks
			0x00040 = Probes
			0x00080 = Lights
			0x00100 = Apparatus
			0x00200 = Repair
			0x00400 = Misc
			0x00800 = Spells
			0x01000 = Magic Items
			0x02000 = Potions
			0x04000 = Training
			0x08000 = Spellmaking
			0x10000 = Enchanting
			0x20000 = Repair Item
	DESC = Description string (optional for non-playable classes) */
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
    std::cout << "ClassRecord::loadFromStream: Error: ID is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read class ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "ClassRecord::loadFromStream: Error while reading ID from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
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
    std::cout << "ClassRecord::loadFromStream: Error: name is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "ClassRecord::loadFromStream: Error while reading name from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return false;
  }
  Name = std::string(Buffer);

  //read CLDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cCLDT)
  {
    UnexpectedRecord(cCLDT, SubRecName);
    return false;
  }
  //CLDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=60)
  {
    std::cout << "Error: Subrecord CLDT of CLAS has invalid length ("<<SubLength
              << "bytes), should be 60 bytes.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read CLDT
  in_File.read((char*) &AttrID1, 4);
  in_File.read((char*) &AttrID2, 4);
  in_File.read((char*) &Specialization, 4);
  in_File.read((char*) &MinorID1, 4);
  in_File.read((char*) &MajorID1, 4);
  in_File.read((char*) &MinorID2, 4);
  in_File.read((char*) &MajorID2, 4);
  in_File.read((char*) &MinorID3, 4);
  in_File.read((char*) &MajorID3, 4);
  in_File.read((char*) &MinorID4, 4);
  in_File.read((char*) &MajorID4, 4);
  in_File.read((char*) &MinorID5, 4);
  in_File.read((char*) &MajorID5, 4);
  in_File.read((char*) &ClassFlags, 4);
  in_File.read((char*) &AutoCalcFlags, 4);
  BytesRead += 60;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CLDT of CLAS.\nFile position: "
              << in_File.tellg()<<" bytes\n";
    return false;
  }

  if (BytesRead<Size)
  {
    //read DESC
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName!=cDESC)
    {
      UnexpectedRecord(cDESC, SubRecName);
      return false;
    }
    //DESC's length
    in_File.read((char*) &SubLength, 4);
    //length check to avoid allocation of large memory block
    if (SubLength>65535)
    {
      std::cout << "ClassRecord::loadFromStream: Error: Description is longer than 64K characters.\n";
      std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
      return false;
    }
    //allocate larger chunk
    char* DescBuffer = new char[SubLength+1];
    //read DESC
    memset(DescBuffer, '\0', SubLength+1);
    in_File.read(DescBuffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "ClassRecord::loadFromStream: Error while reading description from stream.\n";
      std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
      delete[] DescBuffer;
      return false;
    }
    Description = std::string(DescBuffer);
    delete[] DescBuffer;
    DescBuffer = NULL;
    if (Description.empty())
    {
      std::cout << "Error: subrecord DESC of CLAS is empty!\n";
      return false;
    }
  }//if not enough read yet
  else
  {
    Description.clear();
  }

  return in_File.good();
}

} //namespace
