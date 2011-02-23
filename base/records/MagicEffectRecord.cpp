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

#include "MagicEffectRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

bool MGEF_Data::equals(const MGEF_Data& other) const
{
  return ((Index==other.Index)
      and (SpellSchool==other.SpellSchool) and (BaseCost==other.BaseCost)
      and (Flags==other.Flags) and (Red==other.Red) and (Green==other.Green)
      and (Blue==other.Blue) and (SpeedX==other.SpeedX) and (SizeX==other.SizeX)
      and (SizeCap==other.SizeCap) and (EffectIcon==other.EffectIcon)
      and(ParticleTexture==other.ParticleTexture) and (CastingVisual==other.CastingVisual)
      and (BoltVisual==other.BoltVisual) and (HitVisual==other.HitVisual)
      and (AreaVisual==other.AreaVisual) and (Description==other.Description)
      and (CastSound==other.CastSound) and (BoltSound==other.BoltSound)
      and (HitSound==other.HitSound) and (AreaSound==other.AreaSound));
}

bool MGEF_Data::saveToStream(std::ofstream& output) const
{
  //write MGEF
  output.write((char*) &cMGEF, 4);
  int32_t Size;
  Size = 4 /* INDX */ +4 /* four bytes for length */ +4 /* length of index */
        +4 /* MEDT */ +4 /* four bytes for length */ +36 /* length of effect data */
        +4 /* ITEX */ +4 /* four bytes for length */
        +EffectIcon.length()+1 /* length of effect icon +1 byte for NUL-termination */
        +4 /* PTEX */ +4 /* four bytes for length */
        +ParticleTexture.length()+1 /* length of particle texture +1 byte for NUL-termination */;
  if (CastingVisual!="")
  {
    Size += 4 /* CVFX */ +4 /* four bytes for length */
           +CastingVisual.length()+1 /* length of casting visual +1 byte for NUL-termination */;
  }
  if (BoltVisual!="")
  {
    Size += 4 /* BVFX */ +4 /* four bytes for length */
           +BoltVisual.length()+1 /* length of bolt visual +1 byte for NUL-termination */;
  }
  if (HitVisual!="")
  {
    Size += 4 /* HVFX */ +4 /* four bytes for length */
           +HitVisual.length()+1 /* length of hit visual +1 byte for NUL-termination */;
  }
  if (AreaVisual!="")
  {
    Size += 4 /* AVFX */ +4 /* four bytes for length */
           +AreaVisual.length()+1 /* length of area visual +1 byte for NUL-termination */;
  }
  if (Description!="")
  {
    Size += 4 /* DESC */ +4 /* four bytes for length */
           +Description.length() /* length of description (no NUL-termination) */;
  }
  if (CastSound!="")
  {
    Size += 4 /* CSND */ +4 /* four bytes for length */
           +CastSound.length()+1 /* length of casting sound +1 byte for NUL-termination */;
  }
  if (BoltSound!="")
  {
    Size += 4 /* BSND */ +4 /* four bytes for length */
           +BoltSound.length()+1 /* length of bolt sound +1 byte for NUL-termination */;
  }
  if (HitSound!="")
  {
    Size += 4 /* HSND */ +4 /* four bytes for length */
           +HitSound.length()+1 /* length of hit sound +1 byte for NUL-termination */;
  }
  if (AreaSound!="")
  {
    Size += 4 /* ASND */ +4 /* four bytes for length */
           +AreaSound.length()+1 /* length of area sound +1 byte for NUL-termination */;
  }

  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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
    DESC = Description text (optional?)
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  //write INDX
  output.write((char*) &cINDX, 4);
  //INDX's length
  int32_t SubLength;
  SubLength = 4; //fixed length of four bytes
  output.write((char*) &SubLength, 4);
  //write INDX
  output.write((char*) &Index, 4);

  //write MEDT
  output.write((char*) &cMEDT, 4);
  //MEDT's length
  SubLength = 36; //fixed length of 36 bytes
  output.write((char*) &SubLength, 4);
  //write MEDT
  // ---- SpellSchool
  output.write((char*) &SpellSchool, 4);
  // ---- Cost
  output.write((char*) &BaseCost, 4);
  // ---- Flags
  output.write((char*) &Flags, 4);
  // ---- Colour
  output.write((char*) &Red, 4);
  output.write((char*) &Blue, 4);
  output.write((char*) &Green, 4);
  // ---- speed
  output.write((char*) &SpeedX, 4);
  // ---- size
  output.write((char*) &SizeX, 4);
  // ---- size cap
  output.write((char*) &SizeCap, 4);

  //write ITEX
  output.write((char*) &cITEX, 4);
  //ITEX's length
  SubLength = EffectIcon.length()+1; /* length of effect icon +1 byte for NUL-termination */
  output.write((char*) &SubLength, 4);
  //write ITEX
  output.write(EffectIcon.c_str(), SubLength);

  //write PTEX
  output.write((char*) &cPTEX, 4);
  //PTEX's length
  SubLength = ParticleTexture.length()+1; /* length of particle +1 byte for NUL-termination */
  output.write((char*) &SubLength, 4);
  //write PTEX
  output.write(ParticleTexture.c_str(), SubLength);

  if (CastingVisual!="")
  {
    //write CVFX
    output.write((char*) &cCVFX, 4);
    //CVFX's length
    SubLength = CastingVisual.length()+1; /* length of cast visual +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write CVFX
    output.write(CastingVisual.c_str(), SubLength);
  }

  if (BoltVisual!="")
  {
    //write BVFX
    output.write((char*) &cBVFX, 4);
    //BVFX's length
    SubLength = BoltVisual.length()+1; /* length of bolt visual +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write BVFX
    output.write(BoltVisual.c_str(), SubLength);
  }

  if (HitVisual!="")
  {
    //write HVFX
    output.write((char*) &cHVFX, 4);
    //HVFX's length
    SubLength = HitVisual.length()+1; /* length of hit visual +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write HVFX
    output.write(HitVisual.c_str(), SubLength);
  }

  if (AreaVisual!="")
  {
    //write AVFX
    output.write((char*) &cAVFX, 4);
    //AVFX's length
    SubLength = AreaVisual.length()+1; /* length of area visual +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write AVFX
    output.write(AreaVisual.c_str(), SubLength);
  }

  if (Description!="")
  {
    //write DESC
    output.write((char*) &cDESC, 4);
    //DESC's length
    SubLength = AreaVisual.length(); /* length of description (no NUL-termination) */
    output.write((char*) &SubLength, 4);
    //write DESC
    output.write(Description.c_str(), SubLength);
  }

  if (CastSound!="")
  {
    //write CSND
    output.write((char*) &cCSND, 4);
    //CSND's length
    SubLength = CastSound.length()+1; /* length of cast sound +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write CSND
    output.write(CastSound.c_str(), SubLength);
  }

  if (BoltSound!="")
  {
    //write BSND
    output.write((char*) &cBSND, 4);
    //BSND's length
    SubLength = BoltSound.length()+1; /* length of bolt sound +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write BSND
    output.write(BoltSound.c_str(), SubLength);
  }

  if (HitSound!="")
  {
    //write HSND
    output.write((char*) &cHSND, 4);
    //HSND's length
    SubLength = HitSound.length()+1; /* length of hit sound +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write HSND
    output.write(HitSound.c_str(), SubLength);
  }

  if (AreaSound!="")
  {
    //write ASND
    output.write((char*) &cASND, 4);
    //ASND's length
    SubLength = HitSound.length()+1; /* length of area sound +1 byte for NUL-termination */
    output.write((char*) &SubLength, 4);
    //write ASND
    output.write(AreaSound.c_str(), SubLength);
  }

  return output.good();
}

