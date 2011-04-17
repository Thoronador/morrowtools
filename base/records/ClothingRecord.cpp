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

#include "ClothingRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

ClothingRecord::ClothingRecord()
{
  ClothingID = Model = Name = "";
  //clothing data
  ClothingType = 0;
  Weight = 0.0f;
  Value = 0;
  EnchantmentPoints = 0;
  //end of clothing data
  InventoryIcon = "";
  ClothingBodyParts.clear();
  ScriptID = EnchantmentID = "";
}

bool ClothingRecord::equals(const ClothingRecord& other) const
{
  return ((ClothingID==other.ClothingID) and (Model==other.Model)
      and (Name==other.Name) and (ClothingType==other.ClothingType)
      and (Weight==other.Weight) and (Value==other.Value)
      and (EnchantmentPoints==other.EnchantmentPoints) and (InventoryIcon==other.InventoryIcon)
      and (ClothingBodyParts==other.ClothingBodyParts) and (ScriptID==other.ScriptID)
      and (EnchantmentID==other.EnchantmentID));
}

bool ClothingRecord::saveToStream(std::ofstream& output) const
{
  #warning Not completly implemented!
  output.write((char*) &cCLOT, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +ClothingID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +Model.length()+1 /* length of model path +1 byte for NUL termination */
        +4 /* CTDT */ +4 /* 4 bytes for length */ +12 /* fixed length of 12 bytes */;
  if (Name!="")
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
          +Name.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (InventoryIcon!="")
  {
    Size = Size +4 /* ITEX */ +4 /* 4 bytes for length */
          +InventoryIcon.length()+1 /* length of icon +1 byte for NUL termination */;
  }
  unsigned int i;
  //body part stuff
  for (i=0; i<ClothingBodyParts.size(); ++i)
  {
    Size = Size + 4 /* INDX */ +4 /* 4 bytes for length */ +1 /* fixed length of 1 byte */;
    if (ClothingBodyParts[i].MaleBodyPart!="")
    {
      Size = Size + 4 /* BNAM */ +4 /* 4 bytes for length */
            +ClothingBodyParts[i].MaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
    }
    if (ClothingBodyParts[i].FemaleBodyPart!="")
    {
      Size = Size + 4 /* CNAM */ +4 /* 4 bytes for length */
            +ClothingBodyParts[i].FemaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
    }
  }//for

  if (ScriptID!="")
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (EnchantmentID!="")
  {
    Size = Size + 4 /* ENAM */ +4 /* 4 bytes for length */
          +EnchantmentID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = ClothingID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(ClothingID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = Model.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write clothing's model path
  output.write(Model.c_str(), SubLength);

  if (Name!="")
  {
    //write FNAM
    output.write((char*) &cFNAM, 4);
    SubLength = Name.length()+1;
    //write FNAM's length
    output.write((char*) &SubLength, 4);
    //write clothing's "real" name
    output.write(Name.c_str(), SubLength);
  }

  //write CTDT
  output.write((char*) &cCTDT, 4);
  SubLength = 12; //fixed length of 12 bytes
  //write CTDT's length
  output.write((char*) &SubLength, 4);
  //write clothing data
  output.write((char*) &ClothingType, 4);
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 2);
  output.write((char*) &EnchantmentPoints, 2);

  if (ScriptID!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = InventoryIcon.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write clothings's script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  if (InventoryIcon!="")
  {
    //write ITEX
    output.write((char*) &cITEX, 4);
    SubLength = InventoryIcon.length()+1;
    //write ITEX's length
    output.write((char*) &SubLength, 4);
    //write clothing's inventory icon
    output.write(InventoryIcon.c_str(), SubLength);
  }

  //body part associations
  for (i=0; i<ClothingBodyParts.size(); ++i)
  {
    //write INDX
    output.write((char*) &cINDX, 4);
    SubLength = 1; //fixed length of one byte
    //write INDX's length
    output.write((char*) &SubLength, 4);
    //write body part index
    output.write((char*) &(ClothingBodyParts[i].Index), 1);

    if (ClothingBodyParts[i].MaleBodyPart!="")
    {
      //write BNAM
      output.write((char*) &cBNAM, 4);
      SubLength = ClothingBodyParts[i].MaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
      //write BNAM's length
      output.write((char*) &SubLength, 4);
      //write male body part name
      output.write(ClothingBodyParts[i].MaleBodyPart.c_str(), SubLength);
    }
    if (ClothingBodyParts[i].FemaleBodyPart!="")
    {
      //write CNAM
      output.write((char*) &cCNAM, 4);
      SubLength = ClothingBodyParts[i].FemaleBodyPart.length()+1 /* length of ID +1 byte for NUL termination */;
      //write BNAM's length
      output.write((char*) &SubLength, 4);
      //write male body part name
      output.write(ClothingBodyParts[i].FemaleBodyPart.c_str(), SubLength);
    }
  }//for

  if (EnchantmentID!="")
  {
    //write ENAM
    output.write((char*) &cENAM, 4);
    SubLength = EnchantmentID.length()+1;
    //write ENAM's length
    output.write((char*) &SubLength, 4);
    //write enchantment ID
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}

bool ClothingRecord::loadFromStream(std::ifstream& in_File)
{
  #warning Not implemented yet! SCRI is still missing.
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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
    std::cout << "Error: subrecord NAME of CLOT is longer than 255 characters.\n";
    return false;
  }
  //read clothing ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of CLOT!\n";
    return false;
  }
  ClothingID = std::string(Buffer);

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
    std::cout << "Error: subrecord MODL of CLOT is longer than 255 characters.\n";
    return false;
  }
  //read path to clothing model
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of CLOT!\n";
    return false;
  }
  Model = std::string(Buffer);

  //read FNAM (optional) or CTDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cFNAM)
  {
    //FNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord FNAM of CLOT is longer than 255 characters.\n";
      return false;
    }
    //read clothing name (the real one)
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FNAM of CLOT!\n";
      return false;
    }
    Name = std::string(Buffer);

    //read CTDT
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    Name = "";
  }

  //read CTDT
  //already read above
  if (SubRecName!=cCTDT)
  {
    UnexpectedRecord(cCTDT, SubRecName);
    return false;
  }
  //CTDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record CTDT of CLOT has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read clothing data
  in_File.read((char*) &ClothingType, 4);
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 2);
  in_File.read((char*) &EnchantmentPoints, 2);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CTDT of CLOT!\n";
    return false;
  }

  EnchantmentID = "";
  ScriptID = "";
  InventoryIcon = "";
  ClothingBodyParts.clear();
  BodyPartAssociation tempAssoc;
  bool hasENAM = false;
  bool hasITEX = false;
  bool hasSCRI = false;
  int32_t prevSubrecord = 0;
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
             std::cout <<"Error: sub record INDX of CLOT has invalid length ("
                       << SubLength<<" bytes). Should be one byte only.\n";
             return false;
           }
           //read body part index
           in_File.read((char*) &(tempAssoc.Index), 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INDX of CLOT!\n";
             return false;
           }
           tempAssoc.MaleBodyPart = "";
           tempAssoc.FemaleBodyPart = "";
           ClothingBodyParts.push_back(tempAssoc);
           prevSubrecord = cINDX;
           break;
      case cBNAM:
           if (prevSubrecord!=cINDX)
           {
             std::cout << "Error: Subrecord before BNAM of CLOTH was not INDX, "
                       << "but \""<<IntTo4Char(prevSubrecord)<< "\".\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord BNAM of CLOT is longer than 255 characters.\n";
             return false;
           }
           //read male body part name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of CLOT!\n";
             return false;
           }
           ClothingBodyParts.back().MaleBodyPart = std::string(Buffer);
           prevSubrecord = cBNAM;
           break;
      case cCNAM:
           if ((prevSubrecord!=cINDX) and (prevSubrecord!=cBNAM))
           {
             std::cout << "Error: Subrecord before CNAM of CLOT was neither "
                       << "INDX nor BNAM, but \""<<IntTo4Char(prevSubrecord)
                       << "\".\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CNAM of CLOT is longer than 255 characters.\n";
             return false;
           }
           //read female body part name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of CLOT!\n";
             return false;
           }
           ClothingBodyParts.back().FemaleBodyPart = std::string(Buffer);
           prevSubrecord = cCNAM;
           break;
      case cITEX:
           if (hasITEX)
           {
             std::cout << "Error: record CLOT seems to have two ITEX subrecords.\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ITEX of CLOT is longer than 255 characters.\n";
             return false;
           }
           //read path to clothing icon texture
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of CLOT!\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           hasITEX = true;
           prevSubrecord = cITEX;
           break;
      case cENAM:
           if (hasENAM)
           {
             std::cout << "Error: record CLOT seems to have two ENAM subrecords.\n";
             return false;
           }
           //ENAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ENAM of CLOT is longer than 255 characters.\n";
             return false;
           }
           //read clothing enchantment ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ENAM of CLOT!\n";
             return false;
           }
           EnchantmentID = std::string(Buffer);
           hasENAM = true;
           prevSubrecord = cENAM;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cout << "Error: record CLOT seems to have two SCRI subrecords.\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord SCRI of CLOT is longer than 255 characters.\n";
             return false;
           }
           //read clothing script ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of CLOT!\n";
             return false;
           }
           ScriptID = std::string(Buffer);
           hasSCRI = true;
           prevSubrecord = cSCRI;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     << "\" found. Expected INDX, BNAM, CNAM or ENAM.\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}
