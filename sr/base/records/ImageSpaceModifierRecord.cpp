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

#include "ImageSpaceModifierRecord.h"
#include <cstring>
#include <iostream>
#include <set>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ImageSpaceModifierRecord::ImageSpaceModifierRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownDNAM, 0, 244);
  unknownBNAM.setPresence(false);
  unknownVNAM.setPresence(false);
  unknownTNAM.setPresence(false);
  unknownNAM3.setPresence(false);
  unknownRNAM.setPresence(false);
  unknownSNAM.setPresence(false);
  unknownUNAM.setPresence(false);
  unknownNAM1.setPresence(false);
  unknownNAM2.setPresence(false);
  unknownWNAM.setPresence(false);
  unknownXNAM.setPresence(false);
  unknownYNAM.setPresence(false);
  unknownNAM4.setPresence(false);
  unknownx00IAD.setPresence(false);
  unknownx40IAD.setPresence(false);
  otherIADs.clear();
}

ImageSpaceModifierRecord::~ImageSpaceModifierRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ImageSpaceModifierRecord::equals(const ImageSpaceModifierRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownDNAM, other.unknownDNAM, 244)==0)
      and (unknownBNAM==other.unknownBNAM) and (unknownVNAM==other.unknownVNAM)
      and (unknownTNAM==other.unknownTNAM) and (unknownNAM3==other.unknownNAM3)
      and (unknownRNAM==other.unknownRNAM) and (unknownSNAM==other.unknownSNAM)
      and (unknownUNAM==other.unknownUNAM)
      and (unknownNAM1==other.unknownNAM1) and (unknownNAM2==other.unknownNAM2)
      and (unknownWNAM==other.unknownWNAM) and (unknownXNAM==other.unknownXNAM)
      and (unknownYNAM==other.unknownYNAM) and (unknownNAM4==other.unknownNAM4)
      and (unknownx00IAD==other.unknownx00IAD) and (unknownx40IAD==other.unknownx40IAD)
      and (otherIADs==other.otherIADs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ImageSpaceModifierRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +244 /* fixed length */
        +4 /* BNAM */ +2 /* 2 bytes for length */ +unknownBNAM.getSize() /* fixed length of 16 or 32 bytes */
        +4 /* VNAM */ +2 /* 2 bytes for length */ +unknownVNAM.getSize() /* fixed length of 16 or 32 bytes */
        +4 /* TNAM */ +2 /* 2 bytes for length */ +unknownTNAM.getSize() /* fixed length of 40 or 60 bytes*/
        +4 /* NAM3 */ +2 /* 2 bytes for length */ +unknownNAM3.getSize() /* fixed length of 40 or 120 bytes */
        +4 /* RNAM */ +2 /* 2 bytes for length */ +unknownRNAM.getSize() /* fixed length of 16 or 32 bytes*/
        +4 /* SNAM */ +2 /* 2 bytes for length */ +unknownSNAM.getSize() /* fixed length of 16 or 32 bytes*/
        +4 /* UNAM */ +2 /* 2 bytes for length */ +unknownUNAM.getSize() /* fixed length of 16 or 24 bytes*/
        +4 /* NAM1 */ +2 /* 2 bytes for length */ +unknownNAM1.getSize() /* fixed length of 16 or 24 bytes*/
        +4 /* NAM2 */ +2 /* 2 bytes for length */ +unknownNAM2.getSize() /* fixed length of 16 or 24 bytes*/
        +4 /* WNAM */ +2 /* 2 bytes for length */ +unknownWNAM.getSize() /* fixed length of 16 or 32 bytes*/
        +4 /* XNAM */ +2 /* 2 bytes for length */ +unknownXNAM.getSize() /* fixed length of 16 or 32 bytes*/
        +4 /* YNAM */ +2 /* 2 bytes for length */ +unknownYNAM.getSize() /* fixed length of 16 or 32 bytes*/
        +4 /* NAM4 */ +2 /* 2 bytes for length */ +unknownNAM4.getSize() /* fixed length of 16 or 24 bytes*/
        +4 /* \0IAD */ +2 /* 2 bytes for length */ +unknownx00IAD.getSize() /* fixed length of 16 or 24 bytes*/
        +4 /* @IAD */ +2 /* 2 bytes for length */ +unknownx40IAD.getSize() /* fixed length of 16 or 24 bytes*/;
  const unsigned int len = otherIADs.size();
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    writeSize = writeSize +4 /* label */ +2 /* 2 bytes for length */
               +otherIADs[i].subData.getSize() /* length */;
  }//for
  return writeSize;
}

