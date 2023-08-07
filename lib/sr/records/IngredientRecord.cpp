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

#include "IngredientRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t IngredientRecord::cFlagNoAutoCalc = 0x00000001;
const uint32_t IngredientRecord::cFlagFoodItem   = 0x00000002;

IngredientRecord::IngredientRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  keywords(std::vector<uint32_t>()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  value(0),
  weight(0.0f),
  baseCost(0),
  flags(0),
  effects(std::vector<EffectBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool IngredientRecord::equals(const IngredientRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (keywords == other.keywords)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownMODS == other.unknownMODS)
      && (pickupSoundFormID == other.pickupSoundFormID)
      && (putdownSoundFormID == other.putdownSoundFormID)
      && (value == other.value) && (weight == other.weight)
      && (baseCost == other.baseCost) && (flags == other.flags)
      && (effects == other.effects);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t IngredientRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
    + editorID.length() + 1 /* length of name +1 byte for NUL termination */
    + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length of 12 bytes */
    + name.getWriteSize() /* FULL */
    + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length of 8 bytes */
    + 4 /* ENIT */ + 2 /* 2 bytes for length */ + 8 /* fixed length of 8 bytes */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /*VMAD*/ + 2 /* 2 bytes for length */
               + unknownVMAD.size() /* size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length of 4 bytes */
        + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size(); /* fixed length of 4 bytes per elem.*/
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /*MODT*/ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /*MODS*/ + 2 /* 2 bytes for length */
               + unknownMODS.size() /* size */;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length of 4 bytes */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length of 4 bytes */;
  }
  for (const auto& effect: effects)
  {
    writeSize += effect.getWriteSize();
  }
  return writeSize;
}

bool IngredientRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cINGR), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

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
      std::cerr << "Error while writing subrecord VMAD of INGR!\n";
      return false;
    }
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write FULL
  if (!name.saveToStream(output, cFULL))
    return false;

  if (!keywords.empty())
  {
    // write keyword size (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t len = keywords.size();
    output.write(reinterpret_cast<const char*>(&len), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * len; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write keywords' form IDs
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
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
      std::cerr << "Error while writing subrecord MODT of INGR!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    // write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of INGR!\n";
      return false;
    }
  }

  if (pickupSoundFormID != 0)
  {
    // write Pickup Sound form ID (YNAM)
    output.write(reinterpret_cast<const char*>(&cYNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);
  }

  if (putdownSoundFormID != 0)
  {
    // write Putdown Sound form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's stuff
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  // write ENIT
  output.write(reinterpret_cast<const char*>(&cENIT), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&baseCost), 4);
  output.write(reinterpret_cast<const char*>(&flags), 4);

  for (const auto& effect: effects)
  {
    if (!effect.saveToStream(output))
    {
      std::cerr << "Error while writing effect block of INGR!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool IngredientRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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

  //read (VMAD or) OBND
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName == cVMAD)
  {
    // has VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cerr << "Error while reading subrecord VMAD of INGR!\n";
      return false;
    }
    bytesRead = bytesRead + 2 + unknownVMAD.size();

    // read next subrecord (OBND)
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
  }
  else
  {
    // no VMAD here!
    unknownVMAD.setPresence(false);
  }

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
    std::cerr << "Error: Subrecord OBND of INGR has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of INGR!\n";
    return false;
  }

  name.reset();
  keywords.clear();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  bool hasReadDATA = false;
  bool hasReadENIT = false;
  effects.clear();
  EffectBlock tempEffect;
  CTDAData tempCTDA;
  bool hasNonPushedEffect = false;
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: INGR seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: INGR seems to have more than one MODL subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of INGR is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: INGR seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of INGR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: INGR seems to have more than one MODS subrecord!\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of INGR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODS.size());
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: INGR seems to have more than one YNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord YNAM of INGR is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: INGR seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of INGR is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: INGR seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Subrecord DATA of INGR has invalid length ("
                       << subLength << " bytes). Should be 8 bytes!\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of INGR!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cENIT:
           if (hasReadENIT)
           {
             std::cerr << "Error: INGR seems to have more than one ENIT subrecord!\n";
             return false;
           }
           // ENIT's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Subrecord ENIT of INGR has invalid length ("
                       << subLength << " bytes). Should be 8 bytes!\n";
             return false;
           }
           // read ENIT's stuff
           in_File.read(reinterpret_cast<char*>(&baseCost), 4);
           in_File.read(reinterpret_cast<char*>(&flags), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ENIT of INGR!\n";
             return false;
           }
           hasReadENIT = true;
           break;
      case cEFID:
           // check for old effect block
           if (hasNonPushedEffect)
           {
             // need to push
             effects.push_back(tempEffect);
             hasNonPushedEffect = false;
           }
           // new effect block
           tempEffect.unknownCTDA_CIS2s.clear();
           // EFID's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Subrecord EFID of INGR has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read EFID's stuff
           in_File.read(reinterpret_cast<char*>(&tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFID of INGR!\n";
             return false;
           }

           // read EFIT
           in_File.read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cEFIT)
           {
             UnexpectedRecord(cEFIT, subRecName);
             return false;
           }
           // EFIT's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Subrecord EFIT of INGR has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // read EFIT's stuff
           in_File.read(reinterpret_cast<char*>(&tempEffect.magnitude), 4);
           in_File.read(reinterpret_cast<char*>(&tempEffect.areaOfEffect), 4);
           in_File.read(reinterpret_cast<char*>(&tempEffect.duration), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFIT of INGR!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading INGR: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           // read CTDA's stuff
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of INGR!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only FULL, KSIZ, MODL, MODT, MODS, ENIT,"
                     << " EFID or CTDA are allowed here!\n";
           return false;
           break;
    }
  }

  // check for left effect block
  if (hasNonPushedEffect)
  {
    // need to push
    effects.push_back(tempEffect);
    hasNonPushedEffect = false;
  }

  // checks
  if (effects.size() != 4)
  {
    std::cerr << "Error while reading INGR: there aren't four effects!\n";
    return false;
  }
  // more checks
  if (!name.isPresent() || !hasReadDATA || !hasReadENIT)
  {
    std::cerr << "Error while reading record INGR: At least one of the "
              << "subrecords FULL, DATA or ENIT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t IngredientRecord::getRecordType() const
{
  return cINGR;
}

} // namespace
