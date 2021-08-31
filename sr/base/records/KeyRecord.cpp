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

#include "KeyRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

KeyRecord::KeyRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  keywordArray(std::vector<uint32_t>()),
  value(0),
  weight(0.0f)
{
  memset(unknownOBND, 0, 12);
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
      and (name==other.name) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (pickupSoundFormID==other.pickupSoundFormID)
      and (putdownSoundFormID==other.putdownSoundFormID) and (keywordArray==other.keywordArray)
      and (value==other.value) and (weight==other.weight));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t KeyRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +name.getWriteSize() /* FULL */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* length */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }//if MODL
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* length */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (pickupSoundFormID!=0)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (putdownSoundFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  return writeSize;
}

bool KeyRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cKEYM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

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
      std::cerr << "Error while writing VMAD of KEYM!";
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
  if (!name.saveToStream(output, cFULL))
    return false;

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
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing MODT of KEYM!";
      return false;
    }
  }//if MODT

  if (pickupSoundFormID!=0)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write pickup sound form ID
    output.write((const char*) &pickupSoundFormID, 4);
  }//if has YNAM

  if (putdownSoundFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write putdown sound form ID
    output.write((const char*) &putdownSoundFormID, 4);
  }//if has ZNAM

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write KSIZ
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    //write KSIZ
    unsigned int i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &keywordArray[i], 4);
    }//for
  }//if keywords present

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);

  return output.good();
}
#endif

bool KeyRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of KEYM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of KEYM!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND (or VMAD)
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName == cVMAD)
  {
    // read VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cerr << "Error while reading subrecord VMAD of KEYM!\n";
      return false;
    }
    bytesRead = bytesRead + 2 + unknownVMAD.size();

    //read OBND
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    // no VMAD
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
    std::cerr <<"Error: sub record OBND of KEYM has invalid length ("
              <<subLength<<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of KEYM!\n";
    return false;
  }

  //read FULL
  if (!name.loadFromStream(in_File, cFULL, true, bytesRead, localized, table, buffer))
  {
    std::cerr << "Error while reading subrecord FULL of KEYM!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
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
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: KEYM seems to have more than one MODL subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //check content
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of KEYM is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: KEYM seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of KEYM!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cYNAM:
           if (pickupSoundFormID!=0)
           {
             std::cerr << "Error: KEYM seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (pickupSoundFormID==0)
           {
             std::cerr <<"Error: subrecord YNAM of KEYM has value zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID!=0)
           {
             std::cerr << "Error: KEYM seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false)) return false;
           bytesRead += 6;
           //check value
           if (putdownSoundFormID==0)
           {
             std::cerr <<"Error: subrecord ZNAM of KEYM has value zero!\n";
             return false;
           }
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cerr << "Error: KEYM seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: subrecord KSIZ of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord KSIZ of KEYM!\n";
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
             std::cerr <<"Error: sub record KWDA of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cerr << "Error while reading subrecord KWDA of KEYM!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: KEYM seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cerr <<"Error: subrecord DATA of KEYM has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of KEYM!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, YNAM, ZNAM, KSIZ or DATA "
                     << "are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cerr << "Error: at least one subrecord of KEYM is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t KeyRecord::getRecordType() const
{
  return cKEYM;
}

} //namespace
