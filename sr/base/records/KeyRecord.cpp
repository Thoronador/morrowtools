/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#include "KeyRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

KeyRecord::KeyRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  keywords(std::vector<uint32_t>()),
  value(0),
  weight(0.0f)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool KeyRecord::equals(const KeyRecord& other) const
{
  return (equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (modelPath == other.modelPath)
      && (unknownMODT == other.unknownMODT) && (pickupSoundFormID == other.pickupSoundFormID)
      && (putdownSoundFormID == other.putdownSoundFormID) && (keywords == other.keywords)
      && (value == other.value) && (weight == other.weight));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeyRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
      + name.getWriteSize() /* FULL */
      + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* length */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* length */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
                          + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size() /*data length*/;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  return writeSize;
}

bool KeyRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cKEYM), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    // write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing VMAD of KEYM!";
      return false;
    }
  }

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write FULL
  if (!name.saveToStream(output, cFULL))
    return false;

  if (!modelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    // write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing MODT of KEYM!";
      return false;
    }
  }

  if (pickupSoundFormID != 0)
  {
    // write pickup sound form ID (YNAM)
    output.write(reinterpret_cast<const char*>(&cYNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);
  }

  if (putdownSoundFormID != 0)
  {
    // write putdown sound form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  if (!keywords.empty())
  {
    // write size of array (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; //fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write KSIZ
    for (const auto keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  } // if keywords present

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's content
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  return output.good();
}
#endif

bool KeyRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // read OBND (or VMAD)
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName == cVMAD)
  {
    // read VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cerr << "Error while reading subrecord VMAD of KEYM!\n";
      return false;
    }
    bytesRead = bytesRead + 2 + unknownVMAD.size();

    // read OBND
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
  }
  else
  {
    // no VMAD
    unknownVMAD.setPresence(false);
  }

  // record header was already read before...
  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  // OBND's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of KEYM has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of KEYM!\n";
    return false;
  }

  // read FULL
  if (!name.loadFromStream(in_File, cFULL, true, bytesRead, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord FULL of KEYM!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  keywords.clear();
  bool hasReadDATA = false;
  while (bytesRead < readSize)
  {
    // read next subrecord
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: KEYM seems to have more than one MODL subrecord!\n";
             return false;
           }
           // read model path
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           // check content
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of KEYM is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: KEYM seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of KEYM!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: KEYM seems to have more than one YNAM subrecord!\n";
             return false;
           }
           // read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord YNAM of KEYM has value zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: KEYM seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           // read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of KEYM has value zero!\n";
             return false;
           }
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: KEYM seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Subrecord DATA of KEYM has invalid length ("
                       << subLength << " bytes). Should be 8 bytes!\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of KEYM!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, YNAM, ZNAM, KSIZ or DATA "
                     << "are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!hasReadDATA)
  {
    std::cerr << "Error: At least one subrecord of KEYM is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t KeyRecord::getRecordType() const
{
  return cKEYM;
}

} // namespace
