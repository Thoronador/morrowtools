/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "PlacedHazardRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

PlacedHazardRecord::PlacedHazardRecord()
: BasicRecord(),
  nameFormID(0),
  hasXESP(false), unknownXESP(0),
  scale(1.0f)
{
  unknownVMAD.setPresence(false);
  memset(unknownDATA, 0, 24);
}

PlacedHazardRecord::~PlacedHazardRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool PlacedHazardRecord::equals(const PlacedHazardRecord& other) const
{
  return ((equalsBasic(other)) and (unknownVMAD==other.unknownVMAD)
      and (nameFormID==other.nameFormID)
      and (hasXESP==other.hasXESP) and ((unknownXESP==other.unknownXESP) or (!hasXESP))
      and (scale==other.scale)
      and (memcmp(unknownDATA, other.unknownDATA, 24)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t PlacedHazardRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* NAME */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* fixed size */;
  }
  if (hasXESP)
  {
    writeSize = writeSize +4 /* XESP */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (scale!=1.0f)
  {
    writeSize = writeSize +4 /* XSCL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool PlacedHazardRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cPHZD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of PHZD!\n!";
      return false;
    }
  }

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint16_t subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write NAME
  output.write((const char*) &nameFormID, 4);

  if (hasXESP)
  {
    //write XESP
    output.write((const char*) &cXESP, 4);
    //XESP's length
    subLength = 8; // fixed
    output.write((const char*) &subLength, 2);
    //write XESP
    output.write((const char*) &unknownXESP, 8);
  }

  if (scale!=1.0f)
  {
    //write XSCL
    output.write((const char*) &cXSCL, 4);
    //XSCL's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write XSCL
    output.write((const char*) &scale, 4);
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 24; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) unknownDATA, 24);

  return output.good();
}
#endif

bool PlacedHazardRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  unknownVMAD.setPresence(false);
  bool hasReadNAME = false;
  hasXESP = false; unknownXESP = 0;
  scale = 1.0f;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: PHZD seems to have more than one VMAD subrecord.\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of PHZD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownVMAD.getSize() /*data size*/;
           break;
      case cNAME:
           if (hasReadNAME)
           {
             std::cout << "Error: PHZD seems to have more than one NAME subrecord.\n";
             return false;
           }
           //NAME's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record NAME of PHZD has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read PHZD's stuff
           in_File.read((char*) &nameFormID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAME of PHZD!\n";
             return false;
           }
           hasReadNAME = true;
           break;
      case cXESP:
           if (hasXESP)
           {
             std::cout << "Error: PHZD seems to have more than one XESP subrecord.\n";
             return false;
           }
           //XESP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record XESP of PHZD has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read XESP's stuff
           in_File.read((char*) &unknownXESP, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XESP of PHZD!\n";
             return false;
           }
           hasXESP = true;
           break;
      case cXSCL:
           if (scale!=1.0f)
           {
             std::cout << "Error: PHZD seems to have more than one XSCL subrecord.\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record XSCL of PHZD has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XSCL's stuff
           in_File.read((char*) &scale, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSCL of PHZD!\n";
             return false;
           }
           if (scale==1.0f)
           {
             std::cout << "Error while reading subrecord XSCL of PHZD: value is exactly 1.0!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: PHZD seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cout <<"Error: sub record DATA of PHZD has invalid length ("<<subLength
                       <<" bytes). Should be 24 bytes.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 24);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of PHZD!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only VMAD, NAME, XESP, XSCL or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadNAME and hasReadDATA))
  {
    std::cout << "Error: while reading PHZD record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t PlacedHazardRecord::getRecordType() const
{
  return cPHZD;
}

} //namespace
