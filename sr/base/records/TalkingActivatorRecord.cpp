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

#include "TalkingActivatorRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

TalkingActivatorRecord::TalkingActivatorRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  keywordArray(std::vector<uint32_t>()),
  unknownPNAM(0),
  loopingSoundFormID(0),
  unknownFNAM(0),
  voiceTypeFormID(0)
{
  memset(unknownOBND, 0, 12);
}

TalkingActivatorRecord::~TalkingActivatorRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool TalkingActivatorRecord::equals(const TalkingActivatorRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (keywordArray==other.keywordArray)
      and (unknownPNAM==other.unknownPNAM)
      and (loopingSoundFormID==other.loopingSoundFormID)
      and (unknownFNAM==other.unknownFNAM) and (voiceTypeFormID==other.voiceTypeFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t TalkingActivatorRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +2 /* fixed size of 2 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size();
  }
  if (loopingSoundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  if (voiceTypeFormID!=0)
  {
    writeSize = writeSize +4 /* VNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  return writeSize;
}

bool TalkingActivatorRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cTACT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if FULL

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of TACT!\n";
      return false;
    }
  }

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write keyword size
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*keywordArray.size(); //fixed size
    output.write((const char*) &subLength, 2);
    //write keywords
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write PNAM's stuff
  output.write((const char*) &unknownPNAM, 4);

  if (loopingSoundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write looping sound's form ID
    output.write((const char*) &loopingSoundFormID, 4);
  }//if SNAM

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 2; //fixed
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 2);

  if (voiceTypeFormID!=0)
  {
    //write VNAM
    output.write((const char*) &cVNAM, 4);
    //VNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write voice type form ID
    output.write((const char*) &voiceTypeFormID, 4);
  }//if VTYP

  return output.good();
}
#endif

bool TalkingActivatorRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
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
    std::cout <<"Error: sub record EDID of TACT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of TACT!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of TACT has invalid length ("<<subLength
              << " bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  memset(unknownOBND, 0, 12);
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of TACT!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  keywordArray.clear();
  uint32_t k_Size, i, temp;
  bool hasReadPNAM = false;
  loopingSoundFormID = 0;
  bool hasReadFNAM = false;
  voiceTypeFormID = 0;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: record TACT seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           //sanity check
           if ((name.getType()==LocalizedString::lsIndex) and (name.getIndex()==0))
           {
             std::cout << "Error: subrecord FULL of TACT has invalid value zero!\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: record TACT seems to have more than one MODL subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead)) return false;
           //sanity check
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of TACT is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: record TACT seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of TACT!";
             return false;
           }
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: TACT seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;
           if (0==k_Size)
           {
             std::cout << "Error: subrecord KSIZ of TACT is zero!\n";
             return false;
           }

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cout <<"Error: sub record KWDA of TACT has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read keywords
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of TACT!\n";
               return false;
             }
             keywordArray.push_back(temp);
           }//for
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cout << "Error: record TACT seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false)) return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cSNAM:
           if (loopingSoundFormID!=0)
           {
             std::cout << "Error: record TACT seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, loopingSoundFormID, false)) return false;
           bytesRead += 6;
           //sanity check
           if (loopingSoundFormID==0)
           {
             std::cout << "Error: subrecord SNAM of TACT has invalid value zero!\n";
             return false;
           }
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: record TACT seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record FNAM of TACT has invalid length ("
                       <<subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 2);
           bytesRead +=2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of TACT!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cVNAM:
           if (voiceTypeFormID!=0)
           {
             std::cout << "Error: record TACT seems to have more than one VNAM subrecord!\n";
             return false;
           }
           //read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, voiceTypeFormID, false)) return false;
           bytesRead += 6;
           //sanity check
           if (voiceTypeFormID==0)
           {
             std::cout << "Error: subrecord VNAM of TACT has invalid value zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, KSIZ, PNAM, FNAM or VNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!((!modelPath.empty()) and hasReadPNAM and hasReadFNAM))
  {
    std::cout << "Error: At least one required subrecord of TACT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t TalkingActivatorRecord::getRecordType() const
{
  return cTACT;
}

} //namespace
