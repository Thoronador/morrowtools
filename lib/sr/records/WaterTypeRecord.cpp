/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "WaterTypeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

WaterTypeRecord::WaterTypeRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  unknownNNAMs(std::vector<std::string>()),
  unknownANAM(0),
  unknownFNAM(0),
  hasMNAM(false), unknownMNAM(0),
  openSoundFormID(0),
  materialFormID(0),
  imageSpaceFormID(0),
  damagePerSecond(0), //subrecord DATA
  hasNAM0(false),
  hasNAM1(false),
  usesNAM2_3_4_as_NNAM(false)
{
  unknownDNAM.setPresence(false);
  memset(unknownGNAM, 0, 12);
  memset(unknownNAM0, 0, 12);
  memset(unknownNAM1, 0, 12);
}

WaterTypeRecord::~WaterTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool WaterTypeRecord::equals(const WaterTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (name==other.name)
      and (unknownNNAMs==other.unknownNNAMs) and (unknownANAM==other.unknownANAM)
      and (unknownFNAM==other.unknownFNAM) and (hasMNAM==other.hasMNAM)
      and ((unknownMNAM==other.unknownMNAM) or (!hasMNAM))
      and (openSoundFormID==other.openSoundFormID)
      and (materialFormID==other.materialFormID)
      and (imageSpaceFormID==other.imageSpaceFormID)
      and (damagePerSecond==other.damagePerSecond) and (unknownDNAM==other.unknownDNAM)
      and (memcmp(unknownGNAM, other.unknownGNAM, 12)==0) and (hasNAM0==other.hasNAM0)
      and ((memcmp(unknownNAM0, other.unknownNAM0, 12)==0) or (!hasNAM0))
      and (hasNAM1==other.hasNAM1) and ((memcmp(unknownNAM1, other.unknownNAM1, 12)==0)
      or (!hasNAM1))
      and (usesNAM2_3_4_as_NNAM==other.usesNAM2_3_4_as_NNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WaterTypeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* ANAM */ +2 /* 2 bytes for length */ +1 /* fixed length */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +2 /* fixed length */
        +4 /* GNAM */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }//if FULL
  if (hasMNAM)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */;
  }//if MNAM
  if (openSoundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if SNAM
  if (materialFormID!=0)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if TNAM
  if (imageSpaceFormID!=0)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if INAM
  if (unknownDNAM.isPresent())
  {
    writeSize = writeSize + 4 /* DNAM */ + 2 /* 2 bytes for length */ + unknownDNAM.size() /* length */;
  }//if DNAM
  if (hasNAM0)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  }//if NAM0
  if (hasNAM1)
  {
    writeSize = writeSize +4 /* NAM1 */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  }//if NAM1
  if (!unknownNNAMs.empty())
  {
    unsigned int i;
    for (i=0; i<unknownNNAMs.size(); ++i)
    {
      writeSize = writeSize +4 /* NNAM */ +2 /* 2 bytes for length */
                 +unknownNNAMs[i].length()+1 /* length of name +1 byte for NUL termination */;
    }//for
  }//if NNAMs
  return writeSize;
}

