/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

// flag constants
const uint32_t BookRecord::cSkillBookFlag = 0x00000001;
const uint32_t BookRecord::cSpellTomeFlag = 0x00000004;
const uint32_t BookRecord::cNoteOrScrollTypeFlag = 0x0000FF00;

BookRecord::BookRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  title(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  text(LocalizedString()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  keywords(std::vector<uint32_t>()),
  // DATA
  bookFlags(0),
  spellOrSkillID(0),
  bookValue(0),
  weight(0.0f),
  // end of DATA
  inventoryArtFormID(0),
  unknownCNAM(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool BookRecord::equals(const BookRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
    && (unknownVMAD == other.unknownVMAD)
    && (unknownOBND == other.unknownOBND)
    && (title == other.title)
    && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
    && (text == other.text) && (unknownCNAM == other.unknownCNAM)
    && (pickupSoundFormID == other.pickupSoundFormID)
    && (putdownSoundFormID == other.putdownSoundFormID)
    && (keywords == other.keywords)
    && (bookFlags == other.bookFlags) && (spellOrSkillID == other.spellOrSkillID)
    && (bookValue == other.bookValue) && (weight == other.weight)
    && (inventoryArtFormID == other.inventoryArtFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t BookRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
      + 4 /* MODL */ + 2 /* 2 bytes for length */
      + modelPath.length() + 1 /* length of name +1 byte for NUL termination */
      + text.getWriteSize() /* DESC */
      + 4 /* DATA */ + 2 /* 2 bytes for length */ + 16 /* fixed size */
      + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */
               + unknownVMAD.size();
  }
  if (title.isPresent())
  {
    writeSize += title.getWriteSize();
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* size of subrecord */;
  }
  if (pickupSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* YNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (putdownSoundFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* KWDA */ + 2 /* 2 bytes for length */ + keywords.size() * 4 /* n * fixed size */;
  }
  if (inventoryArtFormID != 0)
  {
    writeSize = writeSize + 4 /* INAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  return writeSize;
}

bool BookRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cBOOK), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of BOOK!\n";
      return false;
    }
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (title.isPresent())
  {
    // write FULL
    if (!title.saveToStream(output, cFULL))
      return false;
  }

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  subLength = modelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(modelPath.c_str(), subLength);

  // write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
  {
    std::cerr << "Error while writing subrecord MODT of BOOK!\n";
    return false;
  }

  // write DESC
  if (!text.saveToStream(output, cDESC))
    return false;

  if (pickupSoundFormID != 0)
  {
    // write Pickup Sound form ID (YNAM)
    output.write(reinterpret_cast<const char*>(&cYNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pickupSoundFormID), 4);
  }

  if (putdownSoundFormID != 0)
  {
    // write Putdown Sound form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&putdownSoundFormID), 4);
  }

  if (!keywords.empty())
  {
    // write keyword size (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t k_Size = keywords.size();
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4 * k_Size; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 16;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's content
  output.write(reinterpret_cast<const char*>(&bookFlags), 4);
  output.write(reinterpret_cast<const char*>(&spellOrSkillID), 4);
  output.write(reinterpret_cast<const char*>(&bookValue), 4);
  output.write(reinterpret_cast<const char*>(&weight), 4);

  if (inventoryArtFormID != 0)
  {
    // write Inventory Art form ID (INAM)
    output.write(reinterpret_cast<const char*>(&cINAM), 4);
    subLength = 4; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&inventoryArtFormID), 4);
  }

  // write CNAM
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownCNAM), 4);

  return output.good();
}
#endif

bool BookRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  // read OBND or VMAD
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName == cVMAD)
  {
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cerr << "Error while reading subrecord VMAD of BOOK!\n";
      return false;
    }
    bytesRead = bytesRead + 2 + unknownVMAD.size();
    // read OBND's header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
  }
  else
  {
    // no VMAD subrecord
    unknownVMAD.setPresence(false);
  }

  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  // OBND's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of BOOK has invalid length ("
              << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read OBND
  in_File.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of BOOK!\n";
    return false;
  }

  title.reset();
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  inventoryArtFormID = 0;
  keywords.clear();

  bool hasReadMODL = false;
  text.reset();
  bool hasReadDATA = false;
  bool hasReadCNAM = false;

  while (bytesRead < readSize)
  {
    // read next header
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (title.isPresent())
           {
             std::cerr << "Error: BOOK seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!title.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (hasReadMODL)
           {
             std::cerr << "Error: BOOK seems to have more than one MODL subrecord.\n";
             return false;
           }
           // read model path (MODL)
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           // Check content, path must not be empty.
           if (modelPath.empty())
           {
             std::cerr << "Error: Sub record MODL of BOOK is empty!\n";
             return false;
           }

           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, true))
             return false;
           bytesRead += (4 + 2 + unknownMODT.size());

           // read DESC
           if (!text.loadFromStream(in_File, cDESC, true, bytesRead, localized, table, buffer))
             return false;
           hasReadMODL = true;
           break;
      case cKSIZ:
           if (!loadKeywords(in_File, keywords, bytesRead))
             return false;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: BOOK seems to have more than one DATA subrecord.\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 16)
           {
             std::cerr << "Error: Sub record DATA of BOOK has invalid length ("
                       << subLength << " bytes). Should be 16 bytes.\n";
             return false;
           }
           // read DATA
           in_File.read(reinterpret_cast<char*>(&bookFlags), 4);
           in_File.read(reinterpret_cast<char*>(&spellOrSkillID), 4);
           in_File.read(reinterpret_cast<char*>(&bookValue), 4);
           in_File.read(reinterpret_cast<char*>(&weight), 4);
           bytesRead += 16;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of BOOK!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cINAM:
           if (inventoryArtFormID != 0)
           {
             std::cerr << "Error: BOOK seems to have more than one INAM subrecord.\n";
             return false;
           }
           // read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, inventoryArtFormID, false))
             return false;
           bytesRead += 6;
           if (inventoryArtFormID == 0)
           {
             std::cerr << "Error: Subrecord INAM of BOOK is zero!\n";
             return false;
           }
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cerr << "Error: BOOK seems to have more than one CNAM subrecord.\n";
             return false;
           }
           // read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false))
             return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cYNAM:
           if (pickupSoundFormID != 0)
           {
             std::cerr << "Error: BOOK seems to have more than one YNAM subrecord.\n";
             return false;
           }
           // read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           if (pickupSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord YNAM of BOOK is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID != 0)
           {
             std::cerr << "Error: BOOK seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           // read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           if (putdownSoundFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of BOOK is zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord \""
                     << IntTo4Char(subRecName)
                     << "\", but only MODL, KSIZ, DATA, INAM, CNAM, YNAM or ZNAM are allowed here!\n";
           return false;
    }
  }

  // check presence of all required subrecords
  if (!hasReadMODL || !hasReadDATA || !hasReadCNAM)
  {
    std::cerr << "Error: At least one required subrecord of BOOK was not found!\n";
    return false;
  }
  return in_File.good();
}

uint32_t BookRecord::getRecordType() const
{
  return cBOOK;
}

bool BookRecord::isSkillBook() const
{
  return (bookFlags & cSkillBookFlag) == cSkillBookFlag;
}

bool BookRecord::isSpellTome() const
{
  return (bookFlags & cSpellTomeFlag) == cSpellTomeFlag;
}

} // namespace
