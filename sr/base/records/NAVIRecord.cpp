/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "NAVIRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

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

NAVIRecord::~NAVIRecord()
{
  unknownNVMIs.clear();
}

#ifndef SR_NO_RECORD_EQUALITY
bool NAVIRecord::equals(const NAVIRecord& other) const
{
  return ((equalsBasic(other)) and (unknownNVER==other.unknownNVER)
      and (unknownNVMIs==other.unknownNVMIs) and (unknownNVPP==other.unknownNVPP)
      and (unknownNVSI==other.unknownNVSI));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t NAVIRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* NVER */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  uint32_t i;
  for (i=0; i<unknownNVMIs.size(); ++i)
  {
    writeSize = writeSize +4 /* NVMI */ +2 /* 2 bytes for length */ +unknownNVMIs[i].getSize() /* size */;
  }//for
  if (unknownNVPP.isPresent())
  {
    writeSize = writeSize +4 /* NVPP */ +2 /* 2 bytes for length */ +unknownNVPP.getSize() /* size */;
  }
  if (unknownNVSI!=0)
  {
    writeSize = writeSize +4 /* NVSI */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool NAVIRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cNAVI, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write NVER
  output.write((const char*) &cNVER, 4);
  //NVER's length
  uint16_t subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NVER
  output.write((const char*) &unknownNVER, 4);

  unsigned int i;
  for (i=0; i<unknownNVMIs.size(); ++i)
  {
    //write NVMI
    if (!unknownNVMIs[i].saveToStream(output, cNVMI))
    {
      std::cout << "Error while writing subrecord NVMI of NAVI!\n";
      return false;
    }//if
  }//for

  if (unknownNVPP.isPresent())
  {
    //write NVPP
    if (!unknownNVPP.saveToStream(output, cNVPP))
    {
      std::cout << "Error while writing subrecord NVPP of NAVI!\n";
      return false;
    }//if
  }//if NVPP

  if (unknownNVSI!=0)
  {
    //write NVSI
    output.write((const char*) &cNVSI, 4);
    //NVSI's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write NVSI
    output.write((const char*) &unknownNVSI, 4);
  }//if NVSI

  return output.good();
}
#endif

bool NAVIRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read NVER
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cNVER)
  {
    UnexpectedRecord(cNVER, subRecName);
    return false;
  }
  //NVER's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: sub record NVER of NAVI has invalid length ("<<subLength
              << " bytes). Should be four bytes!\n";
    return false;
  }
  //read EDID's stuff
  in_File.read((char*) &unknownNVER, 4);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NVER of NAVI!\n";
    return false;
  }

  unknownNVMIs.clear();
  unknownNVPP.setPresence(false);
  BinarySubRecord tempBin;
  unknownNVSI = 0;
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cNVMI:
           //read binary subrecord
           if (!tempBin.loadFromStream(in_File, cNVMI, false))
           {
             std::cout << "Error while reading subrecord NVMI of NAVI!\n";
             return false;
           }
           bytesRead = bytesRead +2 +tempBin.getSize();
           unknownNVMIs.push_back(tempBin);
           break;
      case cNVPP:
           if (unknownNVPP.isPresent())
           {
             std::cout << "Error: NAVI seems to have more than one NVPP subrecord!\n";
             return false;
           }
           //read binary subrecord
           if (!unknownNVPP.loadFromStream(in_File, cNVPP, false))
           {
             std::cout << "Error while reading subrecord NVPP of NAVI!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownNVPP.getSize();
           break;
      case cNVSI:
           if (unknownNVSI!=0)
           {
             std::cout << "Error: NAVI seems to have more than one NVSI subrecord!\n";
             return false;
           }
           //load NVSI
           if (!loadUint32SubRecordFromStream(in_File, cNVSI, unknownNVSI, false))
             return false;
           bytesRead += 6;
           if (unknownNVSI==0)
           {
             std::cout << "Error: subrecord NVSI of NAVI is zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only NVMI, NVPP or NVSI are allowed here!\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

uint32_t NAVIRecord::getRecordType() const
{
  return cNAVI;
}

} //namespace
