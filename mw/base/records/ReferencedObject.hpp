/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Dirk Stolle

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

#ifndef MW_REFERENCEDOBJECT_HPP
#define MW_REFERENCEDOBJECT_HPP

#include <string>

namespace MWTP
{

struct RefDoorData
{
  float PosX, PosY, PosZ;
  float RotX, RotY, RotZ;
  std::string ExitName;

  RefDoorData();

  bool operator==(const RefDoorData& other) const;
}; // struct

struct ReferencedObject
{
  uint32_t ObjectIndex;
  std::string ObjectID;
  float Scale;
  // data
  float PosX, PosY, PosZ;
  float RotX, RotY, RotZ;
  // end of data

  bool hasDoorData;
  RefDoorData DoorData;
  bool hasFLTV;
  int32_t LockLevel;
  std::string KeyID;
  std::string TrapID;

  std::string OwnerID;
  std::string OwnerFactionID;
  int32_t FactionRank;
  std::string GlobalVarID;
  std::string SoulCreatureID;
  bool hasXCHG;
  float EnchantCharge;
  uint32_t NumberOfUses;
  bool hasNAM9;
  uint32_t UnknownNAM9;
  bool hasUNAM;
  uint8_t ReferenceBlockedByte;
  bool isDeleted;
  uint32_t DeletionLong;

  ReferencedObject();

  /* comparison operator */
  bool operator==(const ReferencedObject& other) const;

  /* loads the object reference from the given input stream and returns true on success

    parameters:
        in_File   - the input stream
        BytesRead - reference to the 32 bit unsigned int that counts the bytes read
        Buffer    - pointer to a char array that can hold at least 256 bytes
  */
  bool loadFromStream(std::istream& in_File, uint32_t& BytesRead, char* Buffer);

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the object reference to the given output stream and returns true on success

    parameters:
        output - the output stream
  */
  bool saveToStream(std::ostream& output) const;
  #endif

  /* returns the size in bytes of this reference's data written to a stream */
  uint32_t getWrittenSize() const;
}; // struct

} // namespace

#endif // MW_REFERENCEDOBJECT_HPP
