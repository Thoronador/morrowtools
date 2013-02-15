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

#include "IdleAnimationRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* flag constants */
const uint8_t IdleAnimationRecord::cFlagSequence     = 0x02;
const uint8_t IdleAnimationRecord::cFlagNotAttacking = 0x04;
const uint8_t IdleAnimationRecord::cFlagBlocking     = 0x08;


IdleAnimationRecord::IdleAnimationRecord()
: BasicRecord(), editorID(""),
  conditions(std::vector<CTDA_CIS1_compound>()),
  unknownDNAM(""), animEventName(""),
  parentFormID(0), previousFormID(0), //subrecord ANAM
  //subrecord DATA
  minLoopingSeconds(0), maxLoopingSeconds(0),
  flags(0), unknownDATAFour(0),
  replayDelay(0)
  //end of subrecord DATA
{

}

IdleAnimationRecord::~IdleAnimationRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool IdleAnimationRecord::equals(const IdleAnimationRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (conditions==other.conditions)
      and (unknownDNAM==other.unknownDNAM) and (animEventName==other.animEventName)
      and (parentFormID==other.parentFormID) and (previousFormID==other.previousFormID)
      and (minLoopingSeconds==other.minLoopingSeconds) and (maxLoopingSeconds==other.maxLoopingSeconds)
      and (flags==other.flags) and (unknownDATAFour==other.unknownDATAFour)
      and (replayDelay==other.replayDelay)
    );
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t IdleAnimationRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* ANAM */ +2 /* 2 bytes for length */ +8 /* fixed length */
             +4 /* DATA */ +2 /* 2 bytes for length */ +6 /* fixed length */;
  unsigned int i;
  for (i=0; i<conditions.size(); ++i)
  {
    writeSize += conditions[i].getWriteSize();
  }//for
  if (!editorID.empty())
  {
    writeSize = writeSize +4 /* EDID */ +2 /* 2 bytes for length */
               +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!unknownDNAM.empty())
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */
               +unknownDNAM.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (!animEventName.empty())
  {
    writeSize = writeSize +4 /* ENAM */ +2 /* 2 bytes for length */
               +animEventName.length()+1 /* length of name +1 byte for NUL termination */;
  }//if ENAM
  return writeSize;
}

bool IdleAnimationRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cIDLE, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  uint16_t subLength = 0;
  if (!editorID.empty())
  {
    //write EDID
    output.write((const char*) &cEDID, 4);
    //EDID's length
    subLength = editorID.length()+1;
    output.write((const char*) &subLength, 2);
    //write editor ID
    output.write(editorID.c_str(), subLength);
  }//if EDID

  unsigned int i = 0;
  for (i=0; i<conditions.size(); ++i)
  {
    if (!conditions[i].saveToStream(output))
    {
      std::cout << "Error while writing subrecord CTDA of IDLE!\n";
      return false;
    }
  }//for

  if (!unknownDNAM.empty())
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    //DNAM's length
    subLength = unknownDNAM.length()+1;
    output.write((const char*) &subLength, 2);
    //write .hkx path
    output.write(unknownDNAM.c_str(), subLength);
  }

  if (!animEventName.empty())
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = animEventName.length()+1;
    output.write((const char*) &subLength, 2);
    //write Anim Event
    output.write(animEventName.c_str(), subLength);
  }//if ENAM

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = 8;
  output.write((const char*) &subLength, 2);
  //write ANAM's data
  output.write((const char*) &parentFormID, 4);
  output.write((const char*) &previousFormID, 4);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 6;
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &minLoopingSeconds, 1);
  output.write((const char*) &maxLoopingSeconds, 1);
  output.write((const char*) &flags, 1);
  output.write((const char*) &unknownDATAFour, 1);
  output.write((const char*) &replayDelay, 2);

  return output.good();
}
#endif

bool IdleAnimationRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;

  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //buffer for strings
  char buffer[512];

  editorID.clear();
  conditions.clear();
  CTDAData tempCTDA;
  unknownDNAM.clear();
  animEventName.clear();
  bool hasReadANAM = false;
  bool hasReadDATA = false;

  while (bytesRead<readSize)
  {
    //read next record
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cout << "Error: IDLE seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read EDID
           if (!loadString512FromStream(in_File, editorID, buffer, cEDID, false, bytesRead))
             return false;
           //check content
           if (editorID.empty())
           {
             std::cout << "Error: subrecord EDID of IDLE is empty!\n";
             return false;
           }
           break;
      case cCTDA:
           //read CTDA
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
             return false;
           conditions.push_back(CTDA_CIS1_compound(tempCTDA, ""));
           break;
      case cCIS1:
           if (conditions.empty())
           {
             std::cout << "Error: subrecord CIS1 without prior CTDA subrecord!\n";
             return false;
           }
           if (!conditions.back().unknownCIS1.empty())
           {
             std::cout << "Error: IDLE seems to have more than one CIS1 subrecord per CTDA subrecord!\n";
             return false;
           }
           //load CIS1
           if (!loadString512FromStream(in_File, conditions.back().unknownCIS1, buffer, cCIS1, false, bytesRead))
           {
             return false;
           }
           break;
      case cDNAM:
           if (!unknownDNAM.empty())
           {
             std::cout << "Error: IDLE seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read DNAM
           if (!loadString512FromStream(in_File, unknownDNAM, buffer, cDNAM, false, bytesRead))
             return false;
           //check content
           if (unknownDNAM.empty())
           {
             std::cout << "Error: subrecord DNAM of IDLE is empty!\n";
             return false;
           }
           break;
      case cENAM:
           if (!animEventName.empty())
           {
             std::cout << "Error: IDLE seems to have more than one ENAM subrecord.\n";
             return false;
           }
           //read DNAM
           if (!loadString512FromStream(in_File, animEventName, buffer, cENAM, false, bytesRead))
             return false;
           //check content
           if (animEventName.empty())
           {
             std::cout << "Error: subrecord ENAM of IDLE is empty!\n";
             return false;
           }
           break;
      case cANAM:
           if (hasReadANAM)
           {
             std::cout << "Error: IDLE seems to have more than one ANAM subrecord.\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout << "Error: subrecord ANAM of IDLE has invalid length ("
                       << subLength << " bytes). Should be eight bytes!\n";
             return false;
           }
           //read ANAM's stuff
           in_File.read((char*) &parentFormID, 4);
           in_File.read((char*) &previousFormID, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ANAM of IDLE!\n";
             return false;
           }
           hasReadANAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: IDLE seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=6)
           {
             std::cout << "Error: subrecord DATA of IDLE has invalid length ("
                       << subLength << " bytes). Should be six bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &minLoopingSeconds, 1);
           in_File.read((char*) &maxLoopingSeconds, 1);
           in_File.read((char*) &flags, 1);
           in_File.read((char*) &unknownDATAFour, 1);
           in_File.read((char*) &replayDelay, 2);
           bytesRead += 6;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of IDLE!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only EDID, CTDA, CIS1, DNAM, ENAM, ANAM or DATA are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadANAM and hasReadDATA))
  {
    std::cout << "Error: At least one required subrecord of IDLE is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t IdleAnimationRecord::getRecordType() const
{
  return cIDLE;
}

} //namespace
