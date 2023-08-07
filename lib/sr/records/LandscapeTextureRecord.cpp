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

#include "LandscapeTextureRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

LandscapeTextureRecord::LandscapeTextureRecord()
: BasicRecord(), editorID(""),
  hasTNAM(false), unknownTNAM(0),
  unknownMNAM(0),
  unknownHNAM(0),
  unknownSNAM(0),
  unknownGNAMs(std::vector<uint32_t>())
{

}

LandscapeTextureRecord::~LandscapeTextureRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LandscapeTextureRecord::equals(const LandscapeTextureRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasTNAM==other.hasTNAM) and ((unknownTNAM==other.unknownTNAM) or (!hasTNAM))
      and (unknownMNAM==other.unknownMNAM) and (unknownHNAM==other.unknownHNAM)
      and (unknownSNAM==other.unknownSNAM) and (unknownGNAMs==other.unknownGNAMs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LandscapeTextureRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* HNAM */ +2 /* 2 bytes for length */ +2 /* fixed length of two bytes */
        +4 /* SNAM */ +2 /* 2 bytes for length */ +1 /* fixed length of one byte */;
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  if (!unknownGNAMs.empty())
  {
    writeSize = writeSize + unknownGNAMs.size() *
    (4 /* GNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */);
  }
  return writeSize;
}

bool LandscapeTextureRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLTEX, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write TNAM's data
    output.write((const char*) &unknownTNAM, 4);
  }//if has TNAM

  //write MNAM
  output.write((const char*) &cMNAM, 4);
  //MNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write MNAM's data
  output.write((const char*) &unknownMNAM, 4);

  //write HNAM
  output.write((const char*) &cHNAM, 4);
  //HNAM's length
  subLength = 2;
  output.write((const char*) &subLength, 2);
  //write HNAM's data
  output.write((const char*) &unknownHNAM, 2);

  //write SNAM
  output.write((const char*) &cSNAM, 4);
  //SNAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write SNAM's data
  output.write((const char*) &unknownSNAM, 1);

  if (!unknownGNAMs.empty())
  {
    unsigned int i;
    for (i=0; i<unknownGNAMs.size(); ++i)
    {
      //write GNAM
      output.write((const char*) &cGNAM, 4);
      //GNAM's length
      subLength = 4;
      output.write((const char*) &subLength, 2);
      //write GNAM's data
      output.write((const char*) &(unknownGNAMs[i]), 4);
    }//for
  }//if has GNAMs

  return output.good();
}
#endif

bool LandscapeTextureRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cerr <<"Error: sub record EDID of LTEX is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of LTEX!\n";
    return false;
  }
  editorID = std::string(buffer);

  hasTNAM = false;
  bool hasReadMNAM = false;
  bool hasReadHNAM = false;
  bool hasReadSNAM = false;
  unknownGNAMs.clear();
  uint32_t temp = 0;
  while (bytesRead<readSize)
  {
    //read next sub rec name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cTNAM:
           if (hasTNAM)
           {
             std::cerr << "Error: LTEX seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //readTNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM, false))
           {
             std::cerr << "Error while reading subrecord TNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           hasTNAM = true;
           break;
      case cMNAM:
           if (hasReadMNAM)
           {
             std::cerr << "Error: LTEX seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //readMNAM
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM, false))
           {
             std::cerr << "Error while reading subrecord MNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           hasReadMNAM = true;
           break;
      case cHNAM:
           if (hasReadHNAM)
           {
             std::cerr << "Error: LTEX seems to have more than one HNAM subrecord!\n";
             return false;
           }
           //HNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cerr <<"Error: sub record HNAM of LTEX has invalid length ("<<subLength
                       <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read HNAM
           in_File.read((char*) &unknownHNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord HNAM of LTEX!\n";
             return false;
           }
           hasReadHNAM = true;
           break;
      case cSNAM:
           if (hasReadSNAM)
           {
             std::cerr << "Error: LTEX seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //SNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: sub record SNAM of LTEX has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read SNAM
           in_File.read((char*) &unknownSNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord SNAM of LTEX!\n";
             return false;
           }
           hasReadSNAM = true;
           break;
      case cGNAM:
           temp = 0;
           //read GNAM
           if (!loadUint32SubRecordFromStream(in_File, cGNAM, temp, false))
           {
             std::cerr << "Error while reading subrecord GNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           unknownGNAMs.push_back(temp);
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only TNAM, MNAM, HNAM, SNAM or GNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check for presence of required stuff
  if (!(hasReadMNAM and hasReadHNAM and hasReadSNAM))
  {
    std::cerr << "Error: At least one subrecord of LTEX is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t LandscapeTextureRecord::getRecordType() const
{
  return cLTEX;
}

} //namespace
