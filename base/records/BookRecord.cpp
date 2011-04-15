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

#include "BookRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

BookRecord::BookRecord()
{
  BookID = MeshPath = Name = "";
  //book data
  Weight = 0.0f;;
  Value = 0;
  Scroll = 0;
  SkillID = -1;
  EnchantPoints = 0;
  //end of book data
  InventoryIcon = ScriptID = Text = "";
}

BookRecord::BookRecord(const std::string& ID)
{
  BookID = ID;
  MeshPath = Name = "";
  //book data
  Weight = 0.0f;;
  Value = 0;
  Scroll = 0;
  SkillID = -1;
  EnchantPoints = 0;
  //end of book data
  InventoryIcon = ScriptID = Text = "";
}

bool BookRecord::equals(const BookRecord& other) const
{
  return ((BookID==other.BookID) and (MeshPath==other.MeshPath)
      and (Name==other.Name) and (Weight==other.Weight) and (Value==other.Value)
      and (Scroll==other.Scroll) and (SkillID==other.SkillID)
      and (EnchantPoints==other.EnchantPoints) and (InventoryIcon==other.InventoryIcon)
      and (ScriptID==other.ScriptID) and (Text==other.Text));
}

bool operator<(const BookRecord& left, const BookRecord& right)
{
  return (left.BookID.compare(right.BookID)<0);
}

bool BookRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cBOOK, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +BookID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +MeshPath.length()+1 /* length of mesh +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* BKDT */ +4 /* 4 bytes for length */ +20 /* length of BKDT */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of mesh +1 byte for NUL termination */;
  if (Text!="")
  {
    Size = Size +4 /* TEXT */ +4 /* 4 bytes for length */
        +Text.length() /* length of text (no NUL termination, as far as I can see) */;
  }
  if (ScriptID!="")
  {
    Size= Size +4 /* SCRI */ +4 /* 4 bytes for length */
         +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */;
  }
  if (EnchantmentID!="")
  {
    Size= Size +4 /* ENAM */ +4 /* 4 bytes for length */
         +EnchantmentID.length()+1 /* length of enchantment ID +1 byte for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Books
	NAME = Item ID, required
	MODL = Model Name, required
	FNAM = Item Name, required
	BKDT = Book Data (20 bytes), required
		float  Weight
		long   Value
		long   Scroll	(1 is scroll, 0 not)
		long   SkillID	(-1 is no skill)
		long   EnchantPts
	ITEX = Inventory Icon
	SCRI = Script Name (optional)
	TEXT = Book text (not always present, believe it or not!)
	ENAM = name of enchantment (optional)
  */

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = BookID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(BookID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = MeshPath.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write mesh path
  output.write(MeshPath.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = Name.length()+1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
  //write book's name
  output.write(Name.c_str(), SubLength);

  //write BKDT
  output.write((char*) &cBKDT, 4);
  SubLength = 20; /* fixed length: 20 bytes */
  //write BKDT's length
  output.write((char*) &SubLength, 4);
  //write book data
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 4);
  output.write((char*) &Scroll, 4);
  output.write((char*) &SkillID, 4);
  output.write((char*) &EnchantPoints, 4);

  //write ITEX
  output.write((char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((char*) &SubLength, 4);
  //write book's inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (ScriptID!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  if (Text!="")
  {
    //write TEXT
    output.write((char*) &cTEXT, 4);
    SubLength = Text.length()+1;
    //write TEXT's length
    output.write((char*) &SubLength, 4);
    //write book's text
    output.write(Text.c_str(), SubLength);
  }

  if (EnchantmentID!="")
  {
    //write ENAM
    output.write((char*) &cENAM, 4);
    SubLength = EnchantmentID.length()+1;
    //write ENAM's length
    output.write((char*) &SubLength, 4);
    //write book's enchantment
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}

bool BookRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Books:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    BKDT = Book Data (20 bytes), required
        float  Weight
        long   Value
        long   Scroll	(1 is scroll, 0 not)
        long   SkillID	(-1 is no skill)
        long   EnchantPts
    ITEX = Inventory Icon
    SCRI = Script Name (optional)
    TEXT = Book text (not always present, believe it or not!)
    ENAM = name of enchantment (optional) */

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = BytesRead = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  BytesRead += 4;
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord NAME of BOOK is longer than 255 characters.\n";
    return false;
  }
  char* Buffer = NULL;
  Buffer = new char[256];
  //read book ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of BOOK!\n";
    delete[] Buffer;
    return false;
  }
  BookID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    delete[] Buffer;
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord MODL of BOOK is longer than 255 characters.\n";
    delete[] Buffer;
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of BOOK!\n";
    delete[] Buffer;
    return false;
  }
  MeshPath = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    delete[] Buffer;
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord FNAM of BOOK is longer than 255 characters.\n";
    delete[] Buffer;
    return false;
  }
  //read book's name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of BOOK!\n";
    delete[] Buffer;
    return false;
  }
  Name = std::string(Buffer);

  //read BKDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cBKDT)
  {
    UnexpectedRecord(cBKDT, SubRecName);
    delete[] Buffer;
    return false;
  }
  //BKDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=20)
  {
    std::cout << "Error: Subrecord BKDT of BOOK has wrong size ("<<SubLength
              << " bytes), should be 20 bytes.\n";
    delete[] Buffer;
    return false;
  }
  //read book data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Scroll, 4);
  in_File.read((char*) &SkillID, 4);
  in_File.read((char*) &EnchantPoints, 4);
  BytesRead += 20;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord BKDT of BOOK!\n";
    delete[] Buffer;
    return false;
  }

  //Usually, ITEX should be before SCRI, but this can also be the other way,
  // so we have to check here. Also, TEXT and ENAM are not always present, so
  // take care.
  ScriptID = InventoryIcon = Text = EnchantmentID = "";
  //now enter a loop to read the records for inventory icon and script name
  bool Success = false;
  while (BytesRead<Size)
  {
    //read next record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cENAM:
           Success = readSubRecordENAM(in_File, Buffer, BytesRead);
           break;
      case cITEX:
           Success = readSubRecordITEX(in_File, Buffer, BytesRead);
           break;
      case cSCRI:
           Success = readSubRecordSCRI(in_File, Buffer, BytesRead);
           break;
      case cTEXT:
           Success = readSubRecordTEXT(in_File, &Buffer, BytesRead);
           break;
      default:
           //other subrecord means error, so quit here
           delete[] Buffer;
           std::cout << "BookRecord: Error: expected record name ITEX or SCRI "
                     << "or TEXT or ENAM was not found. Instead, \""
                     <<IntTo4Char(SubRecName)<<"\" was found.\nPosition is "
                     <<in_File.tellg()<<".\n";
           return false;
    }//swi
    if (!Success)
    {
      //An error occured, quit here, but delete buffer before that.
      delete[] Buffer;
      return false;
    }//if
  }//while

  delete[] Buffer;
  return in_File.good();
}

