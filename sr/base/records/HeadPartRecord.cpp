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

#include "HeadPartRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

bool HeadPartRecord::NAM0_NAM1_compound::operator==(const HeadPartRecord::NAM0_NAM1_compound& other) const
{
  return ((unknownNAM0==other.unknownNAM0) and (unknownNAM1==other.unknownNAM1));
}

HeadPartRecord::HeadPartRecord()
: BasicRecord(), editorID(""),
  fullName(LocalizedString()),
  modelPath(""),
  unknownDATA(0),
  unknownPNAM(0),
  unknownHNAMs(std::vector<uint32_t>()),
  unknownNAM0_NAM1s(std::vector<NAM0_NAM1_compound>()),
  hasTNAM(false), unknownTNAM(0),
  hasRNAM(false), unknownRNAM(0)
{
  unknownMODT.setPresence(false);
}

HeadPartRecord::~HeadPartRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool HeadPartRecord::equals(const HeadPartRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (fullName==other.fullName)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownDATA==other.unknownDATA) and (unknownPNAM==other.unknownPNAM)
      and (unknownHNAMs==other.unknownHNAMs) and (unknownNAM0_NAM1s==other.unknownNAM0_NAM1s)
      and (hasTNAM==other.hasTNAM) and ((unknownTNAM==other.unknownTNAM) or (!hasTNAM))
      and (hasRNAM==other.hasRNAM) and ((unknownRNAM==other.unknownRNAM) or (!hasRNAM)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t HeadPartRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* fixed size */
        +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +unknownHNAMs.size()*(4 /* HNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  if (fullName.isPresent())
  {
    writeSize += fullName.getWriteSize() /* FULL */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  unsigned int i;
  for (i=0; i<unknownNAM0_NAM1s.size(); ++i)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* NAM1 */ +2 /* 2 bytes for length */
               +unknownNAM0_NAM1s[i].unknownNAM1.length()+1 /* length of string +1 byte for NUL termination */;
  }//for
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasRNAM)
  {
    writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool HeadPartRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cHDPT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (fullName.isPresent())
  {
    //write FULL
    if (!fullName.saveToStream(output, cFULL))
      return false;
  }

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of HDPT!\n";
      return false;
    }
  }//if MODT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 1; // fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &unknownDATA, 1);

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4; // fixed
  output.write((const char*) &subLength, 2);
  //write PNAM
  output.write((const char*) &unknownPNAM, 4);

  unsigned int i;
  for (i=0; i<unknownHNAMs.size(); ++i)
  {
    //write HNAM
    output.write((const char*) &cHNAM, 4);
    //HNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write HNAM
    output.write((const char*) &(unknownHNAMs[i]), 4);
  }//for HNAM


  for (i=0; i<unknownNAM0_NAM1s.size(); ++i)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write NAM0
    output.write((const char*) &(unknownNAM0_NAM1s[i].unknownNAM0), 4);

    //write NAM1
    output.write((const char*) &cNAM1, 4);
    //NAM1's length
    subLength = unknownNAM0_NAM1s[i].unknownNAM1.length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM1
    output.write(unknownNAM0_NAM1s[i].unknownNAM1.c_str(), 4);
  }//for

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write TNAM
    output.write((const char*) &unknownTNAM, 4);
  }//if TNAM

  if (hasRNAM)
  {
    //write RNAM
    output.write((const char*) &cRNAM, 4);
    //RNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write RNAM
    output.write((const char*) &unknownRNAM, 4);
  }//if RNAM

  return output.good();
}
#endif

bool HeadPartRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

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
    std::cerr <<"Error: sub record EDID of HDPT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of HDPT!\n";
    return false;
  }
  editorID = std::string(buffer);

  fullName.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  bool hasReadPNAM = false;
  unknownHNAMs.clear();
  uint32_t tempUint32;
  unknownNAM0_NAM1s.clear();
  NAM0_NAM1_compound tempCompound;
  hasTNAM = false;
  hasRNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (fullName.isPresent())
           {
             std::cerr << "Error: HDPT seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!fullName.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: HDPT seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record MODL of HDPT is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord MODL of HDPT!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: HDPT seems to have more than one MODT subrecord.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of HDPT!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownMODT.size() /*data size*/;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: HDPT seems to have more than one DATA subrecord.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: sub record DATA of HDPT has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) &unknownDATA, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of HDPT!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: HDPT seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false)) return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cHNAM:
           //read HNAM
           tempUint32 = 0;
           if (!loadUint32SubRecordFromStream(in_File, cHNAM, tempUint32, false)) return false;
           bytesRead += 6;
           unknownHNAMs.push_back(tempUint32);
           break;
      case cNAM0:
           //read NAM0's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record NAM0 of HDPT has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read NAM0
           in_File.read((char*) &tempCompound.unknownNAM0, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM0 of HDPT!\n";
             return false;
           }

           //read NAM1
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cNAM1)
           {
             UnexpectedRecord(cNAM1, subRecName);
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record NAM1 of HDPT is longer than 511 characters!\n";
             return false;
           }
           //read NAM1's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM1 of HDPT!\n";
             return false;
           }
           tempCompound.unknownNAM1 = std::string(buffer);
           unknownNAM0_NAM1s.push_back(tempCompound);
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cerr << "Error: HDPT seems to have more than one TNAM subrecord.\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM, false)) return false;
           bytesRead += 6;
           hasTNAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cerr << "Error: HDPT seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //read RNAM
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, unknownRNAM, false)) return false;
           bytesRead += 6;
           hasRNAM = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, DATA, PNAM, "
                     << "HNAM, NAM0, TNAM or RNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDATA and hasReadPNAM))
  {
    std::cerr << "Error while reading HDPT record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t HeadPartRecord::getRecordType() const
{
  return cHDPT;
}

} //namespace
