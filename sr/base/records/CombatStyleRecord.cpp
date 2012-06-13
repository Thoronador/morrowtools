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

#include "CombatStyleRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

CombatStyleRecord::CombatStyleRecord()
: BasicRecord()
{
  editorID = "";
  //subrecord CSGD
  csgdLen = gdl40Bytes;
  offensiveMult = 0.0f;
  defensiveMult = 0.0f;
  groupOffensiveMult = 1.0f;
  melee = 1.0f; //Equipment Score Mult
  magic = 1.0f; //Equipment Score Mult
  ranged = 1.0f; //Equipment Score Mult
  shout = 1.0f; //Equipment Score Mult
  unarmed = 1.0f; //Equipment Score Mult
  staff = 1.0f; //Equipment Score Mult
  avoidThreatChance = 0.2f;
  //end of subrecord CSGD
  //subrecord CSMD
  hasCSMD = false;
  unknownCSMDOne = 0.2f;
  unknownCSMDTwo = 0.2f;
  //end of subrecord CSMD
  unknownCSME.clear();
  //subrecord CSCR
  cscrLen = crl16Bytes;
  circleMult = 0.2f; //Close Range, Dueling
  fallbackMult = 0.2f; //Close Range, Dueling
  flankDistance = 0.2f; //Close Range, Flanking
  stalkTime = 0.2f; //Close Range, Flanking
  //end of subrecord CSCR
  hasCSLR = false;
  longRangeStrafeMult = 0.2f; //subrecord CSLR
  unknownCSFL.clear();
  hasDATA = false;
  unknownDATA = 0;
}

