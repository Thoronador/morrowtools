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

#include "SoundCategoryRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

const uint32_t SoundCategoryRecord::cMuteWhenSubmerged = 0x00000001;
const uint32_t SoundCategoryRecord::cShouldAppearOnMenu = 0x00000002;

SoundCategoryRecord::SoundCategoryRecord()
: BasicRecord()
{
  editorID = "";
  unknownFULL = 0;
  unknownFNAM = 0;
  hasParent = false;
  parentFormID = 0;
  staticVolumeMultUint16 = 65535;
  hasUNAM = false;
  defaultMenuValueUint16 = 65535;
}

SoundCategoryRecord::~SoundCategoryRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool SoundCategoryRecord::equals(const SoundCategoryRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownFULL==other.unknownFULL) and (unknownFNAM==other.unknownFNAM)
      and (hasParent==other.hasParent) and ((parentFormID==other.parentFormID) or (!hasParent))
      and (staticVolumeMultUint16==other.staticVolumeMultUint16)
      and (hasUNAM==other.hasUNAM) and ((defaultMenuValueUint16==other.defaultMenuValueUint16) or (!hasUNAM)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SoundCategoryRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* VNAM */ +2 /* 2 bytes for length */ +2 /* fixed length of 2 bytes */;
  if (hasParent)
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }
  if (hasUNAM)
  {
    writeSize = writeSize +4 /* UNAM */ +2 /* 2 bytes for length */ +2 /* fixed length of 2 bytes */;
  }
  return writeSize;
}

bool SoundCategoryRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSNCT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write FULL
  output.write((const char*) &cFULL, 4);
  //FULL's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FULL's stuff
  output.write((const char*) &unknownFULL, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 4);

  if (hasParent)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write PNAM's stuff
    output.write((const char*) &parentFormID, 4);
  }

  //write VNAM
  output.write((const char*) &cVNAM, 4);
  //VNAM's length
  subLength = 2; //fixed
  output.write((const char*) &subLength, 2);
  //write VNAM's stuff
  output.write((const char*) &staticVolumeMultUint16, 2);

  if (hasUNAM)
  {
    //write UNAM
    output.write((const char*) &cUNAM, 4);
    //UNAM's length
    subLength = 2; //fixed
    output.write((const char*) &subLength, 2);
    //write UNAM's stuff
    output.write((const char*) &defaultMenuValueUint16, 2);
  }

  return output.good();
}
#endif

bool SoundCategoryRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of SNCT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SNCT!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadFULL = false;
  bool hasReadFNAM = false;
  hasParent = false; parentFormID = 0;
  bool hasReadVNAM = false;
  hasUNAM = false; defaultMenuValueUint16 = 65535;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasReadFULL)
           {
             std::cout << "Error: record SNCT seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL, false))
           {
             std::cout << "Error while reading subrecord FULL of SNCT!\n";
             return false;
           }
           bytesRead += 6;
           hasReadFULL = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: record SNCT seems to have more than one FNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false))
           {
             std::cout << "Error while reading subrecord FNAM of SNCT!\n";
             return false;
           }
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cPNAM:
           if (hasParent)
           {
             std::cout << "Error: record SNCT seems to have more than one PNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, parentFormID, false))
           {
             std::cout << "Error while reading subrecord PNAM of SNCT!\n";
             return false;
           }
           bytesRead += 6;
           hasParent = true;
           break;
      case cVNAM:
           if (hasReadVNAM)
           {
             std::cout << "Error: record SNCT seems to have more than one VNAM subrecord!\n";
             return false;
           }
           //VNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record VNAM of SNCT has invalid length ("
                       <<subLength<<" bytes). Should be two bytes!\n";
             return false;
           }
           //read VNAM's stuff
           in_File.read((char*) &staticVolumeMultUint16, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VNAM of SNCT!\n";
             return false;
           }
           hasReadVNAM = true;
           break;
      case cUNAM:
           if (hasUNAM)
           {
             std::cout << "Error: record SNCT seems to have more than one UNAM subrecord!\n";
             return false;
           }
           //UNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record UNAM of SNCT has invalid length ("
                       <<subLength<<" bytes). Should be two bytes!\n";
             return false;
           }
           //read UNAM's stuff
           in_File.read((char*) &defaultMenuValueUint16, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord UNAM of SNCT!\n";
             return false;
           }
           hasUNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, FNAM, PNAM, VNAM or UNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadFULL and  hasReadFNAM and hasReadVNAM))
  {
    std::cout << "Error while reading SNCT: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SoundCategoryRecord::getRecordType() const
{
  return cSNCT;
}

bool SoundCategoryRecord::isMuteWhenSubmerged() const
{
  return ((unknownFNAM & cMuteWhenSubmerged)!=0);
}

bool SoundCategoryRecord::shouldAppearOnMenu() const
{
  return ((unknownFNAM & cShouldAppearOnMenu)!=0);
}

float SoundCategoryRecord::getStaticVolumeMultiplier() const
{
  return static_cast<float>(staticVolumeMultUint16)/65535.0f;
}

float SoundCategoryRecord::getDefaultMenuValue() const
{
  if (hasUNAM) return static_cast<float>(defaultMenuValueUint16)/65535.0f;
  return 1.0f;
}

} //namespace
