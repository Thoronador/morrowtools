/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013  Thoronador

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

#include "IngredientRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

IngredientRecord::IngredientRecord()
{
  recordID = ModelName = IngredientName = "";
  //IngredientData
  Weight = 0.0f;
  Value = 0;
  EffectID[0]=EffectID[1]=EffectID[2]=EffectID[3]=0;
  SkillID[0]=SkillID[1]=SkillID[2]=SkillID[3]=0;
  AttributeID[0]=AttributeID[1]=AttributeID[2]=AttributeID[3]=0;
  // end of Ingredient data
  InventoryIcon = ScriptName = "";
}

bool IngredientRecord::equals(const IngredientRecord& other) const
{
  if ((ModelName==other.ModelName) and (IngredientName==other.IngredientName)
      and (Weight==other.Weight) and (Value==other.Value)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName)
      and (recordID==other.recordID))
  {
    //compare effects
    unsigned int i;
    for (i=0; i<4; ++i)
    {
      if (EffectID[i]!=other.EffectID[i]) return false;
      if (SkillID[i]!=other.SkillID[i]) return false;
      if (AttributeID[i]!=other.AttributeID[i]) return false;
    }//for
    return true;
  }
  return false;
}//equals()

void IngredientRecord::show()
{
  std::cout <<"Name: \""<<IngredientName<<"\"\n"
            <<"Mesh: \""<<ModelName<<"\"\n"
            <<"Weight: "<<Weight<<"\n"
            <<"Value: "<<Value<<"\n"
            <<"Effects: "<<EffectID[0]<<", "<<EffectID[1]<<", "<<EffectID[2]<<", "<<EffectID[3]<<"\n"
            <<"Skills: "<<SkillID[0]<<", "<<SkillID[1]<<", "<<SkillID[2]<<", "<<SkillID[3]<<"\n"
            <<"Attributes: "<<AttributeID[0]<<", "<<AttributeID[1]<<", "<<AttributeID[2]<<", "<<AttributeID[3]<<"\n"
            <<"Icon: \""<<InventoryIcon<<"\"\n"
            <<"Script: \""<<ScriptName<<"\"\n";
}

