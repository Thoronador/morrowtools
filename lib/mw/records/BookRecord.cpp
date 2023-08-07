/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Dirk Stolle

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
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // book data
  Weight(0.0f),
  Value(0),
  Scroll(0),
  SkillID (-1),
  EnchantPoints(0),
  // end of book data
  ScriptID(std::string()),
  InventoryIcon(std::string()),
  Text(std::string()),
  EnchantmentID(std::string())
{}

bool BookRecord::equals(const BookRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight) && (Value == other.Value)
      && (Scroll == other.Scroll) && (SkillID == other.SkillID)
      && (EnchantPoints == other.EnchantPoints) && (ScriptID == other.ScriptID)
      && (InventoryIcon == other.InventoryIcon) && (Text == other.Text)
      && (EnchantmentID == other.EnchantmentID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool BookRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cBOOK), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* MODL */ + 4 /* 4 bytes for length */
      + ModelPath.length() + 1 /* length of path +1 byte for NUL byte */
      + 4 /* FNAM */ + 4 /* 4 bytes for length */
      + Name.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* BKDT */ + 4 /* 4 bytes for length */ + 20 /* length of BKDT */
      + 4 /* ITEX */ + 4 /* 4 bytes for length */
      + InventoryIcon.length() + 1 /* length of mesh +1 byte for NUL byte */;
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of script ID +1 byte for NUL */;
  }
  if (!Text.empty())
  {
    Size = Size + 4 /* TEXT */ + 4 /* 4 bytes for length */
        + Text.length() /* length of text (no NUL termination, as far as I can see) */;
  }
  if (!EnchantmentID.empty())
  {
    Size = Size + 4 /* ENAM */ + 4 /* 4 bytes for length */
         + EnchantmentID.length() + 1 /* length of ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write mesh path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  // write book's name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write book data (BKDT)
  output.write(reinterpret_cast<const char*>(&cBKDT), 4);
  SubLength = 20; /* fixed length: 20 bytes */
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  //write book data
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Scroll), 4);
  output.write(reinterpret_cast<const char*>(&SkillID), 4);
  output.write(reinterpret_cast<const char*>(&EnchantPoints), 4);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write inventory icon path (ITEX)
  output.write(reinterpret_cast<const char*>(&cITEX), 4);
  SubLength = InventoryIcon.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(InventoryIcon.c_str(), SubLength);

  if (!Text.empty())
  {
    // write book's text (TEXT)
    output.write(reinterpret_cast<const char*>(&cTEXT), 4);
    SubLength = Text.length(); // no NUL terminator is written
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Text.c_str(), SubLength);
  }

  if (!EnchantmentID.empty())
  {
    // write enchantment (ENAM)
    output.write(reinterpret_cast<const char*>(&cENAM), 4);
    SubLength = EnchantmentID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool BookRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read NAME
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of BOOK!\n";
    return false;
  }

  // read MODL
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of BOOK!\n";
    return false;
  }

  // read FNAM
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of BOOK!\n";
    return false;
  }

  // read BKDT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cBKDT)
  {
    UnexpectedRecord(cBKDT, SubRecName);
    return false;
  }
  // BKDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 20)
  {
    std::cerr << "Error: Subrecord BKDT of BOOK has wrong size (" << SubLength
              << " bytes), should be 20 bytes.\n";
    return false;
  }
  // read book data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Scroll), 4);
  input.read(reinterpret_cast<char*>(&SkillID), 4);
  input.read(reinterpret_cast<char*>(&EnchantPoints), 4);
  BytesRead += 20;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord BKDT of BOOK!\n";
    return false;
  }

  //Usually, ITEX should be before SCRI, but this can also be the other way,
  // so we have to check here. Also, TEXT and ENAM are not always present, so
  // take care.
  ScriptID.clear();
  InventoryIcon.clear();
  Text.clear();
  EnchantmentID.clear();
  char * TextBuffer = nullptr;
  while (BytesRead < Size)
  {
    // read next record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cENAM:
           if (!EnchantmentID.empty())
           {
             std::cerr << "Error: BOOK seems to have more than one ENAM subrecord!\n";
             return false;
           }
           if (!loadString256(input, EnchantmentID, Buffer, cENAM, BytesRead))
           {
             std::cout << "Error while reading subrecord ENAM of BOOK!\n";
             return false;
           }
           // content check
           if (EnchantmentID.empty())
           {
             std::cerr << "Error: Subrecord ENAM of BOOK is empty!\n";
             return false;
           }
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: BOOK seems to have more than one ITEX subrecord!\n";
             return false;
           }
           if (!loadString256(input, InventoryIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of BOOK!\n";
             return false;
           }
           // content check
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Subrecord ITEX of BOOK is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: BOOK seems to have more than one SCRI subrecord!\n";
             return false;
           }
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of BOOK!\n";
             return false;
           }
           // content check
           if (ScriptID.empty())
           {
             std::cerr << "Error: Subrecord SCRI of BOOK is empty!\n";
             return false;
           }
           break;
      case cTEXT:
           if (!Text.empty())
           {
             std::cerr << "Error: BOOK seems to have more than one TEXT subrecord!\n";
             return false;
           }
           // TEXT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           // check length to avoid exhausting memory allocation
           if (SubLength > 65535)
           {
             std::cerr << "Error: Subrecord TEXT of BOOK is longer than 65535 characters.\n";
             return false;
           }
           // read text of book
           /*allocate buffer for text (could be more than 255 bytes, so we get
              a separate, larger buffer) */
           TextBuffer = new char[SubLength + 1];
           memset(TextBuffer, '\0', SubLength + 1);
           input.read(TextBuffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord TEXT of BOOK!\n";
             delete[] TextBuffer;
             return false;
           }
           Text = std::string(TextBuffer);
           delete[] TextBuffer;
           TextBuffer = nullptr;
           // content check
           if (Text.empty())
           {
             std::cerr << "Error: Subrecord TEXT of BOOK is empty!\n";
             return false;
           }
           break;
      default:
           // other subrecord means error, so quit here
           std::cerr << "BookRecord: Error: Expected record name ITEX or SCRI "
                     << "or TEXT or ENAM was not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
    }
  }

  return true;
}

} // namespace
