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

#include "SpellRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

SpellRecord::SpellRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  name(LocalizedString()),
  menuDisplayObjectFormID(0),
  equipTypeFormID(0),
  description(LocalizedString()),
  data(SpellItem()),
  effects(std::vector<EffectBlock>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool SpellRecord::equals(const SpellRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name)
      && (menuDisplayObjectFormID == other.menuDisplayObjectFormID)
      && (equipTypeFormID == other.equipTypeFormID) && (description == other.description)
      && (data == other.data)
      && (effects == other.effects);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SpellRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
    + editorID.length() + 1 /* length of name +1 byte for NUL termination */
    + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed length */
    + 4 /* ETYP */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
    + description.getWriteSize() /* DESC */
    + 4 /* SPIT */ + 2 /* 2 bytes for length */ + 36 /* fixed length */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /*FULL*/;
  }
  if (menuDisplayObjectFormID != 0)
  {
    writeSize = writeSize + 4 /* MDOB */ + 2 /* 2 bytes for length */ + 4 /* length */;
  }
  for (const auto& effect: effects)
  {
    writeSize += effect.getWriteSize();
  }
  return writeSize;
}

bool SpellRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSPEL), 4);
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
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (menuDisplayObjectFormID != 0)
  {
    // write menu display object's form ID (MDOB)
    output.write(reinterpret_cast<const char*>(&cMDOB), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&menuDisplayObjectFormID), 4);
  }

  // write equip type's form ID (ETYP)
  output.write(reinterpret_cast<const char*>(&cETYP), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&equipTypeFormID), 4);

  // write DESC
  if (!description.saveToStream(output,cDESC))
    return false;

  // write SPIT
  if (!data.saveToStream(output))
    return false;

  for (const auto& effect: effects)
  {
    if (!effect.saveToStream(output))
    {
      std::cerr << "Error while writing effect block of SPEL!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool SpellRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  // There is at least one deleted spell record in Dragonborn.esm (with the
  // editor ID MGRSummonDremoraCOPY0000) that has data after the header, so
  // we check the size value instead of using isDeleted(). Otherwise loading
  // this version of Dragonborn.esm fails.
  // Question: Should this approach be used in other record types, too?
  // This issue needs more research to get a reliable answer.
  if (readSize == 0)
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
    std::cerr << "Error: Subrecord OBND of SPEL has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of SPEL!\n";
    return false;
  }

  name.reset();
  menuDisplayObjectFormID = 0;
  equipTypeFormID = 0;
  description.reset();
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
             std::cerr << "Error: SPEL seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMDOB:
           if (menuDisplayObjectFormID != 0)
           {
             std::cerr << "Error: SPEL seems to have more than one MDOB subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false))
             return false;
           bytesRead += 6;
           if (menuDisplayObjectFormID == 0)
           {
             std::cerr << "Error: Subrecord MDOB of SPEL has value zero!\n";
             return false;
           }
           break;
      case cETYP:
           if (equipTypeFormID != 0)
           {
             std::cerr << "Error: SPEL seems to have more than one ETYP subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cETYP, equipTypeFormID, false))
             return false;
           bytesRead += 6;
           if (equipTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord ETYP of SPEL has value zero!\n";
             return false;
           }
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: SPEL seems to have more than one DESC subrecord!\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cSPIT:
           if (hasReadSPIT)
           {
             std::cerr << "Error: SPEL seems to have more than one SPIT subrecord!\n";
             return false;
           }
           if (!data.loadFromStream(in_File, cSPEL, bytesRead))
             return false;
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
             std::cerr << "Error: Subrecord EFID of SPEL has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // read EFID's stuff
           in_File.read(reinterpret_cast<char*>(&tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFID of SPEL!\n";
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
             std::cerr << "Error: Subrecord EFIT of SPEL has invalid length ("
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
             std::cerr << "Error while reading subrecord EFIT of SPEL!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading SPEL: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           // load CTDA
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of SPEL!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading SPEL: CIS2 found, but there was no EFID/EFIT block!\n";
             return false;
           }
           if (tempEffect.unknownCTDA_CIS2s.empty())
           {
             std::cerr << "Error while reading SPEL: CIS2 found, but there was no CTDA before it!\n";
             return false;
           }
           if (!tempEffect.unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cerr << "Error: SPEL seems to have more than one CIS2 subrecord per CTDA!\n";
             return false;
           }
           // read CIS2
           if (!loadString512FromStream(in_File, tempEffect.unknownCTDA_CIS2s.back().unknownCISx, buffer, cCIS2, false, bytesRead))
             return false;
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
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
  if ((equipTypeFormID == 0) || !description.isPresent() || !hasReadSPIT)
  {
    std::cerr << "Error while reading record SPEL: At least one of the "
              << "subrecords ETYP, DESC or SPIT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SpellRecord::getRecordType() const
{
  return cSPEL;
}

} // namespace
