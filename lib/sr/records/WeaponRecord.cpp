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

#include "WeaponRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

const unsigned int WeaponRecord::cTypeSwordOneHand = 1;
const unsigned int WeaponRecord::cTypeDagger = 2;
const unsigned int WeaponRecord::cTypeAxeOneHand = 3;
const unsigned int WeaponRecord::cTypeMaceOneHand = 4;
const unsigned int WeaponRecord::cTypeSwordTwoHand = 5;
const unsigned int WeaponRecord::cTypeBluntTwoHand = 6; // axes, warhammers
const unsigned int WeaponRecord::cTypeBow = 7;
const unsigned int WeaponRecord::cTypeStaves = 8;

WeaponRecord::WeaponRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  enchantingFormID(0),
  enchantmentAmount(std::nullopt),
  equipTypeFormID(0),
  blockBashImpactDataSetFormID(0),
  alternateBlockMaterialFormID(0),
  keywords(std::vector<uint32_t>()),
  description(LocalizedString()),
  unknownNNAM(""),
  impactDataSetFormID(0),
  firstPersonModelObjectFormID(0),
  attackSoundFormID(0),
  attackSound2DFormID(0),
  attackLoopSoundFormID(0),
  attackFailSoundFormID(0),
  idleSoundFormID(0),
  equipSoundFormID(0),
  unequipSoundFormID(0),
  // DATA
  value(0),
  weight(0.0f),
  baseDamage(0),
  // end of DATA
  unknownDNAM({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknownCRDT(BinarySubRecord()),
  unknownVNAM(0),
  unknownCNAM(std::nullopt)
{
}

uint32_t WeaponRecord::getRecordType() const
{
  return cWEAP;
}

#ifndef SR_NO_RECORD_EQUALITY
bool WeaponRecord::equals(const WeaponRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
    && (unknownVMAD == other.unknownVMAD)
    && (unknownOBND == other.unknownOBND)
    && (name == other.name)
    && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
    && (unknownMODS == other.unknownMODS)
    && (enchantingFormID == other.enchantingFormID)
    && (enchantmentAmount == other.enchantmentAmount)
    && (equipTypeFormID == other.equipTypeFormID)
    && (blockBashImpactDataSetFormID == other.blockBashImpactDataSetFormID)
    && (alternateBlockMaterialFormID == other.alternateBlockMaterialFormID)
    && (keywords == other.keywords) && (description == other.description)
    && (unknownNNAM == other.unknownNNAM)
    && (impactDataSetFormID == other.impactDataSetFormID)
    && (firstPersonModelObjectFormID == other.firstPersonModelObjectFormID)
    && (attackSoundFormID == other.attackSoundFormID)
    && (attackSound2DFormID == other.attackSound2DFormID)
    && (attackLoopSoundFormID == other.attackLoopSoundFormID)
    && (attackFailSoundFormID == other.attackFailSoundFormID)
    && (idleSoundFormID == other.idleSoundFormID)
    && (equipSoundFormID == other.equipSoundFormID)
    && (unequipSoundFormID == other.unequipSoundFormID)
    && (unknownCNAM == other.unknownCNAM)
    && (value == other.value) && (weight == other.weight)
    && (baseDamage == other.baseDamage)
    && (unknownDNAM == other.unknownDNAM)
    && (unknownCRDT == other.unknownCRDT)
    && (unknownVNAM == other.unknownVNAM);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WeaponRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + description.getWriteSize() /* DESC */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 10 /* fixed size */
        + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 100 /* fixed size */
        + 4 /* VNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */
               + unknownVMAD.size() /* subrecord size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* subrecord size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */
               + unknownMODS.size() /* subrecord size */;
  }
  if (enchantingFormID != 0)
  {
    writeSize = writeSize + 4 /* EITM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (enchantmentAmount.has_value())
  {
    writeSize = writeSize + 4 /* EAMT */ + 2 /* 2 bytes for length */ + 2 /* fixed size */;
  }
  if (equipTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* ETYP */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (blockBashImpactDataSetFormID != 0)
  {
    writeSize = writeSize + 4 /* BIDS */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (alternateBlockMaterialFormID != 0)
  {
    writeSize = writeSize + 4 /* BAMT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size() /* n*fixed size */;
  }
  if (!unknownNNAM.empty())
  {
    writeSize = writeSize + 4 /* NNAM */ + 2 /* 2 bytes for length */
        + unknownNNAM.length() + 1 /* length of string +1 byte for NUL-terminaton */;
  }
  if (impactDataSetFormID != 0)
  {
    writeSize = writeSize + 4 /* INAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (firstPersonModelObjectFormID != 0)
  {
    writeSize = writeSize + 4 /* WNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (attackSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (attackSound2DFormID != 0)
  {
    writeSize = writeSize + 4 /* XNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (attackLoopSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM7 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (attackFailSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* TNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (idleSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* UNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (equipSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM9 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unequipSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* NAM8 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownCNAM.has_value())
  {
    writeSize = writeSize + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownCRDT.isPresent())
  {
    writeSize = writeSize + 4 /* CRDT */ + 2 /* 2 bytes for length */
               + unknownCRDT.size() /* subrecord size */;
  }
  return writeSize;
}

bool WeaponRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cWEAP), 4);
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
      std::cerr << "Error while writing subrecord VMAD of WEAP!\n";
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
      std::cerr << "Error while writing subrecord MODT of WEAP!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of WEAP!\n";
      return false;
    }
  }

  if (enchantingFormID != 0)
  {
    // write enchanting form ID (EITM)
    output.write(reinterpret_cast<const char*>(&cEITM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&enchantingFormID), 4);
  }

  if (enchantmentAmount.has_value())
  {
    // write enchantment amount (EAMT)
    output.write(reinterpret_cast<const char*>(&cEAMT), 4);
    subLength = 2;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&enchantmentAmount.value()), 2);
  }

  if (equipTypeFormID != 0)
  {
    // write equip type form ID (ETYP)
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
    // write Alternate Block Material form ID (BAMT)
    output.write(reinterpret_cast<const char*>(&cBAMT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&alternateBlockMaterialFormID), 4);
  }

  if (!keywords.empty())
  {
    // write keywords' size (KSIZ)
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

  // write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

  if(!unknownNNAM.empty())
  {
    // write node name (NNAM)
    output.write(reinterpret_cast<const char*>(&cNNAM), 4);
    subLength = unknownNNAM.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(unknownNNAM.c_str(), subLength);
  }

  if (impactDataSetFormID != 0)
  {
    // write Impact Data Set form ID (INAM)
    output.write(reinterpret_cast<const char*>(&cINAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&impactDataSetFormID), 4);
  }

  if (firstPersonModelObjectFormID != 0)
  {
    // write 1st Person Model Object form ID (WNAM)
    output.write(reinterpret_cast<const char*>(&cWNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&firstPersonModelObjectFormID), 4);
  }

  if (attackSoundFormID != 0)
  {
    // write Attack Sound form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&attackSoundFormID), 4);
  }

  if (attackSound2DFormID != 0)
  {
    // write Attack Sound (2D) form ID (XNAM)
    output.write(reinterpret_cast<const char*>(&cXNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&attackSound2DFormID), 4);
  }

  if (attackLoopSoundFormID != 0)
  {
    // write Attack Loop Sound's form ID (NAM7)
    output.write(reinterpret_cast<const char*>(&cNAM7), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&attackLoopSoundFormID), 4);
  }

  if (attackFailSoundFormID != 0)
  {
    //write Attack Fail sound form ID (TNAM)
    output.write(reinterpret_cast<const char*>(&cTNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&attackFailSoundFormID), 4);
  }

  if (idleSoundFormID != 0)
  {
    // write Idle Sound form ID (UNAM)
    output.write(reinterpret_cast<const char*>(&cUNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&idleSoundFormID), 4);
  }

  if (equipSoundFormID != 0)
  {
    //write Equip Sound form ID (NAM9)
    output.write(reinterpret_cast<const char*>(&cNAM9), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&equipSoundFormID), 4);
  }

  if (unequipSoundFormID != 0)
  {
    // write unequip sound form ID (NAM8)
    output.write(reinterpret_cast<const char*>(&cNAM8), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unequipSoundFormID), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 10;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);
  output.write(reinterpret_cast<const char*>(&baseDamage), 2);

  // write DNAM
  output.write(reinterpret_cast<const char*>(&cDNAM), 4);
  subLength = 100;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownDNAM.data()), 100);

  // write CRDT
  if (unknownCRDT.isPresent())
  {
    if (!unknownCRDT.saveToStream(output, cCRDT))
    {
      std::cerr << "Error while writing subrecord CRDT of WEAP!\n";
      return false;
    }
  }

  // write VNAM
  output.write(reinterpret_cast<const char*>(&cVNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownVNAM), 4);

  if (unknownCNAM.has_value())
  {
    // write CNAM
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownCNAM.value()), 4);
  }

  return output.good();
}
#endif

