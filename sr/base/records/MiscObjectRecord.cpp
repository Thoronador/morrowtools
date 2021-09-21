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

#include "MiscObjectRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

MiscObjectRecord::MiscObjectRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  iconPath(""),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  keywords(std::vector<uint32_t>()),
  value(0),
  weight(0.0f)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool MiscObjectRecord::equals(const MiscObjectRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
    && (unknownVMAD == other.unknownVMAD) && (unknownOBND == other.unknownOBND)
    && (name == other.name)
    && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
    && (unknownMODS == other.unknownMODS) && (iconPath == other.iconPath)
    && (pickupSoundFormID == other.pickupSoundFormID)
    && (putdownSoundFormID == other.putdownSoundFormID)
    && (keywords == other.keywords)
    && (value == other.value) && (weight == other.weight);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MiscObjectRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size of 12 bytes */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed size of 8 bytes */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */
               + unknownVMAD.size() /* length of subrecord */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* length of subrecord */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */
               + unknownMODS.size() /* length of subrecord */;
  }
  if (!iconPath.empty())
  {
    writeSize = writeSize + 4 /* ICON */ + 2 /* 2 bytes for length */
        + iconPath.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */
               + 4 /* KWDA */ + 2 /* 2 bytes for length */+ 4 * keywords.size();
  }
  return writeSize;
}

bool MiscObjectRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cMISC), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  //write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
      return false;
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  // OBND's length
  subLength = 12; /* fixed length */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write OBND's data
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (!modelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  // write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
    return false;

  // write MODS
  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
      return false;
  }

  if (!iconPath.empty())
  {
    // write icon path (ICON)
    output.write(reinterpret_cast<const char*>(&cICON), 4);
    subLength = iconPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(iconPath.c_str(), subLength);
  }

  if (pickupSoundFormID != 0)
  {
    // write Pickup Sound form ID (YNAM)
    output.write(reinterpret_cast<const char*>(&cYNAM), 4);
    subLength = 4; /* fixed length */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);
  }

  if (putdownSoundFormID != 0)
  {
    // write Putdown Sound form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4; /* fixed length */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  if (!keywords.empty())
  {
    // write KSIZ
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; /* fixed length */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write KSIZ's data
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; /* fixed length */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  return output.good();
}
#endif

bool MiscObjectRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  //read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  modelPath.clear();
  iconPath.clear();
  name.reset();
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  keywords.clear();
  bool hasReadOBND = false;
  bool hasReadDATA = false;
  while (bytesRead < readSize)
  {
    // read next subrecord header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: record MISC seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
             return false;
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: record MISC seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading subrecord FULL of MISC!\n";
             return false;
           }
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: record MISC seems to have more than one OBND subrecord.\n";
             return false;
           }
           // OBND's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: sub record OBND of MISC has invalid length ("
                       << subLength << " bytes)! Should be 12 bytes.\n";
             return false;
           }
           // read OBND's stuff
           in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of MISC!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: record MISC seems to have more than one MODL subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of MISC is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: record MISC seems to have more than one MODT subrecord.\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: record MISC seems to have more than one MODS subrecord.\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
             return false;
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cICON:
           if (!iconPath.empty())
           {
             std::cerr << "Error: record MISC seems to have more than one ICON subrecord.\n";
             return false;
           }
           // read ICON
           if (!loadString512FromStream(in_File, iconPath, buffer, cICON, false, bytesRead))
             return false;
           if (iconPath.empty())
           {
             std::cerr <<"Error: subrecord ICON of MISC is empty!\n";
             return false;
           }
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: record MISC seems to have more than one YNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: subrecord YNAM of MISC is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: record MISC seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: subrecord ZNAM of MISC is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: record MISC seems to have more than one DATA subrecord.\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: sub record DATA of MISC has invalid length ("
                       << subLength << " bytes)! Should be 8 bytes.\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of MISC!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      default:
           std::cerr << "Error: unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, only DATA, FULL, OBND, VMAD, MODL, KSIZ, MODT and MODS are allowed here!\n";
           return false;
    }
  }

  // Have all required subrecords been read?
  if (!hasReadDATA || !hasReadOBND)
  {
    std::cerr << "Error: at least one required subrecord of MISC was not found!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MiscObjectRecord::getRecordType() const
{
  return cMISC;
}

} // namespace
