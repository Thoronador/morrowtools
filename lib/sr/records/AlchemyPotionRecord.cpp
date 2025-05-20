/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "AlchemyPotionRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t AlchemyPotionRecord::cFlagNoAutoCalc = 0x00000001;
const uint32_t AlchemyPotionRecord::cFlagFoodItem   = 0x00000002;
const uint32_t AlchemyPotionRecord::cFlagMedicine   = 0x00010000;
const uint32_t AlchemyPotionRecord::cFlagPoison     = 0x00020000;

AlchemyPotionRecord::AlchemyPotionRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  keywords(std::vector<uint32_t>()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  equipTypeFormID(0),
  weight(0.0f),
  value(0),
  flags(0),
  unknownThirdENIT(0),
  addictionChance(0.0f),
  useSoundFormID(0),
  effects(std::vector<EffectBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AlchemyPotionRecord::equals(const AlchemyPotionRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (keywords == other.keywords)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownMODS == other.unknownMODS)
      && (pickupSoundFormID == other.pickupSoundFormID)
      && (putdownSoundFormID == other.putdownSoundFormID)
      && (equipTypeFormID == other.equipTypeFormID) && (weight == other.weight)
      && (value == other.value) && (flags == other.flags)
      && (unknownThirdENIT == other.unknownThirdENIT)
      && (addictionChance == other.addictionChance)
      && (useSoundFormID == other.useSoundFormID)
      && (effects == other.effects);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AlchemyPotionRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
        + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of path +1 byte for NUL termination */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
        + 4 /* ENIT */ + 2 /* 2 bytes for length */ + 20 /* fixed length */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
                          + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size();
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* length */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size() /* length */;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  if (equipTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* ETYP */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  for (const auto& effect: effects)
  {
    writeSize += effect.getWriteSize();
  }
  return writeSize;
}

bool AlchemyPotionRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cALCH), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
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
    //write KWDA's data
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing MODT of ALCH!";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing MODS of ALCH!";
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

  if (putdownSoundFormID!=0)
  {
    // write putdown sound form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  if (equipTypeFormID!=0)
  {
    // write equip type form ID (ETYP)
    output.write(reinterpret_cast<const char*>(&cETYP), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&equipTypeFormID), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  // write ENIT
  output.write(reinterpret_cast<const char*>(&cENIT), 4);
  subLength = 20; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  //write ENIT
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&flags), 4);
  output.write(reinterpret_cast<const char*>(&unknownThirdENIT), 4);
  output.write(reinterpret_cast<const char*>(&addictionChance), 4);
  output.write(reinterpret_cast<const char*>(&useSoundFormID), 4);

  for (const auto& effect: effects)
  {
    if (!effect.saveToStream(output))
    {
      std::cerr << "Error while writing effect blocks of ALCH!\n";
      return false;
    }
  }

  return  output.good();
}
#endif

bool AlchemyPotionRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // read OBND
  input.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  // OBND's length
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of ALCH has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record OBND of ALCH!\n";
    return false;
  }

  name.reset();
  keywords.clear();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  equipTypeFormID = 0;
  bool hasReadDATA = false;
  bool hasReadENIT = false;
  effects.clear();
  EffectBlock tempEffect;
  CTDAData tempCTDA;
  bool hasNonPushedEffect = false;
  while (bytesRead < readSize)
  {
    // read next sub record header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record ALCH seems to have more than one FULL sub record!\n";
             return false;
           }
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!loadKeywords(input, keywords, bytesRead))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: ALCH seems to have more than one MODL sub record!\n";
             return false;
           }
           // read model path
           if (!loadString512FromStream(input, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: Sub record MODL of ALCH is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: ALCH seems to have more than one MODT sub record!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(input, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: ALCH seems to have more than one MODS sub record!\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(input, cMODS, false))
           {
             std::cerr << "Error while reading sub record MODS of ALCH!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: ALCH seems to have more than one YNAM sub record!\n";
             return false;
           }
           // read YNAM
           if (!loadUint32SubRecordFromStream(input, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Sub record YNAM of ALCH has value zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: ALCH seems to have more than one ZNAM sub record!\n";
             return false;
           }
           // read ZNAM
           if (!loadUint32SubRecordFromStream(input, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Sub record ZNAM of ALCH has value zero!\n";
             return false;
           }
           break;
      case cETYP:
           if (equipTypeFormID != 0)
           {
             std::cerr << "Error: ALCH seems to have more than one ETYP sub record!\n";
             return false;
           }
           // read ETYP
           if (!loadUint32SubRecordFromStream(input, cETYP, equipTypeFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (equipTypeFormID == 0)
           {
             std::cerr << "Error: Sub record ETYP of ALCH has value zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: ALCH seems to have more than one DATA sub record!\n";
             return false;
           }
           // DATA's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record DATA of ALCH has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read DATA's stuff
           input.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DATA of ALCH!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cENIT:
           if (hasReadENIT)
           {
             std::cerr << "Error: ALCH seems to have more than one ENIT sub record!\n";
             return false;
           }
           // ENIT's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 20)
           {
             std::cerr << "Error: Sub record ENIT of ALCH has invalid length ("
                       << subLength << " bytes). Should be 20 bytes!\n";
             return false;
           }
           // read ENIT's stuff
           input.read(reinterpret_cast<char*>(&value), 4);
           input.read(reinterpret_cast<char*>(&flags), 4);
           input.read(reinterpret_cast<char*>(&unknownThirdENIT), 4);
           input.read(reinterpret_cast<char*>(&addictionChance), 4);
           input.read(reinterpret_cast<char*>(&useSoundFormID), 4);
           bytesRead += 20;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record ENIT of ALCH!\n";
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
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record EFID of ALCH has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read EFID's stuff
           input.read(reinterpret_cast<char*>(&tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record EFID of ALCH!\n";
             return false;
           }

           // read EFIT
           input.read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cEFIT)
           {
             UnexpectedRecord(cEFIT, subRecName);
             return false;
           }
           // EFIT's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record EFIT of ALCH has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // read EFIT's stuff
           input.read(reinterpret_cast<char*>(&tempEffect.magnitude), 4);
           input.read(reinterpret_cast<char*>(&tempEffect.areaOfEffect), 4);
           input.read(reinterpret_cast<char*>(&tempEffect.duration), 4);
           bytesRead += 12;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record EFIT of ALCH!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      // We don't need CTDA stuff yet, but it might be useful in the future.
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading ALCH: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           // read CTDA's stuff
           if (!tempCTDA.loadFromStream(input, bytesRead))
           {
             std::cerr << "Error while reading sub record CTDA of ALCH!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only KSIZ, MODL, MODT, MODS, YNAM, ZNAM,"
                     << " ETYP, DATA, ENIT, EFID or CTDA are allowed here!\n";
           return false;
           break;
    }
  }

  // check for possibly not yet pushed effect block
  if (hasNonPushedEffect)
  {
    effects.push_back(tempEffect);
  }

  // presence checks
  if (!hasReadDATA || !hasReadENIT)
  {
    std::cerr << "Error: DATA or ENIT sub record of ALCH is missing!\n";
    return false;
  }
  if (effects.empty())
  {
    std::cerr << "Error: ALCH has no effects!\n";
    return false;
  }

  return input.good();
}

uint32_t AlchemyPotionRecord::getRecordType() const
{
  return cALCH;
}

bool AlchemyPotionRecord::doesAutoCalc() const
{
  return (flags & cFlagNoAutoCalc) == 0;
}

} // namespace
