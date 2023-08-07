/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint8_t ContainerRecord::cFlagRespawns  = 0x02;
const uint8_t ContainerRecord::cFlagShowOwner = 0x04;
// ---- header flags
const uint32_t ContainerRecord::cFlagRandomAnimStart = 0x00010000;
const uint32_t ContainerRecord::cFlagObstacle        = 0x02000000;

/* ContainerRecord's functions */

ContainerRecord::ContainerRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  contents(std::vector<ComponentData>()),
  unknownCOED(BinarySubRecord()),
  flags(0),
  weight(0.0f),
  openSoundFormID(0),
  closeSoundFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ContainerRecord::equals(const ContainerRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD==other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (modelPath == other.modelPath)
      && (unknownMODT == other.unknownMODT)
      && (unknownMODS == other.unknownMODS)
      && (contents == other.contents)
      && (unknownCOED == other.unknownCOED) && (flags == other.flags)
      && (weight == other.weight) && (openSoundFormID == other.openSoundFormID)
      && (closeSoundFormID == other.closeSoundFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ContainerRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 5 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size() /* size */;
  }
  if (!contents.empty())
  {
    writeSize = writeSize + 4 /* COCT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + contents.size() * (4 /* CNTO */ + 2 /* 2 bytes for length */ + 8 /* fixed size */);
  }
  if (unknownCOED.isPresent())
  {
    writeSize = writeSize + 4 /* COED */ + 2 /* 2 bytes for length */ + unknownCOED.size() /* size */;
  }
  if (openSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (closeSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* QNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool ContainerRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCONT), 4);
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
      std::cerr << "Error while writing subrecord VMAD of CONT!\n";
      return false;
    }
  }

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
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

  if (unknownMODT.isPresent())
  {
    // write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of CONT!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    // write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of CONT!\n";
      return false;
    }
  }

  if (!contents.empty())
  {
    const uint32_t len = contents.size();

    // write component count (COCT)
    output.write(reinterpret_cast<const char*>(&cCOCT), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&len), 4);

    for (const auto& component: contents)
    {
      // write component object (CNTO)
      output.write(reinterpret_cast<const char*>(&cCNTO), 4);
      subLength = 8; // fixed length
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&component.formID), 4);
      output.write(reinterpret_cast<const char*>(&component.count), 4);
    }
  }

  if (unknownCOED.isPresent())
  {
    // write COED
    if (!unknownCOED.saveToStream(output, cCOED))
    {
      std::cerr << "Error while writing subrecord COED of CONT!\n";
      return false;
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 5; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&flags), 1);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  if (openSoundFormID != 0)
  {
    // write Open Sound Form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&openSoundFormID), 4);
  }

  if (closeSoundFormID != 0)
  {
    // write Close Sound Form ID (QNAM)
    output.write(reinterpret_cast<const char*>(&cQNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&closeSoundFormID), 4);
  }

  return output.good();
}
#endif

bool ContainerRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;

  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  contents.clear();
  ComponentData tempComponent;
  uint32_t tempSize;
  unknownCOED.setPresence(false);
  bool hasReadDATA = false;
  openSoundFormID = 0;
  closeSoundFormID = 0;
  while (bytesRead < readSize)
  {
    // read next subrecord
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: CONT seems to have more than one VMAD subrecord.\n";
             return false;
           }
           // read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
             return false;
           bytesRead += (2 + unknownVMAD.size());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: Record CONT seems to have more than one OBND subrecord!\n";
             return false;
           }
           // OBND's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record OBND of CONT has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read OBND
           in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of CONT!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record CONT seems to have more than one FULL subrecord!\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: CONT seems to have more than one MODL subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of CONT is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: CONT seems to have more than one MODT subrecord.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead += (2 + unknownMODT.size());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: CONT seems to have more than one MODS subrecord.\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
             return false;
           bytesRead += (2 + unknownMODS.size());
           break;
      case cCOCT:
           if (!contents.empty())
           {
             std::cerr << "Error: Record CONT seems to have more than one COCT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCOCT, tempSize, false))
             return false;

           bytesRead += 6;

           for (uint32_t i = 0; i < tempSize; ++i)
           {
             // read CNTO
             in_File.read(reinterpret_cast<char*>(&subRecName), 4);
             bytesRead += 4;
             if (subRecName != cCNTO)
             {
               UnexpectedRecord(cCNTO, subRecName);
               return false;
             }
             // CNTO's length
             in_File.read(reinterpret_cast<char*>(&subLength), 2);
             bytesRead += 2;
             if (subLength != 8)
             {
               std::cerr << "Error: Sub record CNTO of CONT has invalid length ("
                         << subLength << " bytes). Should be eight bytes.\n";
               return false;
             }
             // read COCT
             in_File.read(reinterpret_cast<char*>(&tempComponent.formID), 4);
             in_File.read(reinterpret_cast<char*>(&tempComponent.count), 4);
             bytesRead += 8;
             if (!in_File.good())
             {
               std::cerr << "Error while reading subrecord COCT of CONT!\n";
               return false;
             }
             contents.push_back(tempComponent);
           }
           break;
      case cCOED:
           if (unknownCOED.isPresent())
           {
             std::cerr << "Error: CONT seems to have more than one COED subrecord.\n";
             return false;
           }
           // read COED
           if (!unknownCOED.loadFromStream(in_File, cCOED, false))
             return false;
           bytesRead += (2 + unknownCOED.size());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: Record CONT seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 5)
           {
             std::cerr << "Error: Sub record DATA of CONT has invalid length ("
                       << subLength << " bytes). Should be five bytes.\n";
             return false;
           }
           // read DATA
           in_File.read(reinterpret_cast<char*>(&flags), 1);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 5;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of CONT!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSNAM:
           if (openSoundFormID != 0)
           {
             std::cerr << "Error: Record CONT seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, openSoundFormID, false))
             return false;
           bytesRead += 6;
           if (openSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord SNAM of CONT is zero!\n";
             return false;
           }
           break;
      case cQNAM:
           if (closeSoundFormID != 0)
           {
             std::cerr << "Error: Record CONT seems to have more than one QNAM subrecord!\n";
             return false;
           }
           // read QNAM
           if (!loadUint32SubRecordFromStream(in_File, cQNAM, closeSoundFormID, false))
             return false;
           bytesRead += 6;
           if (closeSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord QNAM of CONT is zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only VMAD, OBND, FULL, MODL, MODT, MODS,"
                     << " COCT, COED, DATA, SNAM or QNAM are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!hasReadOBND || !hasReadDATA)
  {
    std::cerr << "Error: At least one required subrecord of CONT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ContainerRecord::getRecordType() const
{
  return cCONT;
}

} // namespace