bool WaterTypeRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cWATR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (!usesNAM2_3_4_as_NNAM)
  {
    unsigned int i;
    for (i=0; i<unknownNNAMs.size(); ++i)
    {
      //write NNAM
      output.write((const char*) &cNNAM, 4);
      //NNAM's length
      subLength = unknownNNAMs[i].length()+1;
      output.write((const char*) &subLength, 2);
      //write NNAM's path
      output.write(unknownNNAMs[i].c_str(), subLength);
    }//for
  }//if NNAM

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write ANAM's stuff
  output.write((const char*) &unknownANAM, 1);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 1);

  if (hasMNAM)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 1;
    output.write((const char*) &subLength, 2);
    //write MNAM's stuff
    output.write((const char*) &unknownMNAM, 1);
  }//if MNAM

  if (openSoundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write Open Sound's form ID
    output.write((const char*) &openSoundFormID, 4);
  }

  if (materialFormID!=0)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write Material form ID
    output.write((const char*) &materialFormID, 4);
  }

  if (imageSpaceFormID!=0)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write image space form ID
    output.write((const char*) &imageSpaceFormID, 4);
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 2; //fixed
  output.write((const char*) &subLength, 2);
  //write damage per second
  output.write((const char*) &damagePerSecond, 2);

  if (unknownDNAM.isPresent())
  {
    //write DNAM
    if (!unknownDNAM.saveToStream(output, cDNAM))
    {
      std::cerr << "Error while writing subrecord DNAM of WATR!\n";
      return false;
    }
  }//if DNAM is present

  //write GNAM
  output.write((const char*) &cGNAM, 4);
  //GNAM's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write GNAM's stuff
  output.write((const char*) unknownGNAM, 12);

  if (hasNAM0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 12; //fixed
    output.write((const char*) &subLength, 2);
    //write NAM0's stuff
    output.write((const char*) unknownNAM0, 12);
  }//if has NAM0

  if (hasNAM1)
  {
    //write NAM1
    output.write((const char*) &cNAM1, 4);
    //NAM1's length
    subLength = 12; //fixed
    output.write((const char*) &subLength, 2);
    //write NAM1's stuff
    output.write((const char*) unknownNAM1, 12);
  }//if has NAM1

  if (usesNAM2_3_4_as_NNAM)
  {
    if (unknownNNAMs.size()!=3)
    {
      std::cerr << "Error: WATR should have three NAM2, NAM3, NAM4 records, but it has "
                << unknownNNAMs.size() << " instead!\n";
      return false;
    }
    //write NAM2
    output.write((const char*) &cNAM2, 4);
    //NAM2's length
    subLength = unknownNNAMs[0].length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM2's path
    output.write(unknownNNAMs[0].c_str(), subLength);

    //write NAM3
    output.write((const char*) &cNAM3, 4);
    //NAM3's length
    subLength = unknownNNAMs[1].length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM3's path
    output.write(unknownNNAMs[1].c_str(), subLength);

    //write NAM4
    output.write((const char*) &cNAM4, 4);
    //NAM4's length
    subLength = unknownNNAMs[2].length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM4's path
    output.write(unknownNNAMs[2].c_str(), subLength);
  }//if NAM2/NAM3/NAM4

  return output.good();
}
#endif

