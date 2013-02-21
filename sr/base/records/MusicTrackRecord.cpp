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

#include "MusicTrackRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//constant values for track types
const uint32_t MusicTrackRecord::cTypeSingle  = 0x6ED7E048; //"48 E0 D7 6E"
const uint32_t MusicTrackRecord::cTypeSilent  = 0xA1A9C4D5; //"D5 C4 A9 A1"
const uint32_t MusicTrackRecord::cTypePalette = 0x23F678C3; //"C3 78 F6 23"

/* LoopData's functions */

bool MusicTrackRecord::LoopData::operator==(const MusicTrackRecord::LoopData& other) const
{
  if (hasLNAM!=other.hasLNAM) return false;
  if (!hasLNAM) return true;
  return ((loopBegins==other.loopBegins) and (loopEnds==other.loopEnds)
      and (loopCount==other.loopCount));
}


/* MusicTrackRecord's functions */

MusicTrackRecord::MusicTrackRecord()
: BasicRecord(), editorID(""),
  typeIdentifier(0),
  hasFLTV(false), duration(0.0f),
  hasDNAM(false), fadeOut(0.0f),
  trackList(std::vector<uint32_t>()),  //subrecord SNAM
  unknownANAM(""),
  finalePath(""), //subrecord BNAM
  cuePoints(std::vector<float>()), //subrecord FNAM
  conditions(std::vector<CTDAData>())
{
  //subrecord LNAM
  loop.hasLNAM = false;
  loop.loopBegins = 0.0f; //seconds
  loop.loopEnds = 0.0f; //seconds
  loop.loopCount = 0;
  //end of LNAM
}

MusicTrackRecord::~MusicTrackRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MusicTrackRecord::equals(const MusicTrackRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (typeIdentifier==other.typeIdentifier)
      and (hasFLTV==other.hasFLTV) and ((duration==other.duration) or (!hasFLTV))
      and (hasDNAM==other.hasDNAM) and ((fadeOut==other.fadeOut) or (!hasDNAM))
      and (trackList==other.trackList)
      and (unknownANAM==other.unknownANAM) and (finalePath==other.finalePath)
      and (cuePoints==other.cuePoints) and (loop==other.loop)
      and (conditions==other.conditions));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MusicTrackRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  if (hasFLTV)
  {
    writeSize = writeSize +4 /* FLTV */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  if (hasDNAM)
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  if (!trackList.empty())
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4*trackList.size() /* fixed length of four bytes */;
  }
  if (!unknownANAM.empty())
  {
    writeSize = writeSize +4 /* ANAM */ +2 /* 2 bytes for length */
        +unknownANAM.length()+1 /* length of ANAM +1 byte for NUL termination */;
  }
  if (!finalePath.empty())
  {
    writeSize = writeSize +4 /* BNAM */ +2 /* 2 bytes for length */
        +finalePath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (!cuePoints.empty())
  {
    writeSize = writeSize +4 /* FNAM */ +2 /* 2 bytes for length */ +4*cuePoints.size() /* fixed length of four bytes per float */;
  }
  if (loop.hasLNAM)
  {
    writeSize = writeSize +4 /* LNAM */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */;
  }
  if (!conditions.empty())
  {
    writeSize = writeSize +4 /* CITC */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +conditions.size()*(4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed length of 32 bytes */);
  }
  return writeSize;
}

