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

#include "ArmourRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

ArmourRecord::ArmourRecord()
: BasicRecord(),
  recordID(""),
  Model(""),
  Name(""),
  //armour data
  ArmourType(0),
  Weight(0.0f),
  Value(0),
  Health(0),
  EnchantmentPoints(0),
  ArmourRating(0),
  //end of armour data
  InventoryIcon(""),
  ArmourBodyParts(std::vector<BodyPartAssociation>()),
  ScriptID(""),
  EnchantmentID("")
{}

bool ArmourRecord::equals(const ArmourRecord& other) const
{
  return ((recordID==other.recordID) and (Model==other.Model)
      and (Name==other.Name) and (ArmourType==other.ArmourType)
      and (Weight==other.Weight) and (Value==other.Value)
      and (Health==other.Health) and (EnchantmentPoints==other.EnchantmentPoints)
      and (ArmourRating==other.ArmourRating) and (InventoryIcon==other.InventoryIcon)
      and (ArmourBodyParts==other.ArmourBodyParts) and (ScriptID==other.ScriptID)
      and (EnchantmentID==other.EnchantmentID));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ArmourRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cARMO, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +Model.length()+1 /* length of model path +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* AODT */ +4 /* 4 bytes for length */ +24 /* fixed length of 24 bytes */;
  unsigned int i;
  //body part stuff
  for (i=0; i<ArmourBodyParts.size(); ++i)
  {
    Size = Size + 4 /* INDX */ +4 /* 4 bytes for length */ +1 /* fixed length of 1 byte */;
    if (!ArmourBodyParts[i].MaleBodyPart.empty())
    {
      Size = Size + 4 /* BNAM */ +4 /* 4 bytes for length */
            +ArmourBodyParts[i].MaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
    }
    if (!ArmourBodyParts[i].FemaleBodyPart.empty())
    {
      Size = Size + 4 /* CNAM */ +4 /* 4 bytes for length */
            +ArmourBodyParts[i].FemaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
    }
  }//for

  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!EnchantmentID.empty())
  {
    Size = Size + 4 /* ENAM */ +4 /* 4 bytes for length */
          +EnchantmentID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!InventoryIcon.empty())
  {
    Size = Size +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of icon +1 byte for NUL termination */;
  }

  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  SubLength = Model.length()+1;
  //write MODL's length
  output.write((const char*) &SubLength, 4);
  //write armour's model path
  output.write(Model.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = Name.length()+1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write armour's "real" name
  output.write(Name.c_str(), SubLength);

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

  //write AODT
  output.write((const char*) &cAODT, 4);
  SubLength = 24; //fixed length of 24 bytes
  //write AODT's length
  output.write((const char*) &SubLength, 4);
  //write armour data
  output.write((const char*) &ArmourType, 4);
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &Health, 4);
  output.write((const char*) &EnchantmentPoints, 4);
  output.write((const char*) &ArmourRating, 4);

  if (!InventoryIcon.empty())
  {
    //write ITEX
    output.write((const char*) &cITEX, 4);
    SubLength = InventoryIcon.length()+1;
    //write ITEX's length
    output.write((const char*) &SubLength, 4);
    //write armour's inventory icon
    output.write(InventoryIcon.c_str(), SubLength);
  }

  //body part associations
  for (i=0; i<ArmourBodyParts.size(); ++i)
  {
    //write INDX
    output.write((const char*) &cINDX, 4);
    SubLength = 1; //fixed length of one byte
    //write INDX's length
    output.write((const char*) &SubLength, 4);
    //write body part index
    output.write((const char*) &(ArmourBodyParts[i].Index), 1);

    if (!ArmourBodyParts[i].MaleBodyPart.empty())
    {
      //write BNAM
      output.write((const char*) &cBNAM, 4);
      SubLength = ArmourBodyParts[i].MaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
      //write BNAM's length
      output.write((const char*) &SubLength, 4);
      //write male body part name
      output.write(ArmourBodyParts[i].MaleBodyPart.c_str(), SubLength);
    }
    if (!ArmourBodyParts[i].FemaleBodyPart.empty())
    {
      //write CNAM
      output.write((const char*) &cCNAM, 4);
      SubLength = ArmourBodyParts[i].FemaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
      //write BNAM's length
      output.write((const char*) &SubLength, 4);
      //write male body part name
      output.write(ArmourBodyParts[i].FemaleBodyPart.c_str(), SubLength);
    }
  }//for

  if (!EnchantmentID.empty())
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    SubLength = EnchantmentID.length()+1;
    //write ENAM's length
    output.write((const char*) &SubLength, 4);
    //write enchantment ID
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool ArmourRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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

  uint32_t SubRecName;
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
    std::cout << "Error: subrecord NAME of ARMO is longer than 255 characters.\n";
    return false;
  }
  //read armour ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of ARMO!\n";
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
    std::cout << "Error: subrecord MODL of ARMO is longer than 255 characters.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of ARMO!\n";
    return false;
  }
  Model = std::string(Buffer);

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
    std::cout << "Error: subrecord FNAM of ARMO is longer than 255 characters.\n";
    return false;
  }
  //read armour's "real" name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of ARMO!\n";
    return false;
  }
  Name = std::string(Buffer);

  //read SCRI or AODT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord SCRI of ARMO is longer than 255 characters.\n";
      return false;
    }
    //read armour script ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SCRI of ARMO!\n";
      return false;
    }
    ScriptID = std::string(Buffer);

    //read AODT
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }//if
  else
  {
    ScriptID = "";
  }

  //read AODT
  //was already read above
  if (SubRecName!=cAODT)
  {
    UnexpectedRecord(cAODT, SubRecName);
    return false;
  }
  //AODT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=24)
  {
    std::cout <<"Error: sub record AODT of ARMO has invalid length ("<<SubLength
              <<" bytes). Should be 24 bytes.\n";
    return false;
  }
  //read armour data
  in_File.read((char*) &ArmourType, 4);
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Health, 4);
  in_File.read((char*) &EnchantmentPoints, 4);
  in_File.read((char*) &ArmourRating, 4);
  BytesRead += 24;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord AODT of ARMO!\n";
    return false;
  }

  EnchantmentID = "";
  InventoryIcon = "";
  ArmourBodyParts.clear();
  BodyPartAssociation tempAssoc;
  bool hasENAM = false;
  bool hasITEX = false;
  uint32_t prevSubrecord = 0;
  while (BytesRead<Size)
  {
    //read next subrecord's name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cINDX:
           //INDX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=1)
           {
             std::cout <<"Error: sub record INDX of ARMO has invalid length ("
                       << SubLength<<" bytes). Should be one byte only.\n";
             return false;
           }
           //read body part index
           in_File.read((char*) &(tempAssoc.Index), 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INDX of ARMO!\n";
             return false;
           }
           tempAssoc.MaleBodyPart = "";
           tempAssoc.FemaleBodyPart = "";
           ArmourBodyParts.push_back(tempAssoc);
           prevSubrecord = cINDX;
           break;
      case cBNAM:
           if (prevSubrecord!=cINDX)
           {
             std::cout << "Error: Subrecord before BNAM of ARMO was not INDX, "
                       << "but \""<<IntTo4Char(prevSubrecord)<< "\".\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord BNAM of ARMO is longer than 255 characters.\n";
             return false;
           }
           //read male body part name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of ARMO!\n";
             return false;
           }
           ArmourBodyParts.back().MaleBodyPart = std::string(Buffer);
           prevSubrecord = cBNAM;
           break;
      case cCNAM:
           if ((prevSubrecord!=cINDX) and (prevSubrecord!=cBNAM))
           {
             std::cout << "Error: Subrecord before CNAM of ARMO was neither "
                       << "INDX nor BNAM, but \""<<IntTo4Char(prevSubrecord)
                       << "\".\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CNAM of ARMO is longer than 255 characters.\n";
             return false;
           }
           //read female body part name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of ARMO!\n";
             return false;
           }
           ArmourBodyParts.back().FemaleBodyPart = std::string(Buffer);
           prevSubrecord = cCNAM;
           break;
      case cENAM:
           if (hasENAM)
           {
             std::cout << "Error: record ARMO seems to have two ENAM subrecords.\n";
             return false;
           }
           //ENAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ENAM of ARMO is longer than 255 characters.\n";
             return false;
           }
           //read armour enchantment ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ENAM of ARMO!\n";
             return false;
           }
           EnchantmentID = std::string(Buffer);
           hasENAM = true;
           prevSubrecord = cENAM;
           break;
      case cITEX:
           if (hasITEX)
           {
             std::cout << "Error: record ARMO seems to have two ITEX subrecords.\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ITEX of ARMO is longer than 255 characters.\n";
             return false;
           }
           //read path to armour icon texture
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of ARMO!\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           hasITEX = true;
           prevSubrecord = cITEX;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     << "\" found. Expected INDX, BNAM, CNAM or ENAM.\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
