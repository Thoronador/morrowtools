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

#include "MagicEffects.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

MagicEffects::MagicEffects()
{
  //empty
}

MagicEffects::~MagicEffects()
{
  m_Effects.clear();
}

MagicEffects& MagicEffects::getSingleton()
{
  static MagicEffects Instance;
  return Instance;
}

void MagicEffects::addEffect(const int Index, const MGEF_Data& Data)
{
  m_Effects[Index] = Data;
}

bool MagicEffects::hasEffect(const int Index) const
{
  return m_Effects.find(Index)!=m_Effects.end();
}

unsigned int MagicEffects::getNumberOfEffects() const
{
  return m_Effects.size();
}

const MGEF_Data& MagicEffects::getEffect(const int Index) const
{
  std::map<int, MGEF_Data>::const_iterator iter;
  iter = m_Effects.find(Index);
  if (iter!=m_Effects.end())
  {
    return iter->second;
  }
  //nothing found
  std::cout << "No magic effect with index "<<Index<<" found!\n";
  throw 1;
}

bool MagicEffects::readMGEF(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Blue
        long  Green
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string
    PTEX = Particle texture string
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return false;
  }

  int32_t temp_Index = -1;
  in_File.read((char*) &temp_Index, 4); //read the index

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=36)
  {
    std::cout << "Error: sub record MEDT of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 36 bytes.\n";
    return false;
  }

  MGEF_Data tempData;

  //in_File.seekg(36, std::ios::cur); //skip the data
  //SpellSchool
  in_File.read((char*) &(tempData.SpellSchool), 4);
  //Cost
  in_File.read((char*) &(tempData.BaseCost), 4);
  //Flags
  in_File.read((char*) &(tempData.Flags), 4);
  //Colour
  in_File.read((char*) &(tempData.Red), 4);
  in_File.read((char*) &(tempData.Blue), 4);
  in_File.read((char*) &(tempData.Green), 4);
  //speed
  in_File.read((char*) &(tempData.SpeedX), 4);
  //size
  in_File.read((char*) &(tempData.SizeX), 4);
  //size cap
  in_File.read((char*) &(tempData.SizeCap), 4);


  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  char Buffer [1024];
  memset(Buffer, '\0', 1024);
  //read tex name
  in_File.read(Buffer, SubLength);
  tempData.EffectIcon = std::string(Buffer);

  //read PTEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPTEX)
  {
    UnexpectedRecord(cPTEX, SubRecName);
    return false;
  }
  //PTEX's length
  in_File.read((char*) &SubLength, 4);
  //read particle tex name
  memset(Buffer, '\0', 1024);
  in_File.read(Buffer, SubLength);
  tempData.ParticleTexture = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);

  //visual strings, partially optional
  tempData.CastingVisual = "";
  tempData.BoltVisual = "";
  tempData.HitVisual = "";
  tempData.AreaVisual = "";
  //description, optional
  tempData.Description = "";
  //effect sounds, all optional
  tempData.CastSound = "";
  tempData.BoltSound = "";
  tempData.HitSound = "";
  tempData.AreaSound = "";
  bool go_on_with_subs = true;
  do
  {
    switch(SubRecName)
    {
      case cCVFX: //read optional CVFX
           //CVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastingVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBVFX: //read optional BVFX
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHVFX: //read optional HVFX
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cAVFX: //read optional AVFX
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cDESC: //read optional DESC
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           //read effect description
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.Description = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      default:
           //unknown sub record, thus stop here, it's beginning of new record
           go_on_with_subs = false;
    }//switch
  } while (go_on_with_subs);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  if (in_File.good())
  {
    MagicEffects::getSingleton().addEffect(temp_Index, tempData);
    return true;
  }
  return false;
}//readMGEF

EffectListIterator MagicEffects::getBegin() const
{
  return m_Effects.begin();
}

EffectListIterator MagicEffects::getEnd() const
{
  return m_Effects.end();
}

void MagicEffects::clearAll()
{
  m_Effects.clear();
}