CombatStyleRecord::~CombatStyleRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CombatStyleRecord::equals(const CombatStyleRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (csgdLen==other.csgdLen)
      and (offensiveMult==other.offensiveMult) and (defensiveMult==other.defensiveMult)
      and (groupOffensiveMult==other.groupOffensiveMult) and (melee==other.melee)
      and (magic==other.magic) and (ranged==other.ranged)
      and (shout==other.shout) and (unarmed==other.unarmed)
      and (staff==other.staff) and (avoidThreatChance==other.avoidThreatChance)
      and (hasCSMD==other.hasCSMD) and (((unknownCSMDOne==other.unknownCSMDOne)
      and (unknownCSMDTwo==other.unknownCSMDTwo)) or (!hasCSMD))
      and (unknownCSME==other.unknownCSME) and (cscrLen==other.cscrLen)
      and (circleMult==other.circleMult) and (fallbackMult==other.fallbackMult)
      and (flankDistance==other.flankDistance) and (stalkTime==other.stalkTime)
      and (hasCSLR==other.hasCSLR)  and ((longRangeStrafeMult==other.longRangeStrafeMult) or (!hasCSLR))
      and (unknownCSFL==unknownCSFL)
      and (hasDATA==other.hasDATA) and ((unknownDATA==other.unknownDATA) or (!hasDATA)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CombatStyleRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  //CSGD
  if (csgdLen==gdl40Bytes)
  {
    writeSize = writeSize +4 /* CSGD */ +2 /* 2 bytes for length */ +40 /* fixed size of 40 bytes */;
  }
  else if (csgdLen==gdl32Bytes)
  {
    writeSize = writeSize +4 /* CSGD */ +2 /* 2 bytes for length */ +32 /* fixed size of 32 bytes */;
  }
  else //8 byte
  {
    writeSize = writeSize +4 /* CSGD */ +2 /* 2 bytes for length */ +8 /* fixed size of 8 bytes */;
  }
  if (hasCSMD)
  {
    writeSize = writeSize +4 /* CSMD */ +2 /* 2 bytes for length */ +8 /* fixed size of 8 bytes */;
  }//if CSMD
  if (!unknownCSME.empty())
  {
    writeSize = writeSize +4 /* CSME */ +2 /* 2 bytes for length */ +4*unknownCSME.size();
  }//if CSME
  //CSCR
  if (cscrLen==crl16Bytes)
  {
    writeSize = writeSize +4 /* CSCR */ +2 /* 2 bytes for length */ +16 /* fixed size of 16 bytes */;
  }
  else if (cscrLen==crl8Bytes)
  {
    writeSize = writeSize +4 /* CSCR */ +2 /* 2 bytes for length */ +8 /* fixed size of 8 bytes */;
  }
  if (hasCSLR)
  {
    writeSize = writeSize +4 /* CSLR */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (!unknownCSFL.empty())
  {
    writeSize = writeSize +4 /* CSFL */ +2 /* 2 bytes for length */ +4*unknownCSFL.size();
  }
  if (hasDATA)
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  return writeSize;
}

bool CombatStyleRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCSTY, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write CSGD
  output.write((const char*) &cCSGD, 4);
  //CSGD's length
  if (csgdLen==gdl40Bytes)
  {
    subLength = 40; //fixed
  }
  else if (csgdLen==gdl32Bytes)
  {
    subLength = 32; //fixed
  }
  else
  {
    subLength = 8; //fixed
  }
  output.write((const char*) &subLength, 2);
  //write CSGD's stuff
  output.write((const char*) &offensiveMult, 4);
  output.write((const char*) &defensiveMult, 4);
  if (csgdLen>gdl8Bytes)
  {
    output.write((const char*) &groupOffensiveMult, 4);
    output.write((const char*) &melee, 4);
    output.write((const char*) &magic, 4);
    output.write((const char*) &ranged, 4);
    output.write((const char*) &shout, 4);
    output.write((const char*) &unarmed, 4);
  }
  if (csgdLen==gdl40Bytes)
  {
    output.write((const char*) &staff, 4);
    output.write((const char*) &avoidThreatChance, 4);
  }

  if (hasCSMD)
  {
    //write CSMD
    output.write((const char*) &cCSMD, 4);
    //CSMD's length
    subLength = 8;
    output.write((const char*) &subLength, 2);
    //write CSMD's stuff
    output.write((const char*) &unknownCSMDOne, 4);
    output.write((const char*) &unknownCSMDTwo, 4);
  }//if CSMD

  unsigned int i;
  if (!unknownCSME.empty())
  {
    //write CSME
    output.write((const char*) &cCSME, 4);
    //CSME's length
    subLength = 4*unknownCSME.size();
    output.write((const char*) &subLength, 2);
    //write CSME's stuff
    for (i=0; i<unknownCSME.size(); ++i)
    {
      output.write((const char*) &unknownCSME[i], 4);
    }//for
  }//if CSME

  if (cscrLen!=crl0Bytes)
  {
    //write CSCR
    output.write((const char*) &cCSCR, 4);
    //CSCR's length
    if (cscrLen==crl16Bytes)
    {
      subLength = 16; //fixed
    }
    else
    {
      subLength = 8; //fixed
    }
    output.write((const char*) &subLength, 2);
    //write CSCR's stuff
    output.write((const char*) &circleMult, 4);
    output.write((const char*) &fallbackMult, 4);
    if (cscrLen==crl16Bytes)
    {
      output.write((const char*) &flankDistance, 4);
      output.write((const char*) &stalkTime, 4);
    }
  }//if CSCR present

  if (hasCSLR)
  {
    //write CSLR
    output.write((const char*) &cCSLR, 4);
    //CSLR's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write CSLR's stuff
    output.write((const char*) &longRangeStrafeMult, 4);
  }//if has CSLR

  if (!unknownCSFL.empty())
  {
    //write CSFL
    output.write((const char*) &cCSFL, 4);
    //CSFL's length
    subLength = 4*unknownCSFL.size();
    output.write((const char*) &subLength, 2);
    //write CSFL's stuff
    for (i=0; i<unknownCSFL.size(); ++i)
    {
      output.write((const char*) &unknownCSFL[i], 4);
    }//for
  }//if has CSFL

  if (hasDATA)
  {
    //write DATA
    output.write((const char*) &cDATA, 4);
    //DATA's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write DATA's stuff
    output.write((const char*) &unknownDATA, 4);
  }//if has DATA

  return output.good();
}
#endif

bool CombatStyleRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of CSTY is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of CSTY!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read CSGD
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cCSGD)
  {
    UnexpectedRecord(cCSGD, subRecName);
    return false;
  }
  //CSGD's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if ((subLength!=40) and (subLength!=32) and (subLength!=8))
  {
    std::cout <<"Error: sub record CSGD of CSTY has invalid length ("<<subLength
              << " bytes). Should be 40 or 32 bytes or 8 bytes!\n";
    return false;
  }
  //read CSGD's stuff
  if (subLength==40)
  {
    csgdLen = gdl40Bytes;
  }
  else if (subLength==32)
  {
    csgdLen = gdl32Bytes;
  }
  else
  {
    csgdLen = gdl8Bytes;
  }
  in_File.read((char*) &offensiveMult, 4);
  in_File.read((char*) &defensiveMult, 4);
  if (csgdLen>gdl8Bytes)
  {
    in_File.read((char*) &groupOffensiveMult, 4);
    in_File.read((char*) &melee, 4);
    in_File.read((char*) &magic, 4);
    in_File.read((char*) &ranged, 4);
    in_File.read((char*) &shout, 4);
    in_File.read((char*) &unarmed, 4);
  }
  else
  {
    groupOffensiveMult = 1.0f;
    melee = 1.0f;
    magic = 1.0f;
    ranged = 1.0f;
    shout = 1.0f;
    unarmed = 1.0f;
  }
  if (csgdLen==gdl40Bytes)
  {
    in_File.read((char*) &staff, 4);
    in_File.read((char*) &avoidThreatChance, 4);
  }
  else
  {
    staff = 1.0f;
    avoidThreatChance = 0.2f;
  }
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CSGD of CSTY!\n";
    return false;
  }

  hasCSMD = false;
  unknownCSMDOne = 0.2f;
  unknownCSMDTwo = 0.2f;
  unknownCSME.clear();
  float tempFloat = 0.0f;
  unsigned int i, count;
  bool hasReadCSCR = false;
  cscrLen = crl0Bytes;
  circleMult = 0.2f;
  fallbackMult = 0.2f;
  flankDistance = 0.2f;
  stalkTime = 0.2f;
  hasCSLR = false;
  longRangeStrafeMult = 0.2f;
  unknownCSFL.clear();
  hasDATA = false;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cCSMD:
           if (hasCSMD)
           {
             std::cout << "Error: record CSTY seems to have more than one CSMD subrecord!\n";
             return false;
           }
           //CSMD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record CSMD of CSTY has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read CSMD's stuff
           in_File.read((char*) &unknownCSMDOne, 4);
           in_File.read((char*) &unknownCSMDTwo, 4);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CSMD of CSTY!\n";
             return false;
           }
           bytesRead += 8;
           hasCSMD = true;
           break;
      case cCSME:
           if (!unknownCSME.empty())
           {
             std::cout << "Error: record CSTY seems to have more than one CSME subrecord!\n";
             return false;
           }
           //CSME's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength<4) or (subLength%4!=0))
           {
             std::cout <<"Error: sub record CSME of CSTY has invalid length ("<<subLength
                       << " bytes). Should be a positive integral multiple of four bytes!\n";
             return false;
           }
           tempFloat = 0.0f;
           count = subLength/4;
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &tempFloat, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord CSME of CSTY!\n";
               return false;
             }
             unknownCSME.push_back(tempFloat);
           }//for
           bytesRead += subLength;
           break;
      case cCSCR:
           if (hasReadCSCR)
           {
             std::cout << "Error: record CSTY seems to have more than one CSCR subrecord!\n";
             return false;
           }
           //CSCR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength!=16) and (subLength!=8))
           {
             std::cout <<"Error: sub record CSCR of CSTY has invalid length ("
                       <<subLength<<" bytes). Should be 16 bytes or 8 bytes!\n";
             return false;
           }
           //read CSCR's stuff
           if (subLength==16)
           {
             cscrLen = crl16Bytes;
           }
           else
           {
             cscrLen = crl8Bytes;
           }
           in_File.read((char*) &circleMult, 4);
           in_File.read((char*) &fallbackMult, 4);
           if (subLength==16)
           {
             in_File.read((char*) &flankDistance, 4);
             in_File.read((char*) &stalkTime, 4);
           }
           else
           {
             flankDistance = 0.2f;
             stalkTime = 0.2f;
           }
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CSCR of CSTY!\n";
             return false;
           }
           hasReadCSCR = true;
           break;
      case cCSLR:
           if (hasCSLR)
           {
             std::cout << "Error: record CSTY seems to have more than one CSLR subrecord!\n";
             return false;
           }
           //CSLR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout << "Error: sub record CSLR of CSTY has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           //read CSLR's stuff
           in_File.read((char*) &longRangeStrafeMult, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CSLR of CSTY!\n";
             return false;
           }
           hasCSLR = true;
           break;
      case cCSFL:
           if (!unknownCSFL.empty())
           {
             std::cout << "Error: record CSTY seems to have more than one CSFL subrecord!\n";
             return false;
           }
           //CSFL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength<4) or (subLength%4!=0))
           {
             std::cout <<"Error: sub record CSFL of CSTY has invalid length ("<<subLength
                       << " bytes). Should be a positive integral multiple of four bytes!\n";
             return false;
           }
           count = subLength/4;
           unknownCSFL.clear();
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &tempFloat, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord CSFL of CSTY!\n";
               return false;
             }
             unknownCSFL.push_back(tempFloat);
           }//for
           bytesRead += subLength;
           break;
      case cDATA:
           if (hasDATA)
           {
             std::cout << "Error: record CSTY seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, unknownDATA, false)) return false;
           hasDATA = true;
           bytesRead += 6;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only CSMD, CSME, CSCR, CSLR, CSFL or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  // --> no additional checks required, all other fields are optional

  return in_File.good();
}

uint32_t CombatStyleRecord::getRecordType() const
{
  return cCSTY;
}

    std::string editorID;
    //subrecord CSGD
    float offensiveMult;
    float defensiveMult;
    float groupOffensiveMult;
    float melee; //Equipment Score Mult
    float magic; //Equipment Score Mult
    float ranged; //Equipment Score Mult
    float shout; //Equipment Score Mult
    float unarmed; //Equipment Score Mult
    float staff; //Equipment Score Mult
    float avoidThreatChance;
    //end of subrecord CSGD
    std::vector<float> unknownCSME;
    //subrecord CSCR
    float circleMult; //Close Range, Dueling
    float fallbackMult; //Close Range, Dueling
    float flankDistance; //Close Range, Flanking
    float stalkTime; //Close Range, Flanking
    //end of subrecord CSCR
    float longRangeStrafeMult; //subrecord CSLR
    std::vector<float> unknownCSFL;
    uint32_t unknownDATA;

} //namespace