bool BookRecord::readSubRecordITEX(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //ITEX's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord ITEX of BOOK is longer than 255 characters.\n";
    return false;
  }
  //read inventory icon
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of BOOK!\n";
    return false;
  }
  InventoryIcon = std::string(Buffer);
  return true;
}

bool BookRecord::readSubRecordSCRI(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //SCRI's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord SCRI of BOOK is longer than 255 characters.\n";
    return false;
  }
  //read script name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord SCRI of BOOK!\n";
    return false;
  }
  ScriptID = std::string(Buffer);
  return true;
}

bool BookRecord::readSubRecordENAM(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //ENAM's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord ENAM of BOOK is longer than 255 characters.\n";
    return false;
  }
  //read enchantment ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ENAM of BOOK!\n";
    return false;
  }
  EnchantmentID = std::string(Buffer);
  return true;
}

bool BookRecord::readSubRecordTEXT(std::ifstream& in_File, char** Buffer, uint32_t& BytesRead)
{
  //TEXT's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>65535)
  {
    std::cout << "Error: Subrecord TEXT of BOOK is longer than 65535 characters.\n";
    return false;
  }
  //read text of book
  if (SubLength>255)
  {
    //adjust size of buffer
    delete[] (*Buffer);
    *Buffer = NULL;
    *Buffer = new char[SubLength+1];
  }
  memset(*Buffer, '\0', SubLength+1);
  in_File.read(*Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord TEXT of BOOK!\n";
    return false;
  }
  Text = std::string(*Buffer);
  return true;
}
