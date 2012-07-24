/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "KeyRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

KeyRecord::KeyRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  hasYNAM = false;
  unknownYNAM = 0;
  hasZNAM = false;
  unknownZNAM = 0;
  keywordArray.clear();
  memset(unknownDATA, 0, 8);
}

KeyRecord::~KeyRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool KeyRecord::equals(const KeyRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID) and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (nameStringID==other.nameStringID) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (hasYNAM==other.hasYNAM)
      and ((unknownYNAM==other.unknownYNAM) or (!hasYNAM)) and (hasZNAM==other.hasZNAM)
      and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM)) and (keywordArray==other.keywordArray)
      and (memcmp(unknownDATA, other.unknownDATA, 8)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeyRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* length */;
  }//if VMAD
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* length */;
  }//if MODT
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (hasYNAM)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  return writeSize;
}

bool KeyRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cKEYM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing VMAD of KEYM!";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  //write FULL
  output.write((const char*) &cFULL, 4);
  //FULL's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FULL's data
  output.write((const char*) &nameStringID, 4);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing MODT of KEYM!";
      return false;
    }
  }//if MODT

  if (hasYNAM)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write YNAM's data
    output.write((const char*) &unknownYNAM, 4);
  }//if has YNAM

  if (hasZNAM)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ZNAM's data
    output.write((const char*) &unknownZNAM, 4);
  }//if has ZNAM

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) unknownDATA, 8);

  return output.good();
}
#endif

bool KeyRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of KEYM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of KEYM!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND (or VMAD)
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cVMAD)
  {
    //read VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cout << "Error while reading subrecord VMAD of KEYM!\n";
      return false;
    }
    bytesRead = bytesRead +2 +unknownVMAD.getSize();

    //read OBND
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    //no VMAD
    unknownVMAD.setPresence(false);
  }

  //record header was already read before...
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
    std::cout <<"Error: sub record OBND of KEYM has invalid length ("
              <<subLength<<" bytes. Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of KEYM!\n";
    return false;
  }

  //read FULL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cFULL)
  {
    UnexpectedRecord(cFULL, subRecName);
    return false;
  }
  //FULL's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: sub record FULL of KEYM has invalid length ("
              <<subLength<<" bytes. Should be four bytes!\n";
    return false;
  }
  //read FULL's stuff
  in_File.read((char*) &nameStringID, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FULL of KEYM!\n";
    return false;
  }

  //read MODL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, subRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record MODL of KEYM is longer than 511 characters!\n";
    return false;
  }
  //read MODL's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of KEYM!\n";
    return false;
  }
  modelPath = std::string(buffer);

  unknownMODT.setPresence(false);
  hasYNAM = false;
  hasZNAM = false;
  keywordArray.clear();
  uint32_t i, temp, k_Size;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: KEYM seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of KEYM!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cYNAM:
           if (hasYNAM)
           {
             std::cout << "Error: KEYM seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //YNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord YNAM of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read YNAM's stuff
           in_File.read((char*) &unknownYNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord YNAM of KEYM!\n";
             return false;
           }//if
           hasYNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: KEYM seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //ZNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord ZNAM of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read ZNAM's stuff
           in_File.read((char*) &unknownZNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ZNAM of KEYM!\n";
             return false;
           }//if
           hasZNAM = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: KEYM seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord KSIZ of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of KEYM!\n";
             return false;
           }//if

           //on we go...
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
             std::cout <<"Error: sub record KWDA of KEYM has invalid length ("
                       <<subLength<<" bytes. Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of KEYM!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: KEYM seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord DATA of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of KEYM!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                    << "\" found, but only MODT, YNAM, ZNAM, KSIZ or DATA "
                    << "are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cout << "Error: at least one subrecord of KEYM is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t KeyRecord::getRecordType() const
{
  return cKEYM;
}

} //namespace
