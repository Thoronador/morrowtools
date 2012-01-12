/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "CellRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include "../../../base/CompressionFunctions.h"
#include "../../../base/BufferStream.h"

namespace SRTP
{

bool CellRecord::SubrecordXCLC::operator==(const CellRecord::SubrecordXCLC& other) const
{
  if (presence!=other.presence) return false;
  if (presence)
  {
    return ((locationX==other.locationX) and (locationY==other.locationY)
        and (unknownThird==other.unknownThird));
  }
  return true;
}

CellRecord::CellRecord()
: BasicRecord()
{
  editorID = "";
  hasFULL = false;
  nameStringID = 0;
  unknownDATA.setPresence(false);
  unknownTVDT.setPresence(false);
  unknownMHDT.setPresence(false);
  gridLocation.presence = false;
  unknownXCLL.setPresence(false);
  unknownLTMP = 0;
  hasLNAM = false;
  unknownLNAM = 0;
  unknownXCLW = 0.0f;
  unknownXCLR.clear();
  hasXNAM = false;
  unknownXNAM = 0;
  hasXLCN = false;
  unknownXLCN = 0;
  hasXWCN = false;
  unknownXWCN = 0;
  hasXWCS = false;
  unknownXWCS = 0;
  unknownXWCU.setPresence(false);
  hasXCIM = false;
  unknownXCIM = 0;
  hasXEZN = false;
  encounterZoneFormID = 0; //subrecord XEZN
  hasXCWT = false;
  unknownXCWT = 0;
  hasXCMO = false;
  unknownXCMO = 0;
  unknownXWEM = "";
  hasXOWN = false;
  unknownXOWN = 0;
  hasXILL = false;
  unknownXILL = 0;
  hasXCCM = false;
  unknownXCCM = 0;
  hasXCAS = false;
  unknownXCAS = 0;
}

CellRecord::~CellRecord()
{
  //empty
}

bool CellRecord::equals(const CellRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (unknownDATA==other.unknownDATA) and (unknownTVDT==other.unknownTVDT)
      and (unknownMHDT==other.unknownMHDT) and (gridLocation==other.gridLocation)
      and (unknownXCLL==other.unknownXCLL) and (unknownLTMP==other.unknownLTMP)
      and (hasLNAM==other.hasLNAM) and ((unknownLNAM==other.unknownLNAM) or (!hasLNAM))
      and (unknownXCLW==other.unknownXCLW) and (unknownXCLR==other.unknownXCLR)
      and (hasXNAM==other.hasXNAM) and ((unknownXNAM==other.unknownXNAM) or (!hasXNAM))
      and (hasXLCN==other.hasXLCN) and ((unknownXLCN==other.unknownXLCN) or (!hasXLCN))
      and (hasXWCN==other.hasXWCN) and ((unknownXWCN==other.unknownXWCN) or (!hasXWCN))
      and (hasXWCS==other.hasXWCS) and ((unknownXWCS==other.unknownXWCS) or (!hasXWCS))
      and (unknownXWCU==other.unknownXWCU)
      and (hasXCIM==other.hasXCIM) and ((unknownXCIM==other.unknownXCIM) or (!hasXCIM))
      and (hasXEZN==other.hasXEZN) and ((encounterZoneFormID==other.encounterZoneFormID) or (!hasXEZN))
      and (hasXCWT==other.hasXCWT) and ((unknownXCWT==other.unknownXCWT) or (!hasXCWT))
      and (hasXCMO==other.hasXCMO) and ((unknownXCMO==other.unknownXCMO) or (!hasXCMO))
      and (unknownXWEM==other.unknownXWEM)
      and (hasXOWN==other.hasXOWN) and ((unknownXOWN==other.unknownXOWN) or (!hasXOWN))
      and (hasXILL==other.hasXILL) and ((unknownXILL==other.unknownXILL) or (!hasXILL))
      and (hasXCCM==other.hasXCCM) and ((unknownXCCM==other.unknownXCCM) or (!hasXCCM))
      and (hasXCAS==other.hasXCAS) and ((unknownXCAS==other.unknownXCAS) or (!hasXCAS)));
}

