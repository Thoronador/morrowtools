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

#include "DialogueRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

DialogueRecord::DialogueRecord()
: BasicRecord(), editorID(""),
  unknownFULL(0),
  unknownPNAM(0),
  dialogueBranchFormID(0),
  unknownQNAM(0),
  unknownDATA(0),
  unknownSNAM(0),
  unknownTIFC(0)
{

}

DialogueRecord::~DialogueRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DialogueRecord::equals(const DialogueRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownFULL==other.unknownFULL) and (unknownPNAM==other.unknownPNAM)
      and (dialogueBranchFormID==other.dialogueBranchFormID)
      and (unknownQNAM==other.unknownQNAM) and (unknownDATA==other.unknownDATA)
      and (unknownSNAM==other.unknownSNAM) and (unknownTIFC==other.unknownTIFC)
    );
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DialogueRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize = 0;
  writeSize = 4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/
         +4 /* QNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/
         +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/
         +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/
         +4 /* TIFC */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/;
  if (!editorID.empty())
  {
    writeSize = writeSize +4 /* EDID */ +2 /* 2 bytes for length */
               +editorID.length() +1 /* length of ID plus one byte for NUL termination */;
  }
  if (unknownFULL!=0)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/;
  }
  if (dialogueBranchFormID!=0)
  {
    writeSize = writeSize +4 /* BNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/;
  }
  return writeSize;
}

bool DialogueRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cDIAL, 4);
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
    //write EDID
    output.write(editorID.c_str(), subLength);
  }

  if (unknownFULL!=0)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &unknownFULL, 4);
  }//if FULL

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write PNAM
  output.write((const char*) &unknownPNAM, 4);

  if (dialogueBranchFormID!=0)
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    //BNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write BNAM
    output.write((const char*) &dialogueBranchFormID, 4);
  }//if BNAM

  //write QNAM
  output.write((const char*) &cQNAM, 4);
  //QNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write QNAM
  output.write((const char*) &unknownQNAM, 4);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &unknownDATA, 4);

  //write SNAM
  output.write((const char*) &cSNAM, 4);
  //SNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write SNAM
  output.write((const char*) &unknownSNAM, 4);

  //write TIFC
  output.write((const char*) &cTIFC, 4);
  //TIFC's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write TIFC
  output.write((const char*) &unknownTIFC, 4);

  return output.good();
}
#endif

bool DialogueRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  char buffer[512];
  editorID.clear();
  unknownFULL = 0;
  bool hasReadPNAM = false;
  bool hasReadQNAM = false;
  bool hasReadDATA = false;
  bool hasReadSNAM = false;
  bool hasReadTIFC = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cerr << "Error: DIAL seems to have more than one EDID subrecord!\n";
             return false;
           }
           //load EDID
           if (!loadString512FromStream(in_File, editorID, buffer, cEDID, false, bytesRead))
             return false;
           //check content
           if (editorID.empty())
           {
             std::cerr << "Error: EDID of DIAL is empty!\n";
             return false;
           }
           break;
      case cFULL:
           if (unknownFULL!=0)
           {
             std::cerr << "Error: DIAL seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL, false))
             return false;
           bytesRead += 6;
           //check content
           if (unknownFULL==0)
           {
             std::cerr << "Error: subrecord FULL of DIAL is zero!\n";
             return false;
           }
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: DIAL seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false))
             return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cBNAM:
           if (dialogueBranchFormID!=0)
           {
             std::cerr << "Error: DIAL seems to have more than one BNAM subrecord!\n";
             return false;
           }
           //read BNAM
           if (!loadUint32SubRecordFromStream(in_File, cBNAM, dialogueBranchFormID, false))
             return false;
           bytesRead += 6;
           //check content
           if (dialogueBranchFormID==0)
           {
             std::cerr << "Error: subrecord BNAM of DIAL is zero!\n";
             return false;
           }
           break;
      case cQNAM:
           if (hasReadQNAM)
           {
             std::cerr << "Error: DIAL seems to have more than one QNAM subrecord!\n";
             return false;
           }
           //read QNAM
           if (!loadUint32SubRecordFromStream(in_File, cQNAM, unknownQNAM, false))
             return false;
           bytesRead += 6;
           hasReadQNAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: DIAL seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, unknownDATA, false))
             return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cSNAM:
           if (hasReadSNAM)
           {
             std::cerr << "Error: DIAL seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, false))
             return false;
           bytesRead += 6;
           hasReadSNAM = true;
           break;
      case cTIFC:
           if (hasReadTIFC)
           {
             std::cerr << "Error: DIAL seems to have more than one TIFC subrecord!\n";
             return false;
           }
           //read TIFC
           if (!loadUint32SubRecordFromStream(in_File, cTIFC, unknownTIFC, false))
             return false;
           bytesRead += 6;
           hasReadTIFC = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only EDID, PNAM, BNAM, QNAM, DATA, SNAM or TIFC are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence check
  if (!(hasReadPNAM and hasReadQNAM and hasReadDATA and hasReadSNAM and hasReadTIFC))
  {
    std::cerr << "Error: at least one subrecord of DIAL is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t DialogueRecord::getRecordType() const
{
  return cDIAL;
}

} //namespace
