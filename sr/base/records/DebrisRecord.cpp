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

#include "DebrisRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* subrecordDATA's functions */

bool DebrisRecord::subrecordDATA::hasCollisionData() const
{
  return ((flags & 0x01)!=0);
}

bool DebrisRecord::subrecordDATA::operator==(const DebrisRecord::subrecordDATA& other) const
{
  return ((percentage==other.percentage) and (modelPath==other.modelPath)
      and (flags==other.flags));
}

void DebrisRecord::subrecordDATA::clear()
{
  percentage = 0;
  modelPath.clear();
  flags = 0;
}

/* DATA_MODT_compound's functions */

bool DebrisRecord::DATA_MODT_compound::operator==(const DebrisRecord::DATA_MODT_compound& other) const
{
  return ((data==other.data) and (unknownMODT==other.unknownMODT));
}

/* DebrisRecord's functions */

DebrisRecord::DebrisRecord()
: BasicRecord(), editorID(""),
  unknownDATA_MODTs(std::vector<DATA_MODT_compound>())
{

}

DebrisRecord::~DebrisRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DebrisRecord::equals(const DebrisRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDATA_MODTs==other.unknownDATA_MODTs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DebrisRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  unsigned int i;
  for (i=0; i<unknownDATA_MODTs.size(); ++i)
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* length of percentage */
               +unknownDATA_MODTs[i].data.modelPath.length()+1 /* length of path +1 byte for NUL termination */
               +1 /* length of flags*/;
    if (unknownDATA_MODTs[i].unknownMODT.isPresent())
    {
      writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
                 +unknownDATA_MODTs[i].unknownMODT.getSize() /* size */;
    }//if
  }//for
  return writeSize;
}

bool DebrisRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cDEBR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  unsigned int i;
  for (i=0; i<unknownDATA_MODTs.size(); ++i)
  {
    //write EDID
    output.write((const char*) &cEDID, 4);
    //DATA's length
    subLength = unknownDATA_MODTs[i].data.modelPath.length()+3; //length of string, including NUL, +2 bytes
    output.write((const char*) &subLength, 2);
    //write DATA stuff
    output.write((const char*) &(unknownDATA_MODTs[i].data.percentage), 1);
    output.write(unknownDATA_MODTs[i].data.modelPath.c_str(), subLength-2);
    output.write((const char*) &(unknownDATA_MODTs[i].data.flags), 1);
    if (unknownDATA_MODTs[i].unknownMODT.isPresent())
    {
      if (!unknownDATA_MODTs[i].unknownMODT.saveToStream(output, cMODT))
      {
        std::cout << "Error while writing subrecord MODT of DEBR!\n";
        return false;
      }
    }//if
  }//for

  return output.good();
}
#endif

bool DebrisRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of DEBR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of DEBR!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownDATA_MODTs.clear();
  DATA_MODT_compound tempCompound;
  tempCompound.data.clear();
  tempCompound.unknownMODT.setPresence(false);
  bool hasUnpushedDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDATA:
           //is there a previous record?
           if (hasUnpushedDATA)
           {
             //push it to the back of the vector
             unknownDATA_MODTs.push_back(tempCompound);
             //... and clear data for next run
             tempCompound.data.clear();
             tempCompound.unknownMODT.setPresence(false);
             hasUnpushedDATA = false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511+2)
           {
             std::cout <<"Error: model path in sub record DATA of DEBR is longer than 511 characters!\n";
             return false;
           }
           //read DATA's stuff
           // ---- percentage
           in_File.read((char*) &(tempCompound.data.percentage), 1);
           // ---- model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength-2);
           // ---- flags
           in_File.read((char*) &(tempCompound.data.flags), 1);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of DEBR!\n";
             return false;
           }
           tempCompound.data.modelPath = std::string(buffer);
           bytesRead += subLength;
           hasUnpushedDATA = true;
           tempCompound.unknownMODT.setPresence(false);
           break;
      case cMODT:
           if (tempCompound.unknownMODT.isPresent())
           {
             std::cout << "Error: record DEBR seems to have more than one MODT subrecord per DATA!\n";
             return false;
           }
           if (!hasUnpushedDATA)
           {
             std::cout << "Error: record DEBR needs DATA before MODT!\n";
             return false;
           }
           //now read MODT
           if (!tempCompound.unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of DEBR!\n";
             return false;
           }
           bytesRead += (2 /* 2 bytes for length */ +tempCompound.unknownMODT.getSize() /*size*/);
           //push it to the back of the vector
           unknownDATA_MODTs.push_back(tempCompound);
           //... and clear data for next run
           tempCompound.data.clear();
           tempCompound.unknownMODT.setPresence(false);
           hasUnpushedDATA = false;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only DATA or MODT are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  if (hasUnpushedDATA)
  {
    //push it to the back of the vector
    unknownDATA_MODTs.push_back(tempCompound);
  }

  return in_File.good();
}

uint32_t DebrisRecord::getRecordType() const
{
  return cDEBR;
}

} //namespace
