/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2022, 2023  Dirk Stolle

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

#include <cstdint>
#include <optional>
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

  std::optional<RefDoorData> DoorData;
  std::optional<uint32_t> LockLevel;
  std::string KeyID;
  std::string TrapID;

  std::string OwnerID;
  std::string OwnerFactionID;
  int32_t FactionRank;
  std::string GlobalVarID;
  std::string SoulCreatureID;
  std::optional<float> EnchantCharge;
  std::optional<uint32_t> NumberOfUses;
  std::optional<uint32_t> UnknownNAM9;
  std::optional<uint8_t> ReferenceBlockedByte;
  bool isDeleted;
  uint32_t DeletionLong;

  ReferencedObject();

  bool operator==(const ReferencedObject& other) const;

  /** \brief Loads the object reference from the given input stream.
   *
   * \param input    the input stream
   * \param BytesRead  reference to the variable that counts the bytes read
   * \param Buffer     pointer to a char array that can hold at least 256 bytes
   * \return Returns true on success (reference was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& in_File, uint32_t& BytesRead, char* Buffer);

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the object reference to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (data was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;

  /** \brief Gets the size in bytes that the reference data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the reference would need.
   */
  uint32_t getWrittenSize() const;
  #endif
}; // struct

} // namespace

#endif // MW_REFERENCEDOBJECT_HPP
