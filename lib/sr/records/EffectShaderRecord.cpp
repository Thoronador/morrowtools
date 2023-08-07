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

#include "EffectShaderRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

EffectShaderRecord::EffectShaderRecord()
: BasicRecord(), editorID(""),
  unknownICON(""),
  unknownICO2(""),
  unknownNAM7(""),
  hasNAM8(false), unknownNAM8(""),
  hasNAM9(false), unknownNAM9("")
{
  unknownDATA.setPresence(false);
}

EffectShaderRecord::~EffectShaderRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool EffectShaderRecord::equals(const EffectShaderRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownICON==other.unknownICON) and (unknownICO2==other.unknownICO2)
      and (unknownNAM7==other.unknownNAM7)
      and (hasNAM8==other.hasNAM8) and ((unknownNAM8==other.unknownNAM8) or (!hasNAM8))
      and (hasNAM9==other.hasNAM9) and ((unknownNAM9==other.unknownNAM9) or (!hasNAM9))
      and (unknownDATA==other.unknownDATA));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t EffectShaderRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* ICON */ +2 /* 2 bytes for length */
        +unknownICON.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* ICO2 */ +2 /* 2 bytes for length */
        +unknownICO2.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* NAM7 */ +2 /* 2 bytes for length */
        +unknownNAM7.length()+1 /* length of path +1 byte for NUL termination */;
  if (hasNAM8)
  {
    writeSize = writeSize +4 /* NAM8 */ +2 /* 2 bytes for length */
        +unknownNAM8.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (hasNAM9)
  {
    writeSize = writeSize +4 /* NAM9 */ +2 /* 2 bytes for length */
        +unknownNAM9.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize + 4 /* DATA */ + 2 /* 2 bytes for length */ + unknownDATA.size() /* size */;
  }
  return writeSize;
}

bool EffectShaderRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cEFSH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write ICON
  output.write((const char*) &cICON, 4);
  //ICON's length
  subLength = unknownICON.length()+1;
  output.write((const char*) &subLength, 2);
  //write ICON's stuff
  output.write(unknownICON.c_str(), subLength);

  //write ICO2
  output.write((const char*) &cICO2, 4);
  //ICO2's length
  subLength = unknownICO2.length()+1;
  output.write((const char*) &subLength, 2);
  //write ICO2's stuff
  output.write(unknownICO2.c_str(), subLength);

  //write NAM7
  output.write((const char*) &cNAM7, 4);
  //NAM7's length
  subLength = unknownNAM7.length()+1;
  output.write((const char*) &subLength, 2);
  //write NAM7's stuff
  output.write(unknownNAM7.c_str(), subLength);

  if (hasNAM8)
  {
    //write NAM8
    output.write((const char*) &cNAM8, 4);
    //NAM8's length
    subLength = unknownNAM8.length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM8's stuff
    output.write(unknownNAM8.c_str(), subLength);
  }//if NAM8

  if (hasNAM9)
  {
    //write NAM9
    output.write((const char*) &cNAM9, 4);
    //NAM9's length
    subLength = unknownNAM9.length()+1;
    output.write((const char*) &subLength, 2);
    //write NAM9's stuff
    output.write(unknownNAM9.c_str(), subLength);
  }//if NAM9

  if (unknownDATA.isPresent())
  {
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cerr << "Error while writing subrecord DATA of EFSH!\n";
      return false;
    }
  }//if
  else
  {
    //EFSH should always have a DATA subrecord, so quit here, the game won't be
    // able to read the written stuff in that case anyway.
    std::cerr << "Error while writing subrecord DATA of EFSH: no DATA subrecord is present!\n";
    return false;
  }

  return output.good();
}
#endif

bool EffectShaderRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of EFSH is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of EFSH!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadICON = false;
  unknownICON.clear();
  bool hasReadICO2 = false;
  unknownICO2.clear();
  bool hasReadNAM7 = false;
  unknownNAM7.clear();
  hasNAM8 = false; unknownNAM8.clear();
  hasNAM9 = false; unknownNAM9.clear();
  unknownDATA.setPresence(false);
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cICON:
           if (hasReadICON)
           {
             std::cerr << "Error: record EFSH seems to have more than one ICON subrecord!\n";
             return false;
           }
           //read ICON
           if (!loadString512FromStream(in_File, unknownICON, buffer, cICON, false, bytesRead))
             return false;
           hasReadICON = true;
           break;
      case cICO2:
           if (hasReadICO2)
           {
             std::cerr << "Error: record EFSH seems to have more than one ICO2 subrecord!\n";
             return false;
           }
           //read ICO2
           if (!loadString512FromStream(in_File, unknownICO2, buffer, cICO2, false, bytesRead))
             return false;
           hasReadICO2 = true;
           break;
      case cNAM7:
           if (hasReadNAM7)
           {
             std::cerr << "Error: record EFSH seems to have more than one NAM7 subrecord!\n";
             return false;
           }
           //read NAM7
           if (!loadString512FromStream(in_File, unknownNAM7, buffer, cNAM7, false, bytesRead))
             return false;
           hasReadNAM7 = true;
           break;
      case cNAM8:
           if (hasNAM8)
           {
             std::cerr << "Error: record EFSH seems to have more than one NAM8 subrecord!\n";
             return false;
           }
           //read NAM8
           if (!loadString512FromStream(in_File, unknownNAM8, buffer, cNAM8, false, bytesRead))
             return false;
           hasNAM8 = true;
           break;
      case cNAM9:
           if (hasNAM9)
           {
             std::cerr << "Error: record EFSH seems to have more than one NAM9 subrecord!\n";
             return false;
           }
           //read NAM9
           if (!loadString512FromStream(in_File, unknownNAM9, buffer, cNAM9, false, bytesRead))
             return false;
           hasNAM9 = true;
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cerr << "Error: record EFSH seems to have more than one DATA subrecord!\n";
             return false;
           }
           // load DATA
           if (!unknownDATA.loadFromStream(in_File, cDATA, false))
           {
             std::cerr << "Error while reading subrecord NAM9 of EFSH!\n";
             return false;
           }

           // check length of DATA
           if ((unknownDATA.size() > 400) || (unknownDATA.size() < 308) || ((unknownDATA.size() % 4) != 0))
           {
             std::cerr << "Error: sub record DATA of EFSH has invalid length ("
                       << unknownDATA.size() << " bytes). Should be between 400 and 308 bytes and an integral multiple of four bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownDATA.size();
           break;
      default:
           std::cerr << "Error: unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only ICON, ICO2, NAM7, NAM8, NAM9 or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadICON and hasReadICO2 and hasReadNAM7 and unknownDATA.isPresent()))
  {
    std::cerr << "Error while reading EFSH record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t EffectShaderRecord::getRecordType() const
{
  return cEFSH;
}

} //namespace
