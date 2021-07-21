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

#include "MusicTypeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t MusicTypeRecord::FlagPlaysOneTransition = 0x00000001;
const uint32_t MusicTypeRecord::FlagAbruptTransition   = 0x00000002;
const uint32_t MusicTypeRecord::FlagCycleTracks        = 0x00000004;
const uint32_t MusicTypeRecord::FlagMaintainTrackOrder = 0x00000008;
const uint32_t MusicTypeRecord::FlagDucksCurrentTrack  = 0x00000020;


MusicTypeRecord::MusicTypeRecord()
: BasicRecord(), editorID(""),
  flags(0),
  priority(1),
  ducking_dB_uint16(0),
  fadeDuration(0.0f),
  musicTrackFormIDs(std::vector<uint32_t>())
{

}

MusicTypeRecord::~MusicTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MusicTypeRecord::equals(const MusicTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (flags==other.flags) and (priority==other.priority)
      and (ducking_dB_uint16==other.ducking_dB_uint16) and (fadeDuration==other.fadeDuration)
      and (musicTrackFormIDs==other.musicTrackFormIDs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MusicTypeRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
        +4 /* TNAM */ +2 /* 2 bytes for length */ +4*musicTrackFormIDs.size() /* fixed length of 4 bytes per entry */);
}

bool MusicTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMUSC, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write FNAM's data
  output.write((const char*) &flags, 4);

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write PNAM's data
  output.write((const char*) &priority, 2);
  uint16_t chickenCurryWithRice;
  if (ducksCurrentTrack())
  {
    if (ducking_dB_uint16>10000)
    {
      chickenCurryWithRice = 10000;
    }
    else
    {
      chickenCurryWithRice = ducking_dB_uint16;
    }
    output.write((const char*) &chickenCurryWithRice, 2);
  }
  else
  {
    chickenCurryWithRice = 0;
    output.write((const char*) &chickenCurryWithRice, 2);
  }

  //write WNAM
  output.write((const char*) &cWNAM, 4);
  //WNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write WNAM's data
  output.write((const char*) &fadeDuration, 4);

  //write TNAM
  output.write((const char*) &cTNAM, 4);
  //TNAM's length
  subLength = 4*musicTrackFormIDs.size();
  output.write((const char*) &subLength, 2);
  //write TNAM's data
  unsigned int i;
  for (i=0; i<musicTrackFormIDs.size(); ++i)
  {
    output.write((const char*) &(musicTrackFormIDs[i]), 4);
  }//for

  return output.good();
}
#endif

bool MusicTypeRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of MUSC is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MUSC!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadFNAM = false;
  flags = 0;
  bool hasReadPNAM = false;
  priority = 0; ducking_dB_uint16 = 0;
  bool hasReadWNAM = false;
  fadeDuration = 0.0f;
  musicTrackFormIDs.clear();
  uint32_t tempUint32, i, count;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: MUSC seems to have more than one FNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, flags, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cout << "Error: MUSC seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //PNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord PNAM of MUSC has invalid length ("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read PNAM's stuff
           in_File.read((char*) &priority, 2);
           in_File.read((char*) &ducking_dB_uint16, 2);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PNAM of MUSC!\n";
             return false;
           }
           hasReadPNAM = true;
           break;
      case cWNAM:
           if (hasReadWNAM)
           {
             std::cout << "Error: MUSC seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //WNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord WNAM of MUSC has invalid length ("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read WNAM's stuff
           in_File.read((char*) &fadeDuration, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WNAM of MUSC!\n";
             return false;
           }
           if (fadeDuration!=fadeDuration)
           {
             std::cout << "Error while reading subrecord WNAM of MUSC: fade duration is NaN!\n";
             return false;
           }
           hasReadWNAM = true;
           break;
      case cTNAM:
           if (!musicTrackFormIDs.empty())
           {
             std::cout << "Error: MUSC seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //TNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (((subLength%4)!=0) or (subLength==0))
           {
             std::cout <<"Error: subrecord TNAM of MUSC has invalid length ("<<subLength
                       <<" bytes). Should be a positive, integral multiple of four bytes!\n";
             return false;
           }
           count = subLength / 4;
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &tempUint32, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord TNAM of MUSC!\n";
               return false;
             }
             musicTrackFormIDs.push_back(tempUint32);
           }//for
           bytesRead += subLength;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FNAM, PNAM, WNAM or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadFNAM and hasReadPNAM and hasReadWNAM and !musicTrackFormIDs.empty()))
  {
    std::cout << "Error: at least one required subrecord of MUSC is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MusicTypeRecord::getRecordType() const
{
  return cMUSC;
}

bool MusicTypeRecord::playsOneTransition() const
{
  return ((flags & FlagPlaysOneTransition)!=0);
}

bool MusicTypeRecord::hasAbruptTransition() const
{
  return ((flags & FlagAbruptTransition)!=0);
}

bool MusicTypeRecord::cyclesTracks() const
{
  return ((flags & FlagCycleTracks)!=0);
}

bool MusicTypeRecord::maintainsTrackOrder() const
{
  return (cyclesTracks() and ((flags & FlagMaintainTrackOrder)!=0));
}

bool MusicTypeRecord::ducksCurrentTrack() const
{
  return ((flags & FlagDucksCurrentTrack)!=0);
}

float MusicTypeRecord::getDucking_dB() const
{
  if (!ducksCurrentTrack()) return 0.0f;
  if (ducking_dB_uint16>10000) return 100.0f;
  return (ducking_dB_uint16/100.0f);
}

} //namespace
