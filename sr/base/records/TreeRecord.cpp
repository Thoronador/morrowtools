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

#include "TreeRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

TreeRecord::TreeRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  ingredientFormID(0),
  harvestSoundFormID(0),
  unknownPFPC(0),
  name(LocalizedString()),
  unknownCNAM(std::array<uint8_t, 48>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }))
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool TreeRecord::equals(const TreeRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (ingredientFormID == other.ingredientFormID)
      && (harvestSoundFormID == other.harvestSoundFormID)
      && (unknownPFPC == other.unknownPFPC)
      && (name == other.name)
      && (unknownCNAM == other.unknownCNAM);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t TreeRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* PFPC */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 48 /* fixed size */;
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
               + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (ingredientFormID != 0)
  {
    writeSize = writeSize + 4 /* PFIG */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (harvestSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  return writeSize;
}

bool TreeRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cTREE), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

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
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of TREE!\n";
      return false;
    }
  }

  if (ingredientFormID != 0)
  {
    // write Production Ingredient form ID (PFIG)
    output.write(reinterpret_cast<const char*>(&cPFIG), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&ingredientFormID), 4);
  }

  if (harvestSoundFormID != 0)
  {
    // write Harvest Sound form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&harvestSoundFormID), 4);
  }

  // write PFPC
  output.write(reinterpret_cast<const char*>(&cPFPC), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownPFPC), 4);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  // write CNAM
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  subLength = 48; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownCNAM.data()), 48);

  return output.good();
}
#endif

bool TreeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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

  // read OBND
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
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
    std::cerr << "Error: Sub record OBND of TREE has invalid length ("
               << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of TREE!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  ingredientFormID = 0;
  harvestSoundFormID = 0;
  bool hasReadPFPC = false;
  name.reset();
  bool hasReadCNAM = false;
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
             std::cerr << "Error: TREE seems to have more than one MODL subrecord.\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: TREE seems to have more than one MODT subrecord.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of TREE!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownMODT.size() /*data size*/;
           break;
      case cPFIG:
           if (ingredientFormID != 0)
           {
             std::cerr << "Error: TREE seems to have more than one PFIG subrecord.\n";
             return false;
           }
           // read PFIG
           if (!loadUint32SubRecordFromStream(in_File, cPFIG, ingredientFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (ingredientFormID == 0)
           {
             std::cerr << "Error: Subrecord PFIG of TREE is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (harvestSoundFormID != 0)
           {
             std::cerr << "Error: TREE seems to have more than one SNAM subrecord.\n";
             return false;
           }
           // read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, harvestSoundFormID, false))
             return false;
           bytesRead += 6;
           // check content
           if (harvestSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord SNAM of TREE is zero!\n";
             return false;
           }
           break;
      case cPFPC:
           if (hasReadPFPC)
           {
             std::cerr << "Error: TREE seems to have more than one PFCP subrecord.\n";
             return false;
           }
           // read PFCP
           if (!loadUint32SubRecordFromStream(in_File, cPFPC, unknownPFPC, false))
             return false;
           bytesRead += 6;
           hasReadPFPC = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: TREE seems to have more than one FULL subrecord.\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cerr << "Error: TREE seems to have more than one CNAM subrecord.\n";
             return false;
           }
           // CNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 48)
           {
             std::cerr << "Error: Sub record CNAM of TREE has invalid length ("
                       << subLength << " bytes). Should be 48 bytes.\n";
             return false;
           }
           // read CNAM's stuff
           in_File.read(reinterpret_cast<char*>(unknownCNAM.data()), 48);
           bytesRead += 48;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord CNAM of TREE!\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      default:
           std::cerr << "Error: Unexpected record type \""
                     << IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, PFIG, SNAM, PFPC, "
                     << " FULL or CNAM are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (modelPath.empty() || !hasReadPFPC || !hasReadCNAM)
  {
    std::cerr << "Error while reading TREE record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t TreeRecord::getRecordType() const
{
  return cTREE;
}

} // namespace
