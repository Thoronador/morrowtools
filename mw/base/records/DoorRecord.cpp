/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013  Thoronador

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

#include "DoorRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

DoorRecord::DoorRecord()
: BasicRecord(),
  recordID(""),
  Name(""),
  ModelPath(""),
  Script(""),
  SoundOpen(""),
  SoundClose("")
{}

DoorRecord::~DoorRecord()
{
  //empty
}

bool DoorRecord::equals(const DoorRecord& other) const
{
  return ((recordID==other.recordID) and (Name==other.Name)
      and (ModelPath==other.ModelPath) and (Script==other.Script)
      and (SoundOpen==other.SoundOpen) and (SoundClose==other.SoundClose));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool DoorRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cDOOR, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of path +1 byte for NUL termination */;
  if (!Name.empty())
  {
    Size = Size+4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!Script.empty())
  {
    Size = Size +4 /* SCRI */ +4 /* 4 bytes for length */
          +Script.length()+1; /* length of script ID +1 byte for NUL termination */;
  }
  if (!SoundOpen.empty())
  {
    Size = Size +4 /* SNAM */ +4 /* 4 bytes for length */
          +SoundOpen.length()+1; /* length of sound ID +1 byte for NUL termination */;
  }
  if (!SoundClose.empty())
  {
    Size = Size +4 /* ANAM */ +4 /* 4 bytes for length */
          +SoundClose.length()+1; /* length of sound ID +1 byte for NUL termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Door:
    NAME = door ID
    FNAM = door name (optional in some rare cases)
    MODL = NIF model filename
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = recordID.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  if (!Name.empty())
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    SubLength = Name.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write door name
    output.write(Name.c_str(), SubLength);
  }

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  SubLength = ModelPath.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  if (!Script.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    //SCRI's length
    SubLength = Script.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write script ID
    output.write(Script.c_str(), SubLength);
  }

  if (!SoundOpen.empty())
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    SubLength = SoundOpen.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write sound ID
    output.write(SoundOpen.c_str(), SubLength);
  }

  if (!SoundClose.empty())
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    //SNAM's length
    SubLength = SoundClose.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write sound ID
    output.write(SoundClose.c_str(), SubLength);
  }
  return output.good();
}
#endif

bool DoorRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Door:
    NAME = door ID
    FNAM = door name (optional in some rare cases)
    MODL = NIF model filename
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of DOOR is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read Door ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of DOOR!\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read optional part and records with varying order
  Name.clear();
  ModelPath.clear();
  Script.clear();
  SoundOpen.clear();
  SoundClose.clear();
  bool success = false;
  bool hasFNAM = false;
  bool hasMODL = false;
  bool hasSCRI = false;
  bool hasSNAM = false;
  bool hasANAM = false;
  while (BytesRead<Size)
  {
    //read next optional sub record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cFNAM:
           if (hasFNAM)
           {
             std::cout << "Error: record DOOR seems to have two FNAM subrecords.\n";
             return false;
           }
           success = readSubRecordString(in_File, Buffer, BytesRead, Name);
           hasFNAM = true;
           break;
      case cMODL:
           if (hasMODL)
           {
             std::cout << "Error: record DOOR seems to have two MODL subrecords.\n";
             return false;
           }
           success = readSubRecordString(in_File, Buffer, BytesRead, ModelPath);
           hasMODL = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cout << "Error: record DOOR seems to have two SCRI subrecords.\n";
             return false;
           }
           success = readSubRecordString(in_File, Buffer, BytesRead, Script);
           hasSCRI = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: record DOOR seems to have two SNAM subrecords.\n";
             return false;
           }
           success = readSubRecordString(in_File, Buffer, BytesRead, SoundOpen);
           hasSNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cout << "Error: record DOOR seems to have two ANAM subrecords.\n";
             return false;
           }
           success = readSubRecordString(in_File, Buffer, BytesRead, SoundClose);
           hasANAM = true;
           break;
      default:
           std::cout << "Error while reading DOOR: expected record name SCRI, "
                     << "SNAM or ANAM was not found. Instead, \""
                     << IntTo4Char(SubRecName)<<"\" was found.\n";
           return false;
           break;
    }//swi
    if (!success)
    {
      std::cout << "Error while reading DOOR record. Subrecord was "<<IntTo4Char(SubRecName)<<".\n";
      return false;
    }
  }//while BytesRead<Size
  return in_File.good();
}

bool DoorRecord::readSubRecordString(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead, std::string& Destination)
{
  uint32_t SubLength = 0;
  //read string's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord of DOOR is longer than 255 characters.\n";
    return false;
  }
  //read string
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord of DOOR!\n";
    return false;
  }
  Destination = std::string(Buffer);
  return true;
}

} //namespace