bool MGEF_Data::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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
    DESC = Description text (optional?)
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

  Index = -1;
  in_File.read((char*) &Index, 4); //read the index

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

  //SpellSchool
  in_File.read((char*) &SpellSchool, 4);
  //Cost
  in_File.read((char*) &BaseCost, 4);
  //Flags
  in_File.read((char*) &Flags, 4);
  //Colour
  in_File.read((char*) &Red, 4);
  in_File.read((char*) &Blue, 4);
  in_File.read((char*) &Green, 4);
  //speed
  in_File.read((char*) &SpeedX, 4);
  //size
  in_File.read((char*) &SizeX, 4);
  //size cap
  in_File.read((char*) &SizeCap, 4);

  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MEDT of MGEF.\n";
    return false;
  }

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
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ITEX of MGEF.\n";
    return false;
  }
  EffectIcon = std::string(Buffer);

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
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord PTEX of MGEF.\n";
    return false;
  }
  ParticleTexture = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);

  //visual strings, partially optional
  CastingVisual = "";
  BoltVisual = "";
  HitVisual = "";
  AreaVisual = "";
  //description, optional
  Description = "";
  //effect sounds, all optional
  CastSound = "";
  BoltSound = "";
  HitSound = "";
  AreaSound = "";
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
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CVFX of MGEF.\n";
             return false;
           }
           CastingVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBVFX: //read optional BVFX
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BVFX of MGEF.\n";
             return false;
           }
           BoltVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHVFX: //read optional HVFX
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord HVFX of MGEF.\n";
             return false;
           }
           HitVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cAVFX: //read optional AVFX
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AVFX of MGEF.\n";
             return false;
           }
           AreaVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cDESC: //read optional DESC
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           //read effect description
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DESC of MGEF.\n";
             return false;
           }
           Description = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CSND of MGEF.\n";
             return false;
           }
           CastSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BSND of MGEF.\n";
             return false;
           }
           BoltSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord HSND of MGEF.\n";
             return false;
           }
           HitSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ASND of MGEF.\n";
             return false;
           }
           AreaSound = std::string(Buffer);
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
  return in_File.good();
}
