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

#include "StartScriptRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

StartScriptRecord::StartScriptRecord()
: BasicRecord(),
  Data(""),
  recordID("")
{}

/*
StartScriptRecord::StartScriptRecord(const std::string& ID)
{
  Data = "";
  recordID = ID;
}
*/

bool StartScriptRecord::equals(const StartScriptRecord& other) const
{
  return ((Data==other.Data) and (recordID==other.recordID));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool StartScriptRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSSCR, 4);
  uint32_t Size;
  Size = 4 /* DATA */ +4 /* 4 bytes for length */
        +Data.length()+1 /* length of sequence (no NUL termination) */
        +4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length() /* length of name (no NUL termination) */;
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string*/

  //write DATA
  output.write((const char*) &cDATA, 4);
  uint32_t SubLength = Data.length();
  //write DATA's length
  output.write((const char*) &SubLength, 4);
  //write data
  output.write(Data.c_str(), SubLength);

  //write NAME
  output.write((const char*) &cNAME, 4);
  SubLength = recordID.length();
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write Name
  output.write(recordID.c_str(), SubLength);
  return output.good();
}
#endif

bool StartScriptRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string */
  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "StartScriptRecord::loadFromStream: Error: data sequence is "
              << "longer than 255 characters.\nFile position: "<<in_File.tellg()
              <<" bytes.\n";
    return false;
  }
  //read data
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "StartScriptRecord::loadFromStream: Error while reading data sequence from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return false;
  }
  Data = std::string(Buffer);

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "StartScriptRecord::loadFromStream: Error: name is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "StartScriptRecord::loadFromStream: Error while reading name from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return false;
  }
  recordID = std::string(Buffer);

  return in_File.good();
}

} //namespace
