/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "ContainerRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

const uint32_t ContainerFlag_Organic = 1;
const uint32_t ContainerFlag_Respawns = 2;

ContainerRecord::ContainerRecord()
: BasicRecord(),
  recordID(""),
  ModelPath(""),
  Name(""),
  Weight(0.0f),
  Flags(0x00000008),
  ScriptID(""),
  Items(std::vector<ItemRecord>())
{ }

bool ContainerRecord::equals(const ContainerRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight)
      && (Flags == other.Flags) && (ScriptID == other.ScriptID)
      && (Items == other.Items);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ContainerRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCONT), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* CNDT */ + 4 /* 4 bytes for length */ + 4 /* size of data */
        + 4 /* FLAG */ + 4 /* 4 bytes for length */ + 4 /* size of flags */
        + Items.size() * (4 /* NPCO */ +4 /* 4 bytes for length */ + 36 /* fixed length of 36 bytes */);
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of script ID +1 byte for NUL termination */ ;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  // write name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write container data (CNDT)
  output.write(reinterpret_cast<const char*>(&cCNDT), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);

  // write container flags (FLAG)
  output.write(reinterpret_cast<const char*>(&cFLAG), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Flags), 4);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI) - optional, not always present
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write items
  for (const auto& item: Items)
  {
    if (!item.saveToStream(output))
      return false;
  }
  return output.good();
}
#endif

bool ContainerRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t BytesRead = 0;

  // read NAME
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of CONT!\n";
    return false;
  }

  // read MODL
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of CONT!\n";
    return false;
  }

  // read FNAM
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of CONT!\n";
    return false;
  }

  // read CNDT
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cCNDT)
  {
    UnexpectedRecord(cCNDT, SubRecName);
    return false;
  }
  // CNDT's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record CNDT of CONT has invalid length ("
              << SubLength << " bytes). Should be 4 bytes.\n";
    return false;
  }
  // read weight
  input.read(reinterpret_cast<char*>(&Weight), 4);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord CNDT of CONT!\n";
    return false;
  }

  // read FLAG
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  // FLAG's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record FLAG of CONT has invalid length ("
              << SubLength << " bytes). Should be 4 bytes.\n";
    return false;
  }
  // read flags
  input.read(reinterpret_cast<char*>(&Flags), 4);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord FLAG of CONT!\n";
    return false;
  }

  // read optional stuff
  ScriptID.clear();
  Items.clear();
  ItemRecord temp;
  while (BytesRead < Size)
  {
    // read next header
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cNPCO:
           if (!temp.loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading subrecord NPCO of CONT!\n";
             return false;
           }
           Items.emplace_back(temp);
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: CONT seems to have more than one SCRI subrecord.\n";
             return false;
           }
           // read script ID
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of CONT!\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: SCRI of CONT is empty!\n";
             return false;
           }
           break;
      default:
           // other subrecord means error, so quit here
           std::cerr << "Error: Expected record name NPC0 or SCRI was not "
                     << "found. Instead, \"" << IntTo4Char(SubRecName)
                     << "\" was found.\n";
           return false;
    }
  }

  return true;
}

bool ContainerRecord::isOrganic() const
{
  return (Flags & ContainerFlag_Organic) != 0;
}

bool ContainerRecord::doesRespawn() const
{
  return (Flags & ContainerFlag_Respawns) != 0;
}

} // namespace
