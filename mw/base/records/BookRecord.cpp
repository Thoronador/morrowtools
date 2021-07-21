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

#include "BookRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

BookRecord::BookRecord()
: BasicRecord(),
  recordID(""),
  MeshPath(""),
  Name(""),
  //book data
  Weight(0.0f),
  Value(0),
  Scroll(0),
  SkillID (-1),
  EnchantPoints(0),
  //end of book data
  InventoryIcon(""),
  ScriptID(""),
  Text(""),
  EnchantmentID("")
{}

bool BookRecord::equals(const BookRecord& other) const
{
  return ((recordID==other.recordID) and (MeshPath==other.MeshPath)
      and (Name==other.Name) and (Weight==other.Weight) and (Value==other.Value)
      and (Scroll==other.Scroll) and (SkillID==other.SkillID)
      and (EnchantPoints==other.EnchantPoints) and (InventoryIcon==other.InventoryIcon)
      and (ScriptID==other.ScriptID) and (Text==other.Text)
      and (EnchantmentID==other.EnchantmentID));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool BookRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cBOOK, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +MeshPath.length()+1 /* length of mesh +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* BKDT */ +4 /* 4 bytes for length */ +20 /* length of BKDT */
        +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of mesh +1 byte for NUL termination */;
  if (!Text.empty())
  {
    Size = Size +4 /* TEXT */ +4 /* 4 bytes for length */
        +Text.length() /* length of text (no NUL termination, as far as I can see) */;
  }
  if (!ScriptID.empty())
  {
    Size= Size +4 /* SCRI */ +4 /* 4 bytes for length */
         +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */;
  }
  if (!EnchantmentID.empty())
  {
    Size= Size +4 /* ENAM */ +4 /* 4 bytes for length */
         +EnchantmentID.length()+1 /* length of enchantment ID +1 byte for NUL termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  SubLength = MeshPath.length()+1;
  //write MODL's length
  output.write((const char*) &SubLength, 4);
  //write mesh path
  output.write(MeshPath.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = Name.length()+1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write book's name
  output.write(Name.c_str(), SubLength);

  //write BKDT
  output.write((const char*) &cBKDT, 4);
  SubLength = 20; /* fixed length: 20 bytes */
  //write BKDT's length
  output.write((const char*) &SubLength, 4);
  //write book data
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &Scroll, 4);
  output.write((const char*) &SkillID, 4);
  output.write((const char*) &EnchantPoints, 4);

  //write ITEX
  output.write((const char*) &cITEX, 4);
  SubLength = InventoryIcon.length()+1;
  //write ITEX's length
  output.write((const char*) &SubLength, 4);
  //write book's inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((const char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  if (!Text.empty())
  {
    //write TEXT
    output.write((const char*) &cTEXT, 4);
    SubLength = Text.length()+1;
    //write TEXT's length
    output.write((const char*) &SubLength, 4);
    //write book's text
    output.write(Text.c_str(), SubLength);
  }

  if (!EnchantmentID.empty())
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    SubLength = EnchantmentID.length()+1;
    //write ENAM's length
    output.write((const char*) &SubLength, 4);
    //write book's enchantment
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}
#endif

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

  uint32_t SubRecName;
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
  char Buffer[256];
  //read book ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of BOOK!\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord MODL of BOOK is longer than 255 characters.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of BOOK!\n";
    return false;
  }
  MeshPath = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord FNAM of BOOK is longer than 255 characters.\n";
    return false;
  }
  //read book's name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of BOOK!\n";
    return false;
  }
  Name = std::string(Buffer);

  //read BKDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cBKDT)
  {
    UnexpectedRecord(cBKDT, SubRecName);
    return false;
  }
  //BKDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=20)
  {
    std::cout << "Error: Subrecord BKDT of BOOK has wrong size ("<<SubLength
              << " bytes), should be 20 bytes.\n";
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
    return false;
  }

  //Usually, ITEX should be before SCRI, but this can also be the other way,
  // so we have to check here. Also, TEXT and ENAM are not always present, so
  // take care.
  ScriptID.clear();
  InventoryIcon.clear();
  Text.clear();
  EnchantmentID.clear();
  char * TextBuffer = NULL;
  //now enter a loop to read the records for inventory icon and script name
  while (BytesRead<Size)
  {
    //read next record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cENAM:
           if (!EnchantmentID.empty())
           {
             std::cout << "Error: BOOK seems to have more than one ENAM subrecord!\n";
             return false;
           }
           //ENAM's length
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
           //content check
           if (EnchantmentID.empty())
           {
             std::cout << "Error: subrecord ENAM of BOOK is empty!\n";
             return false;
           }
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cout << "Error: BOOK seems to have more than one ITEX subrecord!\n";
             return false;
           }
           //ITEX's length
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
           //content check
           if (InventoryIcon.empty())
           {
             std::cout << "Error: subrecord ITEX of BOOK is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cout << "Error: BOOK seems to have more than one SCRI subrecord!\n";
             return false;
           }
           //SCRI's length
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
           //content check
           if (ScriptID.empty())
           {
             std::cout << "Error: subrecord SCRI of BOOK is empty!\n";
             return false;
           }
           break;
      case cTEXT:
           if (!Text.empty())
           {
             std::cout << "Error: BOOK seems to have more than one TEXT subrecord!\n";
             return false;
           }
           //TEXT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           //check length to avoid exhausting memory allocation
           if (SubLength>65535)
           {
             std::cout << "Error: Subrecord TEXT of BOOK is longer than 65535 characters.\n";
             return false;
           }
           //read text of book
           /*allocate buffer for text (could be more than 255 bytes, so we get
              a separate, larger buffer) */
           TextBuffer = new char[SubLength+1];
           memset(TextBuffer, '\0', SubLength+1);
           in_File.read(TextBuffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord TEXT of BOOK!\n";
             delete[] TextBuffer;
             return false;
           }
           Text = std::string(TextBuffer);
           delete[] TextBuffer;
           TextBuffer = NULL;
           //content check
           if (Text.empty())
           {
             std::cout << "Error: subrecord TEXT of BOOK is empty!\n";
             return false;
           }
           break;
      default:
           //other subrecord means error, so quit here
           std::cout << "BookRecord: Error: expected record name ITEX or SCRI "
                     << "or TEXT or ENAM was not found. Instead, \""
                     <<IntTo4Char(SubRecName)<<"\" was found.\nPosition is "
                     <<in_File.tellg()<<".\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
