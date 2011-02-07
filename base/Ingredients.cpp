/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#include "Ingredients.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

IngredRec::IngredRec()
{
  ModelName= IngredName = "";
  //IngredData
  Weight = 0.0f;
  Value = 0;
  EffectID[0]=EffectID[1]=EffectID[2]=EffectID[3]=0;
  SkillID[0]=SkillID[1]=SkillID[2]=SkillID[3]=0;
  AttributeID[0]=AttributeID[1]=AttributeID[2]=AttributeID[3]=0;
  // end of Ingred data
  InventoryIcon = ScriptName = "";
}

bool IngredRec::equals(const IngredRec& other) const
{
  if ((ModelName==other.ModelName) and (IngredName==other.IngredName)
      and (Weight==other.Weight) and (Value==other.Value)
      and (InventoryIcon==other.InventoryIcon) and (ScriptName==other.ScriptName))
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

void IngredRec::show()
{
  std::cout <<"Name: \""<<IngredName<<"\"\n"
            <<"Mesh: \""<<ModelName<<"\"\n"
            <<"Weight: "<<Weight<<"\n"
            <<"Value: "<<Value<<"\n"
            <<"Effects: "<<EffectID[0]<<", "<<EffectID[1]<<", "<<EffectID[2]<<", "<<EffectID[3]<<"\n"
            <<"Skills: "<<SkillID[0]<<", "<<SkillID[1]<<", "<<SkillID[2]<<", "<<SkillID[3]<<"\n"
            <<"Attributes: "<<AttributeID[0]<<", "<<AttributeID[1]<<", "<<AttributeID[2]<<", "<<AttributeID[3]<<"\n"
            <<"Icon: \""<<InventoryIcon<<"\"\n"
            <<"Script: \""<<ScriptName<<"\"\n";
}

Ingredients::Ingredients()
{
  m_Ingredients.clear();
}

Ingredients::~Ingredients()
{
  m_Ingredients.clear();
}

Ingredients& Ingredients::getSingleton()
{
  static Ingredients Instance;
  return Instance;
}

void Ingredients::addIngredient(const std::string& IngredID, const IngredRec& data)
{
  m_Ingredients[IngredID] = data;
}

void Ingredients::clearAll()
{
  m_Ingredients.clear();
}

unsigned int Ingredients::getNumberOfIngredients() const
{
  return m_Ingredients.size();
}

bool Ingredients::hasIngredient(const std::string& ID) const
{
  return m_Ingredients.find(ID)!=m_Ingredients.end();
}

const IngredRec& Ingredients::getIngredientData(const std::string& ID) const
{
  const std::map<const std::string, IngredRec>::const_iterator iter =
        m_Ingredients.find(ID);
  if (iter!=m_Ingredients.end())
  {
    return iter->second;
  }
  throw 42;
}

void Ingredients::listAll() const
{
  std::cout << "Ingredient list ("<<m_Ingredients.size()<<" items):\n";
  std::map<const std::string, IngredRec>::const_iterator iter = m_Ingredients.begin();
  while (iter!=m_Ingredients.end())
  {
    std::cout << "  "<<iter->first <<" (\""<<iter->second.IngredName<<"\")\n";
    ++iter;
  }//while
}

IngredListIterator Ingredients::getBegin() const
{
  return m_Ingredients.begin();
}
IngredListIterator Ingredients::getEnd() const
{
  return m_Ingredients.end();
}

int Ingredients::readRecordINGR(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

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

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return -1;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Sub record NAME of INGR is longer than 255 characters!\n";
    return -1;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of INGR.\n";
    return -1;
  }
  const std::string IngredID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return -1;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Sub record MODL of INGR is longer than 255 characters!\n";
    return -1;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of INGR.\n";
    return -1;
  }
  IngredRec tempRec;
  tempRec.ModelName = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return -1;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Sub record FNAM of INGR is longer than 255 characters!\n";
    return -1;
  }
  //read "real" name of ingred
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  tempRec.IngredName = std::string(Buffer);

  //read IRDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cIRDT)
  {
    UnexpectedRecord(cIRDT, SubRecName);
    return -1;
  }
  //IRDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=56)
  {
    std::cout <<"Error: sub record IRDT of INGR has invalid length ("<<SubLength
              <<" bytes). Should be 56 bytes.\n";
    return -1;
  }
  //read ingredient data
  in_File.read((char*) &(tempRec.Weight), 4);
  in_File.read((char*) &(tempRec.Value), 4);
  in_File.read((char*) &(tempRec.EffectID[0]), 4);
  in_File.read((char*) &(tempRec.EffectID[1]), 4);
  in_File.read((char*) &(tempRec.EffectID[2]), 4);
  in_File.read((char*) &(tempRec.EffectID[3]), 4);
  in_File.read((char*) &(tempRec.SkillID[0]), 4);
  in_File.read((char*) &(tempRec.SkillID[1]), 4);
  in_File.read((char*) &(tempRec.SkillID[2]), 4);
  in_File.read((char*) &(tempRec.SkillID[3]), 4);
  in_File.read((char*) &(tempRec.AttributeID[0]), 4);
  in_File.read((char*) &(tempRec.AttributeID[1]), 4);
  in_File.read((char*) &(tempRec.AttributeID[2]), 4);
  in_File.read((char*) &(tempRec.AttributeID[3]), 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading IRDT values.\n";
    return -1;
  }

  //read optional SCRI
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    if (SubLength>255)
    {
      std::cout <<"Error: sub record SCRI of INGR is longer than 255 "
                <<"characters.\n";
      return -1;
    }
    memset(Buffer, '\0', 256);
    //read ingredient's script name
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cout << "Error while reading script name of INGR.\n";
      return -1;
    }
    tempRec.ScriptName = std::string(Buffer);
  }
  else
  { //search four bytes towards beginning to land before name of next record
    in_File.seekg(-4, std::ios::cur);
  }

  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return -1;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  //read icon path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading icon path of INGR.\n";
    return -1;
  }
  tempRec.InventoryIcon = std::string(Buffer);

  if (in_File.good())
  {
    //check for presence of ingredient
    if (hasIngredient(IngredID))
    {
      if (getIngredientData(IngredID).equals(tempRec))
      {
        return 0; //return zero, nothing was changed
      }
    }//if
    addIngredient(IngredID, tempRec);
    return 1;
  }
  std::cout << "Error while reading ingredient record.\n";
  return -1;
}//ReadINGR
