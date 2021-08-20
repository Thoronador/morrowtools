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

#include "LocationRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

LocationRecord::LocationRecord()
: BasicRecord(), editorID(""),
  unknownLCECs(std::vector<BinarySubRecord>()),
  name(LocalizedString()),
  keywordArray(std::vector<uint32_t>()),
  hasPNAM(false), unknownPNAM(0),
  hasFNAM(false), unknownFNAM(0),
  hasMNAM(false), unknownMNAM(0),
  hasRNAM(false), unknownRNAM(0),
  hasNAM0(false), unknownNAM0(0),
  hasCNAM(false), unknownCNAM(0),
  hasNAM1(false), unknownNAM1(0)
{
  unknownLCSR.setPresence(false);
  unknownLCPR.setPresence(false);
  unknownLCEP.setPresence(false);
  unknownLCUN.setPresence(false);
  unknownLCID.setPresence(false);
}

LocationRecord::~LocationRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LocationRecord::equals(const LocationRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownLCSR==other.unknownLCSR) and (unknownLCPR==other.unknownLCPR)
      and (unknownLCECs==other.unknownLCECs) and (unknownLCEP==other.unknownLCEP)
      and (unknownLCUN==other.unknownLCUN) and (unknownLCID==other.unknownLCID)
      and (name==other.name)
      and (keywordArray==other.keywordArray)
      and (hasPNAM==other.hasPNAM) and ((unknownPNAM==other.unknownPNAM) or (!hasPNAM))
      and (hasFNAM==other.hasFNAM) and ((unknownFNAM==other.unknownFNAM) or (!hasFNAM))
      and (hasMNAM==other.hasMNAM) and ((unknownMNAM==other.unknownMNAM) or (!hasMNAM))
      and (hasRNAM==other.hasRNAM) and ((unknownRNAM==other.unknownRNAM) or (!hasRNAM))
      and (hasNAM0==other.hasNAM0) and ((unknownNAM0==other.unknownNAM0) or (!hasNAM0))
      and (hasCNAM==other.hasCNAM) and ((unknownCNAM==other.unknownCNAM) or (!hasCNAM))
      and (hasNAM1==other.hasNAM1) and ((unknownNAM1==other.unknownNAM1) or (!hasNAM1)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LocationRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (unknownLCSR.isPresent())
  {
    writeSize = writeSize + 4 /* LCSR */ + 2 /* 2 bytes for length */ + unknownLCSR.size() /* size */;
  }
  if (unknownLCPR.isPresent())
  {
    writeSize = writeSize + 4 /* LCPR */ + 2 /* 2 bytes for length */ + unknownLCPR.size() /* size */;
  }
  unsigned int i;
  for (i=0; i<unknownLCECs.size(); ++i)
  {
    writeSize = writeSize + 4 /* LCEC */ + 2 /* 2 bytes for length */ + unknownLCECs[i].size() /* size */;
  }//for
  if (unknownLCEP.isPresent())
  {
    writeSize = writeSize + 4 /* LCEP */ + 2 /* 2 bytes for length */ + unknownLCEP.size() /* size */;
  }
  if (unknownLCUN.isPresent())
  {
    writeSize = writeSize + 4 /* LCUN */ + 2 /* 2 bytes for length */ + unknownLCUN.size() /* size */;
  }
  if (unknownLCID.isPresent())
  {
    writeSize = writeSize + 4 /* LCID */ + 2 /* 2 bytes for length */ + unknownLCID.size() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /* 4 bytes per element */;
  }
  if (hasPNAM)
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasFNAM)
  {
    writeSize = writeSize +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasMNAM)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasRNAM)
  {
    writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasNAM0)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasCNAM)
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasNAM1)
  {
    writeSize = writeSize +4 /* NAM1 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool LocationRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLCTN, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownLCSR.isPresent())
  {
    //write LCSR
    if (!unknownLCSR.saveToStream(output, useACxx() ? cACSR : cLCSR))
    {
      std::cerr << "Error while writing subrecord LCSR of LCTN!\n";
      return false;
    }
  }//if LCSR

  if (unknownLCPR.isPresent())
  {
    //write LCPR
    if (!unknownLCPR.saveToStream(output, useACxx() ? cACPR : cLCPR))
    {
      std::cerr << "Error while writing subrecord LCPR of LCTN!\n";
      return false;
    }
  }//if LCPR

  unsigned int i;
  for (i=0; i<unknownLCECs.size(); ++i)
  {
    //write LCEC
    if (!unknownLCECs[i].saveToStream(output, useACxx() ? cACEC : cLCEC))
    {
      std::cerr << "Error while writing subrecord LCEC of LCTN!\n";
      return false;
    }
  }//for

  if (unknownLCEP.isPresent())
  {
    //write LCEP
    if (!unknownLCEP.saveToStream(output, cLCEP))
    {
      std::cerr << "Error while writing subrecord LCEP of LCTN!\n";
      return false;
    }
  }//if LCEP

  if (unknownLCUN.isPresent())
  {
    //write LCUN
    if (!unknownLCUN.saveToStream(output, useACxx() ? cACUN : cLCUN))
    {
      std::cerr << "Error while writing subrecord LCUN of LCTN!\n";
      return false;
    }
  }//if LCUN

  if (unknownLCID.isPresent())
  {
    //write LCID
    if (!unknownLCID.saveToStream(output, useACxx() ? cACID : cLCID))
    {
      std::cerr << "Error while writing subrecord LCID of LCTN!\n";
      return false;
    }
  }//if LCID

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if FULL

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write keyword count
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    //write keywords
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  if (hasPNAM)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write PNAM
    output.write((const char*) &unknownPNAM, 4);
  }//if PNAM

  if (hasFNAM)
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write FNAM
    output.write((const char*) &unknownFNAM, 4);
  }//if FNAM

  if (hasMNAM)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write MNAM
    output.write((const char*) &unknownMNAM, 4);
  }//if MNAM

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

  if (hasNAM0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write NAM0
    output.write((const char*) &unknownNAM0, 4);
  }//if NAM0

  if (hasCNAM)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write CNAM
    output.write((const char*) &unknownCNAM, 4);
  }//if CNAM

  if (hasNAM1)
  {
    //write NAM1
    output.write((const char*) &cNAM1, 4);
    //NAM1's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write NAM1
    output.write((const char*) &unknownNAM1, 4);
  }//if NAM1

  return output.good();
}
#endif

bool LocationRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of LCTN is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of LCTN!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownLCSR.setPresence(false);
  unknownLCPR.setPresence(false);
  unknownLCECs.clear();
  BinarySubRecord tempBin;
  unknownLCEP.setPresence(false);
  unknownLCUN.setPresence(false);
  unknownLCID.setPresence(false);
  name.reset();
  keywordArray.clear();
  uint32_t i, k_Size, tempUint32;
  hasPNAM = false;
  hasFNAM = false;
  hasMNAM = false;
  hasRNAM = false;
  hasNAM0 = false;
  hasCNAM = false;
  hasNAM1 = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cLCSR:
      case cACSR:
           if (unknownLCSR.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one LCSR/ACSR subrecord.\n";
             return false;
           }
           if (useACxx() != (subRecName == cACSR))
           {
             std::cerr << "Error: LCSR/ACSR mismatch in LCTN record!\n";
             return false;
           }
           // read LCSR/ACSR
           if (!unknownLCSR.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord "<<IntTo4Char(subRecName)<<" of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownLCSR.size() /*data size*/;
           break;
      case cLCPR:
      case cACPR:
           if (unknownLCPR.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one LCPR/ACPR subrecord.\n";
             return false;
           }
           if (useACxx() != (subRecName == cACPR))
           {
             std::cerr << "Error: LCPR/ACPR mismatch in LCTN record!\n";
             return false;
           }
           // read LCPR/ACPR
           if (!unknownLCPR.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord " << IntTo4Char(subRecName) << " of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ +unknownLCPR.size() /*data size*/;
           break;
      case cLCEC:
      case cACEC:
           if (useACxx() != (subRecName == cACEC))
           {
             std::cerr << "Error: LCEC/ACEC mismatch in LCTN record!\n";
             return false;
           }
           tempBin.setPresence(false);
           // read LCEC/ACEC
           if (!tempBin.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord "<< IntTo4Char(subRecName) << " of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + tempBin.size() /*data size*/;
           unknownLCECs.push_back(tempBin);
           break;
      case cLCEP:
      case cACEP:
           if (unknownLCEP.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one LCEP/ACEP subrecord.\n";
             return false;
           }
           if (useACxx() != (subRecName == cACEP))
           {
             std::cerr << "Error: LCEP/ACEP mismatch in LCTN record!\n";
             return false;
           }
           // read LCEP
           if (!unknownLCEP.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord " << IntTo4Char(subRecName) << " of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownLCEP.size() /*data size*/;
           break;
      case cLCUN:
      case cACUN:
           if (unknownLCUN.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one LCUN/ACUN subrecord.\n";
             return false;
           }
           if (useACxx()!=(subRecName==cACUN))
           {
             std::cerr << "Error: LCUN/ACUN mismatch in LCTN record!\n";
             return false;
           }
           // read LCUN/ACUN
           if (!unknownLCUN.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord " << IntTo4Char(subRecName) << " of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownLCUN.size() /*data size*/;
           break;
      case cLCID:
      case cACID:
           if (unknownLCID.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one LCID/ACID subrecord.\n";
             return false;
           }
           if (useACxx()!=(subRecName==cACID))
           {
             std::cerr << "Error: LCID/ACID mismatch in LCTN record!\n";
             return false;
           }
           //read LCID/ACID
           if (!unknownLCID.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while reading subrecord "<<IntTo4Char(subRecName)<<" of LCTN!\n";
             return false;
           }
           bytesRead = bytesRead + 2 /*length value*/ + unknownLCID.size() /*data size*/;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: LCTN seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cerr << "Error: LCTN seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;
           if (k_Size==0)
           {
             std::cerr << "Error: LCTN's KSIZ value is zero, but that's not allowed!\n";
             return false;
           }

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cerr <<"Error: sub record KWDA of LCTN has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes.\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &tempUint32, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cerr << "Error while reading subrecord KWDA of LCTN!\n";
               return false;
             }
             keywordArray.push_back(tempUint32);
           }//for
           break;
      case cPNAM:
           if (hasPNAM)
           {
             std::cerr << "Error: LCTN seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false)) return false;
           bytesRead += 6;
           hasPNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cerr << "Error: LCTN seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false)) return false;
           bytesRead += 6;
           hasFNAM = true;
           break;
      case cMNAM:
           if (hasMNAM)
           {
             std::cerr << "Error: LCTN seems to have more than one MNAM subrecord.\n";
             return false;
           }
           //read MNAM
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM, false)) return false;
           bytesRead += 6;
           hasMNAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cerr << "Error: LCTN seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //read RNAM
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, unknownRNAM, false)) return false;
           bytesRead += 6;
           hasRNAM = true;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cerr << "Error: LCTN seems to have more than one NAM0 subrecord.\n";
             return false;
           }
           //read NAM0
           if (!loadUint32SubRecordFromStream(in_File, cNAM0, unknownNAM0, false)) return false;
           bytesRead += 6;
           hasNAM0 = true;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cerr << "Error: LCTN seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false)) return false;
           bytesRead += 6;
           hasCNAM = true;
           break;
      case cNAM1:
           if (hasNAM1)
           {
             std::cerr << "Error: LCTN seems to have more than one NAM1 subrecord.\n";
             return false;
           }
           //read NAM1
           if (!loadUint32SubRecordFromStream(in_File, cNAM1, unknownNAM1, false)) return false;
           bytesRead += 6;
           hasNAM1 = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only LCSR, LCPR, LCEC, LCEP, LCUN, LCID,"
                     << " FULL, KSIZ, PNAM, FNAM, MNAM, RNAM, NAM0, CNAM or "
                     << "NAM1 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

uint32_t LocationRecord::getRecordType() const
{
  return cLCTN;
}

bool LocationRecord::useACxx() const
{
  return (headerVersion>=43);
}

} //namespace
