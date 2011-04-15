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

#include "SoundGeneratorRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

SoundGeneratorRecord::SoundGeneratorRecord()
{
  Name = "";
  SoundID = "";
  Type = -1;
  CreatureID = "";
}

bool SoundGeneratorRecord::equals(const SoundGeneratorRecord& other) const
{
  return ((Name==other.Name) and (SoundID==other.SoundID) and (Type==other.Type)
      and (CreatureID==other.CreatureID));
}

bool SoundGeneratorRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSNDG, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* DATA's length */ +4 /*size of sound gen data (DATA)*/
        +4 /* SNAM */ +4 /* 4 bytes for SNAM's length */
        +SoundID.length()+1 /*length of name plus one for NUL-termination */;
  if (CreatureID!="")
  {
    Size = Size +4 /* CNAM */ +4 /* 4 bytes for length */
          +CreatureID.length()+1; /* length of ID +1 byte for NUL termination */
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Sound Generator:
    NAME = Name? (DEFAULT0001, ALIT0001, etc...)
    DATA = Sound Type Data (4 bytes, long)
        (0 = Left Foot,1 = Right Foot,2 = Swim Left,3 = Swim Right,4 = Moan,
         5 = Roar,6 = Scream,7 = Land)
    CNAM = Creature name (optional)
    SNAM = Sound ID string */


  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = Name.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write name/ID
  output.write(Name.c_str(), SubLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  SubLength = 4; //length of DATA (sound generator data) is always four bytes
  output.write((char*) &SubLength, 4);
  // write sound data
  output.write((char*) &Type, 4);

  if (CreatureID!="")
  {
    //write CNAM
    output.write((char*) &cCNAM, 4);
    //CNAM's length
    SubLength = CreatureID.length()+1;//length of string plus one for NUL-termination
    output.write((char*) &SubLength, 4);
    //write creature name/ID
    output.write(CreatureID.c_str(), SubLength);
  }

  //write SNAM
  output.write((char*) &cSNAM, 4);
  //SNAM's length
  SubLength = SoundID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write sound ID
  output.write(SoundID.c_str(), SubLength);

  return output.good();
}

bool SoundGeneratorRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Sound Generator:
    NAME = Name? (DEFAULT0001, ALIT0001, etc...)
    DATA = Sound Type Data (4 bytes, long)
        (0 = Left Foot,1 = Right Foot,2 = Swim Left,3 = Swim Right,4 = Moan,
         5 = Roar,6 = Scream,7 = Land)
    CNAM = Creature name (optional)
    SNAM = Sound ID string */

  int32_t SubRecName;
  uint32_t SubLength;
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
    std::cout << "Error: subrecord NAME of SNDG is longer than 255 characters.\n";
    return false;
  }
  //read the name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of SNDG.\n";
    return false;
  }
  Name = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record DATA of SNDG has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  //read the sound generator data
  in_File.read((char*) &Type, 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of SNDG.\n";
    return false;
  }

  //read optional CNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cCNAM)
  {
    //CNAM's length
    in_File.read((char*) &SubLength, 4);
    if (SubLength>255)
    {
      std::cout << "Error: subrecord CNAM of SNDG is longer than 255 characters.\n";
      return false;
    }
    //read creature name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNAM of SNDG.\n";
      return false;
    }
    CreatureID = std::string(Buffer);
    //read next record name
    in_File.read((char*) &SubRecName, 4);
  }
  else
  {
    //no CNAM found, so set creature name to empty string
    CreatureID = "";
  }

  //read SNAM (record name already read above
  if (SubRecName!=cSNAM)
  {
    UnexpectedRecord(cSNAM, SubRecName);
    return false;
  }
  //SNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: subrecord SNAM of SNDG is longer than 255 characters.\n";
    return false;
  }
  //read sound ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord SNAM of SNDG.\n";
    return false;
  }
  SoundID = std::string(Buffer);

  return in_File.good();
}
