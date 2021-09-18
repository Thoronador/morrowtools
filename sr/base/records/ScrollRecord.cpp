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

#include "ScrollRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ScrollRecord::ScrollRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  keywords(std::vector<uint32_t>()),
  menuDisplayObjectFormID(0),
  equipTypeFormID(0),
  description(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  value(0),
  weight(0.0f),
  unknownSPIT(std::array<uint8_t, 36>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  effects(std::vector<EffectBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool ScrollRecord::equals(const ScrollRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (menuDisplayObjectFormID == other.menuDisplayObjectFormID)
      && (keywords == other.keywords)
      && (equipTypeFormID == other.equipTypeFormID) && (description == other.description)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (value == other.value) && (weight == other.weight)
      && (unknownSPIT == other.unknownSPIT)
      && (effects == other.effects);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ScrollRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
      + 4 /* MDOB */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
      + 4 /* ETYP */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
      + description.getWriteSize() /* DESC */
      + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* DATA */ + 2 /* 2 bytes for length */ + 8 /* fixed length */
      + 4 /* SPIT */ + 2 /* 2 bytes for length */ + 36 /* fixed length */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
        + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size(); /* fixed length of 4 bytes per elem. */
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /*MODT*/ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* size */;
  }
  for (const auto& effect: effects)
  {
    writeSize += effect.getWriteSize();
  }
  return writeSize;
}

bool ScrollRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSCRL), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

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
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t len = keywords.size();
    output.write(reinterpret_cast<const char*>(&len), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * len;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write keywords' form IDs
    for (const auto keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  } // if keywords

  // write menu display object's form ID (MDOB)
  output.write(reinterpret_cast<const char*>(&cMDOB), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&menuDisplayObjectFormID), 4);

  //write equip type's form ID (ETYP)
  output.write(reinterpret_cast<const char*>(&cETYP), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&equipTypeFormID), 4);

  // write description (DESC)
  if (!description.saveToStream(output, cDESC))
    return false;

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of SCRL!\n";
      return false;
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 8; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&value), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  // write spell item (SPIT)
  output.write(reinterpret_cast<const char*>(&cSPIT), 4);
  subLength = 36; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownSPIT.data()), 36);

  for (const auto& effect: effects)
  {
    if (!effect.saveToStream(output))
    {
      std::cerr << "Error while writing effect block of SCRL!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool ScrollRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr << "Error: subrecord OBND of SCRL has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of SCRL!\n";
    return false;
  }

  name.reset();
  keywords.clear();
  uint32_t tempKeyword, kwdaLength;
  bool hasReadMDOB = false;
  bool hasReadETYP = false;
  description.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  bool hasReadSPIT = false;
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
             std::cerr << "Error: SCRL seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!keywords.empty())
           {
             std::cerr << "Error: SCRL seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           // read KSIZ
           kwdaLength = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, kwdaLength, false))
             return false;
           bytesRead += 6;

           // read KWDA
           in_File.read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cKWDA)
           {
             UnexpectedRecord(cOBND, subRecName);
             return false;
           }
           // KWDA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != kwdaLength * 4)
           {
             std::cerr << "Error: subrecord KWDA of SCRL has invalid length ("
                       << subLength << " bytes). Should be " << kwdaLength * 4
                       << " bytes!\n";
             return false;
           }
           // read KWDA's stuff
           for (uint32_t i = 0; i < kwdaLength; ++i)
           {
             in_File.read(reinterpret_cast<char*>(&tempKeyword), 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cerr << "Error while reading subrecord KWDA of SCRL!\n";
               return false;
             }
             keywords.push_back(tempKeyword);
           }
           break;
      case cMDOB:
           if (hasReadMDOB)
           {
             std::cerr << "Error: SCRL seems to have more than one MDOB subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false))
             return false;
           bytesRead += 6;
           if (menuDisplayObjectFormID == 0)
           {
             std::cerr << "Error: Subrecord MDOB of SCRL is zero!\n";
             return false;
           }
           hasReadMDOB = true;
           break;
      case cETYP:
           if (hasReadETYP)
           {
             std::cerr << "Error: SCRL seems to have more than one ETYP subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false))
             return false;
           bytesRead += 6;
           if (equipTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord ETYP of SCRL is zero!\n";
             return false;
           }
           hasReadETYP = true;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: SCRL seems to have more than one DESC subrecord!\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: SCRL seems to have more than one MODL subrecord!\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: SCRL seems to have more than one MODT subrecord!\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of SCRL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: SCRL seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: subrecord DATA of SCRL has invalid length ("
                       << subLength << " bytes). Should be 8 bytes!\n";
             return false;
           }
           // read DATA's stuff
           in_File.read(reinterpret_cast<char*>(&value), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of SCRL!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSPIT:
           if (hasReadSPIT)
           {
             std::cerr << "Error: SCRL seems to have more than one SPIT subrecord!\n";
             return false;
           }
           // SPIT's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 36)
           {
             std::cerr << "Error: subrecord SPIT of SCRL has invalid length ("
                       << subLength << " bytes). Should be 36 bytes!\n";
             return false;
           }
           // read SPIT's stuff
           in_File.read(reinterpret_cast<char*>(unknownSPIT.data()), 36);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord SPIT of SCRL!\n";
             return false;
           }
           hasReadSPIT = true;
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
             std::cerr << "Error: subrecord EFID of SCRL has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read EFID's stuff
           in_File.read(reinterpret_cast<char*>(&tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFID of SCRL!\n";
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
             std::cerr << "Error: subrecord EFIT of SCRL has invalid length ("
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
             std::cerr << "Error while reading subrecord EFIT of SCRL!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading SCRL: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           // read CTDA's stuff
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of SCRL!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading SCRL: CIS2 found, but there was no EFID/EFIT block!\n";
             return false;
           }
           if (tempEffect.unknownCTDA_CIS2s.empty())
           {
             std::cerr << "Error while reading SCRL: CIS2 found, but there was no CTDA before it!\n";
             return false;
           }
           if (!tempEffect.unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cerr << "Error: SCRL seems to have more than one CIS2 subrecord per CTDA!\n";
             return false;
           }
           // read CIS2
           if (!loadString512FromStream(in_File, tempEffect.unknownCTDA_CIS2s.back().unknownCISx, buffer, cCIS2, false, bytesRead))
             return false;
           break;
      default:
           std::cerr << "Error: unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only FULL, MDOB, ETYP, DESC, SPIT, EFID,"
                     << " CTDA or CIS2 are allowed here!\n";
           return false;
           break;
    }
  }

  // check possibly not yet pushed effect block
  if (hasNonPushedEffect)
  {
    effects.push_back(tempEffect);
  }

  // check presence
  if (!hasReadMDOB || !hasReadETYP || !description.isPresent() || modelPath.empty() || !hasReadDATA || !hasReadSPIT)
  {
    std::cerr << "Error while reading record SCRL: at least one of the "
              << "subrecords MDOB, ETYP, DESC, MODL, DATA or SPIT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ScrollRecord::getRecordType() const
{
  return cSCRL;
}

} // namespace
