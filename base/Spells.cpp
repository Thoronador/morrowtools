/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#include "Spells.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

const size_t Spells::cMaximumSpellNameLength = 31;

bool SpellRecord::saveToStream(std::ofstream& output, const std::string& SpellID) const
{
  output.write((char*) &cSPEL, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +SpellID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for FNAM's lenght */
        +Name.length()+1 /*lenght of name plus one for NUL-termination */
        +4 /* SPDT */ +4 /* SPDT's length */ +12 /*size of spell data (SPDT)*/;
  //now calculate additional size of enchantment data
  Size = Size + Enchs.size()
         *(4 /*ENAM*/ +4 /*ENAM's length*/ +24 /*size of enchantment data*/);
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */


  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  int32_t SubLength;
  SubLength = SpellID.length()+1; //lenght of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(SpellID.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  SubLength = Name.length()+1; //lenght of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write spell name
  output.write(Name.c_str(), SubLength);

  //write SPDT
  output.write((char*) &cSPDT, 4);
  //SPDT's length
  SubLength = 12; //lenght of SPDT (spell data) is always 12 bytes
  output.write((char*) &SubLength, 4);
  // ---- write spell data
  //type
  output.write((char*) &(this->Type), 4);
  //cost
  output.write((char*) &(this->Cost), 4);
  //flags
  output.write((char*) &(this->Flags), 4);
  if (!output.good())
  {
    std::cout << "Error while writing sub record SPDT of SPEL.\n";
    return false;
  }

  //write multiple ENAM records
  unsigned int i;
  for (i=0; i<Enchs.size(); ++i)
  {
    output.write((char*) &cENAM, 4);
    //ENAM's length
    SubLength = 24; //ENAM's length is fixed at 24 bytes
    output.write((char*) &SubLength, 4);
    //write enchantment data
    output.write((char*) &(Enchs[i].EffectID), 2);
    output.write((char*) &(Enchs[i].SkillID), 1);
    output.write((char*) &(Enchs[i].AttributeID), 1);
    output.write((char*) &(Enchs[i].RangeType), 4);
    output.write((char*) &(Enchs[i].Area), 4);
    output.write((char*) &(Enchs[i].Duration), 4);
    output.write((char*) &(Enchs[i].MagnitudeMin), 4);
    output.write((char*) &(Enchs[i].MagnitudeMax), 4);
    if (!output.good())
    {
      std::cout << "Error while writing sub record ENAM of SPEL.\n";
      return false;
    }//if
  }//for
  return output.good();
}

Spells::Spells()
{
  //empty
}

Spells::~Spells()
{
  m_Spells.clear();
}

Spells& Spells::getSingleton()
{
  static Spells Instance;
  return Instance;
}

void Spells::addSpell(const std::string& ID, const SpellRecord& record)
{
  if (ID!="")
  {
    m_Spells[ID] = record;
    //truncate names that are too long (more than 31 characters) in order to avoid errors
    if (record.Name.length()>cMaximumSpellNameLength)
    {
      m_Spells[ID].Name = record.Name.substr(0, cMaximumSpellNameLength);
    }//if (inner)
  }
}

unsigned int Spells::getNumberOfSpells() const
{
  return m_Spells.size();
}

bool Spells::hasSpell(const std::string& ID) const
{
  return m_Spells.find(ID)!=m_Spells.end();
}

const SpellRecord& Spells::getSpell(const std::string& ID) const
{
  std::map<std::string, SpellRecord>::const_iterator iter;
  iter = m_Spells.find(ID);
  if (iter!=m_Spells.end())
  {
    return iter->second;
  }
  //nothing found
  std::cout << "Error: No spell with ID \""<<ID<<"\" found!\n";
  throw 2;
}

bool Spells::readSPEL(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "ReadSPEL: Error: ID is longer than 255 characters.\n";
    std::cout << "File pos.: "<<in_File.tellg()<<"\n";
    return false;
  }
  char Buffer[256];
  Buffer[0] = Buffer[255] = '\0';
  //read ID
  in_File.read(Buffer, SubLength);
  const std::string SpellID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "ReadSPEL: Error: name is longer than 255 characters.\n";
    std::cout << "File pos.: "<<in_File.tellg()<<"\n";
    return false;
  }
  //read spell name
  Buffer[0] = Buffer[255] = '\0';
  in_File.read(Buffer, SubLength);
  SpellRecord temp;
  temp.Name = std::string(Buffer);

  //read SPDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSPDT)
  {
    UnexpectedRecord(cSPDT, SubRecName);
    return false;
  }
  //SPDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout << "Error: sub record SPDT of SPEL has invalid length ("
              <<SubLength<< " bytes). Should be 12 bytes.\n";
    return false;
  }
  // ---- read spell data
  //type
  in_File.read((char*) &(temp.Type), 4);
  //cost
  in_File.read((char*) &(temp.Cost), 4);
  //flags
  in_File.read((char*) &(temp.Flags), 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SPDT of SPEL.\n";
    return false;
  }

  temp.Enchs.clear();
  //read multiple ENAM records
  do
  {
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cENAM)
    {
      //ENAM's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=24)
      {
        std::cout << "Error: sub record ENAM of SPEL has invalid length ("
                  <<SubLength<< " bytes). Should be 24 bytes.\n";
        return false;
      }
      //read enchantment data
      EnchantmentData ench;
      in_File.read((char*) &(ench.EffectID), 2);
      in_File.read((char*) &(ench.SkillID), 1);
      in_File.read((char*) &(ench.AttributeID), 1);
      in_File.read((char*) &(ench.RangeType), 4);
      in_File.read((char*) &(ench.Area), 4);
      in_File.read((char*) &(ench.Duration), 4);
      in_File.read((char*) &(ench.MagnitudeMin), 4);
      in_File.read((char*) &(ench.MagnitudeMax), 4);
      if (in_File.good())
      {
        temp.Enchs.push_back(ench);
      }
      else
      {
        std::cout << "Error while reading sub record ENAM of SPEL.\n";
        return false;
      }
    }
  } while (SubRecName == cENAM);
  //seek four bytes towards beginning to land before the name of the next record
  in_File.seekg(-4, std::ios::cur);
  addSpell(SpellID, temp);
  return in_File.good();
}//readSPEL

SpellListIterator Spells::getBegin() const
{
  return m_Spells.begin();
}

SpellListIterator Spells::getEnd() const
{
  return m_Spells.end();
}

bool Spells::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Spells::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  SpellListIterator iter = m_Spells.begin();
  const SpellListIterator end_iter = m_Spells.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output, iter->first))
    {
      std::cout << "Spells::saveAllToStream: Error while writing record.\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

bool Spells::removeSpell(const std::string& ID)
{
  const std::map<std::string, SpellRecord>::iterator iter = m_Spells.find(ID);
  if (iter!=m_Spells.end())
  {
    m_Spells.erase(iter);
    return true;
  }
  return false;
}

void Spells::clearAll()
{
  m_Spells.clear();
}
