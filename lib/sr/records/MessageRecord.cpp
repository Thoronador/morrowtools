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

#include "MessageRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* ITXT_CTDA_compound's functions */

MessageRecord::ITXT_CTDA_compound::ITXT_CTDA_compound()
: unknownITXT(0),
  hasCTDA(false)
{

}

bool MessageRecord::ITXT_CTDA_compound::operator==(const MessageRecord::ITXT_CTDA_compound& other) const
{
  return ((unknownITXT==other.unknownITXT) and (hasCTDA==other.hasCTDA)
      and ((unknownCTDA==other.unknownCTDA) or (!hasCTDA)));
}

/* MessageRecord's functions */

MessageRecord::MessageRecord()
: BasicRecord(), editorID(""),
  messageTextStringID(0),
  hasFULL(false), titleStringID(0),
  unknownINAM(0),
  hasQNAM(false), ownerQuestFormID(0),
  unknownDNAM(0),
  unknownITXT_CTDAs(std::vector<ITXT_CTDA_compound>()),
  hasTNAM(false), displayTime(0)
{

}

MessageRecord::~MessageRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MessageRecord::equals(const MessageRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (messageTextStringID==other.messageTextStringID)
      and (hasFULL==other.hasFULL) and ((titleStringID==other.titleStringID) or (!hasFULL))
      and (unknownINAM==other.unknownINAM)
      and (hasQNAM==other.hasQNAM) and ((ownerQuestFormID==other.ownerQuestFormID) or (!hasQNAM))
      and (unknownDNAM==other.unknownDNAM) and (unknownITXT_CTDAs==other.unknownITXT_CTDAs)
      and (hasTNAM==other.hasTNAM) and ((displayTime==other.displayTime) or (!hasTNAM)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MessageRecord::getWriteSize() const
{
  uint32_t writeSize =
        4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasQNAM)
  {
    writeSize = writeSize +4 /* QNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!unknownITXT_CTDAs.empty())
  {
    const unsigned int len = unknownITXT_CTDAs.size();
    writeSize = writeSize +len*(4 /* ITXT */ +2 /* 2 bytes for length */ +4 /* fixed size */);
    unsigned int i;
    for (i=0; i<len; ++i)
    {
      if ( unknownITXT_CTDAs[i].hasCTDA)
      {
        writeSize = writeSize +4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */;
      }//if
    }//for i
  }//if ITXT/CTDA
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool MessageRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cMESG, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &messageTextStringID, 4);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &titleStringID, 4);
  }//if has FULL

  //write INAM
  output.write((const char*) &cINAM, 4);
  //INAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write INAM's stuff
  output.write((const char*) &unknownINAM, 4);

  if (hasQNAM)
  {
    //write QNAM
    output.write((const char*) &cQNAM, 4);
    //QNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write QNAM's stuff
    output.write((const char*) &ownerQuestFormID, 4);
  }//if has QNAM

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) &unknownDNAM, 4);

  if (!unknownITXT_CTDAs.empty())
  {
    const unsigned int len = unknownITXT_CTDAs.size();
    unsigned int i;
    for (i=0; i<len; ++i)
    {
      //write ITXT
      output.write((const char*) &cITXT, 4);
      //ITXT's length
      subLength = 4; //fixed
      output.write((const char*) &subLength, 2);
      //write ITXT's stuff
      output.write((const char*) &(unknownITXT_CTDAs[i]), 4);

      if (unknownITXT_CTDAs[i].hasCTDA)
      {
        if (!unknownITXT_CTDAs[i].unknownCTDA.saveToStream(output))
        {
          std::cerr << "Error while writing subrecord CTDA of MESG!\n";
          return false;
        }
      }//if
    }//for
  }//if ITXTs

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write TNAM's stuff
    output.write((const char*) &displayTime, 4);
  }//if has TNAM

  return output.good();
}
#endif

bool MessageRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of MESG is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of MESG!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadDESC = false;
  hasFULL = false; titleStringID = 0;
  bool hasReadINAM = false;
  hasQNAM = false; ownerQuestFormID = 0;
  bool hasReadDNAM = false;
  unknownITXT_CTDAs.clear();
  ITXT_CTDA_compound tempITXT_CTDA;
  bool hasUnpushedITXT = false;
  hasTNAM = false; displayTime = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDESC:
           if (hasReadDESC)
           {
             std::cerr << "Error: record MESG seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, messageTextStringID, false)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cerr << "Error: record MESG seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, titleStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cINAM:
           if (hasReadINAM)
           {
             std::cerr << "Error: record MESG seems to have more than one INAM subrecord!\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM, false)) return false;
           bytesRead += 6;
           hasReadINAM = true;
           break;
      case cQNAM:
           if (hasQNAM)
           {
             std::cerr << "Error: record MESG seems to have more than one QNAM subrecord!\n";
             return false;
           }
           //read QNAM
           if (!loadUint32SubRecordFromStream(in_File, cQNAM, ownerQuestFormID, false)) return false;
           bytesRead += 6;
           hasQNAM = true;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cerr << "Error: record MESG seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read DNAM
           if (!loadUint32SubRecordFromStream(in_File, cDNAM, unknownDNAM, false)) return false;
           bytesRead += 6;
           hasReadDNAM = true;
           break;
      case cITXT:
           if (hasUnpushedITXT)
           {
             unknownITXT_CTDAs.push_back(tempITXT_CTDA);
           }
           //read ITXT
           if (!loadUint32SubRecordFromStream(in_File, cITXT, tempITXT_CTDA.unknownITXT, false)) return false;
           bytesRead += 6;
           hasUnpushedITXT = true;
           tempITXT_CTDA.hasCTDA = false;
           break;
      case cCTDA:
           if (!hasUnpushedITXT)
           {
             std::cerr << "Error while reading record of type MESG: there has to be a ITXT before CTDA subrecord!\n";
             return false;
           }
           if (tempITXT_CTDA.hasCTDA)
           {
             std::cerr << "Error: record MESG seems to have more than one CTDA subrecord per ITXT!\n";
             return false;
           }
           //read CTDA
           if (!tempITXT_CTDA.unknownCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of MESG!\n";
             return false;
           }
           unknownITXT_CTDAs.push_back(tempITXT_CTDA);
           hasUnpushedITXT = false;
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cerr << "Error: record MESG seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, displayTime, false)) return false;
           bytesRead += 6;
           hasTNAM = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only DESC, FULL, INAM, QNAM, DNAM, ITXT, CTDA or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  if (hasUnpushedITXT)
  {
    unknownITXT_CTDAs.push_back(tempITXT_CTDA);
  }

  //presence checks
  if (!(hasReadDESC and hasReadINAM and hasReadDNAM))
  {
    std::cerr << "Error: at least one required subrecord of MESG is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MessageRecord::getRecordType() const
{
  return cMESG;
}

} //namespace