bool ImageSpaceModifierRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cIMAD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 244; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) unknownDNAM, 244);

  if (unknownBNAM.isPresent())
  {
    //write BNAM
    if (!unknownBNAM.saveToStream(output, cBNAM))
    {
      std::cout << "Error while writing subrecord BNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //BNAM is required!
    std::cout << "Error: Cannot write subrecord BNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownVNAM.isPresent())
  {
    //write VNAM
    if (!unknownVNAM.saveToStream(output, cVNAM))
    {
      std::cout << "Error while writing subrecord VNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //VNAM is required!
    std::cout << "Error: Cannot write subrecord VNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownTNAM.isPresent())
  {
    //write TNAM
    if (!unknownTNAM.saveToStream(output, cTNAM))
    {
      std::cout << "Error while writing subrecord TNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //TNAM is required!
    std::cout << "Error: Cannot write subrecord TNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownNAM3.isPresent())
  {
    //write NAM3
    if (!unknownNAM3.saveToStream(output, cNAM3))
    {
      std::cout << "Error while writing subrecord NAM3 of IMAD!\n";
      return false;
    }
  }
  else
  {
    //NAM3 is required!
    std::cout << "Error: Cannot write subrecord NAM3 of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownRNAM.isPresent())
  {
    //write RNAM
    if (!unknownRNAM.saveToStream(output, cRNAM))
    {
      std::cout << "Error while writing subrecord RNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //RNAM is required!
    std::cout << "Error: Cannot write subrecord RNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownSNAM.isPresent())
  {
    //write SNAM
    if (!unknownSNAM.saveToStream(output, cSNAM))
    {
      std::cout << "Error while writing subrecord SNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //SNAM is required!
    std::cout << "Error: Cannot write subrecord SNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownUNAM.isPresent())
  {
    //write UNAM
    if (!unknownUNAM.saveToStream(output, cUNAM))
    {
      std::cout << "Error while writing subrecord UNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //UNAM is required!
    std::cout << "Error: Cannot write subrecord UNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownNAM1.isPresent())
  {
    //write NAM1
    if (!unknownNAM1.saveToStream(output, cNAM1))
    {
      std::cout << "Error while writing subrecord NAM1 of IMAD!\n";
      return false;
    }
  }
  else
  {
    //NAM1 is required!
    std::cout << "Error: Cannot write subrecord NAM1 of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownNAM2.isPresent())
  {
    //write NAM2
    if (!unknownNAM2.saveToStream(output, cNAM2))
    {
      std::cout << "Error while writing subrecord NAM2 of IMAD!\n";
      return false;
    }
  }
  else
  {
    //NAM2 is required!
    std::cout << "Error: Cannot write subrecord NAM2 of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownWNAM.isPresent())
  {
    //write WNAM
    if (!unknownWNAM.saveToStream(output, cWNAM))
    {
      std::cout << "Error while writing subrecord WNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //WNAM is required!
    std::cout << "Error: Cannot write subrecord WNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownXNAM.isPresent())
  {
    //write XNAM
    if (!unknownXNAM.saveToStream(output, cXNAM))
    {
      std::cout << "Error while writing subrecord XNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //XNAM is required!
    std::cout << "Error: Cannot write subrecord XNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownYNAM.isPresent())
  {
    //write YNAM
    if (!unknownYNAM.saveToStream(output, cYNAM))
    {
      std::cout << "Error while writing subrecord YNAM of IMAD!\n";
      return false;
    }
  }
  else
  {
    //YNAM is required!
    std::cout << "Error: Cannot write subrecord YNAM of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownNAM4.isPresent())
  {
    //write NAM4
    if (!unknownNAM4.saveToStream(output, cNAM4))
    {
      std::cout << "Error while writing subrecord NAM4 of IMAD!\n";
      return false;
    }
  }
  else
  {
    //NAM4 is required!
    std::cout << "Error: Cannot write subrecord NAM4 of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownx00IAD.isPresent())
  {
    //write \0IAD
    if (!unknownx00IAD.saveToStream(output, cx00IAD))
    {
      std::cout << "Error while writing subrecord x00IAD of IMAD!\n";
      return false;
    }
  }
  else
  {
    //\0IAD is required!
    std::cout << "Error: Cannot write subrecord x00IAD of IMAD, because it's missing!\n";
    return false;
  }

  if (unknownx40IAD.isPresent())
  {
    //write @IAD
    if (!unknownx40IAD.saveToStream(output, cx40IAD))
    {
      std::cout << "Error while writing subrecord @IAD of IMAD!\n";
      return false;
    }
  }
  else
  {
    //@IAD is required!
    std::cout << "Error: Cannot write subrecord @IAD of IMAD, because it's missing!\n";
    return false;
  }

  const unsigned int len = otherIADs.size();
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    if (!otherIADs[i].subData.saveToStream(output, otherIADs[i].subType))
    {
      std::cout << "Error while writing xIAD record of IMAD!\n";
      return false;
    }//if
  }//for

  return output.good();
}
#endif

bool ImageSpaceModifierRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
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
    std::cout <<"Error: sub record EDID of IMAD is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of IMAD!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadDNAM = false;
  unknownBNAM.setPresence(false);
  unknownVNAM.setPresence(false);
  unknownTNAM.setPresence(false);
  unknownNAM3.setPresence(false);
  unknownRNAM.setPresence(false);
  unknownSNAM.setPresence(false);
  unknownUNAM.setPresence(false);
  unknownNAM1.setPresence(false);
  unknownNAM2.setPresence(false);
  unknownWNAM.setPresence(false);
  unknownXNAM.setPresence(false);
  unknownYNAM.setPresence(false);
  unknownNAM4.setPresence(false);
  unknownx00IAD.setPresence(false);
  unknownx40IAD.setPresence(false);
  otherIADs.clear();
  std::set<int32_t> readxIADs;
  SubBlock tempSubBlock;
  uint32_t first;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: IMAD seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read DNAM
           if (!loadBufferFromStream(in_File, 244, unknownDNAM, cDNAM, false, bytesRead))
           {
             return false;
           }
           hasReadDNAM = true;
           break;
      case cBNAM:
           if (unknownBNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one BNAM subrecord!\n";
             return false;
           }
           //read BNAM
           if (!unknownBNAM.loadFromStream(in_File, cBNAM, false))
           {
             std::cout << "Error while reading subrecord BNAM of IMAD!\n";
             return false;
           }
           subLength = unknownBNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40)
               and (subLength!=48) and (subLength!=56) and (subLength!=64) and (subLength!=80)
               and (subLength!=88) and (subLength!=344))
           {
             std::cout <<"Error: sub record BNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 or 48 or 56 or 64 or 80 or 88 or 344 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cVNAM:
           if (unknownVNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one VNAM subrecord!\n";
             return false;
           }
           //read VNAM
           if (!unknownVNAM.loadFromStream(in_File, cVNAM, false))
           {
             std::cout << "Error while reading subrecord VNAM of IMAD!\n";
             return false;
           }
           subLength = unknownVNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40) and (subLength!=56)
             and (subLength!=64) and (subLength!=72) and (subLength!=80) and (subLength!=344))
           {
             std::cout <<"Error: sub record VNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 or 56 or 64 or 72 or 80 or 344 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cTNAM:
           if (unknownTNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!unknownTNAM.loadFromStream(in_File, cTNAM, false))
           {
             std::cout << "Error while reading subrecord TNAM of IMAD!\n";
             return false;
           }
           subLength = unknownTNAM.getSize();
           if ((subLength!=40) and (subLength!=60) and (subLength!=80) and (subLength!=100) and (subLength!=140) and (subLength!=280))
           {
             std::cout <<"Error: sub record TNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 40 or 60 or 80 or 100 or 140 or 280 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cNAM3:
           if (unknownNAM3.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one NAM3 subrecord!\n";
             return false;
           }
           //read NAM3
           if (!unknownNAM3.loadFromStream(in_File, cNAM3, false))
           {
             std::cout << "Error while reading subrecord NAM3 of IMAD!\n";
             return false;
           }
           subLength = unknownNAM3.getSize();
           if ((subLength!=40) and (subLength!=60) and (subLength!=80) and (subLength!=100)
                and (subLength!=120) and (subLength!=140) and (subLength!=160))
           {
             std::cout <<"Error: sub record NAM3 of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 40 or 60 or 80 or 100 or 120 or 140 or 160 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cRNAM:
           if (unknownRNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one RNAM subrecord!\n";
             return false;
           }
           //read RNAM
           if (!unknownRNAM.loadFromStream(in_File, cRNAM, false))
           {
             std::cout << "Error while reading subrecord RNAM of IMAD!\n";
             return false;
           }
           subLength = unknownRNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40)
               and (subLength!=48) and (subLength!=56) and (subLength!=72))
           {
             std::cout <<"Error: sub record RNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 or 48 or 56 or 72 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cSNAM:
           if (unknownSNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!unknownSNAM.loadFromStream(in_File, cSNAM, false))
           {
             std::cout << "Error while reading subrecord SNAM of IMAD!\n";
             return false;
           }
           subLength = unknownSNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40) and (subLength!=48) and (subLength!=64))
           {
             std::cout <<"Error: sub record SNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 or 48 or 64 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cUNAM:
           if (unknownUNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one UNAM subrecord!\n";
             return false;
           }
           //read UNAM
           if (!unknownUNAM.loadFromStream(in_File, cUNAM, false))
           {
             std::cout << "Error while reading subrecord UNAM of IMAD!\n";
             return false;
           }
           subLength = unknownUNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40) and (subLength!=88))
           {
             std::cout <<"Error: sub record UNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 or 88 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cNAM1:
           if (unknownNAM1.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one NAM1 subrecord!\n";
             return false;
           }
           //read NAM1
           if (!unknownNAM1.loadFromStream(in_File, cNAM1, false))
           {
             std::cout << "Error while reading subrecord NAM1 of IMAD!\n";
             return false;
           }
           subLength = unknownNAM1.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40))
           {
             std::cout <<"Error: sub record NAM1 of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cNAM2:
           if (unknownNAM2.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one NAM2 subrecord!\n";
             return false;
           }
           //read NAM2
           if (!unknownNAM2.loadFromStream(in_File, cNAM2, false))
           {
             std::cout << "Error while reading subrecord NAM2 of IMAD!\n";
             return false;
           }
           subLength = unknownNAM2.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=40))
           {
             std::cout <<"Error: sub record NAM2 of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 40 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cWNAM:
           if (unknownWNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //read WNAM
           if (!unknownWNAM.loadFromStream(in_File, cWNAM, false))
           {
             std::cout << "Error while reading subrecord WNAM of IMAD!\n";
             return false;
           }
           subLength = unknownWNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32) and (subLength!=48))
           {
             std::cout <<"Error: sub record WNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 or 48 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cXNAM:
           if (unknownXNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one XNAM subrecord!\n";
             return false;
           }
           //read XNAM
           if (!unknownXNAM.loadFromStream(in_File, cXNAM, false))
           {
             std::cout << "Error while reading subrecord WNAM of IMAD!\n";
             return false;
           }
           subLength = unknownXNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32))
           {
             std::cout <<"Error: sub record XNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cYNAM:
           if (unknownYNAM.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //read YNAM
           if (!unknownYNAM.loadFromStream(in_File, cYNAM, false))
           {
             std::cout << "Error while reading subrecord YNAM of IMAD!\n";
             return false;
           }
           subLength = unknownYNAM.getSize();
           if ((subLength!=16) and (subLength!=24) and (subLength!=32))
           {
             std::cout <<"Error: sub record YNAM of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 or 32 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cNAM4:
           if (unknownNAM4.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one NAM4 subrecord!\n";
             return false;
           }
           //read NAM4
           if (!unknownNAM4.loadFromStream(in_File, cNAM4, false))
           {
             std::cout << "Error while reading subrecord NAM4 of IMAD!\n";
             return false;
           }
           subLength = unknownNAM4.getSize();
           if ((subLength!=16) and (subLength!=24) /*and (subLength!=32)*/)
           {
             std::cout <<"Error: sub record NAM4 of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cx00IAD:
           if (unknownx00IAD.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one x00IAD subrecord!\n";
             return false;
           }
           //read \0IAD
           if (!unknownx00IAD.loadFromStream(in_File, cx00IAD, false))
           {
             std::cout << "Error while reading subrecord x00IAD of IMAD!\n";
             return false;
           }
           subLength = unknownx00IAD.getSize();
           if ((subLength!=16) and (subLength!=24))
           {
             std::cout <<"Error: sub record x00IAD of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      case cx40IAD:
           if (unknownx40IAD.isPresent())
           {
             std::cout << "Error: IMAD seems to have more than one @IAD subrecord!\n";
             return false;
           }
           //read @IAD
           if (!unknownx40IAD.loadFromStream(in_File, cx40IAD, false))
           {
             std::cout << "Error while reading subrecord x40IAD of IMAD!\n";
             return false;
           }
           subLength = unknownx40IAD.getSize();
           if ((subLength!=16) and (subLength!=24))
           {
             std::cout <<"Error: sub record x40IAD of IMAD has invalid length ("
                       <<subLength<<" bytes). Should be 16 or 24 bytes!\n";
             return false;
           }
           bytesRead += (2+subLength);
           break;
      default:
           //should be some ?IAD here
           if ((subRecName&0xFFFFFF00)!=cx00IAD)
           {
             std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                       << "\" found, but only DNAM, BNAM, VNAM, TNAM, NAM3, RNAM,"
                       << " SNAM, UNAM, NAM1, NAM2, WNAM, XNAM, YNAM or NAM4 are allowed here!\n";
             return false;
           }
           //some IAD here
           /*if ((subRecName==cx00IAD) and (!hasReadx00IAD))
           {
             //read \0IAD
             if (!loadBufferFromStream(in_File, 16, unknownx00IAD, cx00IAD, false, bytesRead))
             {
               return false;
             }
             hasReadx00IAD = true;
           }//if first \0IAD
           else
           {*/
             //some other ?IAD record
             first = (subRecName&0x000000FF);
             if (readxIADs.find(subRecName)!=readxIADs.end())
             {
               std::cout << "Error: IMAD seems to have more than one ?IAD "
                         << "subrecord! ?="<< first<<".\n";
               return false;
             }
             if (!(((first>=1) and (first<=20)) or ((first>='A') and (first<='T'))))
             {
               std::cout << "Error: unexpected ?IAD record type \""
                         << IntTo4Char(subRecName) << "\" found, but only values"
                         << " between AIAD and SIAD or x01IAD and x20IAD are "
                         << "allowed here! First value was "<<first<<".\n";
               return false;
             }

             //read it
             tempSubBlock.subType = subRecName;
             if (!tempSubBlock.subData.loadFromStream(in_File, subRecName, false))
             {
               std::cout << "Error while reading subrecord ?IAD of IMAD! ?="
                         << (subRecName&0x000000FF)<<".\n";
               return false;
             }
             //length check
             subLength = tempSubBlock.subData.getSize();
             if (
                ((subLength<16) or (subLength>56) or ((subLength%8)!=0))
                and
                ((subLength!=80) and (subLength!=104) and (subLength!=120) and (subLength!=136)
                 and (subLength!=144)  and (subLength!=152)and (subLength!=376) )
                )
             {
               std::cout <<"Error: sub record ?IAD of IMAD has invalid length ("
                         <<subLength<<" bytes). Should be between 16 and 56 "
                         <<" bytes and an integral multiple of 8 bytes, or 80 or 104 or 120 or 136 or 144 or 152 or 376 bytes! First is "
                         <<first<<".\n";
               return false;
             }
             //add it
             readxIADs.insert(subRecName);
             otherIADs.push_back(tempSubBlock);
             bytesRead += (2+subLength);
           //}//if other ?IAD
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDNAM and unknownBNAM.isPresent() and unknownVNAM.isPresent()
      and unknownTNAM.isPresent() and unknownNAM3.isPresent() and unknownRNAM.isPresent() and unknownSNAM.isPresent()
      and unknownUNAM.isPresent() and unknownNAM1.isPresent() and unknownNAM2.isPresent() and unknownWNAM.isPresent()
      and unknownXNAM.isPresent() and unknownYNAM.isPresent() and unknownNAM4.isPresent() and unknownx00IAD.isPresent()
      and unknownx40IAD.isPresent() and (otherIADs.size()==40) ))
  {
    std::cout << "Error: at least one required subrecord of IMAD is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ImageSpaceModifierRecord::getRecordType() const
{
  return cIMAD;
}

}; //namespace
