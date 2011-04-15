/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "LeveledCreatureRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

LeveledCreatureRecord::LeveledCreatureRecord()
{
  LevCreatureID = "";
  ListData = 0;
  ChanceNone = 0;
  NumberOfCreatures = 0;
  Entries.clear();
}

bool LeveledCreatureRecord::equals(const LeveledCreatureRecord& other) const
{
  if ((LevCreatureID!=other.LevCreatureID) or (ListData!=other.ListData)
    or (ChanceNone!=other.ChanceNone) or (NumberOfCreatures!=other.NumberOfCreatures)
    or (Entries.size()!=other.Entries.size()))
  {
    return false;
  }
  //compare entries
  unsigned int i;
  for (i=0; i<Entries.size(); ++i)
  {
    if ((Entries[i].Level!=other.Entries[i].Level)
        or (Entries[i].CreatureID!=other.Entries[i].CreatureID))
    {
      return false;
    }
  }//for
  return true;
}

bool LeveledCreatureRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLEVC, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +LevCreatureID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +4 /* size of list data */
        +4 /* NNAM */ +4 /* 4 bytes for length */ +1 /* size of chance byte */
        +4 /* INDX */ +4 /* INDX's length */ +4 /*size of index (always 4 bytes)*/;
  unsigned int i;
  for (i=0;i<Entries.size(); ++i)
  {
    Size = Size +4 /* CNAM */ +4 /* 4 bytes for length */
          +Entries[i].CreatureID.length()+1 /* length of creature ID +1 byte for NUL termination */
          + 4 /* INTV */ +4 /* 4 bytes for length */ +2 /* size of INTV */;
  }//for
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Levelled Creatures:
	NAME = ID of levelled list
	DATA = List data (4 bytes, long)
		1 = Calc from all levels <= PC level
	NNAM = Chance None? (1 byte)
	INDX = Number of items in list (4 bytes, long)
	CNAM = ID string of list item
	INTV = PC level for previous CNAM (2 bytes, short)
		The CNAM/INTV can occur many times in pairs
  */

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = LevCreatureID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write leveled list's ID
  output.write(LevCreatureID.c_str(), SubLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  SubLength = 4; //length is always four bytes
  output.write((char*) &SubLength, 4);
  //write leveled list data
  output.write((char*) &ListData, 4);

  //write NNAM
  output.write((char*) &cNNAM, 4);
  //NNAM's length
  SubLength = 1; //length is always one byte
  output.write((char*) &SubLength, 4);
  //write chance
  output.write((char*) &ChanceNone, 1);

  //write INDX
  output.write((char*) &cINDX, 4);
  //INDX's length
  SubLength = 4; //length is always four bytes
  output.write((char*) &SubLength, 4);
  //write number of items in list
  output.write((char*) &NumberOfCreatures, 4);

  //write list's entries
  for (i=0; i<Entries.size(); ++i)
  {
    //write CNAM
    output.write((char*) &cCNAM, 4);
    //CNAM's length
    SubLength = Entries[i].CreatureID.length()+1;//length of string plus one for NUL-termination
    output.write((char*) &SubLength, 4);
    //write creature's ID
    output.write(Entries[i].CreatureID.c_str(), SubLength);

    //write INTV
    output.write((char*) &cINTV, 4);
    //INTV's length
    SubLength = 2;//length is always 2 bytes
    output.write((char*) &SubLength, 4);
    //write creature's level
    output.write((char*) &(Entries[i].Level), 2);
  }//for

  return output.good();
}

bool LeveledCreatureRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Levelled Creatures:
	NAME = ID of levelled list
	DATA = List data (4 bytes, long)
		1 = Calc from all levels <= PC level
	NNAM = Chance None? (1 byte)
	INDX = Number of items in list (4 bytes, long)
	CNAM = ID string of list item
	INTV = PC level for previous CNAM (2 bytes, short)
		The CNAM/INTV can occur many times in pairs
  */

  int32_t SubRecName;
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
    std::cout << "Error: subrecord NAME of LEVC is longer than 255 characters.\n";
    return false;
  }
  //read leveled creature ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of LEVC.\n";
    return false;
  }
  LevCreatureID = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: subrecord DATA of LEVC has invalid length ("<<SubLength
              << " bytes). Should be four bytes.\n";
    return false;
  }
  //read leveled list data
  in_File.read((char*) &ListData, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of LEVC.\n";
    return false;
  }

  //read NNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cNNAM)
  {
    UnexpectedRecord(cNNAM, SubRecName);
    return false;
  }
  //NNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=1)
  {
    std::cout << "Error: subrecord NNAM of LEVC has invalid length ("<<SubLength
              << " bytes). Should be one byte.\n";
    return false;
  }
  //read chance none
  in_File.read((char*) &ChanceNone, 1);
  BytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NNAM of LEVC.\n";
    return false;
  }

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: subrecord INDX of LEVC has invalid length ("<<SubLength
              << " bytes). Should be four bytes.\n";
    return false;
  }
  //read number of creatures in list
  in_File.read((char*) &NumberOfCreatures, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord INDX of LEVC.\n";
    return false;
  }

  Entries.clear();

  while (BytesRead<Size)
  {
    //read CNAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cCNAM)
    {
      UnexpectedRecord(cCNAM, SubRecName);
      return false;
    }
    //CNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord CNAM of LEVC is longer than 255 bytes.\n";
      return false;
    }
    //read CNAM
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNAM of LEVC.\n";
      return false;
    }
    LevCreatureListEntry temp;
    temp.CreatureID = std::string(Buffer);

    //read INTV
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cINTV)
    {
      UnexpectedRecord(cINTV, SubRecName);
      return false;
    }
    //INTV's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=2)
    {
      std::cout << "Error: subrecord INTV of LEVC has invalid length ("
                << SubLength<< " bytes). Should be two bytes.\n";
      return false;
    }
    //read INTV
    in_File.read((char*) &(temp.Level), 2);
    BytesRead += 2;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord INTV of LEVC.\n";
      return false;
    }
    //add the read data to the list
    Entries.push_back(temp);
  }//while

  //check if we really read all the stuff
  if (NumberOfCreatures!=Entries.size())
  {
    std::cout << "Error: record LEVC should have "<<NumberOfCreatures
              << " entries, but "<<Entries.size()<<" entries were read.\n";
    return false;
  }

  return in_File.good();
}
