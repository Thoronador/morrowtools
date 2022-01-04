/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2022  Dirk Stolle

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

#include "ClothingRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

ClothingRecord::ClothingRecord()
: BasicRecord(),
  recordID(""),
  ModelPath(""),
  Name(""),
  //clothing data
  ClothingType(0),
  Weight(0.0f),
  Value(0),
  EnchantmentPoints(0),
  //end of clothing data
  InventoryIcon(""),
  BodyParts(std::vector<BodyPartAssociation>()),
  ScriptID(""),
  EnchantmentID("")
{}

bool ClothingRecord::equals(const ClothingRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (ClothingType == other.ClothingType)
      && (Weight == other.Weight) && (Value == other.Value)
      && (EnchantmentPoints == other.EnchantmentPoints) && (InventoryIcon == other.InventoryIcon)
      && (BodyParts == other.BodyParts) && (ScriptID == other.ScriptID)
      && (EnchantmentID == other.EnchantmentID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ClothingRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCLOT), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of model path +1 byte for NUL */
        + 4 /* CTDT */ + 4 /* 4 bytes for length */ + 12 /* data size */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for length */
         + Name.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!InventoryIcon.empty())
  {
    Size = Size + 4 /* ITEX */ + 4 /* 4 bytes for length */
         + InventoryIcon.length() + 1 /* length of icon +1 byte for NUL */;
  }
  // body part stuff
  for (const auto& part: BodyParts)
  {
    Size += part.getWriteSize();
  }

  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!EnchantmentID.empty())
  {
    Size = Size + 4 /* ENAM */ + 4 /* 4 bytes for length */
         + EnchantmentID.length() + 1 /* length of ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Clothing:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name (optional in at least one case)
    CTDT = Clothing Data (12 bytes), required
        long  Type (0=Pants,1=Shoes,2=Shirt,3=Belt,4=Robe,5=Right Glove,
                    6=Left Glove,7=Skirt,8=Ring,9=Amulet)
        float Weight
        short Value
        short EnchantPts
    SCRI = Script Name (optional)
    ITEX = Inventory Icon (optional)
    INDX = Body Part Index (1 byte)
    BNAM = Male Body Part Name
    CNAM = Female Body Part Name
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    ENAM = Enchantment Name (optional) */

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write clothing's model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  if (!Name.empty())
  {
    // write displayed name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  // write clothing data (CTDT)
  output.write(reinterpret_cast<const char*>(&cCTDT), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write clothing data
  output.write(reinterpret_cast<const char*>(&ClothingType), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 2);
  output.write(reinterpret_cast<const char*>(&EnchantmentPoints), 2);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  if (!InventoryIcon.empty())
  {
    // write inventory icon (ITEX)
    output.write(reinterpret_cast<const char*>(&cITEX), 4);
    SubLength = InventoryIcon.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(InventoryIcon.c_str(), SubLength);
  }

  // body part associations
  for (const auto& part: BodyParts)
  {
    if (!part.saveToStream(output))
      return false;
  }

  if (!EnchantmentID.empty())
  {
    // write enchantment ID (ENAM)
    output.write(reinterpret_cast<const char*>(&cENAM), 4);
    SubLength = EnchantmentID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool ClothingRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Clothing:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name (optional in at least one case)
    CTDT = Clothing Data (12 bytes), required
        long  Type (0=Pants,1=Shoes,2=Shirt,3=Belt,4=Robe,5=Right Glove,
                    6=Left Glove,7=Skirt,8=Ring,9=Amulet)
        float Weight
        short Value
        short EnchantPts
    SCRI = Script Name (optional)
    ITEX = Inventory Icon (optional)
    INDX = Body Part Index (1 byte)
    BNAM = Male Body Part Name
    CNAM = Female Body Part Name
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    ENAM = Enchantment Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read record ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of CLOT!\n";
    return false;
  }

  // read MODL
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of CLOT!\n";
    return false;
  }

  //read FNAM (optional) or CTDT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cFNAM)
  {
    // read clothing name (the real one, FNAM)
    if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
    {
      std::cerr << "Error while reading subrecord FNAM of CLOT!\n";
      return false;
    }

    // read CTDT
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    Name.clear();
  }

  // read CTDT
  // header was already read above
  if (SubRecName != cCTDT)
  {
    UnexpectedRecord(cCTDT, SubRecName);
    return false;
  }
  // CTDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record CTDT of CLOT has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read clothing data
  input.read(reinterpret_cast<char*>(&ClothingType), 4);
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 2);
  input.read(reinterpret_cast<char*>(&EnchantmentPoints), 2);
  BytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord CTDT of CLOT!\n";
    return false;
  }

  EnchantmentID.clear();
  ScriptID .clear();
  InventoryIcon.clear();
  BodyParts.clear();
  BodyPartAssociation tempAssoc;
  uint32_t prevSubrecord = 0;
  while (BytesRead < Size)
  {
    // read next subrecord's name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cINDX:
           // INDX's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 1)
           {
             std::cerr << "Error: sub record INDX of CLOT has invalid length ("
                       << SubLength << " bytes). Should be one byte only.\n";
             return false;
           }
           // read body part index
           input.read(reinterpret_cast<char*>(&tempAssoc.Index), 1);
           BytesRead += 1;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord INDX of CLOT!\n";
             return false;
           }
           tempAssoc.MaleBodyPart.clear();
           tempAssoc.FemaleBodyPart.clear();
           BodyParts.push_back(tempAssoc);
           prevSubrecord = cINDX;
           break;
      case cBNAM:
           if (prevSubrecord != cINDX)
           {
             std::cerr << "Error: Subrecord before BNAM of CLOTH was not INDX, "
                       << "but \"" << IntTo4Char(prevSubrecord) << "\".\n";
             return false;
           }
           // read male body part name (BNAM)
           if (!loadString256(input, BodyParts.back().MaleBodyPart, Buffer, cBNAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord BNAM of CLOT!\n";
             return false;
           }
           prevSubrecord = cBNAM;
           break;
      case cCNAM:
           if ((prevSubrecord != cINDX) && (prevSubrecord != cBNAM))
           {
             std::cerr << "Error: Subrecord before CNAM of CLOT was neither "
                       << "INDX nor BNAM, but \"" << IntTo4Char(prevSubrecord)
                       << "\".\n";
             return false;
           }
           // read female body part name (CNAM)
           if (!loadString256(input, BodyParts.back().FemaleBodyPart, Buffer, cCNAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord CNAM of CLOT!\n";
             return false;
           }
           prevSubrecord = cCNAM;
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: record CLOT seems to have two ITEX subrecords.\n";
             return false;
           }
           // read path to clothing icon texture (ITEX)
           if (!loadString256(input, InventoryIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of CLOT!\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Subrecord ITEX of CLOT is empty!\n";
             return false;
           }
           prevSubrecord = cITEX;
           break;
      case cENAM:
           if (!EnchantmentID.empty())
           {
             std::cerr << "Error: Record CLOT seems to have two ENAM subrecords.\n";
             return false;
           }
           // read clothing enchantment ID (ENAM)
           if (!loadString256(input, EnchantmentID, Buffer, cENAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord ENAM of CLOT!\n";
             return false;
           }
           if (EnchantmentID.empty())
           {
             std::cerr << "Error: Subrecord ENAM of CLOT is empty!\n";
             return false;
           }
           prevSubrecord = cENAM;
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: Record CLOT seems to have two SCRI subrecords.\n";
             return false;
           }
           // read clothing script ID (SCRI)
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of CLOT!\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: Subrecord SCRI of CLOT is empty!\n";
             return false;
           }
           prevSubrecord = cSCRI;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected INDX, BNAM, CNAM or ENAM.\n";
           return false;
    }
  }

  return input.good();
}

} // namespace
