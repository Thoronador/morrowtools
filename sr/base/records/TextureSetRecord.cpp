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

#include "TextureSetRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

TextureSetRecord::TextureSetRecord()
: BasicRecord(), editorID(""),
  texture00(""),
  texture01(""),
  texture02(""),
  texture03(""),
  texture04(""),
  texture05(""),
  texture07(""),
  unknownDNAM(0)
{
  memset(unknownOBND, 0, 12);
}

TextureSetRecord::~TextureSetRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool TextureSetRecord::equals(const TextureSetRecord& other) const
{
  if ((equalsBasic(other)) and (editorID==other.editorID)
      and (texture00==other.texture00) and (texture01==other.texture01)
      and (texture02==other.texture02) and (texture03==other.texture03)
      and (texture04==other.texture04) and (texture05==other.texture05)
      and (texture07==other.texture07)
      and (unknownDNAM==other.unknownDNAM) and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (hasDODT==other.hasDODT))
  {
    if (hasDODT) return (memcmp(unknownDODT, other.unknownDODT, 36)==0);
    return true;
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t TextureSetRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* TX00 */ +2 /* 2 bytes for length */
        +texture00.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  if (!texture01.empty())
  {
    writeSize = writeSize +4 /* TX01 */ +2 /* 2 bytes for length */
        +texture01.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture02.empty())
  {
    writeSize = writeSize +4 /* TX02 */ +2 /* 2 bytes for length */
        +texture02.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture03.empty())
  {
    writeSize = writeSize +4 /* TX03 */ +2 /* 2 bytes for length */
        +texture03.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture04.empty())
  {
    writeSize = writeSize +4 /* TX04 */ +2 /* 2 bytes for length */
        +texture04.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture05.empty())
  {
    writeSize = writeSize +4 /* TX05 */ +2 /* 2 bytes for length */
        +texture05.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture07.empty())
  {
    writeSize = writeSize +4 /* TX07 */ +2 /* 2 bytes for length */
        +texture07.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (hasDODT)
  {
    writeSize = writeSize +4 /* DODT */ +2 /* 2 bytes for length */ +36 /* fixed size */;
  }
  return writeSize;
}

bool TextureSetRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cTXST, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; /* fixed size */
  output.write((const char*) &subLength, 2);
  //write OBND's data
  output.write((const char*) unknownOBND, 12);

  //write TX00
  output.write((const char*) &cTX00, 4);
  //TX00's length
  subLength = texture00.length()+1;
  output.write((const char*) &subLength, 2);
  //write zeroeth texture
  output.write(texture00.c_str(), subLength);

  if (!texture01.empty())
  {
    //write TX01
    output.write((const char*) &cTX01, 4);
    //TX01's length
    subLength = texture01.length()+1;
    output.write((const char*) &subLength, 2);
    //write first texture
    output.write(texture01.c_str(), subLength);
  }//if tex01

  if (!texture02.empty())
  {
    //write TX02
    output.write((const char*) &cTX02, 4);
    //TX02's length
    subLength = texture02.length()+1;
    output.write((const char*) &subLength, 2);
    //write second texture
    output.write(texture02.c_str(), subLength);
  }//if tex02

  if (!texture03.empty())
  {
    //write TX03
    output.write((const char*) &cTX03, 4);
    //TX03's length
    subLength = texture03.length()+1;
    output.write((const char*) &subLength, 2);
    //write third texture
    output.write(texture03.c_str(), subLength);
  }//if tex03

  if (!texture04.empty())
  {
    //write TX04
    output.write((const char*) &cTX04, 4);
    //TX04's length
    subLength = texture04.length()+1;
    output.write((const char*) &subLength, 2);
    //write 4th texture
    output.write(texture04.c_str(), subLength);
  }//if tex04

  if (!texture05.empty())
  {
    //write TX05
    output.write((const char*) &cTX05, 4);
    //TX05's length
    subLength = texture05.length()+1;
    output.write((const char*) &subLength, 2);
    //write fifth texture
    output.write(texture05.c_str(), subLength);
  }//if tex05

  if (!texture07.empty())
  {
    //write TX07
    output.write((const char*) &cTX07, 4);
    //TX07's length
    subLength = texture07.length()+1;
    output.write((const char*) &subLength, 2);
    //write seventh texture
    output.write(texture07.c_str(), subLength);
  }//if tex07

  if (hasDODT)
  {
    //write DODT
    output.write((const char*) &cDODT, 4);
    //DODT's length
    subLength = 36; /* fixed size */
    output.write((const char*) &subLength, 2);
    //write DODT's data
    output.write((const char*) unknownDODT, 36);
  }//if DODT

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 2; /* fixed size */
  output.write((const char*) &subLength, 2);
  //write DNAM's data
  output.write((const char*) (&unknownDNAM), 2);

  return output.good();
}
#endif

bool TextureSetRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of TXST is longer than 511 characters!\n";
    return false;
  }
  //read TXST's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of TXST!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of TXST has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of TXST!\n";
    return false;
  }

  //read TX00
  if (!loadString512FromStream(in_File, texture00, buffer, cTX00, true, bytesRead))
    return false;

  //DODT or DNAM next
  bool hasReadDNAM = false;
  bool hasReadDODT = false;
  hasDODT = false;
  texture01.clear();
  texture02.clear();
  texture03.clear();
  texture04.clear();
  texture05.clear();
  texture07.clear();
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: record TXST seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record DNAM of TXST has invalid length ("
                       <<subLength <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &unknownDNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of TXST!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cDODT:
           if (hasReadDODT)
           {
             std::cout << "Error: record TXST seems to have more than one DODT subrecord!\n";
             return false;
           }
           //DODT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=36)
           {
             std::cout <<"Error: sub record DODT of TXST has invalid length ("<<subLength
                       <<" bytes). Should be 36 bytes.\n";
             return false;
           }
           //read DODT's stuff
           in_File.read((char*) &unknownDODT, 36);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DODT of TXST!\n";
             return false;
           }
           hasReadDODT = true;
           hasDODT = true;
           break;
      case cTX01:
           if (!texture01.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX01 subrecord!\n";
             return false;
           }
           //read TX01
           if (!loadString512FromStream(in_File, texture01, buffer, cTX01, false, bytesRead))
             return false;
           break;
      case cTX02:
           if (!texture02.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX02 subrecord!\n";
             return false;
           }
           //read TX02
           if (!loadString512FromStream(in_File, texture02, buffer, cTX02, false, bytesRead))
             return false;
           break;
      case cTX03:
           if (!texture03.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX03 subrecord!\n";
             return false;
           }
           //read TX03
           if (!loadString512FromStream(in_File, texture03, buffer, cTX03, false, bytesRead))
             return false;
           break;
      case cTX04:
           if (!texture04.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX04 subrecord!\n";
             return false;
           }
           //read TX04
           if (!loadString512FromStream(in_File, texture04, buffer, cTX04, false, bytesRead))
             return false;
           break;
      case cTX05:
           if (!texture05.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX05 subrecord!\n";
             return false;
           }
           //read TX05
           if (!loadString512FromStream(in_File, texture05, buffer, cTX05, false, bytesRead))
             return false;
           break;
      case cTX07:
           if (!texture07.empty())
           {
             std::cout << "Error: record TXST seems to have more than one TX07 subrecord!\n";
             return false;
           }
           //read TX07
           if (!loadString512FromStream(in_File, texture07, buffer, cTX07, false, bytesRead))
             return false;
           break;
      default:
           std::cout << "Error: expected record name DNAM, DODT, TX01, TX02, "
                     << "TX03, TX04, TX05 or TX07 was not found. Instead, \""
                     << IntTo4Char(subRecName)<<"\" was found.\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

uint32_t TextureSetRecord::getRecordType() const
{
  return cTXST;
}

} //namespace
