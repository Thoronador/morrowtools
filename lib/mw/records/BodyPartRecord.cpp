/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "BodyPartRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

BodyPartRecord::BodyPartRecord(const std::string& id)
: BasicRecord(),
  recordID(id),
  ModelPath(std::string()),
  RaceID(std::string()),
  Part(0),
  Vampire(0),
  Flags(0),
  PartType(0)
{}

bool BodyPartRecord::equals(const BodyPartRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (RaceID == other.RaceID) && (Part == other.Part)
      && (Vampire == other.Vampire) && (Flags == other.Flags)
      && (PartType == other.PartType);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool BodyPartRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cBODY), 4);
  const uint32_t Size = 4 /* NAME */ +4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + RaceID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* BYDT */ + 4 /* 4 bytes for BYDT's length */ + 4 /* size of BYDT */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write mesh path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  // write race ID (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = RaceID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(RaceID.c_str(), SubLength);

  // write BYDT
  output.write(reinterpret_cast<const char*>(&cBYDT), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write actual data
  output.write(reinterpret_cast<const char*>(&Part), 1);
  output.write(reinterpret_cast<const char*>(&Vampire), 1);
  output.write(reinterpret_cast<const char*>(&Flags), 1);
  output.write(reinterpret_cast<const char*>(&PartType), 1);

  return output.good();
}
#endif

bool BodyPartRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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
  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t bytesRead = 0;

  // read ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, bytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of BODY!\n";
    return false;
  }

  // read MODL
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, bytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of BODY!\n";
    return false;
  }

  // read Race Name FNAM
  if (!loadString256WithHeader(input, RaceID, Buffer, cFNAM, bytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of BODY!\n";
    return false;
  }

  // read BYDT (Body part data)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cBYDT)
  {
    UnexpectedRecord(cBYDT, SubRecName);
    return false;
  }
  // BYDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record BYDT has invalid length (" << SubLength
              << " bytes), should be 4 bytes.\n";
    return false;
  }
  // read BYDT
  input.read(reinterpret_cast<char*>(&Part), 1);
  input.read(reinterpret_cast<char*>(&Vampire), 1);
  input.read(reinterpret_cast<char*>(&Flags), 1);
  input.read(reinterpret_cast<char*>(&PartType), 1);
  return input.good();
}

bool operator<(const BodyPartRecord& left, const BodyPartRecord& right)
{
  return left.recordID.compare(right.recordID) < 0;
}

} // namespace
