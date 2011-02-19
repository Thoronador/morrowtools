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

#include "StaticRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

bool StaticRecord::equals(const StaticRecord& other) const
{
  return ((StaticID==other.StaticID) and (Mesh==other.Mesh));
}

bool StaticRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSTAT, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +StaticID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for MODL's length */
        +Mesh.length()+1 /*length of mesh plus one for NUL-termination */;
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  //write NAME
  output.write((char*) &cNAME, 4);
  int32_t SubLength = StaticID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write NAME/ID
  output.write(StaticID.c_str() ,SubLength);
  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = Mesh.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write MODL/ mesh path
  output.write(Mesh.c_str() ,SubLength);
  return output.good();
}

bool StaticRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;
  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return -1;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of STAT is longer than 255 characters.\n";
    return -1;
  }
  //read NAME
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of STAT.\n";
    return -1;
  }
  StaticID = std::string(Buffer);
  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return -1;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord MODL of STAT is longer than 255 characters.\n";
    return -1;
  }
  //read MODL
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of STAT.\n";
    return -1;
  }
  Mesh = std::string(Buffer);
  return true;
}
