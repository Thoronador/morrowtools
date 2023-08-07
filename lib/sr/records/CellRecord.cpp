/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Dirk Stolle

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

#include "CellRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"
#include "../../base/CompressionFunctions.hpp"
#include "../../base/BufferStream.hpp"

namespace SRTP
{

CellRecord::SubrecordXCLC::SubrecordXCLC()
: presence(false),
  locationX(0),
  locationY(0),
  unknownThird(0)
{
}

bool CellRecord::SubrecordXCLC::operator==(const CellRecord::SubrecordXCLC& other) const
{
  if (presence != other.presence)
    return false;
  if (presence)
  {
    return (locationX == other.locationX) && (locationY == other.locationY)
        && (unknownThird == other.unknownThird);
  }
  return true;
}

CellRecord::CellRecord()
: BasicRecord(),
  editorID(""),
  name(LocalizedString()),
  unknownDATA(BinarySubRecord()),
  gridLocation(SubrecordXCLC()),
  unknownTVDT(BinarySubRecord()),
  unknownMHDT(BinarySubRecord()),
  unknownXCLL(BinarySubRecord()),
  lightingTemplateFormID(0),
  unknownLNAM(std::nullopt),
  unknownXCLW(0.0f),
  unknownXCLR(std::vector<uint32_t>()),
  unknownXNAM(std::nullopt),
  unknownXWCN(std::nullopt),
  unknownXWCS(std::nullopt),
  unknownXWCU(BinarySubRecord()),
  imageSpaceFormID(0),
  locationFormID(0),
  encounterZoneFormID(0),
  unknownXCWT(std::nullopt),
  ownerFactionFormID(0),
  lockListFormID(0),
  musicTypeFormID(0),
  regionFormID(0),
  environmentMap(""),
  defaultAcousticSpaceFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool CellRecord::equals(const CellRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name)
      && (unknownDATA == other.unknownDATA) && (gridLocation == other.gridLocation)
      && (unknownTVDT == other.unknownTVDT) && (unknownMHDT == other.unknownMHDT)
      && (unknownXCLL == other.unknownXCLL) && (lightingTemplateFormID == other.lightingTemplateFormID)
      && (unknownLNAM == other.unknownLNAM)
      && (unknownXCLW == other.unknownXCLW) && (unknownXCLR == other.unknownXCLR)
      && (unknownXNAM == other.unknownXNAM)
      && (unknownXWCN == other.unknownXWCN)
      && (unknownXWCS == other.unknownXWCS)
      && (unknownXWCU == other.unknownXWCU)
      && (imageSpaceFormID == other.imageSpaceFormID)
      && (locationFormID == other.locationFormID)
      && (encounterZoneFormID == other.encounterZoneFormID)
      && (unknownXCWT == other.unknownXCWT)
      && (ownerFactionFormID == other.ownerFactionFormID)
      && (lockListFormID == other.lockListFormID)
      && (musicTypeFormID == other.musicTypeFormID)
      && (regionFormID == other.regionFormID)
      && (environmentMap == other.environmentMap)
      && (defaultAcousticSpaceFormID == other.defaultAcousticSpaceFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CellRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* LTMP */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* XCLW */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize + 4 /* DATA */ + 2 /* 2 bytes for length */ + unknownDATA.size() /* size */;
  }
  if (!editorID.empty())
  {
    writeSize = writeSize + 4 /* EDID */ +2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (gridLocation.presence)
  {
    writeSize = writeSize + 4 /* XCLC */ + 2 /* 2 bytes for length */ + 12 /* fixed size */;
  }
  if (unknownTVDT.isPresent())
  {
    writeSize = writeSize + 4 /* TVDT */ + 2 /* 2 bytes for length */ + unknownTVDT.size() /* size of subrecord */;
  }
  if (unknownMHDT.isPresent())
  {
    writeSize = writeSize + 4 /* MHDT */ + 2 /* 2 bytes for length */ + unknownMHDT.size() /* size of subrecord */;
  }
  if (unknownLNAM.has_value())
  {
    writeSize = writeSize + 4 /* LNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!unknownXCLR.empty())
  {
    writeSize = writeSize + 4 /* XCLR */ + 2 /* 2 bytes for length */ + 4 * unknownXCLR.size() /* 4 bytes per entry */;
  }
  if (unknownXNAM.has_value())
  {
    writeSize = writeSize + 4 /* XNAM */ + 2 /* 2 bytes for length */ + 1 /* fixed size */;
  }
  if (unknownXCLL.isPresent())
  {
    writeSize = writeSize + 4 /* XCLL */ + 2 /* 2 bytes for length */ + unknownXCLL.size() /* size of subrecord */;
  }
  if (unknownXWCN.has_value())
  {
    writeSize = writeSize + 4 /* XWCN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownXWCS.has_value())
  {
    writeSize = writeSize + 4 /* XWCS */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownXWCU.isPresent())
  {
    writeSize = writeSize + 4 /* XWCU */ + 2 /* 2 bytes for length */ + unknownXWCU.size() /* size of subrecord */;
  }
  if (imageSpaceFormID != 0)
  {
    writeSize = writeSize + 4 /* XCIM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (locationFormID != 0)
  {
    writeSize = writeSize + 4 /* XLCN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (encounterZoneFormID != 0)
  {
    writeSize = writeSize + 4 /* XEZN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownXCWT.has_value())
  {
    writeSize = writeSize + 4 /* XCWT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (ownerFactionFormID != 0)
  {
    writeSize = writeSize + 4 /* XOWN */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (lockListFormID != 0)
  {
    writeSize = writeSize + 4 /* XILL */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (musicTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* XCMO */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (regionFormID != 0)
  {
    writeSize = writeSize + 4 /* XCCM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!environmentMap.empty())
  {
    writeSize = writeSize + 4 /* XWEM */ + 2 /* 2 bytes for length */
        + environmentMap.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (defaultAcousticSpaceFormID != 0)
  {
    writeSize = writeSize + 4 /* XCAS */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool CellRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCELL), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  uint16_t subLength;
  if (!editorID.empty())
  {
    // write editor ID (EDID)
    output.write(reinterpret_cast<const char*>(&cEDID), 4);
    subLength = editorID.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(editorID.c_str(), subLength);
  }

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (unknownDATA.isPresent())
  {
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cerr << "Error while writing subrecord DATA of CELL!\n!";
      return false;
    }
  }
  else
  {
    std::cerr << "Error while writing CELL record: no DATA subrecord is present!\n!";
    return false;
  }

  if (gridLocation.presence)
  {
    // write XCLC
    output.write(reinterpret_cast<const char*>(&cXCLC), 4);
    subLength = 12;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write XCLC's content
    output.write(reinterpret_cast<const char*>(&gridLocation.locationX), 4);
    output.write(reinterpret_cast<const char*>(&gridLocation.locationY), 4);
    output.write(reinterpret_cast<const char*>(&gridLocation.unknownThird), 4);
  }

  if (unknownTVDT.isPresent())
  {
    if (!unknownTVDT.saveToStream(output, cTVDT))
    {
      std::cerr << "Error while writing subrecord TVDT of CELL!\n!";
      return false;
    }
  }

  if (unknownMHDT.isPresent())
  {
    if (!unknownMHDT.saveToStream(output, cMHDT))
    {
      std::cerr << "Error while writing subrecord MHDT of CELL!\n!";
      return false;
    }
  }

  if (unknownXCLL.isPresent())
  {
    if (!unknownXCLL.saveToStream(output, cXCLL))
    {
      std::cerr << "Error while writing subrecord XCLL of CELL!\n!";
      return false;
    }
  }

  // write lightning template form ID (LTMP)
  output.write(reinterpret_cast<const char*>(&cLTMP), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&lightingTemplateFormID), 4);

  if (unknownLNAM.has_value())
  {
    // write LNAM
    output.write(reinterpret_cast<const char*>(&cLNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownLNAM.value()), 4);
  }

  // write XCLW
  output.write(reinterpret_cast<const char*>(&cXCLW), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownXCLW), 4);

  if (!unknownXCLR.empty())
  {
    // write XCLR
    output.write(reinterpret_cast<const char*>(&cXCLR), 4);
    subLength = 4 * unknownXCLR.size();
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write XCLR
    for (const uint32_t value: unknownXCLR)
    {
      output.write(reinterpret_cast<const char*>(&value), 4);
    }
  }

  if (unknownXNAM.has_value())
  {
    // write XNAM
    output.write(reinterpret_cast<const char*>(&cXNAM), 4);
    subLength = 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownXNAM.value()), 1);
  }

