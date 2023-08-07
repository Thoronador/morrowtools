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

#include "PlacedGrenadeRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

PlacedGrenadeRecord::PlacedGrenadeRecord()
: BasicRecord(),
  nameFormID(0),
  hasXESP(false), unknownXESP(0),
  hasXOWN(false), ownerFormID(0),
  scale(1.0f)
{
  memset(unknownDATA, 0, 24);
}

PlacedGrenadeRecord::~PlacedGrenadeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool PlacedGrenadeRecord::equals(const PlacedGrenadeRecord& other) const
{
  return ((equalsBasic(other)) and (nameFormID==other.nameFormID)
      and (hasXESP==other.hasXESP) and ((unknownXESP==other.unknownXESP) or (!hasXESP))
      and (hasXOWN==other.hasXOWN) and ((ownerFormID==other.ownerFormID) or (!hasXOWN))
      and (scale==other.scale)
      and (memcmp(unknownDATA, other.unknownDATA, 24)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t PlacedGrenadeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* NAME */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed size */;
  if (hasXESP)
  {
    writeSize = writeSize +4 /* XESP */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (hasXOWN)
  {
    writeSize = writeSize +4 /* XOWN */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (scale!=1.0f)
  {
    writeSize = writeSize +4 /* XSCL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool PlacedGrenadeRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cPGRE, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

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

  if (hasXOWN)
  {
    //write XOWN
    output.write((const char*) &cXOWN, 4);
    //XOWN's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write XOWN
    output.write((const char*) &ownerFormID, 4);
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

bool PlacedGrenadeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read NAME
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, subRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cerr <<"Error: sub record NAME of PGRE has invalid length ("<<subLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) &nameFormID, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord NAME of PGRE!\n";
    return false;
  }

  hasXESP = false; unknownXESP = 0;
  hasXOWN = false; ownerFormID = 0;
  scale = 1.0f;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cXESP:
           if (hasXESP)
           {
             std::cerr << "Error: PGRE seems to have more than one XESP subrecord.\n";
             return false;
           }
           //XESP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cerr <<"Error: sub record XESP of PGRE has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read XESP's stuff
           in_File.read((char*) &unknownXESP, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XESP of PGRE!\n";
             return false;
           }
           hasXESP = true;
           break;
      case cXOWN:
           if (hasXOWN)
           {
             std::cerr << "Error: PGRE seems to have more than one XOWN subrecord.\n";
             return false;
           }
           //XOWN's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record XOWN of PGRE has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XOWN's stuff
           in_File.read((char*) &ownerFormID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XOWN of PGRE!\n";
             return false;
           }
           hasXOWN = true;
           break;
      case cXSCL:
           if (scale!=1.0f)
           {
             std::cerr << "Error: PGRE seems to have more than one XSCL subrecord.\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record XSCL of PGRE has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XSCL's stuff
           in_File.read((char*) &scale, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XSCL of PGRE!\n";
             return false;
           }
           if (scale==1.0f)
           {
             std::cerr << "Error while reading subrecord XSCL of PGRE: value is exactly 1.0!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: PGRE seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cerr <<"Error: sub record DATA of PGRE has invalid length ("<<subLength
                       <<" bytes). Should be 24 bytes.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 24);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of PGRE!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only XESP, XOWN, XSCL or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cerr << "Error: while reading PGRE record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t PlacedGrenadeRecord::getRecordType() const
{
  return cPGRE;
}

} //namespace
