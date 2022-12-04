/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "NAVIRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

NAVIRecord::NAVIRecord()
: BasicRecord(),
  unknownNVER(0),
  unknownNVMIs(std::vector<BinarySubRecord>()),
  unknownNVPP(BinarySubRecord()),
  unknownNVSI(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool NAVIRecord::equals(const NAVIRecord& other) const
{
  return equalsBasic(other) && (unknownNVER == other.unknownNVER)
      && (unknownNVMIs == other.unknownNVMIs) && (unknownNVPP == other.unknownNVPP)
      && (unknownNVSI == other.unknownNVSI);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t NAVIRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* NVER */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  for (const auto& nvmi: unknownNVMIs)
  {
    writeSize = writeSize + 4 /* NVMI */ + 2 /* 2 bytes for length */ + nvmi.size() /* size */;
  }
  if (unknownNVPP.isPresent())
  {
    writeSize = writeSize + 4 /* NVPP */ + 2 /* 2 bytes for length */ + unknownNVPP.size() /* size */;
  }
  if (unknownNVSI != 0)
  {
    writeSize = writeSize + 4 /* NVSI */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool NAVIRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cNAVI), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write NVER
  output.write(reinterpret_cast<const char*>(&cNVER), 4);
  uint16_t subLength = 4; //fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownNVER), 4);

  // write NVMIs
  for (const auto& nvmi: unknownNVMIs)
  {

    if (!nvmi.saveToStream(output, cNVMI))
    {
      std::cerr << "Error while writing sub record NVMI of NAVI!\n";
      return false;
    }
  }

  if (unknownNVPP.isPresent())
  {
    if (!unknownNVPP.saveToStream(output, cNVPP))
    {
      std::cerr << "Error while writing subrecord NVPP of NAVI!\n";
      return false;
    }
  }

  if (unknownNVSI != 0)
  {
    // write NVSI
    output.write(reinterpret_cast<const char*>(&cNVSI), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownNVSI), 4);
  }

  return output.good();
}
#endif

bool NAVIRecord::loadFromStream(std::istream& in_File,
                                [[maybe_unused]] const bool localized,
                                [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;

  // read NVER
  if (!loadUint32SubRecordFromStream(in_File, cNVER, unknownNVER, true))
    return false;
  uint32_t bytesRead = 10;

  unknownNVMIs.clear();
  unknownNVPP.setPresence(false);
  BinarySubRecord tempNVMI;
  unknownNVSI = 0;
  while (bytesRead < readSize)
  {
    // read next sub record header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cNVMI:
           if (!tempNVMI.loadFromStream(in_File, cNVMI, false))
           {
             std::cerr << "Error while reading sub record NVMI of NAVI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempNVMI.size();
           unknownNVMIs.emplace_back(tempNVMI);
           break;
      case cNVPP:
           if (unknownNVPP.isPresent())
           {
             std::cerr << "Error: NAVI seems to have more than one NVPP sub record!\n";
             return false;
           }
           // read binary subrecord
           if (!unknownNVPP.loadFromStream(in_File, cNVPP, false))
           {
             std::cerr << "Error while reading sub record NVPP of NAVI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownNVPP.size();
           break;
      case cNVSI:
           if (unknownNVSI != 0)
           {
             std::cerr << "Error: NAVI seems to have more than one NVSI sub record!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNVSI, unknownNVSI, false))
             return false;
           bytesRead += 6;
           if (unknownNVSI == 0)
           {
             std::cerr << "Error: Sub record NVSI of NAVI is zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Found unexpected sub record \""
                     << IntTo4Char(subRecName)
                     << "\", but only NVMI, NVPP or NVSI are allowed here!\n";
           return false;
    }
  }

  return in_File.good();
}

uint32_t NAVIRecord::getRecordType() const
{
  return cNAVI;
}

} // namespace
