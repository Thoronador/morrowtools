/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2021  Thoronador

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

#include "FurnitureRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

FurnitureRecord::FurnitureRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(BinarySubRecord()),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  destruction(DestructionData()),
  keywords(std::vector<uint32_t>()),
  unknownPNAM(0), unknownFNAM(0),
  interactionKeywordFormID(0),
  unknownMNAM(0), unknownWBDT(0),
  unknownENAMs(std::vector<uint32_t>()),
  unknownNAM0s(std::vector<uint32_t>()),
  unknownFNMKs(std::vector<uint32_t>()),
  unknownFNPRs(std::vector<uint32_t>()),
  markerModelPath("")
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool FurnitureRecord::equals(const FurnitureRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD) && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownMODS == other.unknownMODS)
      && (destruction == other.destruction)
      && (keywords == other.keywords)
      && (unknownPNAM == other.unknownPNAM) && (unknownFNAM == other.unknownFNAM)
      && (interactionKeywordFormID == other.interactionKeywordFormID)
      && (unknownMNAM == other.unknownMNAM) && (unknownWBDT == other.unknownWBDT)
      && (unknownENAMs == other.unknownENAMs) && (unknownNAM0s == other.unknownNAM0s)
      && (unknownFNMKs == other.unknownFNMKs)
      && (unknownFNPRs == other.unknownFNPRs)
      && (markerModelPath == other.markerModelPath);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FurnitureRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + destruction.getWriteSize()
      + 4 /* PNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
      + 4 /* FNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed length */
      + 4 /* MNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
      + 4 /* WBDT */ + 2 /* 2 bytes for length */ + 2 /* fixed length */
      + unknownENAMs.size() * (4 /* ENAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */)
      + unknownNAM0s.size() * (4 /* NAM0 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */)
      + unknownFNMKs.size() * (4 /* FNMK */ + 2 /* 2 bytes for length */ + 4 /* fixed size */)
      + unknownFNPRs.size() * (4 /* FNPR */ + 2 /* 2 bytes for length */ + 4 /* fixed length */);
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size */;
  }
  if (unknownOBND.isPresent())
  {
    writeSize = writeSize + 4 /* OBND */ + 2 /* 2 bytes for length */ + unknownOBND.size() /* size */;
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
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
         + 4 /* KWDA */ + 2 /* 2 bytes for length */ + keywords.size() * 4 /* 4 bytes per element */;
  }
  if (interactionKeywordFormID != 0)
  {
    writeSize = writeSize + 4 /* KNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (!markerModelPath.empty())
  {
    writeSize = writeSize + 4 /* XMKR */ + 2 /* 2 bytes for length */
        + markerModelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  return writeSize;
}

bool FurnitureRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cFURN), 4);
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
      std::cerr << "Error while writing subrecord VMAD of FURN!\n";
      return false;
    }
  }

  if (unknownOBND.isPresent())
  {
    // write OBND
    if (!unknownOBND.saveToStream(output, cOBND))
    {
      std::cerr << "Error while writing subrecord OBND of FURN!\n";
      return false;
    }
  }

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
      std::cerr << "Error while writing subrecord MODT of FURN!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    // write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of FURN!\n";
      return false;
    }
  }

  if (!destruction.saveToStream(output))
  {
    std::cerr << "Error while writing destruction data of FURN!\n";
    return false;
  }

  if (!keywords.empty())
  {
    // write keyword size (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
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

  // write PNAM
  output.write(reinterpret_cast<const char*>(&cPNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownPNAM), 4);

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  subLength = 2;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownFNAM), 2);

  if (interactionKeywordFormID != 0)
  {
    // write KNAM
    output.write(reinterpret_cast<const char*>(&cKNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&interactionKeywordFormID), 4);
  }

  // write MNAM
  output.write(reinterpret_cast<const char*>(&cMNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownMNAM), 4);

  // write WBDT
  output.write(reinterpret_cast<const char*>(&cWBDT), 4);
  subLength = 2;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownWBDT), 2);

  for (const uint32_t elem: unknownENAMs)
  {
    // write ENAM
    output.write(reinterpret_cast<const char*>(&cENAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&elem), 4);
  }

  for (const uint32_t nam0: unknownNAM0s)
  {
    // write NAM0
    output.write(reinterpret_cast<const char*>(&cNAM0), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&nam0), 4);
  }

  for (const uint32_t fnmk: unknownFNMKs)
  {
    // write FNMK
    output.write(reinterpret_cast<const char*>(&cFNMK), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&fnmk), 4);
  }

  for (const uint32_t fnpr: unknownFNPRs)
  {
    // write FNPR
    output.write(reinterpret_cast<const char*>(&cFNPR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&fnpr), 4);
  }

  if (!markerModelPath.empty())
  {
    // write marker model path (XMRK)
    output.write(reinterpret_cast<const char*>(&cXMRK), 4);
    subLength = markerModelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(markerModelPath.c_str(), subLength);
  }

  return output.good();
}
#endif