bool WaterTypeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of WATR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of WATR!\n";
    return false;
  }
  editorID = std::string(buffer);

  name.reset();
  unknownNNAMs.clear();
  usesNAM2_3_4_as_NNAM = false;
  bool hasReadANAM = false;
  bool hasReadFNAM = false;
  hasMNAM = false;
  openSoundFormID = 0;
  materialFormID = 0;
  imageSpaceFormID = 0;
  bool hasReadDATA = false;
  unknownDNAM.setPresence(false);
  bool hasReadGNAM = false;
  hasNAM0 = false;
  hasNAM1 = false;
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
             std::cerr << "Error: WATR seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading subrecord FULL of WATR!\n";
             return false;
           }//if
           break;
      case cNNAM:
           if (usesNAM2_3_4_as_NNAM)
           {
             std::cerr << "Error: WATR has NNAM subrecord, but there are already NAM2/NAM3/NAM4 subrecords!\n";
             return false;
           }
           if (unknownNNAMs.size()>=3)
           {
             std::cerr << "Error: WATR seems to have more than three NNAM subrecords!\n";
             return false;
           }
           //NNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record NNAM of WATR is longer than 511 characters!\n";
             return false;
           }
           //read NNAM's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NNAM of WATR!\n";
             return false;
           }//if
           unknownNNAMs.push_back(std::string(buffer));
           break;
      case cANAM:
           if (hasReadANAM)
           {
             std::cerr << "Error: WATR seems to have more than one ANAM subrecord!\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: subrecord ANAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read ANAM's stuff
           in_File.read((char*) &unknownANAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ANAM of WATR!\n";
             return false;
           }//if
           hasReadANAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: WATR seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: subrecord FNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of WATR!\n";
             return false;
           }//if
           hasReadFNAM = true;
           break;
      case cMNAM:
           if (hasMNAM)
           {
             std::cerr << "Error: WATR seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //MNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: subrecord MNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read MNAM's stuff
           in_File.read((char*) &unknownMNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord MNAM of WATR!\n";
             return false;
           }//if
           hasMNAM = true;
           break;
      case cSNAM:
           if (openSoundFormID!=0)
           {
             std::cerr << "Error: WATR seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, openSoundFormID, false))
             return false;
           bytesRead += 6;
           if (0==openSoundFormID)
           {
             std::cerr << "Error: subrecord SNAM of WATR is zero!\n";
             return false;
           }
           break;
      case cTNAM:
           if (materialFormID!=0)
           {
             std::cerr << "Error: WATR seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, materialFormID, false))
             return false;
           bytesRead += 6;
           if (0==materialFormID)
           {
             std::cerr << "Error: subrecord TNAM of WATR is zero!\n";
             return false;
           }
           break;
      case cINAM:
           if (imageSpaceFormID!=0)
           {
             std::cerr << "Error: WATR seems to have more than one INAM subrecord!\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, imageSpaceFormID, false))
             return false;
           bytesRead += 6;
           if (0==imageSpaceFormID)
           {
             std::cerr << "Error: subrecord INAM of WATR is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: WATR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cerr <<"Error: subrecord DATA of WATR has invalid length ("<<subLength
                       <<" bytes). Should be two bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &damagePerSecond, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of WATR!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      case cDNAM:
           if (unknownDNAM.isPresent())
           {
             std::cerr << "Error: WATR seems to have more than one DNAM subrecord!\n";
             return false;
           }
           // read DNAM
           if (!unknownDNAM.loadFromStream(in_File, cDNAM, false))
           {
             std::cerr << "Error while reading subrecord DNAM of WATR!\n";
             return false;
           }
           // check length
           if (unknownDNAM.size() != 228)
           {
             std::cerr << "Error: subrecord DNAM of WATR has invalid length ("
                       << unknownDNAM.size() << " bytes). Should be 228 bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + 228;
           break;
      case cGNAM:
           if (hasReadGNAM)
           {
             std::cerr << "Error: WATR seems to have more than one GNAM subrecord!\n";
             return false;
           }
           //GNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: subrecord GNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read GNAM's stuff
           in_File.read((char*) unknownGNAM, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord GNAM of WATR!\n";
             return false;
           }//if
           hasReadGNAM = true;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cerr << "Error: WATR seems to have more than one NAM0 subrecord!\n";
             return false;
           }
           //NAM0's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: subrecord NAM0 of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read NAM0's stuff
           in_File.read((char*) unknownNAM0, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM0 of WATR!\n";
             return false;
           }//if
           hasNAM0 = true;
           break;
      case cNAM1:
           if (hasNAM1)
           {
             std::cerr << "Error: WATR seems to have more than one NAM1 subrecord!\n";
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: subrecord NAM1 of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read NAM1's stuff
           in_File.read((char*) unknownNAM1, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM1 of WATR!\n";
             return false;
           }//if
           hasNAM1 = true;
           break;
      case cNAM2:
      case cNAM3:
      case cNAM4:
           if (!unknownNNAMs.empty() and !usesNAM2_3_4_as_NNAM)
           {
             std::cerr << "Error: WATR has NAM2/NAM3/NAM4 subrecord, but there are already NNAM subrecords!\n";
             return false;
           }
           if (unknownNNAMs.size()>=3)
           {
             std::cerr << "Error: WATR seems to have more than three NAM2/NAM3/NAM4 subrecords!\n";
             return false;
           }
           usesNAM2_3_4_as_NNAM = true;
           unknownNNAMs.push_back("");
           if (!loadString512FromStream(in_File, unknownNNAMs.back(), buffer, subRecName, false, bytesRead))
             return false;
           if (((subRecName==cNAM2) and (unknownNNAMs.size()!=1))
               or ((subRecName==cNAM3) and (unknownNNAMs.size()!=2))
               or ((subRecName==cNAM4) and (unknownNNAMs.size()!=3)))
           {
             std::cerr << "Error: subrecord "<<IntTo4Char(subRecName)<<" of WATR does not appear in expected sequence!\n";
             return false;
           }//if
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                    << "\" found, but only NNAM, ANAM, FNAM, MNAM, SNAM, INAM,"
                    << " DATA, DNAM, GNAM, NAM0, NAM1, NAM2, NAM3 or NAM4 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadANAM and hasReadFNAM and hasReadDATA and unknownDNAM.isPresent()
       and hasReadGNAM))
  {
    std::cerr << "Error: at least one subrecord of WATR is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t WaterTypeRecord::getRecordType() const
{
  return cWATR;
}

} //namespace