bool WeaponRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  enchantingFormID = 0;
  enchantmentAmount.reset();
  equipTypeFormID = 0;
  blockBashImpactDataSetFormID = 0;
  alternateBlockMaterialFormID = 0;
  keywords.clear();
  description.reset();
  unknownNNAM.clear();
  attackSoundFormID = 0;
  bool hasReadDATA = false;
  impactDataSetFormID = 0;
  firstPersonModelObjectFormID = 0;
  attackSound2DFormID = 0;
  attackLoopSoundFormID = 0;
  attackFailSoundFormID = 0;
  idleSoundFormID = 0;
  equipSoundFormID = 0;
  unequipSoundFormID = 0;
  unknownCNAM.reset();
  unknownCRDT.setPresence(false);
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: WEAP seems to have more than one VMAD subrecord!\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: WEAP seems to have more than one OBND subrecord!\n";
             return false;
           }
           // OBND's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record OBND of WEAP has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read OBND
           in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of WEAP!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: WEAP seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: WEAP seems to have more than one MODL subrecord!\n";
             return false;
           }
           // MODL's length
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: WEAP seems to have an empty MODL subrecord!\n";
             return false;
           }

           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, true))
           {
             std::cerr << "Error while reading subrecord MODT of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead + 4 + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: WEAP seems to have more than one MODS subrecord!\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cEITM:
           if (enchantingFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one EITM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cEITM, enchantingFormID, false))
             return false;
           bytesRead += 6;
           if (enchantingFormID == 0)
           {
             std::cerr << "Error: Subrecord EITM of WEAP is zero!\n";
             return false;
           }
           break;
      case cEAMT:
           if (enchantmentAmount.has_value())
           {
             std::cerr << "Error: WEAP seems to have more than one EAMT subrecord!\n";
             return false;
           }
           // EAMT's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record EAMT of WEAP has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read EAMT
           enchantmentAmount = 0;
           in_File.read(reinterpret_cast<char*>(&enchantmentAmount.value()), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EAMT of WEAP!\n";
             return false;
           }
           break;
      case cETYP:
           if (equipTypeFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one ETYP subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false))
             return false;
           bytesRead += 6;
           if (equipTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord ETYP of WEAP is zero!\n";
             return false;
           }
           break;
      case cBIDS:
           if (blockBashImpactDataSetFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one BIDS subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, blockBashImpactDataSetFormID, false))
             return false;
           bytesRead += 6;
           if (blockBashImpactDataSetFormID == 0)
           {
             std::cerr << "Error: Subrecord BIDS of WEAP is zero!\n";
             return false;
           }
           break;
      case cBAMT:
           if (alternateBlockMaterialFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one BAMT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, alternateBlockMaterialFormID, false))
             return false;
           bytesRead += 6;
           if (alternateBlockMaterialFormID == 0)
           {
             std::cerr << "Error: Subrecord BAMT of WEAP is zero!\n";
             return false;
           }
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: WEAP seems to have more than one DESC subrecord!\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cNNAM:
           if (!unknownNNAM.empty())
           {
             std::cerr << "Error: WEAP seems to have more than one NNAM subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, unknownNNAM, buffer, cNNAM, false, bytesRead))
             return false;
           if (unknownNNAM.empty())
           {
             std::cerr << "Error: Subrecord NNAM of WEAP is empty!\n";
             return false;
           }
           break;
      case cINAM:
           if (impactDataSetFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one INAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cINAM, impactDataSetFormID, false))
             return false;
           bytesRead += 6;
           if (impactDataSetFormID == 0)
           {
             std::cerr << "Error: Subrecord INAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cWNAM:
           if (firstPersonModelObjectFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one WNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, firstPersonModelObjectFormID, false))
             return false;
           bytesRead += 6;
           if (firstPersonModelObjectFormID == 0)
           {
             std::cerr << "Error: Subrecord WNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (attackSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one SNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, attackSoundFormID, false))
             return false;
           bytesRead += 6;
           if (attackSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord SNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cXNAM:
           if (attackSound2DFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one XNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXNAM, attackSound2DFormID, false))
             return false;
           bytesRead += 6;
           if (attackSound2DFormID == 0)
           {
             std::cerr << "Error: Subrecord XNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cNAM7:
           if (attackLoopSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one NAM7 subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAM7, attackSound2DFormID, false))
             return false;
           bytesRead += 6;
           if (attackLoopSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord NAM7 of WEAP is zero!\n";
             return false;
           }
           break;
      case cTNAM:
           if (attackFailSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one TNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, attackFailSoundFormID, false))
             return false;
           bytesRead += 6;
           if (attackFailSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord TNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cUNAM:
           if (idleSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one UNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cUNAM, idleSoundFormID, false))
             return false;
           bytesRead += 6;
           if (idleSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord UNAM of WEAP is zero!\n";
             return false;
           }
           break;
      case cNAM9:
           if (equipSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one NAM9 subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAM9, equipSoundFormID, false))
             return false;
           bytesRead += 6;
           if (equipSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord NAM9 of WEAP is zero!\n";
             return false;
           }
           break;
      case cNAM8:
           if (unequipSoundFormID != 0)
           {
             std::cerr << "Error: WEAP seems to have more than one NAM8 subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAM8, unequipSoundFormID, false))
             return false;
           bytesRead += 6;
           if (unequipSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord NAM8 of WEAP is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: WEAP seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 10)
           {
             std::cerr << "Error: Sub record DATA of WEAP has invalid length ("
                       << subLength << " bytes). Should be 10 bytes.\n";
             return false;
           }
           // read DATA's content
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           in_File.read(reinterpret_cast<char*>(&baseDamage), 2);
           bytesRead += 10;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of WEAP!\n";
             return false;
           }
           hasReadDATA = true;

           // read DNAM
           in_File.read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cDNAM)
           {
             UnexpectedRecord(cDNAM, subRecName);
             return false;
           }
           // DNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 100)
           {
             std::cerr << "Error: Sub record DNAM of WEAP has invalid length ("
                       << subLength << " bytes). Should be 100 bytes.\n";
             return false;
           }
           // read DNAM's content
           in_File.read(reinterpret_cast<char*>(unknownDNAM.data()), 100);
           bytesRead += 100;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DNAM of WEAP!\n";
             return false;
           }

           // read CRDT
           if (!unknownCRDT.loadFromStream(in_File, cCRDT, true))
           {
             std::cerr << "Error while reading subrecord CRDT of WEAP!\n";
             return false;
           }
           bytesRead += 6 + unknownCRDT.size();
           if ((unknownCRDT.size() != 16) && (unknownCRDT.size() != 24))
           {
             std::cerr << "Error: Sub record CRDT of WEAP has invalid length ("
                       << unknownCRDT.size() << " bytes). Should be either 16 "
                       << "or 24 bytes.\n";
             return false;
           }

           // read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, unknownVNAM, true))
             return false;
           bytesRead += 10;
           break;
      case cCNAM:
           if (unknownCNAM.has_value())
           {
             std::cerr << "Error: WEAP seems to have more than one CNAM subrecord!\n";
             return false;
           }
           // read CNAM
           unknownCNAM = 0;
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM.value(), false))
             return false;
           bytesRead += 6;
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only VMAD, OBND, FULL, MODL, MODS, EITM,"
                     << " EAMT, ETYP, BIDS, BAMT, KSIZ, DESC, NNAM, INAM, WNAM,"
                     << " XNAM, NAM7, TNAM, UNAM, NAM9, NAM8, SNAM, DATA or CNAM are allowed!\n";
           return false;
           break;
    }
  }

  if (!hasReadOBND || !description.isPresent() || !hasReadDATA)
  {
    std::cerr << "Error: WEAP's OBND or DESC or DATA subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

} // namespace
