/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "LandscapeTextureRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

LandscapeTextureRecord::LandscapeTextureRecord()
: BasicRecord()
{
  editorID = "";
  hasTNAM = false;
  unknownTNAM = 0;
  unknownMNAM = 0;
  unknownHNAM = 0;
  unknownSNAM = 0;
  unknownGNAMs.clear();
}

LandscapeTextureRecord::~LandscapeTextureRecord()
{
  //empty
}

bool LandscapeTextureRecord::equals(const LandscapeTextureRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasTNAM==other.hasTNAM) and ((unknownTNAM==other.unknownTNAM) or (!hasTNAM))
      and (unknownMNAM==other.unknownMNAM) and (unknownHNAM==other.unknownHNAM)
      and (unknownSNAM==other.unknownSNAM) and (unknownGNAMs==other.unknownGNAMs));
}

bool LandscapeTextureRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLTEX, 4);
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
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

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

bool LandscapeTextureRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of LTEX is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of LTEX!\n";
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
             std::cout << "Error: LTEX seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //skip back 4 bytes
           in_File.seekg(-4, std::ios_base::cur);
           //readTNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM))
           {
             std::cout << "Error while reading subrecord TNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           hasTNAM = true;
           break;
      case cMNAM:
           if (hasReadMNAM)
           {
             std::cout << "Error: LTEX seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //skip back 4 bytes
           in_File.seekg(-4, std::ios_base::cur);
           //readMNAM
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM))
           {
             std::cout << "Error while reading subrecord MNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           hasReadMNAM = true;
           break;
      case cHNAM:
           if (hasReadHNAM)
           {
             std::cout << "Error: LTEX seems to have more than one HNAM subrecord!\n";
             return false;
           }
           //HNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record HNAM of LTEX has invalid length ("<<subLength
                       <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read HNAM
           in_File.read((char*) &unknownHNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord HNAM of LTEX!\n";
             return false;
           }
           hasReadHNAM = true;
           break;
      case cSNAM:
           if (hasReadSNAM)
           {
             std::cout << "Error: LTEX seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //SNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record SNAM of LTEX has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read SNAM
           in_File.read((char*) &unknownSNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNAM of LTEX!\n";
             return false;
           }
           hasReadSNAM = true;
           break;
      case cGNAM:
           temp = 0;
           //skip back 4 bytes
           in_File.seekg(-4, std::ios_base::cur);
           //read GNAM
           if (!loadUint32SubRecordFromStream(in_File, cGNAM, temp))
           {
             std::cout << "Error while reading subrecord GNAM of LTEX!\n";
             return false;
           }
           bytesRead += 6;
           unknownGNAMs.push_back(temp);
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only TNAM, MNAM, HNAM, SNAM or GNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check for presence of required stuff
  if (!(hasReadMNAM and hasReadHNAM and hasReadSNAM))
  {
    std::cout << "Error: At least one subrecord of LTEX is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t LandscapeTextureRecord::getRecordType() const
{
  return cLTEX;
}

} //namespace