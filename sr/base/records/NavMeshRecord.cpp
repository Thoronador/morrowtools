/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

#include "NavMeshRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../base/BufferStream.h"
#include "../../../base/CompressionFunctions.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

NavMeshRecord::NavMeshRecord()
: BasicRecord()
{
  unknownNVNM.setPresence(false);
  unknownONAM.setPresence(false);
  unknownNNAM.setPresence(false);
  unknownPNAM.setPresence(false);
}

NavMeshRecord::~NavMeshRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool NavMeshRecord::equals(const NavMeshRecord& other) const
{
  return ((equalsBasic(other)) and (unknownNVNM==other.unknownNVNM)
      and (unknownONAM==other.unknownONAM) and (unknownNNAM==other.unknownNNAM)
      and (unknownPNAM==other.unknownPNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t NavMeshRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize = 0;

  if (unknownNVNM.isPresent())
  {
    if (unknownNVNM.getSize()<65536)
    {
      writeSize = writeSize +4 /* NVNM */ +2 /* 2 bytes for length */ +unknownNVNM.getSize() /* size */;
    }
    else
    {
      writeSize = writeSize +4 /* XXXX */ +2 /* 2 bytes for length */ +4 /* fixed size */
                 +4 /* NVNM */ +2 /* 2 bytes for length */ +unknownNVNM.getSize() /* size */;
    }
  }//if NVNM

  if (unknownONAM.isPresent())
  {
    writeSize = writeSize +4 /* ONAM */ +2 /* 2 bytes for length */ +unknownONAM.getSize() /* size */;
  }//if ONAM

  if (unknownNNAM.isPresent())
  {
    writeSize = writeSize +4 /* NNAM */ +2 /* 2 bytes for length */ +unknownNNAM.getSize() /* size */;
  }//if NNAM

  if (unknownPNAM.isPresent())
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +unknownPNAM.getSize() /* size */;
  }//if PNAM

  return writeSize;
}

bool NavMeshRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cNAVM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  if (unknownNVNM.isPresent())
  {
    //write NVNM
    if (!unknownNVNM.saveToStream(output, cNVNM))
    {
      std::cout << "Error while writing subrecord NVNM of NAVM!\n";
      return false;
    }
  }//if NVNM

  if (unknownONAM.isPresent())
  {
    //write ONAM
    if (!unknownONAM.saveToStream(output, cONAM))
    {
      std::cout << "Error while writing subrecord ONAM of NAVM!\n";
      return false;
    }
  }//if ONAM

  if (unknownNNAM.isPresent())
  {
    //write NNAM
    if (!unknownNNAM.saveToStream(output, cNNAM))
    {
      std::cout << "Error while writing subrecord NNAM of NAVM!\n";
      return false;
    }
  }//if NNAM

  if (unknownPNAM.isPresent())
  {
    //write PNAM
    if (!unknownPNAM.saveToStream(output, cPNAM))
    {
      std::cout << "Error while writing subrecord PNAM of NAVM!\n";
      return false;
    }
  }//if PNAM

  return output.good();
}
#endif

bool NavMeshRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName;
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
      std::cout << "Error while reading decompression size of NAVM!\n";
      return false;
    }
    if (readSize<=4)
    {
      std::cout << "Error: size of compressed NAVM record is too small to contain any compressed data!\n";
      return false;
    }
    //buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize-4];
    in_File.read((char*) stream_buffer, readSize-4);
    if (!in_File.good())
    {
      std::cout << "Error while reading compressed data of NAVM!\n";
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
  }//if is compressed record

  unknownNVNM.setPresence(false);
  uint32_t sizeXXXX = 0;
  unknownONAM.setPresence(false);
  unknownNNAM.setPresence(false);
  unknownPNAM.setPresence(false);

  while (bytesRead<readSize)
  {
    //read next header
    actual_in->read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cNVNM:
           if (unknownNVNM.isPresent())
           {
             std::cout << "Error: NAVM seems to have more than one NVNM subrecord.\n";
             return false;
           }

           //read NVNM
           if (0==sizeXXXX)
           {
             if (!unknownNVNM.loadFromStream(*actual_in, cNVNM, false))
             {
               std::cout << "Error while reading subrecord NVNM of NAVM!\n";
               return false;
             }
           }
           else
           {
             if (!unknownNVNM.loadFromStreamExtended(*actual_in, cNVNM, false, sizeXXXX))
             {
               std::cout << "Error while reading subrecord NVNM of NAVM!\n";
               return false;
             }
             sizeXXXX = 0;
           }//else - load extended version of subrecord
           bytesRead += (2 /*length value*/ +unknownNVNM.getSize() /*data size*/);
           break;
      case cXXXX:
           if (sizeXXXX!=0)
           {
             std::cout << "Error: NAVM seems to have more than one XXXX subrecord in a row.\n";
             return false;
           }
           //read XXXX
           if (!loadUint32SubRecordFromStream(*actual_in, cXXXX, sizeXXXX, false)) return false;
           bytesRead += 6;
           if (0==sizeXXXX)
           {
             std::cout << "Error: subrecord XXXX of NAVM has value zero!\n";
             return false;
           }
           break;
      case cONAM:
           if (unknownONAM.isPresent())
           {
             std::cout << "Error: NAVM seems to have more than one ONAM subrecord.\n";
             return false;
           }
           //read ONAM
           if (!unknownONAM.loadFromStream(*actual_in, cONAM, false))
           {
             std::cout << "Error while reading subrecord ONAM of NAVM!\n";
             return false;
           }
           bytesRead += (2 + unknownONAM.getSize());
           break;
      case cNNAM:
           if (unknownNNAM.isPresent())
           {
             std::cout << "Error: NAVM seems to have more than one NNAM subrecord.\n";
             return false;
           }
           //read NNAM
           if (!unknownNNAM.loadFromStream(*actual_in, cNNAM, false))
           {
             std::cout << "Error while reading subrecord NNAM of NAVM!\n";
             return false;
           }
           bytesRead += (2 + unknownNNAM.getSize());
           break;
      case cPNAM:
           if (unknownPNAM.isPresent())
           {
             std::cout << "Error: NAVM seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //read PNAM
           if (!unknownPNAM.loadFromStream(*actual_in, cPNAM, false))
           {
             std::cout << "Error while reading subrecord PNAM of NAVM!\n";
             return false;
           }
           bytesRead += (2 + unknownPNAM.getSize());
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only NVNM, ONAM, NNAM, PNAM or XXXX are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence check
  if (!unknownNVNM.isPresent())
  {
    std::cout << "Error: while reading NAVM record: subrecord NVNM is missing!\n";
    return false;
  }//if

  return in_File.good();
}

uint32_t NavMeshRecord::getRecordType() const
{
  return cNAVM;
}

} //namespace