uint32_t CellRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* LTMP */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* XCLW */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +unknownDATA.getSize() /* size */;
  }
  if (!editorID.empty())
  {
    writeSize = writeSize + 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownTVDT.isPresent())
  {
    writeSize = writeSize +4 /* TVDT */ +2 /* 2 bytes for length */ +unknownTVDT.getSize() /* size of subrecord */;
  }
  if (unknownMHDT.isPresent())
  {
    writeSize = writeSize +4 /* MHDT */ +2 /* 2 bytes for length */ +unknownMHDT.getSize() /* size of subrecord */;
  }
  if (gridLocation.presence)
  {
    writeSize = writeSize +4 /* XCLC */ +2 /* 2 bytes for length */ +12 /* fixed size */;
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
    writeSize = writeSize +4 /* XCLL */ +2 /* 2 bytes for length */ +unknownXCLL.getSize() /* size of subrecord */;
  }
  if (hasXLCN)
  {
    writeSize = writeSize +4 /* XLCN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
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
    writeSize = writeSize +4 /* XWCU */ +2 /* 2 bytes for length */ +unknownXWCU.getSize() /* size of subrecord */;
  }
  if (hasXCIM)
  {
    writeSize = writeSize +4 /* XCIM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXEZN)
  {
    writeSize = writeSize +4 /* XEZN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXCWT)
  {
    writeSize = writeSize +4 /* XCWT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXCMO)
  {
    writeSize = writeSize +4 /* XCMO */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!unknownXWEM.empty())
  {
    writeSize = writeSize + 4 /* XWEM */ +2 /* 2 bytes for length */
        +unknownXWEM.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (hasXOWN)
  {
    writeSize = writeSize +4 /* XOWN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXILL)
  {
    writeSize = writeSize +4 /* XILL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXCCM)
  {
    writeSize = writeSize +4 /* XCCM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasXCAS)
  {
    writeSize = writeSize +4 /* XCAS */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool CellRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCELL, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  uint16_t subLength;
  if (!editorID.empty())
  {
    //write EDID
    output.write((char*) &cEDID, 4);
    //EDID's length
    subLength = editorID.length()+1;
    output.write((char*) &subLength, 2);
    //write editor ID
    output.write(editorID.c_str(), subLength);
  }

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }

  if (unknownDATA.isPresent())
  {

    //write DATA
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cout << "Error while writing subrecord DATA of CELL!\n!";
      return false;
    }
  }
  else
  {
    std::cout << "Error while writing CELL record: no DATA subrecord is present!\n!";
    return false;
  }

  if (unknownTVDT.isPresent())
  {
    //write TVDT
    if (!unknownTVDT.saveToStream(output, cTVDT))
    {
      std::cout << "Error while writing subrecord TVDT of CELL!\n!";
      return false;
    }
  }

  if (unknownMHDT.isPresent())
  {
    //write MHDT
    if (!unknownMHDT.saveToStream(output, cMHDT))
    {
      std::cout << "Error while writing subrecord MHDT of CELL!\n!";
      return false;
    }
  }

  if (gridLocation.presence)
  {
    //write XCLC
    output.write((char*) &cXCLC, 4);
    //XCLC's length
    subLength = 12; //fixed size
    output.write((char*) &subLength, 2);
    //write XCLC
    output.write((const char*) &(gridLocation.locationX), 4);
    output.write((const char*) &(gridLocation.locationY), 4);
    output.write((const char*) &(gridLocation.unknownThird), 4);
  }

  if (unknownXCLL.isPresent())
  {
    //write XCLL
    if (!unknownXCLL.saveToStream(output, cXCLL))
    {
      std::cout << "Error while writing subrecord XCLL of CELL!\n!";
      return false;
    }
  }

  //write LTMP
  output.write((char*) &cLTMP, 4);
  //LTMP's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write LTMP
  output.write((const char*) &unknownLTMP, 4);

  if (hasLNAM)
  {
    //write LNAM
    output.write((char*) &cLNAM, 4);
    //LNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write LNAM
    output.write((const char*) &unknownLNAM, 4);
  }

  //write XCLW
  output.write((char*) &cXCLW, 4);
  //XCLW's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write XCLW
  output.write((const char*) &unknownXCLW, 4);

  if (!unknownXCLR.empty())
  {
    //write XCLR
    output.write((char*) &cXCLR, 4);
    //XCLR's length
    const unsigned int count = unknownXCLR.size();
    subLength = 4*count;
    output.write((char*) &subLength, 2);
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
    output.write((char*) &cXNAM, 4);
    //XNAM's length
    subLength = 1; //fixed size
    output.write((char*) &subLength, 2);
    //write XNAM
    output.write((const char*) &unknownXNAM, 1);
  }

  if (hasXLCN)
  {
    //write XLCN
    output.write((char*) &cXLCN, 4);
    //XLCN's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XLCN
    output.write((const char*) &unknownXLCN, 4);
  }

  if (hasXWCN)
  {
    //write XWCN
    output.write((char*) &cXWCN, 4);
    //XWCN's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XWCN
    output.write((const char*) &unknownXWCN, 4);
  }

  if (hasXWCS)
  {
    //write XWCS
    output.write((char*) &cXWCS, 4);
    //XWCS's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XWCS
    output.write((const char*) &unknownXWCS, 4);
  }

  if (unknownXWCU.isPresent())
  {
    //write XWCU
    if (!unknownXWCU.saveToStream(output, cXWCU))
    {
      std::cout << "Error while writing subrecord XWCU of CELL!\n!";
      return false;
    }
  }

  if (hasXCIM)
  {
    //write XCIM
    output.write((char*) &cXCIM, 4);
    //XCIM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XCIM
    output.write((const char*) &unknownXCIM, 4);
  }

  if (hasXEZN)
  {
    //write XEZN
    output.write((char*) &cXEZN, 4);
    //XEZN's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XEZN
    output.write((const char*) &encounterZoneFormID, 4);
  }

  if (hasXCWT)
  {
    //write XCWT
    output.write((char*) &cXCWT, 4);
    //XCWT's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XCWT
    output.write((const char*) &unknownXCWT, 4);
  }

  if (hasXCMO)
  {
    //write XCMO
    output.write((char*) &cXCMO, 4);
    //XCMO's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XCMO
    output.write((const char*) &unknownXCMO, 4);
  }

  if (!unknownXWEM.empty())
  {
    //write XWEM
    output.write((char*) &cXWEM, 4);
    //XWEM's length
    subLength = unknownXWEM.length()+1;
    output.write((char*) &subLength, 2);
    //write XWEM path
    output.write(unknownXWEM.c_str(), subLength);
  }

  if (hasXOWN)
  {
    //write XOWN
    output.write((char*) &cXOWN, 4);
    //XOWN's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XOWN
    output.write((const char*) &unknownXOWN, 4);
  }

  if (hasXILL)
  {
    //write XILL
    output.write((char*) &cXILL, 4);
    //XILL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XILL
    output.write((const char*) &unknownXILL, 4);
  }

  if (hasXCCM)
  {
    //write XCCM
    output.write((char*) &cXCCM, 4);
    //XCCM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XCCM
    output.write((const char*) &unknownXCCM, 4);
  }

  if (hasXCAS)
  {
    //write XCAS
    output.write((char*) &cXCAS, 4);
    //XCAS's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write XCAS
    output.write((const char*) &unknownXCAS, 4);
  }

  return output;
}

