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

#include "AnimatedObjectRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

AnimatedObjectRecord::AnimatedObjectRecord()
: BasicRecord(),
  editorID(""),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownBNAM("")
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AnimatedObjectRecord::equals(const AnimatedObjectRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownBNAM == other.unknownBNAM);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AnimatedObjectRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
      + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of string +1 byte for NUL-termination */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size();
  }
  if (!unknownBNAM.empty())
  {
    writeSize = writeSize + 4 /* BNAM */ + 2 /* 2 bytes for length */
        + unknownBNAM.length() + 1 /* length of string +1 byte for NUL-termination */;
  }
  return writeSize;
}

bool AnimatedObjectRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cANIO), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(modelPath.c_str(), subLength);

  // write MODT
  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of ACTI!\n";
      return false;
    }
  }

  if (!unknownBNAM.empty())
  {
    // write BNAM
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    subLength = unknownBNAM.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(unknownBNAM.c_str(), subLength);
  }

  return output.good();
}
#endif

bool AnimatedObjectRecord::loadFromStream(std::istream& in_File,
                                          [[maybe_unused]] const bool localized,
                                          [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint32_t bytesRead = 0;

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownBNAM.clear();
  while (bytesRead < readSize)
  {
    // read next record
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: ANIO seems to have more than one MODL sub record.\n";
             return false;
           }
           // read model path
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: ANIO seems to have empty MODL sub record.\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: ANIO seems to have more than one MODT sub record.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead += (2 + unknownMODT.size());
           break;
      case cBNAM:
           if (!unknownBNAM.empty())
           {
             std::cerr << "Error: ANIO seems to have more than one BNAM sub record.\n";
             return false;
           }
           // read BNAM
           if (!loadString512FromStream(in_File, unknownBNAM, buffer, cBNAM, false, bytesRead))
             return false;
           if (unknownBNAM.empty())
           {
             std::cerr << "Error: ANIO seems to have empty BNAM sub record.\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Found unexpected sub record \"" << IntTo4Char(subRecName)
                     << "\", but only MODL, MODT or BNAM are allowed here!\n";
           return false;
    }
  }

  // presence checks
  if (modelPath.empty())
  {
    std::cerr << "Error: At least one required sub record of ANIO was not found!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AnimatedObjectRecord::getRecordType() const
{
  return cANIO;
}

} // namespace