bool FurnitureRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;

  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  unknownOBND.setPresence(false);
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  destruction.clear();
  keywords.clear();
  uint32_t tempUint32;
  bool hasReadPNAM = false; unknownPNAM = 0;
  bool hasReadFNAM = false; unknownFNAM = 0;
  interactionKeywordFormID = 0;
  bool hasReadMNAM = false; unknownMNAM = 0;
  bool hasReadWBDT = false; unknownWBDT = 0;
  unknownENAMs.clear();
  unknownNAM0s.clear();
  unknownFNMKs.clear();
  unknownFNPRs.clear();
  markerModelPath.clear();

  while (bytesRead < readSize)
  {
    // read next record
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: FURN seems to have more than one VMAD subrecord!\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
             return false;
           bytesRead += (2 + unknownVMAD.size());
           break;
      case cOBND:
           if (unknownOBND.isPresent())
           {
             std::cerr << "Error: FURN seems to have more than one OBND subrecord!\n";
             return false;
           }
           if (!unknownOBND.loadFromStream(in_File, cOBND, false))
             return false;
           bytesRead += (2 + unknownOBND.size());
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: FURN seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: FURN seems to have more than one MODL subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           // check content
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of FURN is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: FURN seems to have more than one MODT subrecord!\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead += (2 + unknownMODT.size());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: FURN seems to have more than one MODS subrecord!\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
             return false;
           bytesRead += (2 + unknownMODS.size());
           break;
      case cDEST:
           if (destruction.isPresent)
           {
             std::cerr << "Error: FURN seems to have more than one DEST subrecord.\n";
             return false;
           }
           // read DEST and possible DSTD, DMDL, DMDT, DSTF subrecords
           if (!destruction.loadFromStream(in_File, cFURN, buffer, bytesRead))
             return false;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: FURN seems to have more than one PNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false))
             return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: FURN seems to have more than one FNAM subrecord!\n";
             return false;
           }
           // FNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: subrecord FNAM of FURN has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           // read FNAM's stuff
           in_File.read(reinterpret_cast<char*>(&unknownFNAM), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of FURN!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cKNAM:
           if (interactionKeywordFormID != 0)
           {
             std::cerr << "Error: FURN seems to have more than one KNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cKNAM, interactionKeywordFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (interactionKeywordFormID == 0)
           {
             std::cerr << "Error: subrecord KNAM of FURN is zero!\n";
             return false;
           }
           break;
      case cMNAM:
           if (hasReadMNAM)
           {
             std::cerr << "Error: FURN seems to have more than one MNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM, false))
             return false;
           bytesRead += 6;
           hasReadMNAM = true;
           break;
      case cWBDT:
           if (hasReadWBDT)
           {
             std::cerr << "Error: FURN seems to have more than one WBDT subrecord!\n";
             return false;
           }
           // WBDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: subrecord WBDT of FURN has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           // read WBDT's stuff
           in_File.read(reinterpret_cast<char*>(&unknownWBDT), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord WBDT of FURN!\n";
             return false;
           }
           hasReadWBDT = true;
           break;
      case cENAM:
           if (!loadUint32SubRecordFromStream(in_File, cENAM, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownENAMs.push_back(tempUint32);
           break;
      case cNAM0:
           if (!loadUint32SubRecordFromStream(in_File, cNAM0, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownNAM0s.push_back(tempUint32);
           break;
      case cFNMK:
           if (!loadUint32SubRecordFromStream(in_File, cFNMK, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownFNMKs.push_back(tempUint32);
           break;
      case cFNPR:
           if (!loadUint32SubRecordFromStream(in_File, cFNPR, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownFNPRs.push_back(tempUint32);
           break;
      case cXMRK:
           if (!markerModelPath.empty())
           {
             std::cerr << "Error: FURN seems to have more than one XMRK subrecord!\n";
             return false;
           }
           // read XMRK
           if (!loadString512FromStream(in_File, markerModelPath, buffer, cXMRK, false, bytesRead))
             return false;
           // check content
           if (markerModelPath.empty())
           {
             std::cerr << "Error: subrecord XMRK of FURN is empty!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \"" << IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, FULL, MODL, MODT, MODS, DEST,"
                     << " KSIZ, PNAM, FNAM, KNAM, MNAM, WBDT, ENAM, NAM0, FNMK,"
                     << " FNPR or XMRK are allowed here!\n";
           return false;
    }
  }

  // presence checks
  if (!hasReadPNAM || !hasReadFNAM || !hasReadMNAM || !hasReadWBDT)
  {
    std::cerr << "Error: At least one required subrecord of FURN is missing!\n";
    std::cerr << "PNAM: " << hasReadPNAM << ", FNAM: " << hasReadFNAM
              << ", MNAM: " << hasReadMNAM << ", WBDT: " << hasReadWBDT << ".\n";
    return false;
  }

  return in_File.good();
}

uint32_t FurnitureRecord::getRecordType() const
{
  return cFURN;
}

} // namespace
