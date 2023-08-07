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

#include "ArmourRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

ArmourRecord::ArmourRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // armour data
  ArmourType(0),
  Weight(0.0f),
  Value(0),
  Health(0),
  EnchantmentPoints(0),
  ArmourRating(0),
  // end of armour data
  InventoryIcon(std::string()),
  BodyParts(std::vector<BodyPartAssociation>()),
  ScriptID(std::string()),
  EnchantmentID(std::string())
{}

bool ArmourRecord::equals(const ArmourRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (ArmourType == other.ArmourType)
      && (Weight == other.Weight) && (Value == other.Value)
      && (Health == other.Health) && (EnchantmentPoints == other.EnchantmentPoints)
      && (ArmourRating == other.ArmourRating) && (InventoryIcon == other.InventoryIcon)
      && (BodyParts == other.BodyParts) && (ScriptID == other.ScriptID)
      && (EnchantmentID == other.EnchantmentID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ArmourRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cARMO), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of model path +1 byte for NUL */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* AODT */ + 4 /* 4 bytes for length */ + 24 /* data size */;
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
  if (!InventoryIcon.empty())
  {
    Size = Size + 4 /* ITEX */ + 4 /* 4 bytes for length */
         + InventoryIcon.length() + 1 /* length of icon path +1 byte for NUL */;
  }

  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Armor pieces:
    NAME = Item ID, required
    MODL = Model Filename, required
    FNAM = Item Name, required
    SCRI = Script Name (optional)
    AODT = Armour Data, required (24 bytes)
        long  Type (0 = Helmet,1 = Cuirass,2 = L. Pauldron,3 = R. Pauldron,
                    4 = Greaves,5 = Boots,6 = L. Gauntlet,7 = R. Gauntlet,
                    8 = Shield,9 = L. Bracer,10 = R. Bracer)
        float Weight
        long  Value
        long  Health
        long  EnchantPts
        long  Armour
    ITEX = Icon Filename, required
    INDX = Body Part Index (1 byte)
        (0=Head,1=Hair,2=Neck,3=Cuirass,4=Groin,5=Skirt,6=Right Hand,
         7=Left Hand,8=Right Wrist,9=Left Wrist,10=Shield,11=Right Forearm,
         12=Left Forearm,13=Right Upper Arm,14=Left Upper Arm,15=Right Foot,
         16=Left Foot,17=Right Ankle,18=Left Ankle,19=Right Knee,20=Left Knee,
         21=Right Upper Leg,22=Left Upper Leg,23=Right Pauldron,24=Left Pauldron,
         25=Weapon,26=Tail)
    BNAM = Male Part Name
    CNAM = Female Body Part Name (0 occurrences)
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    ENAM = Enchantment Name (optional) */

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write armour's model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  // write armour's "real" name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write armour data (AODT)
  output.write(reinterpret_cast<const char*>(&cAODT), 4);
  SubLength = 24;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write armour data
  output.write(reinterpret_cast<const char*>(&ArmourType), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Health), 4);
  output.write(reinterpret_cast<const char*>(&EnchantmentPoints), 4);
  output.write(reinterpret_cast<const char*>(&ArmourRating), 4);

  if (!InventoryIcon.empty())
  {
    // write inventory icon path (ITEX)
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

bool ArmourRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Armor pieces:
    NAME = Item ID, required
    MODL = Model Filename, required
    FNAM = Item Name, required
    SCRI = Script Name (optional)
    AODT = Armour Data, required (24 bytes)
        long  Type (0 = Helmet,1 = Cuirass,2 = L. Pauldron,3 = R. Pauldron,
                    4 = Greaves,5 = Boots,6 = L. Gauntlet,7 = R. Gauntlet,
                    8 = Shield,9 = L. Bracer,10 = R. Bracer)
        float Weight
        long  Value
        long  Health
        long  EnchantPts
        long  Armour
    ITEX = Icon Filename (optional)
    INDX = Body Part Index (1 byte)
        (0=Head,1=Hair,2=Neck,3=Cuirass,4=Groin,5=Skirt,6=Right Hand,
         7=Left Hand,8=Right Wrist,9=Left Wrist,10=Shield,11=Right Forearm,
         12=Left Forearm,13=Right Upper Arm,14=Left Upper Arm,15=Right Foot,
         16=Left Foot,17=Right Ankle,18=Left Ankle,19=Right Knee,20=Left Knee,
         21=Right Upper Leg,22=Left Upper Leg,23=Right Pauldron,24=Left Pauldron,
         25=Weapon,26=Tail)
    BNAM = Male Part Name
    CNAM = Female Body Part Name (0 occurrences)
        INDX and BNAM/CNAM are grouped together.  INDX first followed
        by an optional BNAM (no BNAM indicates a NULL field for that index).
        Up to 7 pairs allowed.
    ENAM = Enchantment Name (optional) */

  uint32_t BytesRead = 0;

  // read record ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of ARMO!\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of ARMO!\n";
    return false;
  }

  // read armour's "real" name (FNAM)
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of ARMO!\n";
    return false;
  }

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;

  // read SCRI or AODT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cSCRI)
  {
    // read script ID (SCRI)
    if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
    {
      std::cerr << "Error while reading subrecord SCRI of ARMO!\n";
      return false;
    }
    if (ScriptID.empty())
    {
      std::cerr << "Error: Subrecord SCRI of ARMO is empty!\n";
      return false;
    }

    // read AODT
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    ScriptID.clear();
  }

  // read AODT
  // header was already read above
  if (SubRecName != cAODT)
  {
    UnexpectedRecord(cAODT, SubRecName);
    return false;
  }
  // AODT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 24)
  {
    std::cerr << "Error: Sub record AODT of ARMO has invalid length ("
              << SubLength << " bytes). Should be 24 bytes.\n";
    return false;
  }
  // read armour data
  input.read(reinterpret_cast<char*>(&ArmourType), 4);
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Health), 4);
  input.read(reinterpret_cast<char*>(&EnchantmentPoints), 4);
  input.read(reinterpret_cast<char*>(&ArmourRating), 4);
  BytesRead += 24;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord AODT of ARMO!\n";
    return false;
  }

  EnchantmentID.clear();
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
             std::cerr << "Error: Sub record INDX of ARMO has invalid length ("
                       << SubLength << " bytes). Should be one byte only.\n";
             return false;
           }
           // read body part index
           input.read(reinterpret_cast<char*>(&tempAssoc.Index), 1);
           BytesRead += 1;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord INDX of ARMO!\n";
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
             std::cerr << "Error: Subrecord before BNAM of ARMO was not INDX, "
                       << "but \"" << IntTo4Char(prevSubrecord) << "\".\n";
             return false;
           }
           // read male body part name (BNAM)
           if (!loadString256(input, BodyParts.back().MaleBodyPart, Buffer, cBNAM, BytesRead))
           {
             std::cout << "Error while reading subrecord BNAM of ARMO!\n";
             return false;
           }
           prevSubrecord = cBNAM;
           break;
      case cCNAM:
           if ((prevSubrecord != cINDX) && (prevSubrecord != cBNAM))
           {
             std::cerr << "Error: Subrecord before CNAM of ARMO was neither "
                       << "INDX nor BNAM, but \"" << IntTo4Char(prevSubrecord)
                       << "\".\n";
             return false;
           }
           // read female body part name (CNAM)
           if (!loadString256(input, BodyParts.back().FemaleBodyPart, Buffer, cCNAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord CNAM of ARMO!\n";
             return false;
           }
           prevSubrecord = cCNAM;
           break;
      case cENAM:
           if (!EnchantmentID.empty())
           {
             std::cerr << "Error: Record ARMO seems to have two ENAM subrecords.\n";
             return false;
           }
           // read armour enchantment ID
           if (!loadString256(input, EnchantmentID, Buffer, cENAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord ENAM of ARMO!\n";
             return false;
           }
           if (EnchantmentID.empty())
           {
             std::cerr << "Error: Subrecord ENAM of ARMO is empty!\n";
             return false;
           }
           prevSubrecord = cENAM;
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: Record ARMO seems to have two ITEX subrecords.\n";
             return false;
           }
           // read path to armour icon texture (ITEX)
           if (!loadString256(input, InventoryIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of ARMO!\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Subrecord ITEX of ARMO is empty!\n";
             return false;
           }
           prevSubrecord = cITEX;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected INDX, BNAM, CNAM or ENAM.\n";
           return false;
    }
  }

  return true;
}

} // namespace
