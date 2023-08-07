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

#include "TalkingActivatorRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

TalkingActivatorRecord::TalkingActivatorRecord()
: BasicRecord(), editorID(""),
  unknownOBND(std::array<uint8_t, 12>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  keywords(std::vector<uint32_t>()),
  unknownPNAM(0),
  loopingSoundFormID(0),
  unknownFNAM(0),
  voiceTypeFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool TalkingActivatorRecord::equals(const TalkingActivatorRecord& other) const
{
  return (equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (name == other.name) && (modelPath == other.modelPath)
      && (unknownMODT == other.unknownMODT) && (keywords == other.keywords)
      && (unknownPNAM == other.unknownPNAM)
      && (loopingSoundFormID == other.loopingSoundFormID)
      && (unknownFNAM == other.unknownFNAM) && (voiceTypeFormID == other.voiceTypeFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t TalkingActivatorRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
      + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of path +1 byte for NUL termination */
      + 4 /* PNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */
      + 4 /* FNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed size of 2 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size();
  }
  if (loopingSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */;
  }
  if (voiceTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* VNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */;
  }
  return writeSize;
}

bool TalkingActivatorRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cTACT), 4);
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
    // write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(modelPath.c_str(), subLength);

  // write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
  {
    std::cerr << "Error while writing subrecord MODT of TACT!\n";
    return false;
  }

  if (!keywords.empty())
  {
    // write size of keyword array (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * keywords.size();
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write keywords
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  } // if keywords

  // write PNAM
  output.write(reinterpret_cast<const char*>(&cPNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownPNAM), 4);

  if (loopingSoundFormID != 0)
  {
    // write looping sound's form ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&loopingSoundFormID), 4);
  }

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  subLength = 2; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownFNAM), 2);

  if (voiceTypeFormID != 0)
  {
    // write voice type form ID (VNAM)
    output.write(reinterpret_cast<const char*>(&cVNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&voiceTypeFormID), 4);
  }

  return output.good();
}
#endif

bool TalkingActivatorRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor (EDID)
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
    std::cerr << "Error: Sub record OBND of TACT has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  unknownOBND.fill(0);
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of TACT!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  keywords.clear();
  bool hasReadPNAM = false;
  loopingSoundFormID = 0;
  bool hasReadFNAM = false;
  voiceTypeFormID = 0;

  while (bytesRead < readSize)
  {
    // read next subrecord header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record TACT seems to have more than one FULL subrecord!\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           // sanity check
           if ((name.getType() == LocalizedString::Type::Index) && (name.getIndex() == 0))
           {
             std::cerr << "Error: Subrecord FULL of TACT has invalid value zero!\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: Record TACT seems to have more than one MODL subrecord!\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           // sanity check
           if (modelPath.empty())
           {
             std::cerr << "Error: Subrecord MODL of TACT is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: Record TACT seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of TACT!";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: Record TACT seems to have more than one PNAM subrecord!\n";
             return false;
           }
           // read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false))
             return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cSNAM:
           if (loopingSoundFormID != 0)
           {
             std::cerr << "Error: Record TACT seems to have more than one SNAM subrecord!\n";
             return false;
           }
           // read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, loopingSoundFormID, false))
             return false;
           bytesRead += 6;
           // sanity check
           if (loopingSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord SNAM of TACT has invalid value zero!\n";
             return false;
           }
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: Record TACT seems to have more than one FNAM subrecord!\n";
             return false;
           }
           // FNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record FNAM of TACT has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           // read FNAM's stuff
           in_File.read(reinterpret_cast<char*>(&unknownFNAM), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of TACT!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cVNAM:
           if (voiceTypeFormID != 0)
           {
             std::cerr << "Error: Record TACT seems to have more than one VNAM subrecord!\n";
             return false;
           }
           // read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, voiceTypeFormID, false))
             return false;
           bytesRead += 6;
           // sanity check
           if (voiceTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord VNAM of TACT has invalid value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, KSIZ, PNAM, FNAM or VNAM are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (modelPath.empty() || !hasReadPNAM || !hasReadFNAM)
  {
    std::cerr << "Error: At least one required subrecord of TACT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t TalkingActivatorRecord::getRecordType() const
{
  return cTACT;
}

} // namespace