bool CellRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  MWTP::BufferStream decompStream(NULL, 0);
  std::basic_istream<char>* actual_in = &in_File;

  if (isCompressed())
  {
    uint32_t decompressedSize = 0;
    //read size of decompressed data
    in_File.read((char*) &decompressedSize, 4);
    if (!in_File.good())
    {
      std::cout << "Error while reading decompression size of CELL!\n";
      return false;
    }
    if (readSize<=4)
    {
      std::cout << "Error: size of compressed CELL record is too small to contain any compressed data!\n";
      return false;
    }
    //buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize-4];
    in_File.read((char*) stream_buffer, readSize-4);
    if (!in_File.good())
    {
      std::cout << "Error while reading compressed data of CELL!\n";
      delete[] stream_buffer;
      return false;
    }
    //now decompress here
    uint8_t * decompBuffer = new uint8_t [decompressedSize];
    if (!MWTP::zlibDecompress(stream_buffer, readSize-4, decompBuffer, decompressedSize))
    {
      std::cout << "Error while executing decompression function!\n";
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
  hasFULL = false; nameStringID = 0;
  unknownDATA.setPresence(false);
  unknownTVDT.setPresence(false);
  unknownMHDT.setPresence(false);
  gridLocation.presence = false;
  unknownXCLL.setPresence(false);
  bool hasReadLTMP = false;
  hasLNAM = false; unknownLNAM = 0;
  bool hasReadXCLW = false;
  unknownXCLR.clear();
  uint32_t tempUint32, i, count;
  hasXNAM = false; unknownXNAM = 0;
  hasXLCN = false; unknownXLCN = 0;
  hasXWCN = false; unknownXWCN = 0;
  hasXWCS = false; unknownXWCS = 0;
  unknownXWCU.setPresence(false);
  hasXCIM = false; unknownXCIM = 0;
  hasXEZN = false; encounterZoneFormID = 0;
  hasXCWT = false; unknownXCWT = 0;
  hasXCMO = false; unknownXCMO = 0;
  unknownXWEM.clear();
  hasXOWN = false; unknownXOWN = 0;
  hasXILL = false; unknownXILL = 0;
  hasXCCM = false; unknownXCCM = 0;
  hasXCAS = false; unknownXCAS = 0;
  while (bytesRead<readSize)
  {
      //read next header
    actual_in->read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cout << "Error: CELL seems to have more than one EDID subrecord.\n";
             return false;
           }
           //EDID's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record EDID of CELL is longer than 511 characters!\n";
             return false;
           }
           //read EDID's stuff
           memset(buffer, 0, 512);
           actual_in->read(buffer, subLength);
           bytesRead += subLength;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord EDID of CELL!\n";
             return false;
           }
           editorID = std::string(buffer);
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: CELL seems to have more than one FULL subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(*actual_in, cFULL, nameStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cout << "Error: CELL seems to have more than one DATA subrecord.\n";
             return false;
           }
           //read DATA
           if (!unknownDATA.loadFromStream(*actual_in, cDATA, false))
           {
             std::cout << "Error while reading subrecord DATA of CELL!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownDATA.getSize();
           //DATA's length
           subLength = unknownDATA.getSize();
           if ((subLength!=2) and (subLength!=1))
           {
             std::cout <<"Error: sub record DATA of CELL has invalid length ("<<subLength
                       <<" bytes). Should be one or two bytes.\n";
             return false;
           }
           break;
      case cTVDT:
           if (unknownTVDT.isPresent())
           {
             std::cout << "Error: CELL seems to have more than one TVDT subrecord.\n";
             return false;
           }
           if (!unknownTVDT.loadFromStream(*actual_in, cTVDT, false))
           {
             std::cout << "Error while reading subrecord TVDT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownTVDT.getSize();
           //check size
           if (unknownTVDT.getSize()!=684)
           {
             std::cout <<"Error: sub record TVDT of CELL has invalid length ("
                       <<unknownTVDT.getSize()<<" bytes). Should be 684 bytes.\n";
             return false;
           }
           break;
      case cMHDT:
           if (unknownMHDT.isPresent())
           {
             std::cout << "Error: CELL seems to have more than one MHDT subrecord.\n";
             return false;
           }
           if (!unknownMHDT.loadFromStream(*actual_in, cMHDT, false))
           {
             std::cout << "Error while reading subrecord MHDT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMHDT.getSize();
           //check size
           if (unknownMHDT.getSize()!=1028)
           {
             std::cout <<"Error: sub record MHDT of CELL has invalid length ("
                       <<unknownMHDT.getSize()<<" bytes). Should be 1028 bytes.\n";
             return false;
           }
           break;
      case cXCLC:
           if (gridLocation.presence)
           {
             std::cout << "Error: CELL seems to have more than one XCLC subrecord.\n";
             return false;
           }
           //XCLC's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record XCLC of CELL has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read XCLC
           actual_in->read((char*) &(gridLocation.locationX), 4);
           actual_in->read((char*) &(gridLocation.locationY), 4);
           actual_in->read((char*) &(gridLocation.unknownThird), 4);
           bytesRead += 12;
           if (!(actual_in->good()))
           {
             std::cout << "Error while reading subrecord XCLC of CELL!\n";
             return false;
           }
           gridLocation.presence = true;
           break;
      case cXCLL:
           if (unknownXCLL.isPresent())
           {
             std::cout << "Error: CELL seems to have more than one XCLL subrecord.\n";
             return false;
           }
           if (!unknownXCLL.loadFromStream(*actual_in, cXCLL, false))
           {
             std::cout << "Error while reading subrecord XCLL of CELL!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownXCLL.getSize();
           break;
      case cLTMP:
           if (hasReadLTMP)
           {
             std::cout << "Error: CELL seems to have more than one LTMP subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read LTMP
           if (!loadUint32SubRecordFromStream(*actual_in, cLTMP, unknownLTMP)) return false;
           bytesRead += 6;
           hasReadLTMP = true;
           break;
      case cLNAM:
           if (hasLNAM)
           {
             std::cout << "Error: CELL seems to have more than one LNAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read LNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cLNAM, unknownLNAM)) return false;
           bytesRead += 6;
           hasLNAM = true;
           break;
      case cXCLW:
           if (hasReadXCLW)
           {
             std::cout << "Error: CELL seems to have more than one XCLW subrecord.\n";
             return false;
           }
           //XCLW's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record XCLW of CELL has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XCLW
           actual_in->read((char*) &unknownXCLW, 4);
           bytesRead += 4;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord XCLW of CELL!\n";
             return false;
           }
           hasReadXCLW = true;
           break;
      case cXCLR:
           if (!unknownXCLR.empty())
           {
             std::cout << "Error: CELL seems to have more than one XCLR subrecord.\n";
             return false;
           }
           //XCLR's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength==0) or ((subLength%4)!=0))
           {
             std::cout <<"Error: sub record XCLR of CELL has invalid length ("<<subLength
                       <<" bytes). Should be an integral, non-zero multiple of four bytes.\n";
             return false;
           }
           //read XCLR
           count = subLength / 4;
           for (i=0; i<count; ++i)
           {
             actual_in->read((char*) &tempUint32, 4);
             bytesRead += 4;
             if (!actual_in->good())
             {
               std::cout << "Error while reading subrecord XCLR of CELL!\n";
               return false;
             }
             unknownXCLR.push_back(tempUint32);
           }//for
           break;
      case cXNAM:
           if (hasXNAM)
           {
             std::cout << "Error: CELL seems to have more than one XNAM subrecord.\n";
             return false;
           }
           //XNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record XNAM of CELL has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read XNAM
           actual_in->read((char*) &unknownXNAM, 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord XNAM of CELL!\n";
             return false;
           }
           hasXNAM = true;
           break;
      case cXLCN:
           if (hasXLCN)
           {
             std::cout << "Error: CELL seems to have more than one XLCN subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XLCN
           if (!loadUint32SubRecordFromStream(*actual_in, cXLCN, unknownXLCN)) return false;
           bytesRead += 6;
           hasXLCN = true;
           break;
      case cXWCN:
           if (hasXWCN)
           {
             std::cout << "Error: CELL seems to have more than one XWCN subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XWCN
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCN, unknownXWCN)) return false;
           bytesRead += 6;
           hasXWCN = true;
           break;
      case cXWCS:
           if (hasXWCS)
           {
             std::cout << "Error: CELL seems to have more than one XWCS subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XWCS
           if (!loadUint32SubRecordFromStream(*actual_in, cXWCS, unknownXWCS)) return false;
           bytesRead += 6;
           hasXWCS = true;
           break;
      case cXWCU:
           if (unknownXWCU.isPresent())
           {
             std::cout << "Error: CELL seems to have more than one XWCU subrecord.\n";
             return false;
           }
           if (!unknownXWCU.loadFromStream(*actual_in, cXWCU, false))
           {
             std::cout << "Error while reading subrecord XWCUT of CELL!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownXWCU.getSize();
           //check size
           subLength = unknownXWCU.getSize();
           if ((subLength==0) or ((subLength%16)!=0))
           {
             std::cout <<"Error: sub record XWCU of CELL has invalid length ("
                       <<unknownXWCU.getSize()<<" bytes). Should be an integral multiple of 16 bytes.\n";
             return false;
           }
           break;
      case cXCIM:
           if (hasXCIM)
           {
             std::cout << "Error: CELL seems to have more than one XCIM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XCIM
           if (!loadUint32SubRecordFromStream(*actual_in, cXCIM, unknownXCIM)) return false;
           bytesRead += 6;
           hasXCIM = true;
           break;
      case cXEZN:
           if (hasXEZN)
           {
             std::cout << "Error: CELL seems to have more than one XEZN subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XEZN
           if (!loadUint32SubRecordFromStream(*actual_in, cXEZN, encounterZoneFormID)) return false;
           bytesRead += 6;
           hasXEZN = true;
           break;
      case cXCWT:
           if (hasXCWT)
           {
             std::cout << "Error: CELL seems to have more than one XCWT subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XCWT
           if (!loadUint32SubRecordFromStream(*actual_in, cXCWT, unknownXCWT)) return false;
           bytesRead += 6;
           hasXCWT = true;
           break;
      case cXCMO:
           if (hasXCMO)
           {
             std::cout << "Error: CELL seems to have more than one XCMO subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XCMO
           if (!loadUint32SubRecordFromStream(*actual_in, cXCMO, unknownXCMO)) return false;
           bytesRead += 6;
           hasXCMO = true;
           break;
      case cXWEM:
           if (!unknownXWEM.empty())
           {
             std::cout << "Error: CELL seems to have more than one XWEM subrecord.\n";
             return false;
           }
           //XWEM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record XWEM of CELL is longer than 511 characters!\n";
             return false;
           }
           //read XWEM's stuff
           memset(buffer, 0, 512);
           actual_in->read(buffer, subLength);
           bytesRead += subLength;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord XWEM of CELL!\n";
             return false;
           }
           unknownXWEM = std::string(buffer);
           break;
      case cXOWN:
           if (hasXOWN)
           {
             std::cout << "Error: CELL seems to have more than one XOWN subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XOWN
           if (!loadUint32SubRecordFromStream(*actual_in, cXOWN, unknownXOWN)) return false;
           bytesRead += 6;
           hasXOWN = true;
           break;
      case cXILL:
           if (hasXILL)
           {
             std::cout << "Error: CELL seems to have more than one XILL subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XILL
           if (!loadUint32SubRecordFromStream(*actual_in, cXILL, unknownXILL)) return false;
           bytesRead += 6;
           hasXILL = true;
           break;
      case cXCCM:
           if (hasXCCM)
           {
             std::cout << "Error: CELL seems to have more than one XCCM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XCCM
           if (!loadUint32SubRecordFromStream(*actual_in, cXCCM, unknownXCCM)) return false;
           bytesRead += 6;
           hasXCCM = true;
           break;
      case cXCAS:
           if (hasXCAS)
           {
             std::cout << "Error: CELL seems to have more than one XCAS subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read XCAS
           if (!loadUint32SubRecordFromStream(*actual_in, cXCAS, unknownXCAS)) return false;
           bytesRead += 6;
           hasXCAS = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only EDID, FULL, DATA, TVDT, MHDT, XCLC, XCLL,"
                     << " LTMP, LNAM, XCLW, XCLR, XNAM, XLCN, XWCN, XWCS, XWCU,"
                     << " XCIM, XEZN, XCWT, XCMO, XWEM, XOWN, XILL, XCCM or XCAS are allowed here!\n";
           std::cout << "Local stream position is "<< actual_in->tellg()
                     << " bytes. Bytes read so far: "<<bytesRead<<".\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(unknownDATA.isPresent() and hasReadLTMP and hasReadXCLW))
  {
    std::cout << "Error: At least one required subrecord of CELL was not found!\n"
              << "DATA: "<<unknownDATA.isPresent()<<"    LTMP: "<<hasReadLTMP<<"    XCLW: "<<hasReadXCLW
              << "\n";
    return false;
  }

  return actual_in->good();
}

int32_t CellRecord::getRecordType() const
{
  return cCELL;
}



} //namespace
