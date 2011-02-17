/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "GlobalRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

GlobalRecord::GlobalRecord()
{
  GlobalID = "";
  Type = globShort;
  shortVal = 0;
  longVal = 0;
  floatVal = 0.0f;
}

GlobalRecord::GlobalRecord(const std::string& ID)
{
  GlobalID = ID;
  Type = globShort;
  shortVal = 0;
  longVal = 0;
  floatVal = 0.0f;
}

bool GlobalRecord::equals(const GlobalRecord& other) const
{
  if (Type!=other.Type)
  {
    return false;
  }
  switch (Type)
  {
    case globShort:
         return (shortVal==other.shortVal);
         break;
    case globLong:
         return (longVal==other.longVal);
         break;
    case globFloat:
         return (floatVal==other.floatVal);
         break;
  }//swi
  std::cout << "Unhandled case in GlobalRecord::equals!\n";
  throw 42;
}

bool GlobalRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cGLOB, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +GlobalID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */ +1 /* length of FNAM */
        +4 /* FLTV */ +4 /* 4 bytes for length */ +4 /* length of FLTV */;
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */

  //write NAME
  output.write((char*) &cNAME, 4);
  int32_t SubLength = GlobalID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(GlobalID.c_str(), SubLength);
  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = 1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write FNAM (type of global)
  char var_type = '\0';
  switch (Type)
  {
    case globFloat:
         var_type = 'f';
         break;
    case globShort:
         var_type = 's';
         break;
    case globLong:
         var_type = 'l';
         break;
  }//swi
  output.write(&var_type, 1);
  //write FLTV
  output.write((char*) &cFLTV, 4);
  SubLength = 4;
  //write FLTV's length
  output.write((char*) &SubLength, 4);
  //write FLTV (value of global var)
  switch(Type)
  {
    case globFloat:
         output.write((char*) &floatVal, 4);
         break;
    case globLong:
         output.write((char*) &longVal, 4);
         break;
    case globShort:
         SubLength = shortVal;
         output.write((char*) &SubLength, 4);
         break;
  }//swi
  return output.good();
}


bool GlobalRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */
  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;
  char TypeChar = '\0';

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
    std::cout << "GlobalRecord::loadFromStream: Error: ID is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "GlobalRecord::loadFromStream: Error while reading ID from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return false;
  }
  GlobalID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=1)
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  in_File.read(&TypeChar, 1);
  if ((TypeChar != 'f') && (TypeChar != 'l') && (TypeChar != 's'))
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid value ("<<TypeChar
              <<"). Should be 'f', 'l' or 's'.\n";
    return false;
  }

  //read FLTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFLTV)
  {
    UnexpectedRecord(cFLTV, SubRecName);
    return false;
  }
  //FLTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLTV of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read value
  switch(TypeChar)
  {
    case 'f':
         in_File.read((char*) &floatVal, 4);
         Type = globFloat;
         break;
    case 'l':
         in_File.read((char*) &longVal, 4);
         Type = globLong;
         break;
    case 's':
         in_File.read((char*) &longVal, 4);
         shortVal = longVal;
         Type = globShort;
         break;
  }//swi

  if (!in_File.good())
  {
    std::cout << "Error while reading data of record GLOB.\n";
    return false;
  }
  return true;
} //loadFromStream

bool operator<(const GlobalRecord& left, const GlobalRecord& right)
{
  return (left.GlobalID.compare(right.GlobalID)<0);
}
