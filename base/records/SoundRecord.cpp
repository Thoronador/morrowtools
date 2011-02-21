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

#include "SoundRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

SoundRecord::SoundRecord()
{
  SoundID = "";
  Filename = "";
  Volume = 0;
  MinRange = 0;
  MaxRange = 0;
}

bool SoundRecord::equals(const SoundRecord& other) const
{
  return ((SoundID==other.SoundID) and (Filename==other.Filename)
      and (Volume==other.Volume) and (MinRange==other.MinRange)
      and (MaxRange==other.MaxRange));
}

bool SoundRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSOUN, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +SoundID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for FNAM's length */
        +Filename.length()+1 /*length of name plus one for NUL-termination */
        +4 /* DATA */ +4 /* DATA's length */ +3 /*size of sound data (DATA)*/;
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  int32_t SubLength;
  SubLength = SoundID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write name/ID
  output.write(SoundID.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  SubLength = Filename.length()+1; //length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write file name
  output.write(Filename.c_str(), SubLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  SubLength = 3; //length of DATA (sound data) is always three bytes
  output.write((char*) &SubLength, 4);
  // write sound data
  output.write((char*) &Volume, 1);
  output.write((char*) &MinRange, 1);
  output.write((char*) &MaxRange, 1);

  return output.good();
}

bool SoundRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, H_Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &H_Flags, 4);

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

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
    std::cout << "Error: subrecord NAME of SOUN is longer than 255 characters.\n";
    return false;
  }
  //read the name/ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of SOUN.\n";
    return false;
  }
  SoundID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord FNAM of SOUN is longer than 255 characters.\n";
    return false;
  }
  //read file name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of SOUN.\n";
    return false;
  }
  Filename = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=3)
  {
    std::cout << "Error: sub record DATA of SOUN has invalid length ("
              << SubLength << " bytes). Should be 3 bytes.\n";
    return false;
  }
  //read sound data
  in_File.read((char*) &Volume, 1);
  in_File.read((char*) &MinRange, 1);
  in_File.read((char*) &MaxRange, 1);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of SOUN.\n";
  }
  return in_File.good();
}
