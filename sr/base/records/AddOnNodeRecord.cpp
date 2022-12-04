/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "AddOnNodeRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

// flag constants
const uint16_t AddOnNodeRecord::cFlagUnknown      = 0x01;
const uint16_t AddOnNodeRecord::cFlagAlwaysLoaded = 0x02;

AddOnNodeRecord::AddOnNodeRecord()
: BasicRecord(), editorID(""),
  unknownOBND({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownDATA(0),
  soundDescriptorFormID(0),
  MasterParticleSystemCap(0),
  flags(1) // yes, one
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AddOnNodeRecord::equals(const AddOnNodeRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownDATA == other.unknownDATA)
      && (soundDescriptorFormID == other.soundDescriptorFormID)
      && (MasterParticleSystemCap == other.MasterParticleSystemCap)
      && (flags == other.flags);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AddOnNodeRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
    + editorID.length() + 1 /* length of name +1 byte for NUL termination */
    + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
    + 4 /* MODL */ + 2 /* 2 bytes for length */
    + modelPath.length() + 1 /* length of path +1 byte for NUL termination */
    + 4 /* DATA */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */
    + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (soundDescriptorFormID != 0)
  {
    writeSize = writeSize + 4 /* SNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of 4 bytes */;
  }
  return writeSize;
}

bool AddOnNodeRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cADDN), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write EDID
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  // EDID's length
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write editor ID
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  // OBND's length
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write OBND's stuff
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write MODL
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  // MODL's length
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write model path
  output.write(modelPath.c_str(), subLength);

  // write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
  {
    std::cerr << "Error while writing subrecord MODT of ADDN!\n";
    return false;
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 4; // fixed size of four bytes
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write value
  output.write(reinterpret_cast<const char*>(&unknownDATA), 4);

  if (soundDescriptorFormID != 0)
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    // SNAM's length
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write SNAM
    output.write(reinterpret_cast<const char*>(&soundDescriptorFormID), 4);
  }

  // write DNAM
  output.write(reinterpret_cast<const char*>(&cDNAM), 4);
  subLength = 4; // fixed
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DNAM's content
  output.write(reinterpret_cast<const char*>(&MasterParticleSystemCap), 2);
  output.write(reinterpret_cast<const char*>(&flags), 2);

  return output.good();
}
#endif

bool AddOnNodeRecord::loadFromStream(std::istream& in_File,
                                     [[maybe_unused]] const bool localized,
                                     [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read EDID
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
    std::cerr << "Error: Sub record OBND of ADDN has invalid length ("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  // read OBND's stuff
  unknownOBND.fill(0);
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading sub record OBND of ADDN!\n";
    return false;
  }

  // read MODL
  if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, true, bytesRead))
    return false;

  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  soundDescriptorFormID = 0;
  bool hasReadDNAM = false;
  while (bytesRead < readSize)
  {
    // read next sub record
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: Record ADDN seems to have more than one MODT sub record!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of ADDN!";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: Record ADDN seems to have more than one DATA sub record!\n";
             return false;
           }
           // read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, unknownDATA, false))
             return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cSNAM:
           if (soundDescriptorFormID != 0)
           {
             std::cerr << "Error: Record ADDN seems to have more than one SNAM sub record!\n";
             return false;
           }
           // read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, soundDescriptorFormID, false))
             return false;
           bytesRead += 6;
           if (soundDescriptorFormID == 0)
           {
             std::cerr << "Error: Sub record SNAM of ADDN has value zero!\n";
             return false;
           }
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cerr << "Error: Record ADDN seems to have more than one DNAM sub record!\n";
             return false;
           }
           // DNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record DNAM of ADDN has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           // read DNAM
           in_File.read(reinterpret_cast<char*>(&MasterParticleSystemCap), 2);
           in_File.read(reinterpret_cast<char*>(&flags), 2);
           bytesRead += 4;
           hasReadDNAM = true;
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only MODT, DATA, SNAM or DNAM are allowed here!\n";
           return false;
           break;
    }
  } // while

  // presence checks
  if (!(hasReadDATA && hasReadDNAM))
  {
    std::cerr << "Error: At least one required sub record of ADDN is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AddOnNodeRecord::getRecordType() const
{
  return cADDN;
}

bool AddOnNodeRecord::alwaysLoaded() const
{
  return (flags & cFlagAlwaysLoaded) != 0;
}

} // namespace