#ifndef MW_UNSAVEABLE_RECORDS
bool IngredientRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cINGR, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL-termination */
        +4 /* MODL */ +4 /* 4 bytes for MODL's lenght */
        +ModelName.length()+1 /*length of name plus one for NUL-termination */
        +4 /* FNAM */ +4 /* 4 bytes for FNAM's lenght */
        +IngredientName.length()+1 /*length of name plus one for NUL-termination */
        +4 /* IRDT */ +4 /* IRDT's length */ +56 /*size of ingredient data (IRDT)*/
        +4 /* ITEX */ +4 /* ITEX's length */
        +InventoryIcon.length() +1 /* length of path +1 byte for NUL-termination */;
  if (!ScriptName.empty())
  {
    Size = Size +4 /* SCRI */ +4 /* 4 bytes for SCRI's lenght */
          +ScriptName.length()+1 /* length of script ID +1 byte for NUL-termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = recordID.length()+1; //length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  SubLength = ModelName.length()+1; //length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write model path
  output.write(ModelName.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  SubLength = IngredientName.length()+1; //length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ingredient name
  output.write(IngredientName.c_str(), SubLength);

  //write IRDT
  output.write((const char*) &cIRDT, 4);
  //IRDT's length
  SubLength = 56; //Fixed size for IRDT is 56 bytes.
  output.write((const char*) &SubLength, 4);
  //write ingredient data
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &(EffectID[0]), 4);
  output.write((const char*) &(EffectID[1]), 4);
  output.write((const char*) &(EffectID[2]), 4);
  output.write((const char*) &(EffectID[3]), 4);
  output.write((const char*) &(SkillID[0]), 4);
  output.write((const char*) &(SkillID[1]), 4);
  output.write((const char*) &(SkillID[2]), 4);
  output.write((const char*) &(SkillID[3]), 4);
  output.write((const char*) &(AttributeID[0]), 4);
  output.write((const char*) &(AttributeID[1]), 4);
  output.write((const char*) &(AttributeID[2]), 4);
  output.write((const char*) &(AttributeID[3]), 4);

  //write ITEX
  output.write((const char*) &cITEX, 4);
  //MODL's length
  SubLength = InventoryIcon.length()+1; //length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptName.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    SubLength = ScriptName.length()+1; //length of string plus one for NUL-termination
    //write SCRI's length
    output.write((const char*) &SubLength, 4);
    //write Script ID
    output.write(ScriptName.c_str(), SubLength);
  }//if script ID present
  return output.good();
}
#endif

bool IngredientRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

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
    std::cout << "Sub record NAME of INGR is longer than 255 characters!\n";
    return false;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of INGR.\n";
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
    std::cout << "Sub record MODL of INGR is longer than 255 characters!\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of INGR.\n";
    return false;
  }
  ModelName = std::string(Buffer);

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
    std::cout << "Sub record FNAM of INGR is longer than 255 characters!\n";
    return false;
  }
  //read "real" name of ingred
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of INGR.\n";
    return false;
  }
  IngredientName = std::string(Buffer);

  //read IRDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cIRDT)
  {
    UnexpectedRecord(cIRDT, SubRecName);
    return false;
  }
  //IRDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=56)
  {
    std::cout <<"Error: sub record IRDT of INGR has invalid length ("<<SubLength
              <<" bytes). Should be 56 bytes.\n";
    return false;
  }
  //read ingredient data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &EffectID[0], 4);
  in_File.read((char*) &(EffectID[1]), 4);
  in_File.read((char*) &(EffectID[2]), 4);
  in_File.read((char*) &(EffectID[3]), 4);
  in_File.read((char*) &(SkillID[0]), 4);
  in_File.read((char*) &(SkillID[1]), 4);
  in_File.read((char*) &(SkillID[2]), 4);
  in_File.read((char*) &(SkillID[3]), 4);
  in_File.read((char*) &(AttributeID[0]), 4);
  in_File.read((char*) &(AttributeID[1]), 4);
  in_File.read((char*) &(AttributeID[2]), 4);
  in_File.read((char*) &(AttributeID[3]), 4);
  BytesRead += 56;
  if (!in_File.good())
  {
    std::cout << "Error while reading IRDT values.\n";
    return false;
  }

  //read SCRI (optional) and ITEX (mandatory)
  ScriptName.clear();
  InventoryIcon.clear();
  while (BytesRead<Size)
  {
    //read next record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cout << "Error: INGR seems to have more than one ITEX subrecord!\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout <<"Error: sub record ITEX of INGR is longer than 255 characters.\n";
             return false;
           }
           //read icon path
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading icon path of INGR.\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           //content check
           if (InventoryIcon.empty())
           {
             std::cout << "Error: subrecord ITEX of INGR is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptName.empty())
           {
             std::cout << "Error: INGR seems to have more than one SCRI subrecord!\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout <<"Error: sub record SCRI of INGR is longer than 255 characters.\n";
             return false;
           }
           memset(Buffer, '\0', 256);
           //read ingredient's script name
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of INGR.\n";
             return false;
           }
           ScriptName = std::string(Buffer);
           //content check
           if (ScriptName.empty())
           {
             std::cout << "Error: subrecord SCRI of INGR is empty!\n";
             return false;
           }
           break;
      default:
           //other subrecord means error, so quit here
           std::cout << "IngredientRecord: Error: expected record name ITEX or SCRI. "
                     << "Instead, \""<<IntTo4Char(SubRecName)<<"\" was found.\n"
                     << "Position is "<<in_File.tellg()<<".\n";
           return false;
    }//swi
  }//while
  if (InventoryIcon.empty())
  {
    std::cout << "IngredientRecord::loadFromStream: Warning: Empty or no inventory "
              << "icon path read.\n";
    /* Maybe we should return false here and not just put a warning. This way
       the application knows that something was not quite right. */
    return false;
  }
  return in_File.good();
}

} //namespace