  if (unknownXWCN.has_value())
  {
    // write XWCN
    output.write(reinterpret_cast<const char*>(&cXWCN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownXWCN.value()), 4);
  }

  if (unknownXWCS.has_value())
  {
    // write XWCS
    output.write(reinterpret_cast<const char*>(&cXWCS), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownXWCS.value()), 4);
  }

  if (unknownXWCU.isPresent())
  {
    if (!unknownXWCU.saveToStream(output, cXWCU))
    {
      std::cerr << "Error while writing subrecord XWCU of CELL!\n!";
      return false;
    }
  }

  if (imageSpaceFormID != 0)
  {
    // write image space form ID (XCIM)
    output.write(reinterpret_cast<const char*>(&cXCIM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&imageSpaceFormID), 4);
  }

  if (locationFormID != 0)
  {
    //write location form ID (XLCN)
    output.write(reinterpret_cast<const char*>(&cXLCN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&locationFormID), 4);
  }

  if (encounterZoneFormID != 0)
  {
    // write encounter zone form ID (XEZN)
    output.write(reinterpret_cast<const char*>(&cXEZN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&encounterZoneFormID), 4);
  }

  if (unknownXCWT.has_value())
  {
    // write XCWT
    output.write(reinterpret_cast<const char*>(&cXCWT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownXCWT.value()), 4);
  }

  if (ownerFactionFormID != 0)
  {
    // write owner faction form ID (XOWN)
    output.write(reinterpret_cast<const char*>(&cXOWN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&ownerFactionFormID), 4);
  }

  if (lockListFormID != 0)
  {
    // write lock list form ID (XILL)
    output.write(reinterpret_cast<const char*>(&cXILL), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&lockListFormID), 4);
  }

  if (musicTypeFormID != 0)
  {
    // write music type form ID (XCMO)
    output.write(reinterpret_cast<const char*>(&cXCMO), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&musicTypeFormID), 4);
  }

  if (regionFormID != 0)
  {
    // write region form ID (XCCM)
    output.write(reinterpret_cast<const char*>(&cXCCM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&regionFormID), 4);
  }

  if (!environmentMap.empty())
  {
    // write environment map path (XWEM)
    output.write(reinterpret_cast<const char*>(&cXWEM), 4);
    subLength = environmentMap.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(environmentMap.c_str(), subLength);
  }

  if (defaultAcousticSpaceFormID != 0)
  {
    // write acoustic space form ID (XCAS)
    output.write(reinterpret_cast<const char*>(&cXCAS), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&defaultAcousticSpaceFormID), 4);
  }

  return output.good();
}
#endif

