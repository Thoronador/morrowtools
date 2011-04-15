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

#include "BodyPartRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

BodyPartRecord::BodyPartRecord()
{
  BodyPartID = "";
  MeshPath = "";
  RaceID = "";
  //body data
  Part = 0;
  Vampire = 0;
  Flags = 0;
  PartType = 0;
}

BodyPartRecord::BodyPartRecord(const std::string& ID)
{
  BodyPartID = ID;
  MeshPath = "";
  RaceID = "";
  //body data
  Part = 0;
  Vampire = 0;
  Flags = 0;
  PartType = 0;
}

bool BodyPartRecord::equals(const BodyPartRecord& other) const
{
  return ((BodyPartID==other.BodyPartID) and (MeshPath==other.MeshPath)
      and (RaceID==other.RaceID) and (Part==other.Part)
      and (Vampire==other.Vampire) and (Flags==other.Flags)
      and (PartType==other.PartType));
}

bool BodyPartRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cBODY, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +BodyPartID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +MeshPath.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +RaceID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* BYDT */ +4 /* 4 bytes for BYDT's length */ +4 /* size of BYDT */;
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*BodyParts:
    NAME = Body Part ID
    MODL = Path to Mesh
    FNAM = Race ID
    BYDT = Body part data (4 bytes)
    byte Part
			0 = Head
			1 = Hair
			2 = Neck
			3 = Chest
			4 = Groin
			5 = Hand
			6 = Wrist
			7 = Forearm
			8 = Upperarm
			9 = Foot
			10 = Ankle
			11 = Knee
			12 = Upperleg
			13 = Clavicle
			14 = Tail
		byte Vampire
		byte Flags
			1 = Female
			2 = Playable
		byte PartType
			0 = Skin
			1 = Clothing
			2 = Armor
  */

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = BodyPartID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write NAME/ID
  output.write(BodyPartID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = MeshPath.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write mesh path
  output.write(MeshPath.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = RaceID.length()+1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write race ID
  output.write(RaceID.c_str(), SubLength);

  //write BYDT
  output.write((char*) &cBYDT, 4);
  SubLength = 4; /* fixed length is four bytes */
  //write BYDT's length
  output.write((char*) &SubLength, 4);
  //write BYDT
  output.write((char*) &Part, 1);
  output.write((char*) &Vampire, 1);
  output.write((char*) &Flags, 1);
  output.write((char*) &PartType, 1);

  return output.good();
}

bool BodyPartRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*BodyParts:
    NAME = Body Part ID
    MODL = Path to Mesh
    FNAM = Race ID
    BYDT = Body part data (4 bytes)
    byte Part
			0 = Head
			1 = Hair
			2 = Neck
			3 = Chest
			4 = Groin
			5 = Hand
			6 = Wrist
			7 = Forearm
			8 = Upperarm
			9 = Foot
			10 = Ankle
			11 = Knee
			12 = Upperleg
			13 = Clavicle
			14 = Tail
		byte Vampire
		byte Flags
			1 = Female
			2 = Playable
		byte PartType
			0 = Skin
			1 = Clothing
			2 = Armor
  */
  int32_t SubRecName;
  uint32_t SubLength;
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
  if (SubLength>255)
  {
    std::cout << "Subrecord NAME of BODY is longer than 255 characters!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read name
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of BODY!\n";
    return false;
  }
  BodyPartID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Subrecord MODL of BODY is longer than 255 characters!\n";
    return false;
  }
  //read MODL
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of BODY!\n";
    return false;
  }
  MeshPath = std::string(Buffer);

  //read Race Name FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Subrecord FNAM of BODY is longer than 255 characters!\n";
    return false;
  }
  //read FNAM
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of BODY!\n";
    return false;
  }
  RaceID = std::string(Buffer);

  //read BYDT (Body part data)
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cBYDT)
  {
    UnexpectedRecord(cBYDT, SubRecName);
    return false;
  }
  //BYDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength != 4)
  {
    std::cout << "Error: Sub record BYDT has invalid length ("<<SubLength
              << " bytes), should be 4 bytes.\n";
    return false;
  }
  //read BYDT
  in_File.read((char*) &Part, 1);
  in_File.read((char*) &Vampire, 1);
  in_File.read((char*) &Flags, 1);
  in_File.read((char*) &PartType, 1);
  return in_File.good();
}

bool operator<(const BodyPartRecord& left, const BodyPartRecord& right)
{
  return (left.BodyPartID.compare(right.BodyPartID)<0);
}