bool MusicTrackRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMUST, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write CNAM's data
  output.write((const char*) &typeIdentifier, 4);

  if (hasFLTV)
  {
    //write FLTV
    output.write((const char*) &cFLTV, 4);
    //FLTV's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write duration
    output.write((const char*) &duration, 4);
  }//if FLTV

  if (hasDNAM)
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    //DNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write fade-out time
    output.write((const char*) &fadeOut, 4);
  }//if DNAM

  if (!trackList.empty())
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    const unsigned int len = trackList.size();
    subLength = 4*len;
    output.write((const char*) &subLength, 2);
    //write track form IDs
    unsigned int i;
    for (i=0; i<len; ++i)
    {
      output.write((const char*) &(trackList[i]), 4);
    }//for
  }//if track list

  if (!unknownANAM.empty())
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    //ANAM's length
    subLength = unknownANAM.length()+1;
    output.write((const char*) &subLength, 2);
    //write ANAM's path
    output.write(unknownANAM.c_str(), subLength);
  }

  if (!finalePath.empty())
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    //BNAM's length
    subLength = finalePath.length()+1;
    output.write((const char*) &subLength, 2);
    //write BNAM's path
    output.write(finalePath.c_str(), subLength);
  }//if BNAM

  if (!cuePoints.empty())
  {
    const unsigned int len = cuePoints.size();
    unsigned int i;
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    subLength = 4*len;
    output.write((const char*) &subLength, 2);
    //write FNAM's stuff
    for (i=0; i<len; ++i)
    {
      output.write((const char*) &cuePoints[i], 4);
    }//for
  }//if

  if (loop.hasLNAM)
  {
    //write LNAM
    output.write((const char*) &cLNAM, 4);
    //LNAM's length
    subLength = 12;
    output.write((const char*) &subLength, 2);
    //write loop data
    output.write((const char*) &(loop.loopBegins), 4);
    output.write((const char*) &(loop.loopEnds), 4);
    output.write((const char*) &(loop.loopCount), 4);
  }//if has LNAM

  if (!conditions.empty())
  {
    const uint32_t count = conditions.size();
    //write CITC
    output.write((const char*) &cCITC, 4);
    //CITC's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write Condition Count
    output.write((const char*) &count, 4);

    //write CTDA
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write CTDA
      if (!conditions[i].saveToStream(output))
      {
        std::cout << "Error while writing subrecord CTDA of MUST!\n";
        return false;
      }
    }//for
  }//if conditions

  return output.good();
}
#endif