bool CellRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  MWTP::BufferStream decompStream(nullptr, 0);
  std::basic_istream<char>* actual_in = &in_File;

  if (isCompressed())
  {
    uint32_t decompressedSize = 0;
    // read size of decompressed data
    in_File.read(reinterpret_cast<char*>(&decompressedSize), 4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading decompression size of CELL!\n";
      return false;
    }
    if (readSize <= 4)
    {
      std::cerr << "Error: Size of compressed CELL record is too small to contain any compressed data!\n";
      return false;
    }
    // buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize - 4];
    in_File.read(reinterpret_cast<char*>(stream_buffer), readSize - 4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading compressed data of CELL!\n";
      delete[] stream_buffer;
      return false;
    }
    // now decompress here
    uint8_t * decompBuffer = new uint8_t [decompressedSize];
    if (!MWTP::zlibDecompress(stream_buffer, readSize - 4, decompBuffer, decompressedSize))
    {
      std::cerr << "Error while executing decompression function!\n";
      delete[] stream_buffer;
      stream_buffer = NULL;
      delete[] decompBuffer;
      decompBuffer = NULL;
      return false;
    }
    delete[] stream_buffer;
    stream_buffer = NULL;

    // set underlying buffer for decompressed stream
    decompStream.buffer(reinterpret_cast<char*>(decompBuffer), decompressedSize);
    actual_in = &decompStream;
    // update read size
    readSize = decompressedSize;
  }

  editorID.clear();
  char buffer[512];
  name.reset();
  unknownDATA.setPresence(false);
  gridLocation.presence = false;
  unknownTVDT.setPresence(false);
  unknownMHDT.setPresence(false);
  unknownXCLL.setPresence(false);
  bool hasReadLTMP = false;
  unknownLNAM.reset();
  bool hasReadXCLW = false;
  unknownXCLR.clear();
  uint32_t tempUint32, count;
  unknownXNAM.reset();
  locationFormID = 0;
  unknownXWCN.reset();
  unknownXWCS.reset();
  unknownXWCU.setPresence(false);
  imageSpaceFormID = 0;
  encounterZoneFormID = 0;
  unknownXCWT.reset();
  ownerFactionFormID = 0;
  lockListFormID = 0;
  musicTypeFormID = 0;
  regionFormID = 0;
  environmentMap.clear();
  defaultAcousticSpaceFormID = 0;
  while (bytesRead < readSize)
  {
    // read next header
    actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cerr << "Error: CELL seems to have more than one EDID subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(*actual_in, editorID, buffer, cEDID, false, bytesRead))
             return false;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(*actual_in, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one DATA subrecord.\n";
             return false;
           }
           if (!unknownDATA.loadFromStream(*actual_in, cDATA, false))
           {
             std::cerr << "Error while reading subrecord DATA of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownDATA.size();
           // check DATA's length
           subLength = unknownDATA.size();
           if ((subLength != 2) && (subLength != 1))
           {
             std::cerr << "Error: Sub record DATA of CELL has invalid length ("
                       << subLength << " bytes). Should be one or two bytes.\n";
             return false;
           }
           break;
      case cXCLC:
           if (gridLocation.presence)
           {
             std::cerr << "Error: CELL seems to have more than one XCLC subrecord.\n";
             return false;
           }
           // XCLC's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record XCLC of CELL has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read XCLC
           actual_in->read(reinterpret_cast<char*>(&gridLocation.locationX), 4);
           actual_in->read(reinterpret_cast<char*>(&gridLocation.locationY), 4);
           actual_in->read(reinterpret_cast<char*>(&gridLocation.unknownThird), 4);
           bytesRead += 12;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord XCLC of CELL!\n";
             return false;
           }
           gridLocation.presence = true;
           break;
      case cTVDT:
           if (unknownTVDT.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one TVDT subrecord.\n";
             return false;
           }
           if (!unknownTVDT.loadFromStream(*actual_in, cTVDT, false))
           {
             std::cerr << "Error while reading subrecord TVDT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownTVDT.size();
           // check size
           if (unknownTVDT.size() != 684)
           {
             std::cerr << "Error: Sub record TVDT of CELL has invalid length ("
                       << unknownTVDT.size() << " bytes). Should be 684 bytes.\n";
             return false;
           }
           break;
      case cMHDT:
           if (unknownMHDT.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one MHDT subrecord.\n";
             return false;
           }
           if (!unknownMHDT.loadFromStream(*actual_in, cMHDT, false))
           {
             std::cerr << "Error while reading subrecord MHDT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMHDT.size();
           // check size
           if (unknownMHDT.size() != 1028)
           {
             std::cerr << "Error: Sub record MHDT of CELL has invalid length ("
                       << unknownMHDT.size() << " bytes). Should be 1028 bytes.\n";
             return false;
           }
           break;
      case cXCLL:
           if (unknownXCLL.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one XCLL subrecord.\n";
             return false;
           }
           if (!unknownXCLL.loadFromStream(*actual_in, cXCLL, false))
           {
             std::cerr << "Error while reading subrecord XCLL of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownXCLL.size();
           break;
      case cLTMP:
           if (hasReadLTMP)
           {
             std::cerr << "Error: CELL seems to have more than one LTMP subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cLTMP, lightingTemplateFormID, false))
             return false;
           bytesRead += 6;
           hasReadLTMP = true;
           break;
      case cLNAM:
           if (unknownLNAM.has_value())
           {
             std::cerr << "Error: CELL seems to have more than one LNAM subrecord.\n";
             return false;
           }
           unknownLNAM = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cLNAM, unknownLNAM.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cXCLW:
           if (hasReadXCLW)
           {
             std::cerr << "Error: CELL seems to have more than one XCLW subrecord.\n";
             return false;
           }
           // XCLW's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: Sub record XCLW of CELL has invalid length ("
                       << subLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read XCLW
           actual_in->read(reinterpret_cast<char*>(&unknownXCLW), 4);
           bytesRead += 4;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord XCLW of CELL!\n";
             return false;
           }
           hasReadXCLW = true;
           break;
      case cXCLR:
           if (!unknownXCLR.empty())
           {
             std::cerr << "Error: CELL seems to have more than one XCLR subrecord.\n";
             return false;
           }
           // XCLR's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if ((subLength == 0) || ((subLength % 4) != 0))
           {
             std::cerr << "Error: Sub record XCLR of CELL has invalid length ("
                       << subLength
                       << " bytes). Should be an integral, non-zero multiple of four bytes.\n";
             return false;
           }
           // read XCLR
           count = subLength / 4;
           for (uint32_t i = 0; i < count; ++i)
           {
             actual_in->read(reinterpret_cast<char*>(&tempUint32), 4);
             bytesRead += 4;
             if (!actual_in->good())
             {
               std::cerr << "Error while reading subrecord XCLR of CELL!\n";
               return false;
             }
             unknownXCLR.push_back(tempUint32);
           }
           break;
      case cXNAM:
           if (unknownXNAM.has_value())
           {
             std::cerr << "Error: CELL seems to have more than one XNAM subrecord.\n";
             return false;
           }
           // read XNAM's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record XNAM of CELL has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read XNAM
           unknownXNAM = 0;
           actual_in->read(reinterpret_cast<char*>(&unknownXNAM.value()), 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord XNAM of CELL!\n";
             return false;
           }
           break;
      case cXWCN:
           if (unknownXWCN.has_value())
           {
             std::cerr << "Error: CELL seems to have more than one XWCN subrecord.\n";
             return false;
           }
           unknownXWCN = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCN, unknownXWCN.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cXWCS:
           if (unknownXWCS.has_value())
           {
             std::cerr << "Error: CELL seems to have more than one XWCS subrecord.\n";
             return false;
           }
           unknownXWCS = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCS, unknownXWCS.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cXWCU:
           if (unknownXWCU.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one XWCU subrecord.\n";
             return false;
           }
           if (!unknownXWCU.loadFromStream(*actual_in, cXWCU, false))
           {
             std::cerr << "Error while reading subrecord XWCUT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownXWCU.size();
           // check size
           subLength = unknownXWCU.size();
           if ((subLength == 0) || ((subLength % 16) != 0))
           {
             std::cerr << "Error: Sub record XWCU of CELL has invalid length ("
                       << unknownXWCU.size()
                       << " bytes). Should be an integral multiple of 16 bytes.\n";
             return false;
           }
           break;
      case cXCIM:
           if (imageSpaceFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCIM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXCIM, imageSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (imageSpaceFormID == 0)
           {
             std::cerr << "Error: Subrecord XCIM of CELL has value zero!\n";
             return false;
           }
           break;
      case cXLCN:
           if (locationFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XLCN subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXLCN, locationFormID, false))
             return false;
           bytesRead += 6;
           if (locationFormID == 0)
           {
             std::cerr << "Error: Subrecord XLCN of CELL has value zero!\n";
             return false;
           }
           break;
      case cXEZN:
           if (encounterZoneFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XEZN subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXEZN, encounterZoneFormID, false))
             return false;
           bytesRead += 6;
           if (encounterZoneFormID == 0)
           {
             std::cerr << "Error: Subrecord XEZN of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCWT:
           if (unknownXCWT.has_value())
           {
             std::cerr << "Error: CELL seems to have more than one XCWT subrecord.\n";
             return false;
           }
           unknownXCWT = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cXCWT, unknownXCWT.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cXOWN:
           if (ownerFactionFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XOWN subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXOWN, ownerFactionFormID, false))
             return false;
           bytesRead += 6;
           if (ownerFactionFormID == 0)
           {
             std::cerr << "Error: Subrecord XOWN of CELL has value zero!\n";
             return false;
           }
           break;
      case cXILL:
           if (lockListFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XILL subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXILL, lockListFormID, false))
             return false;
           bytesRead += 6;
           if (lockListFormID == 0)
           {
             std::cerr << "Error: Subrecord XILL of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCMO:
           if (musicTypeFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCMO subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXCMO, musicTypeFormID, false))
             return false;
           bytesRead += 6;
           if (musicTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord XCMO of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCCM:
           if (regionFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCCM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXCCM, regionFormID, false))
             return false;
           bytesRead += 6;
           if (regionFormID == 0)
           {
             std::cerr << "Error: Subrecord XCCM of CELL has value zero!\n";
             return false;
           }
           break;
      case cXWEM:
           if (!environmentMap.empty())
           {
             std::cerr << "Error: CELL seems to have more than one XWEM subrecord.\n";
             return false;
           }
           // read XWEM
           if (!loadString512FromStream(*actual_in, environmentMap, buffer, cXWEM, false, bytesRead))
             return false;
           break;
      case cXCAS:
           if (defaultAcousticSpaceFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCAS subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXCAS, defaultAcousticSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (defaultAcousticSpaceFormID == 0)
           {
             std::cerr << "Error: Subrecord XCAS of CELL has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord \"" << IntTo4Char(subRecName)
                     << "\", but only EDID, FULL, DATA, TVDT, MHDT, XCLC, XCLL,"
                     << " LTMP, LNAM, XCLW, XCLR, XNAM, XLCN, XWCN, XWCS, XWCU,"
                     << " XCIM, XEZN, XCWT, XCMO, XOWN, XILL, XCCM, XWEM or XCAS are allowed here!\n";
           std::cerr << "Local stream position is "<< actual_in->tellg()
                     << " bytes. Bytes read so far: " << bytesRead << ".\n";
           return false;
    }
  }

  // presence checks
  if (!unknownDATA.isPresent() || !hasReadLTMP || !hasReadXCLW)
  {
    std::cerr << "Error: At least one required subrecord of CELL was not found!\n"
              << "DATA: " << unknownDATA.isPresent() << "    LTMP: "
              << hasReadLTMP << "    XCLW: " << hasReadXCLW << "\n";
    return false;
  }

  return actual_in->good();
}

uint32_t CellRecord::getRecordType() const
{
  return cCELL;
}

} // namespace
