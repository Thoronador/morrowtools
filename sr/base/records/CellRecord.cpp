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

#include "CellRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"
#include "../../../base/CompressionFunctions.hpp"
#include "../../../base/BufferStream.hpp"

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
  hasLNAM(false), unknownLNAM(0),
  unknownXCLW(0.0f),
  unknownXCLR(std::vector<uint32_t>()),
  hasXNAM(false), unknownXNAM(0),
  hasXWCN(false), unknownXWCN(0),
  hasXWCS(false), unknownXWCS(0),
  unknownXWCU(BinarySubRecord()),
  imageSpaceFormID(0),
  locationFormID(0),
  encounterZoneFormID(0),
  hasXCWT(0), unknownXCWT(0),
  musicTypeFormID(0),
  unknownXWEM(""),
  ownerFactionFormID(0),
  lockListFormID(0),
  regionFormID(0),
  defaultAcousticSpaceFormID(0)
{
}

CellRecord::~CellRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CellRecord::equals(const CellRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (name == other.name)
      && (unknownDATA == other.unknownDATA) && (gridLocation == other.gridLocation)
      && (unknownTVDT == other.unknownTVDT) && (unknownMHDT == other.unknownMHDT)
      && (unknownXCLL == other.unknownXCLL) && (lightingTemplateFormID == other.lightingTemplateFormID)
      && (hasLNAM == other.hasLNAM) && ((unknownLNAM == other.unknownLNAM) || !hasLNAM)
      && (unknownXCLW == other.unknownXCLW) && (unknownXCLR == other.unknownXCLR)
      && (hasXNAM == other.hasXNAM) && ((unknownXNAM == other.unknownXNAM) || !hasXNAM)
      && (hasXWCN == other.hasXWCN) && ((unknownXWCN == other.unknownXWCN) || !hasXWCN)
      && (hasXWCS == other.hasXWCS) && ((unknownXWCS == other.unknownXWCS) || !hasXWCS)
      && (unknownXWCU == other.unknownXWCU)
      && (imageSpaceFormID == other.imageSpaceFormID)
      && (locationFormID == other.locationFormID)
      && (encounterZoneFormID == other.encounterZoneFormID)
      && (hasXCWT == other.hasXCWT) && ((unknownXCWT == other.unknownXCWT) || (!hasXCWT))
      && (musicTypeFormID == other.musicTypeFormID)
      && (unknownXWEM == other.unknownXWEM)
      && (ownerFactionFormID == other.ownerFactionFormID)
      && (lockListFormID == other.lockListFormID)
      && (regionFormID == other.regionFormID)
      && (defaultAcousticSpaceFormID == other.defaultAcousticSpaceFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CellRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize;
  writeSize = 4 /* LTMP */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* XCLW */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize + 4 /* DATA */ + 2 /* 2 bytes for length */ + unknownDATA.size() /* size */;
  }
  if (!editorID.empty())
  {
    writeSize = writeSize + 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (gridLocation.presence)
  {
    writeSize = writeSize +4 /* XCLC */ +2 /* 2 bytes for length */ +12 /* fixed size */;
  }
  if (unknownTVDT.isPresent())
  {
    writeSize = writeSize + 4 /* TVDT */ + 2 /* 2 bytes for length */ + unknownTVDT.size() /* size of subrecord */;
  }
  if (unknownMHDT.isPresent())
  {
    writeSize = writeSize + 4 /* MHDT */ + 2 /* 2 bytes for length */ + unknownMHDT.size() /* size of subrecord */;
  }
  if (hasLNAM)
  {
    writeSize = writeSize +4 /* LNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!unknownXCLR.empty())
  {
    writeSize = writeSize +4 /* XCLR */ +2 /* 2 bytes for length */ +4*unknownXCLR.size() /* 4 bytes per entry */;
  }
  if (hasXNAM)
  {
    writeSize = writeSize +4 /* XNAM */ +2 /* 2 bytes for length */ +1 /* fixed size */;
  }
  if (unknownXCLL.isPresent())
  {
    writeSize = writeSize + 4 /* XCLL */ + 2 /* 2 bytes for length */ + unknownXCLL.size() /* size of subrecord */;
  }
  if (hasXWCN)
  {
    writeSize = writeSize +4 /* XWCN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXWCS)
  {
    writeSize = writeSize +4 /* XWCS */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownXWCU.isPresent())
  {
    writeSize = writeSize + 4 /* XWCU */ + 2 /* 2 bytes for length */ + unknownXWCU.size() /* size of subrecord */;
  }
  if (imageSpaceFormID!=0)
  {
    writeSize = writeSize +4 /* XCIM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (locationFormID!=0)
  {
    writeSize = writeSize +4 /* XLCN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (encounterZoneFormID!=0)
  {
    writeSize = writeSize +4 /* XEZN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXCWT)
  {
    writeSize = writeSize +4 /* XCWT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (musicTypeFormID!=0)
  {
    writeSize = writeSize +4 /* XCMO */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!unknownXWEM.empty())
  {
    writeSize = writeSize + 4 /* XWEM */ +2 /* 2 bytes for length */
        +unknownXWEM.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (ownerFactionFormID!=0)
  {
    writeSize = writeSize +4 /* XOWN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (lockListFormID!=0)
  {
    writeSize = writeSize +4 /* XILL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (regionFormID!=0)
  {
    writeSize = writeSize +4 /* XCCM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (defaultAcousticSpaceFormID!=0)
  {
    writeSize = writeSize +4 /* XCAS */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool CellRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCELL, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  uint16_t subLength;
  if (!editorID.empty())
  {
    //write EDID
    output.write((const char*) &cEDID, 4);
    //EDID's length
    subLength = editorID.length()+1;
    output.write((const char*) &subLength, 2);
    //write editor ID
    output.write(editorID.c_str(), subLength);
  }

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (unknownDATA.isPresent())
  {

    //write DATA
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
    output.write((const char*) &cXCLC, 4);
    // XCLC's length
    subLength = 12; // fixed size
    output.write((const char*) &subLength, 2);
    // write XCLC's content
    output.write((const char*) &(gridLocation.locationX), 4);
    output.write((const char*) &(gridLocation.locationY), 4);
    output.write((const char*) &(gridLocation.unknownThird), 4);
  }

  if (unknownTVDT.isPresent())
  {
    //write TVDT
    if (!unknownTVDT.saveToStream(output, cTVDT))
    {
      std::cerr << "Error while writing subrecord TVDT of CELL!\n!";
      return false;
    }
  }

  if (unknownMHDT.isPresent())
  {
    //write MHDT
    if (!unknownMHDT.saveToStream(output, cMHDT))
    {
      std::cerr << "Error while writing subrecord MHDT of CELL!\n!";
      return false;
    }
  }

  if (unknownXCLL.isPresent())
  {
    //write XCLL
    if (!unknownXCLL.saveToStream(output, cXCLL))
    {
      std::cerr << "Error while writing subrecord XCLL of CELL!\n!";
      return false;
    }
  }

  //write LTMP
  output.write((const char*) &cLTMP, 4);
  //LTMP's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write LTMP
  output.write((const char*) &lightingTemplateFormID, 4);

  if (hasLNAM)
  {
    //write LNAM
    output.write((const char*) &cLNAM, 4);
    //LNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write LNAM
    output.write((const char*) &unknownLNAM, 4);
  }

  //write XCLW
  output.write((const char*) &cXCLW, 4);
  //XCLW's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write XCLW
  output.write((const char*) &unknownXCLW, 4);

  if (!unknownXCLR.empty())
  {
    //write XCLR
    output.write((const char*) &cXCLR, 4);
    //XCLR's length
    const unsigned int count = unknownXCLR.size();
    subLength = 4*count;
    output.write((const char*) &subLength, 2);
    //write XCLR
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      output.write((const char*) &(unknownXCLR[i]), 4);
    }//for
  }

  if (hasXNAM)
  {
    //write XNAM
    output.write((const char*) &cXNAM, 4);
    //XNAM's length
    subLength = 1; //fixed size
    output.write((const char*) &subLength, 2);
    //write XNAM
    output.write((const char*) &unknownXNAM, 1);
  }

  if (hasXWCN)
  {
    //write XWCN
    output.write((const char*) &cXWCN, 4);
    //XWCN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XWCN
    output.write((const char*) &unknownXWCN, 4);
  }

  if (hasXWCS)
  {
    //write XWCS
    output.write((const char*) &cXWCS, 4);
    //XWCS's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XWCS
    output.write((const char*) &unknownXWCS, 4);
  }

  if (unknownXWCU.isPresent())
  {
    //write XWCU
    if (!unknownXWCU.saveToStream(output, cXWCU))
    {
      std::cerr << "Error while writing subrecord XWCU of CELL!\n!";
      return false;
    }
  }

  if (imageSpaceFormID!=0)
  {
    //write XCIM
    output.write((const char*) &cXCIM, 4);
    //XCIM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XCIM
    output.write((const char*) &imageSpaceFormID, 4);
  }

  if (locationFormID!=0)
  {
    //write XLCN
    output.write((const char*) &cXLCN, 4);
    //XLCN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XLCN
    output.write((const char*) &locationFormID, 4);
  }

  if (encounterZoneFormID!=0)
  {
    //write XEZN
    output.write((const char*) &cXEZN, 4);
    //XEZN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XEZN
    output.write((const char*) &encounterZoneFormID, 4);
  }

  if (hasXCWT)
  {
    //write XCWT
    output.write((const char*) &cXCWT, 4);
    //XCWT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XCWT
    output.write((const char*) &unknownXCWT, 4);
  }

  if (musicTypeFormID!=0)
  {
    //write XCMO
    output.write((const char*) &cXCMO, 4);
    //XCMO's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XCMO
    output.write((const char*) &musicTypeFormID, 4);
  }

  if (!unknownXWEM.empty())
  {
    //write XWEM
    output.write((const char*) &cXWEM, 4);
    //XWEM's length
    subLength = unknownXWEM.length()+1;
    output.write((const char*) &subLength, 2);
    //write XWEM path
    output.write(unknownXWEM.c_str(), subLength);
  }

  if (ownerFactionFormID!=0)
  {
    //write XOWN
    output.write((const char*) &cXOWN, 4);
    //XOWN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XOWN
    output.write((const char*) &ownerFactionFormID, 4);
  }

  if (lockListFormID!=0)
  {
    //write XILL
    output.write((const char*) &cXILL, 4);
    //XILL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XILL
    output.write((const char*) &lockListFormID, 4);
  }

  if (regionFormID!=0)
  {
    //write XCCM
    output.write((const char*) &cXCCM, 4);
    //XCCM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XCCM
    output.write((const char*) &regionFormID, 4);
  }

  if (defaultAcousticSpaceFormID!=0)
  {
    //write XCAS
    output.write((const char*) &cXCAS, 4);
    //XCAS's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XCAS
    output.write((const char*) &defaultAcousticSpaceFormID, 4);
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
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  MWTP::BufferStream decompStream(nullptr, 0);
  std::basic_istream<char>* actual_in = &in_File;

  if (isCompressed())
  {
    uint32_t decompressedSize = 0;
    //read size of decompressed data
    in_File.read((char*) &decompressedSize, 4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading decompression size of CELL!\n";
      return false;
    }
    if (readSize<=4)
    {
      std::cerr << "Error: size of compressed CELL record is too small to contain any compressed data!\n";
      return false;
    }
    //buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize-4];
    in_File.read((char*) stream_buffer, readSize-4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading compressed data of CELL!\n";
      delete[] stream_buffer;
      return false;
    }
    //now decompress here
    uint8_t * decompBuffer = new uint8_t [decompressedSize];
    if (!MWTP::zlibDecompress(stream_buffer, readSize-4, decompBuffer, decompressedSize))
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

    //set underlying buffer for decompressed stream
    decompStream.buffer((char*) decompBuffer, decompressedSize);
    actual_in = &decompStream;
    //update read size
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
  hasLNAM = false; unknownLNAM = 0;
  bool hasReadXCLW = false;
  unknownXCLR.clear();
  uint32_t tempUint32, count;
  hasXNAM = false; unknownXNAM = 0;
  locationFormID = 0;
  hasXWCN = false; unknownXWCN = 0;
  hasXWCS = false; unknownXWCS = 0;
  unknownXWCU.setPresence(false);
  imageSpaceFormID = 0;
  encounterZoneFormID = 0;
  hasXCWT = false; unknownXCWT = 0;
  musicTypeFormID = 0;
  unknownXWEM.clear();
  ownerFactionFormID = 0;
  lockListFormID = 0;
  regionFormID = 0;
  defaultAcousticSpaceFormID = 0;
  while (bytesRead < readSize)
  {
    // read next header
    actual_in->read((char*) &subRecName, 4);
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
           //read FULL
           if (!name.loadFromStream(*actual_in, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cerr << "Error: CELL seems to have more than one DATA subrecord.\n";
             return false;
           }
           //read DATA
           if (!unknownDATA.loadFromStream(*actual_in, cDATA, false))
           {
             std::cerr << "Error while reading subrecord DATA of CELL!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownDATA.size();
           // DATA's length
           subLength = unknownDATA.size();
           if ((subLength != 2) && (subLength != 1))
           {
             std::cerr << "Error: sub record DATA of CELL has invalid length ("
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
             std::cerr << "Error: sub record XCLC of CELL has invalid length ("
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
             std::cerr << "Error: sub record TVDT of CELL has invalid length ("
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
             std::cerr << "Error: sub record MHDT of CELL has invalid length ("
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
           if (hasLNAM)
           {
             std::cerr << "Error: CELL seems to have more than one LNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cLNAM, unknownLNAM, false))
             return false;
           bytesRead += 6;
           hasLNAM = true;
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
             std::cerr << "Error: sub record XCLW of CELL has invalid length ("
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
             std::cerr << "Error: sub record XCLR of CELL has invalid length ("
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
           if (hasXNAM)
           {
             std::cerr << "Error: CELL seems to have more than one XNAM subrecord.\n";
             return false;
           }
           //XNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: sub record XNAM of CELL has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read XNAM
           actual_in->read((char*) &unknownXNAM, 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord XNAM of CELL!\n";
             return false;
           }
           hasXNAM = true;
           break;
      case cXWCN:
           if (hasXWCN)
           {
             std::cerr << "Error: CELL seems to have more than one XWCN subrecord.\n";
             return false;
           }
           //read XWCN
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCN, unknownXWCN, false))
             return false;
           bytesRead += 6;
           hasXWCN = true;
           break;
      case cXWCS:
           if (hasXWCS)
           {
             std::cerr << "Error: CELL seems to have more than one XWCS subrecord.\n";
             return false;
           }
           //read XWCS
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCS, unknownXWCS, false))
             return false;
           bytesRead += 6;
           hasXWCS = true;
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
             std::cerr << "Error: sub record XWCU of CELL has invalid length ("
                       << unknownXWCU.size() << " bytes). Should be an integral multiple of 16 bytes.\n";
             return false;
           }
           break;
      case cXCIM:
           if (imageSpaceFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCIM subrecord.\n";
             return false;
           }
           //read XCIM
           if (!loadUint32SubRecordFromStream(*actual_in, cXCIM, imageSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (imageSpaceFormID == 0)
           {
             std::cerr << "Error: subrecord XCIM of CELL has value zero!\n";
             return false;
           }
           break;
      case cXLCN:
           if (locationFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XLCN subrecord.\n";
             return false;
           }
           //read XLCN
           if (!loadUint32SubRecordFromStream(*actual_in, cXLCN, locationFormID, false))
             return false;
           bytesRead += 6;
           if (locationFormID == 0)
           {
             std::cerr << "Error: subrecord XLCN of CELL has value zero!\n";
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
             std::cerr << "Error: subrecord XEZN of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCWT:
           if (hasXCWT)
           {
             std::cerr << "Error: CELL seems to have more than one XCWT subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cXCWT, unknownXCWT, false))
             return false;
           bytesRead += 6;
           hasXCWT = true;
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
      case cXWEM:
           if (!unknownXWEM.empty())
           {
             std::cerr << "Error: CELL seems to have more than one XWEM subrecord.\n";
             return false;
           }
           //XWEM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record XWEM of CELL is longer than 511 characters!\n";
             return false;
           }
           //read XWEM's stuff
           memset(buffer, 0, 512);
           actual_in->read(buffer, subLength);
           bytesRead += subLength;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord XWEM of CELL!\n";
             return false;
           }
           unknownXWEM = std::string(buffer);
           break;
      case cXOWN:
           if (ownerFactionFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XOWN subrecord.\n";
             return false;
           }
           //read XOWN
           if (!loadUint32SubRecordFromStream(*actual_in, cXOWN, ownerFactionFormID, false))
             return false;
           bytesRead += 6;
           if (ownerFactionFormID == 0)
           {
             std::cerr << "Error: subrecord XOWN of CELL has value zero!\n";
             return false;
           }
           break;
      case cXILL:
           if (lockListFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XILL subrecord.\n";
             return false;
           }
           //read XILL
           if (!loadUint32SubRecordFromStream(*actual_in, cXILL, lockListFormID, false))
             return false;
           bytesRead += 6;
           if (lockListFormID == 0)
           {
             std::cerr << "Error: subrecord XILL of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCCM:
           if (regionFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCCM subrecord.\n";
             return false;
           }
           //read XCCM
           if (!loadUint32SubRecordFromStream(*actual_in, cXCCM, regionFormID, false))
             return false;
           bytesRead += 6;
           if (regionFormID == 0)
           {
             std::cerr << "Error: subrecord XCCM of CELL has value zero!\n";
             return false;
           }
           break;
      case cXCAS:
           if (defaultAcousticSpaceFormID != 0)
           {
             std::cerr << "Error: CELL seems to have more than one XCAS subrecord.\n";
             return false;
           }
           //read XCAS
           if (!loadUint32SubRecordFromStream(*actual_in, cXCAS, defaultAcousticSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (defaultAcousticSpaceFormID == 0)
           {
             std::cerr << "Error: subrecord XCAS of CELL has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only EDID, FULL, DATA, TVDT, MHDT, XCLC, XCLL,"
                     << " LTMP, LNAM, XCLW, XCLR, XNAM, XLCN, XWCN, XWCS, XWCU,"
                     << " XCIM, XEZN, XCWT, XCMO, XWEM, XOWN, XILL, XCCM or XCAS are allowed here!\n";
           std::cerr << "Local stream position is "<< actual_in->tellg()
                     << " bytes. Bytes read so far: "<<bytesRead<<".\n";
           return false;
    }//swi
  }//while

  // presence checks
  if (!(unknownDATA.isPresent() and hasReadLTMP and hasReadXCLW))
  {
    std::cerr << "Error: At least one required subrecord of CELL was not found!\n"
              << "DATA: "<<unknownDATA.isPresent()<<"    LTMP: "<<hasReadLTMP<<"    XCLW: "<<hasReadXCLW
              << "\n";
    return false;
  }

  return actual_in->good();
}

uint32_t CellRecord::getRecordType() const
{
  return cCELL;
}

} //namespace