bool MusicTrackRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
  bytesRead += 4;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of MUST is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MUST!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadCNAM = false;
  typeIdentifier = 0;
  hasFLTV = false;
  duration = 0.0f;
  hasDNAM = false;
  fadeOut = 0.0f;
  trackList.clear();
  bool hasReadANAM = false;
  unknownANAM.clear();
  bool hasReadBNAM = false;
  finalePath.clear();
  cuePoints.clear(); //subrecord FNAM
  float tempFloat32;
  unsigned int i, count;
  loop.hasLNAM = false;
  bool hasReadCITC = false;
  conditions.clear();
  uint32_t cond_count;
  CTDAData tempCTDA;

  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: MUST seems to have more than one CNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, typeIdentifier, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cFLTV:
           if (hasFLTV)
           {
             std::cout << "Error: MUST seems to have more than one FLTV subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord FLTV of MUST has invalid length ("
                       <<subLength <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FLTV
           in_File.read((char*) &duration, 4);
           bytesRead += 4;
           if (duration<0.0f)
           {
             std::cout << "Error while reading subrecord FLTV of MUST: value is less than zero!\n";
             return false;
           }
           if (duration!=duration)
           {
             std::cout << "Error while reading subrecord FLTV of MUST: value is NaN!\n";
             return false;
           }
           hasFLTV = true;
           break;
      case cDNAM:
           if (hasDNAM)
           {
             std::cout << "Error: MUST seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord DNAM of MUST has invalid length ("
                       <<subLength <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DNAM
           in_File.read((char*) &fadeOut, 4);
           bytesRead += 4;
           if (fadeOut<0.0f)
           {
             std::cout << "Error while reading subrecord DNAM of MUST: value is less than zero!\n";
             return false;
           }
           if (fadeOut!=fadeOut)
           {
             std::cout << "Error while reading subrecord DNAM of MUST: value is NaN!\n";
             return false;
           }
           if (fadeOut>20.0f)
           {
             std::cout << "Error while reading subrecord DNAM of MUST: value is greater than 20.0!\n";
             return false;
           }
           hasDNAM = true;
           break;
      case cSNAM:
           if (!trackList.empty())
           {
             std::cout << "Error: MUST seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //SNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (((subLength%4)!=0) or (subLength==0))
           {
             std::cout <<"Error: subrecord SNAM of MUST has invalid length ("<<subLength
                       <<" bytes). Should be a positive, integral multiple of four bytes!\n";
             return false;
           }
           count = subLength / 4;
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &cond_count, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord SNAM of MUST!\n";
               return false;
             }
             trackList.push_back(cond_count);
           }//for
           bytesRead += subLength;
           break;
      case cANAM:
           if (hasReadANAM)
           {
             std::cout << "Error: MUST seems to have more than one ANAM subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, unknownANAM, buffer, cANAM, false, bytesRead))
           {
             return false;
           }
           hasReadANAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cout << "Error: MUST seems to have more than one BNAM subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, finalePath, buffer, cBNAM, false, bytesRead))
           {
             return false;
           }
           hasReadBNAM = true;
           break;
      case cFNAM:
           if (!cuePoints.empty())
           {
             std::cout << "Error: MUST seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (((subLength%4)!=0) or (subLength==0))
           {
             std::cout <<"Error: subrecord FNAM of MUST has invalid length ("<<subLength
                       <<" bytes). Should be a positive, integral multiple of four bytes!\n";
             return false;
           }
           count = subLength / 4;
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &tempFloat32, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord FNAM of MUST!\n";
               return false;
             }
             if (tempFloat32<0.0f)
             {
               std::cout << "Error while reading subrecord FNAM of MUST: value is less than zero!\n";
               return false;
             }
             if (tempFloat32!=tempFloat32)
             {
               std::cout << "Error while reading subrecord FNAM of MUST: value is NaN!\n";
               return false;
             }
             cuePoints.push_back(tempFloat32);
           }//for
           bytesRead += subLength;
           break;
      case cLNAM:
           if (loop.hasLNAM)
           {
             std::cout << "Error: MUST seems to have more than one LNAM subrecord!\n";
             return false;
           }
           //LNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord LNAM of MUST has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read LNAM
           in_File.read((char*) &loop.loopBegins, 4);
           in_File.read((char*) &loop.loopEnds, 4);
           in_File.read((char*) &loop.loopCount, 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord LNAM of MUST!\n";
             return false;
           }
           if (loop.loopBegins<0.0f)
           {
             std::cout << "Error while reading subrecord LNAM of MUST: start value is less than zero!\n";
             return false;
           }
           if (loop.loopBegins!=loop.loopBegins)
           {
             std::cout << "Error while reading subrecord LNAM of MUST: start value is NaN!\n";
             return false;
           }
           if (loop.loopEnds<loop.loopBegins)
           {
             std::cout << "Error while reading subrecord LNAM of MUST: end value is less than start value!\n";
             return false;
           }
           if (loop.loopEnds!=loop.loopEnds)
           {
             std::cout << "Error while reading subrecord LNAM of MUST: end value is NaN!\n";
             return false;
           }
           loop.hasLNAM = true;
           break;
      case cCITC:
           if (hasReadCITC)
           {
             std::cout << "Error: MUST seems to have more than one CITC subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCITC, cond_count, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadCITC = true;

           //now read the conditions one by one
           for (i=0; i<cond_count; ++i)
           {
             //read CTDA
             in_File.read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cCTDA)
             {
               UnexpectedRecord(cCTDA, subRecName);
               return false;
             }
             //read CTDA's data
             if (!tempCTDA.loadFromStream(in_File, bytesRead))
             {
               std::cout << "Error while reading subrecord CTDA of MUST!\n";
               return false;
             }
             conditions.push_back(tempCTDA);
           }//for
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only CNAM, FLTV, DNAM, SNAM, ANAM, BNAM, FNAM, LNAM or CITC are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadCNAM)
  {
    std::cout << "Error: at least one required subrecord of MUST is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MusicTrackRecord::getRecordType() const
{
  return cMUST;
}

MusicTrackRecord::TrackType MusicTrackRecord::getTrackType() const
{
  switch(typeIdentifier)
  {
    case cTypeSingle:
         return ttSingleTrack;
         break;
    case cTypePalette:
         return ttPalette;
         break;
    case cTypeSilent:
         return ttSilentTrack;
         break;
  }//swi

  //we usually shouldn't get to this point for a filled record, but well... guess.
  if (!trackList.empty()) return ttPalette;
  //If both ANAM and BNAM are missing, this is a silent track in all cases for Skyrim.esm.
  if (unknownANAM.empty() and finalePath.empty()) return ttSilentTrack;
  //well... I guess it's a single track after all.
  return ttSingleTrack;
}

} //namespace
