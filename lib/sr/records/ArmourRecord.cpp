/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "ArmourRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

ArmourRecord::ArmourRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  enchantingFormID(0),
  modelPath(""),
  unknownMO2T(BinarySubRecord()),
  unknownMO2S(BinarySubRecord()),
  mod4Path(""),
  unknownMO4T(BinarySubRecord()),
  unknownMO4S(BinarySubRecord()),
  unknownBODT(BinarySubRecord()),
  unknownBOD2(BinarySubRecord()),
  equipTypeFormID(0),
  blockBashImpactDataSetFormID(0),
  alternateBlockMaterialFormID(0),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  unknownRNAM(0),
  keywords(std::vector<uint32_t>()),
  description(LocalizedString()),
  models(std::vector<uint32_t>()),
  value(0),
  weight(0.0f),
  unknownDNAM(0),
  templateArmorFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ArmourRecord::equals(const ArmourRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (enchantingFormID == other.enchantingFormID)
      && (modelPath == other.modelPath) && (unknownMO2T == other.unknownMO2T)
      && (unknownMO2S == other.unknownMO2S)
      && (mod4Path == other.mod4Path) && (unknownMO4T == other.unknownMO4T)
      && (unknownMO4S == other.unknownMO4S) && (unknownBODT == other.unknownBODT)
      && (unknownBOD2 == other.unknownBOD2)
      && (equipTypeFormID == other.equipTypeFormID)
      && (blockBashImpactDataSetFormID == other.blockBashImpactDataSetFormID)
      && (alternateBlockMaterialFormID == other.alternateBlockMaterialFormID)
      && (pickupSoundFormID == other.pickupSoundFormID)
      && (putdownSoundFormID == other.putdownSoundFormID)
      && (unknownRNAM == other.unknownRNAM) && (keywords == other.keywords)
      && (description == other.description)
      && (models == other.models)
      && (value == other.value) && (weight == other.weight)
      && (unknownDNAM == other.unknownDNAM)
      && (templateArmorFormID == other.templateArmorFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ArmourRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
        + 4 /* RNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length */
        + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* length */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }

  if (enchantingFormID != 0)
  {
    writeSize = writeSize + 4 /* EITM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MOD2 */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO2T.isPresent())
  {
    writeSize = writeSize + 4 /* MO2T */ + 2 /* 2 bytes for length */ + unknownMO2T.size();
  }
  if (unknownMO2S.isPresent())
  {
    writeSize = writeSize + 4 /* MO2S */ + 2 /* 2 bytes for length */ + unknownMO2S.size();
  }
  if (!mod4Path.empty())
  {
    writeSize = writeSize + 4 /* MOD4 */ + 2 /* 2 bytes for length */
        + mod4Path.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO4T.isPresent())
  {
    writeSize = writeSize + 4 /* MO4T */ + 2 /* 2 bytes for length */ + unknownMO4T.size();
  }
  if (unknownMO4S.isPresent())
  {
    writeSize = writeSize + 4 /* MO4S */ + 2 /* 2 bytes for length */ +unknownMO4S.size();
  }
  if (unknownBODT.isPresent())
  {
    writeSize = writeSize + 4 /* BODT */ + 2 /* 2 bytes for length */ + unknownBODT.size();
  }
  if (unknownBOD2.isPresent())
  {
    writeSize = writeSize + 4 /* BOD2 */ + 2 /* 2 bytes for length */ + unknownBOD2.size();
  }
  if (equipTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* ETYP */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (blockBashImpactDataSetFormID != 0)
  {
    writeSize = writeSize + 4 /* BIDS */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (alternateBlockMaterialFormID != 0)
  {
    writeSize = writeSize + 4 /* BAMT */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
                          + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size();
  }
  if (description.isPresent())
  {
    writeSize += description.getWriteSize();
  }
  if (!models.empty())
  {
    writeSize = writeSize + models.size() * (4 /* MODL */ + 2 /* 2 bytes for length */ + 4 /* fixed length */);
  }
  if (templateArmorFormID != 0)
  {
    writeSize = writeSize + 4 /* TNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  return writeSize;
}

bool ArmourRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cARMO), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing VMAD of ARMO!";
      return false;
    }
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (enchantingFormID != 0)
  {
    // write Enchanting form ID (EITM)
    output.write(reinterpret_cast<const char*>(&cEITM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&enchantingFormID), 4);
  }

  if (!modelPath.empty())
  {
    // write model path (MOD2)
    output.write(reinterpret_cast<const char*>(&cMOD2), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMO2T.isPresent())
  {
    if (!unknownMO2T.saveToStream(output, cMO2T))
    {
      std::cerr << "Error while writing MO2T of ARMO!";
      return false;
    }
  }

  if (unknownMO2S.isPresent())
  {
    if (!unknownMO2S.saveToStream(output, cMO2S))
    {
      std::cerr << "Error while writing MO2S of ARMO!";
      return false;
    }
  }

  if (!mod4Path.empty())
  {
    // write model path (MOD4)
    output.write(reinterpret_cast<const char*>(&cMOD4), 4);
    subLength = mod4Path.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(mod4Path.c_str(), subLength);
  }

  if (unknownMO4T.isPresent())
  {
    if (!unknownMO4T.saveToStream(output, cMO4T))
    {
      std::cerr << "Error while writing MO4T of ARMO!";
      return false;
    }
  }

  if (unknownMO4S.isPresent())
  {
    if (!unknownMO4S.saveToStream(output, cMO4S))
    {
      std::cerr << "Error while writing MO4S of ARMO!";
      return false;
    }
  }

  if (unknownBODT.isPresent())
  {
    if (!unknownBODT.saveToStream(output, cBODT))
    {
      std::cerr << "Error while writing BODT of ARMO!";
      return false;
    }
  }

  if (unknownBOD2.isPresent())
  {
    if (!unknownBOD2.saveToStream(output, cBOD2))
    {
      std::cerr << "Error while writing BOD2 of ARMO!";
      return false;
    }
  }

  if (equipTypeFormID != 0)
  {
    // write Equip Type's form ID (ETYP)
    output.write(reinterpret_cast<const char*>(&cETYP), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&equipTypeFormID), 4);
  }

  if (blockBashImpactDataSetFormID != 0)
  {
    // write Block Bash Impact Data Set's form ID (BIDS)
    output.write(reinterpret_cast<const char*>(&cBIDS), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&blockBashImpactDataSetFormID), 4);
  }

  if (alternateBlockMaterialFormID != 0)
  {
    // write Alternate Block Material's form ID (BAMT)
    output.write((const char*) &cBAMT, 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&alternateBlockMaterialFormID), 4);
  }

  if (pickupSoundFormID != 0)
  {
    // write Pickup Sound form ID (YNAM)
    output.write(reinterpret_cast<const char*>(&cYNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);
  }

  if (putdownSoundFormID != 0)
  {
    // write Putdown Sound Form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  // write RNAM
  output.write(reinterpret_cast<const char*>(&cRNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownRNAM), 4);

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

  if (description.isPresent())
  {
    if (!description.saveToStream(output, cDESC))
      return false;
  }

  for (const uint32_t model: models)
  {
    // write form ids (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&model), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  // write DNAM
  output.write(reinterpret_cast<const char*>(&cDNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownDNAM), 4);

  if (templateArmorFormID != 0)
  {
    // write Template Armor's form ID (TNAM)
    output.write(reinterpret_cast<const char*>(&cTNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&templateArmorFormID), 4);
  }

  return output.good();
}
#endif

bool ArmourRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  enchantingFormID = 0;
  modelPath.clear();
  unknownMO2T.setPresence(false);
  unknownMO2S.setPresence(false);
  mod4Path.clear();
  unknownMO4T.setPresence(false);
  unknownBODT.setPresence(false);
  unknownBOD2.setPresence(false);
  equipTypeFormID = 0;
  blockBashImpactDataSetFormID = 0;
  alternateBlockMaterialFormID = 0;
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  bool hasReadRNAM = false;
  keywords.clear();
  description.reset();
  models.clear();
  bool hasReadDATA = false;
  bool hasReadDNAM = false;
  templateArmorFormID = 0;
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
             std::cerr << "Error: ARMO seems to have more than one VMAD subrecord!\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: ARMO seems to have more than one OBND subrecord!\n";
             return false;
           }
           // OBND's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record OBND of ARMO has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // read OBND's stuff
           in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of ARMO!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record ARMO seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cEITM:
           if (enchantingFormID != 0)
           {
             std::cerr << "Error: Record ARMO seems to have more than one EITM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cEITM, enchantingFormID, false))
             return false;
           bytesRead += 6;
           if (enchantingFormID == 0)
           {
             std::cerr << "Error: Sub record EITM of ARMO is zero!\n";
             return false;
           }
           break;
      case cMOD2:
           if (!modelPath.empty())
           {
             std::cerr << "Error: Record ARMO seems to have more than one MOD2 subrecord!\n";
             return false;
           }
           // read male model path (MOD2)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMOD2, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Sub record MOD2 of ARMO is empty!\n";
             return false;
           }
           break;
      case cMO2T:
           if (unknownMO2T.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one MO2T subrecord!\n";
             return false;
           }
           if (!unknownMO2T.loadFromStream(in_File, cMO2T, false))
           {
             std::cerr << "Error while reading subrecord MO2T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMO2T.size();
           break;
      case cMO2S:
           if (unknownMO2S.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one MO2S subrecord!\n";
             return false;
           }
           if (!unknownMO2S.loadFromStream(in_File, cMO2S, false))
           {
             std::cerr << "Error while reading subrecord MO2S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMO2S.size();
           break;
      case cMOD4:
           if (!mod4Path.empty())
           {
             std::cerr << "Error: Record ARMO seems to have more than one MOD4 subrecord!\n";
             return false;
           }
           // read female armour model path (MOD4)
           if (!loadString512FromStream(in_File, mod4Path, buffer, cMOD4, false, bytesRead))
             return false;
           if (mod4Path.empty())
           {
             std::cerr << "Error: Sub record MOD4 of ARMO is empty!\n";
             return false;
           }
           break;
      case cMO4T:
           if (unknownMO4T.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one MO4T subrecord!\n";
             return false;
           }
           if (!unknownMO4T.loadFromStream(in_File, cMO4T, false))
           {
             std::cerr << "Error while reading subrecord MO4T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMO4T.size();
           break;
      case cMO4S:
           if (unknownMO4S.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one MO4S subrecord!\n";
             return false;
           }
           if (!unknownMO4S.loadFromStream(in_File, cMO4S, false))
           {
             std::cerr << "Error while reading subrecord MO4S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMO4S.size();
           break;
      case cBODT:
           if (unknownBODT.isPresent())
           {
             std::cerr << "Error: Record ARMO seems to have more than one BODT subrecord!\n";
             return false;
           }
           if (!unknownBODT.loadFromStream(in_File, cBODT, false))
           {
             std::cerr << "Error while reading subrecord BODT of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownBODT.size();
           // check BODT's length
           if ((unknownBODT.size() != 12) && (unknownBODT.size() != 8))
           {
             std::cerr << "Error: Sub record BODT of ARMO has invalid length ("
                       << unknownBODT.size() << " bytes). Should be 8 or 12 bytes!\n";
             return false;
           }
           break;
      case cBOD2:
           if (unknownBOD2.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one BOD2 subrecord!\n";
             return false;
           }
           if (!unknownBOD2.loadFromStream(in_File, cBOD2, false))
             return false;
           bytesRead += (2 + unknownBOD2.size());
           break;
      case cETYP:
           if (equipTypeFormID != 0)
           {
             std::cerr << "Error: Record ARMO seems to have more than one ETYP subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false))
             return false;
           bytesRead += 6;
           if (equipTypeFormID == 0)
           {
             std::cerr << "Error: Sub record ETYP of ARMO is zero!\n";
             return false;
           }
           break;
      case cBIDS:
           if (blockBashImpactDataSetFormID != 0)
           {
             std::cerr << "Error: Record ARMO seems to have more than one BIDS subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, blockBashImpactDataSetFormID, false))
             return false;
           bytesRead += 6;
           if (blockBashImpactDataSetFormID == 0)
           {
             std::cerr << "Error: Sub record BIDS of ARMO is zero!\n";
             return false;
           }
           break;
      case cBAMT:
           if (alternateBlockMaterialFormID != 0)
           {
             std::cerr << "Error: Record ARMO seems to have more than one BAMT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, alternateBlockMaterialFormID, false))
             return false;
           bytesRead += 6;
           if (alternateBlockMaterialFormID == 0)
           {
             std::cerr << "Error: Sub record BAMT of ARMO is zero!\n";
             return false;
           }
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: ARMO seems to have more than one YNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord YNAM of ARMO is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: ARMO seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of ARMO is zero!\n";
             return false;
           }
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cerr << "Error: ARMO seems to have more than one RNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, unknownRNAM, false))
             return false;
           bytesRead += 6;
           hasReadRNAM = true;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: ARMO seems to have more than one DESC subrecord!\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading subrecord DESC of ARMO!\n";
             return false;
           }
           break;
      case cMODL:
           models.push_back(0);
           if (!loadUint32SubRecordFromStream(in_File, cMODL, models.back(), false))
             return false;
           bytesRead += 6;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: Record ARMO seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record DATA of ARMO has invalid length ("
                       << subLength << " bytes). Should be 8 bytes!\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of ARMO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cerr << "Error: ARMO seems to have more than one DNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cDNAM, unknownDNAM, false))
             return false;
           bytesRead += 6;
           hasReadDNAM = true;
           break;
      case cTNAM:
           if (templateArmorFormID != 0)
           {
             std::cerr << "Error: ARMO seems to have more than one TNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, templateArmorFormID, false))
             return false;
           bytesRead += 6;
           if (templateArmorFormID == 0)
           {
             std::cerr << "Error: Sub record TNAM of ARMO is zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only VMAD, OBND, FULL, EITM, MOD2, MO2T,"
                     << " MOD4, MO4T, BODT, BOD2, ETYP, BIDS, BAMT, YNAM, ZNAM,"
                     << " RNAM, KSIZ, DESC, MODL, DATA, DNAM, or TNAM are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!(hasReadOBND && (unknownBODT.isPresent() || unknownBOD2.isPresent()) && hasReadRNAM && hasReadDATA && hasReadDNAM))
  {
    std::cerr << "Error: Subrecord OBND, BODT/BOD2, DATA, DNAM or RNAM of ARMO is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ArmourRecord::getRecordType() const
{
  return cARMO;
}

} // namespace
