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

#include "MagicEffectRecord.h"
#include <iostream>
#include <cstring>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

MGEF_Data::MGEF_Data()
: Index(-1),
  SpellSchool(-1),
  BaseCost(0.0f),
  Flags(0),
  Red(0), Green(0), Blue(0),
  SpeedX(0.0f), SizeX(0.0f), SizeCap(0.0f),
  EffectIcon(""), ParticleTexture(""), CastingVisual(""), BoltVisual(""), HitVisual(""),
  AreaVisual(""), Description(""), CastSound(""), BoltSound(""), HitSound(""), AreaSound("")
{
}

bool MGEF_Data::equals(const MGEF_Data& other) const
{
  return ((Index==other.Index)
      and (SpellSchool==other.SpellSchool) and (BaseCost==other.BaseCost)
      and (Flags==other.Flags) and (Red==other.Red) and (Green==other.Green)
      and (Blue==other.Blue) and (SpeedX==other.SpeedX) and (SizeX==other.SizeX)
      and (SizeCap==other.SizeCap) and (EffectIcon==other.EffectIcon)
      and (ParticleTexture==other.ParticleTexture) and (CastingVisual==other.CastingVisual)
      and (BoltVisual==other.BoltVisual) and (HitVisual==other.HitVisual)
      and (AreaVisual==other.AreaVisual) and (Description==other.Description)
      and (CastSound==other.CastSound) and (BoltSound==other.BoltSound)
      and (HitSound==other.HitSound) and (AreaSound==other.AreaSound));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool MGEF_Data::saveToStream(std::ofstream& output) const
{
  //write MGEF
  output.write((const char*) &cMGEF, 4);
  uint32_t Size;
  Size = 4 /* INDX */ +4 /* four bytes for length */ +4 /* length of index */
        +4 /* MEDT */ +4 /* four bytes for length */ +36 /* length of effect data */;

  if (!EffectIcon.empty())
  {
    Size += 4 /* ITEX */ +4 /* four bytes for length */
           +EffectIcon.length()+1 /* length of effect icon +1 byte for NUL-termination */;
  }
  if (!ParticleTexture.empty())
  {
    Size += 4 /* PTEX */ +4 /* four bytes for length */
           +ParticleTexture.length()+1 /* length of particle texture +1 byte for NUL-termination */;
  }
  if (!CastingVisual.empty())
  {
    Size += 4 /* CVFX */ +4 /* four bytes for length */
           +CastingVisual.length()+1 /* length of casting visual +1 byte for NUL-termination */;
  }
  if (!BoltVisual.empty())
  {
    Size += 4 /* BVFX */ +4 /* four bytes for length */
           +BoltVisual.length()+1 /* length of bolt visual +1 byte for NUL-termination */;
  }
  if (!HitVisual.empty())
  {
    Size += 4 /* HVFX */ +4 /* four bytes for length */
           +HitVisual.length()+1 /* length of hit visual +1 byte for NUL-termination */;
  }
  if (!AreaVisual.empty())
  {
    Size += 4 /* AVFX */ +4 /* four bytes for length */
           +AreaVisual.length()+1 /* length of area visual +1 byte for NUL-termination */;
  }
  if (!Description.empty())
  {
    Size += 4 /* DESC */ +4 /* four bytes for length */
           +Description.length() /* length of description (no NUL-termination) */;
  }
  if (!CastSound.empty())
  {
    Size += 4 /* CSND */ +4 /* four bytes for length */
           +CastSound.length()+1 /* length of casting sound +1 byte for NUL-termination */;
  }
  if (!BoltSound.empty())
  {
    Size += 4 /* BSND */ +4 /* four bytes for length */
           +BoltSound.length()+1 /* length of bolt sound +1 byte for NUL-termination */;
  }
  if (!HitSound.empty())
  {
    Size += 4 /* HSND */ +4 /* four bytes for length */
           +HitSound.length()+1 /* length of hit sound +1 byte for NUL-termination */;
  }
  if (!AreaSound.empty())
  {
    Size += 4 /* ASND */ +4 /* four bytes for length */
           +AreaSound.length()+1 /* length of area sound +1 byte for NUL-termination */;
  }

  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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
    ITEX = Effect Icon string (optional)
    PTEX = Particle texture string (optional)
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
  output.write((const char*) &cINDX, 4);
  //INDX's length
  uint32_t SubLength;
  SubLength = 4; //fixed length of four bytes
  output.write((const char*) &SubLength, 4);
  //write INDX
  output.write((const char*) &Index, 4);

  //write MEDT
  output.write((const char*) &cMEDT, 4);
  //MEDT's length
  SubLength = 36; //fixed length of 36 bytes
  output.write((const char*) &SubLength, 4);
  //write MEDT
  // ---- SpellSchool
  output.write((const char*) &SpellSchool, 4);
  // ---- Cost
  output.write((const char*) &BaseCost, 4);
  // ---- Flags
  output.write((const char*) &Flags, 4);
  // ---- Colour
  output.write((const char*) &Red, 4);
  output.write((const char*) &Blue, 4);
  output.write((const char*) &Green, 4);
  // ---- speed
  output.write((const char*) &SpeedX, 4);
  // ---- size
  output.write((const char*) &SizeX, 4);
  // ---- size cap
  output.write((const char*) &SizeCap, 4);

  if (!EffectIcon.empty())
  {
    //write ITEX
    output.write((const char*) &cITEX, 4);
    //ITEX's length
    SubLength = EffectIcon.length()+1; /* length of effect icon +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write ITEX
    output.write(EffectIcon.c_str(), SubLength);
  }

  if (!ParticleTexture.empty())
  {
    //write PTEX
    output.write((const char*) &cPTEX, 4);
    //PTEX's length
    SubLength = ParticleTexture.length()+1; /* length of particle +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write PTEX
    output.write(ParticleTexture.c_str(), SubLength);
  }

  if (!CastingVisual.empty())
  {
    //write CVFX
    output.write((const char*) &cCVFX, 4);
    //CVFX's length
    SubLength = CastingVisual.length()+1; /* length of cast visual +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write CVFX
    output.write(CastingVisual.c_str(), SubLength);
  }

  if (!BoltVisual.empty())
  {
    //write BVFX
    output.write((const char*) &cBVFX, 4);
    //BVFX's length
    SubLength = BoltVisual.length()+1; /* length of bolt visual +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write BVFX
    output.write(BoltVisual.c_str(), SubLength);
  }

  if (!HitVisual.empty())
  {
    //write HVFX
    output.write((const char*) &cHVFX, 4);
    //HVFX's length
    SubLength = HitVisual.length()+1; /* length of hit visual +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write HVFX
    output.write(HitVisual.c_str(), SubLength);
  }

  if (!AreaVisual.empty())
  {
    //write AVFX
    output.write((const char*) &cAVFX, 4);
    //AVFX's length
    SubLength = AreaVisual.length()+1; /* length of area visual +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write AVFX
    output.write(AreaVisual.c_str(), SubLength);
  }

  if (!Description.empty())
  {
    //write DESC
    output.write((const char*) &cDESC, 4);
    //DESC's length
    SubLength = Description.length(); /* length of description (no NUL-termination) */
    output.write((const char*) &SubLength, 4);
    //write DESC
    output.write(Description.c_str(), SubLength);
  }

  if (!CastSound.empty())
  {
    //write CSND
    output.write((const char*) &cCSND, 4);
    //CSND's length
    SubLength = CastSound.length()+1; /* length of cast sound +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write CSND
    output.write(CastSound.c_str(), SubLength);
  }

  if (!BoltSound.empty())
  {
    //write BSND
    output.write((const char*) &cBSND, 4);
    //BSND's length
    SubLength = BoltSound.length()+1; /* length of bolt sound +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write BSND
    output.write(BoltSound.c_str(), SubLength);
  }

  if (!HitSound.empty())
  {
    //write HSND
    output.write((const char*) &cHSND, 4);
    //HSND's length
    SubLength = HitSound.length()+1; /* length of hit sound +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write HSND
    output.write(HitSound.c_str(), SubLength);
  }

  if (!AreaSound.empty())
  {
    //write ASND
    output.write((const char*) &cASND, 4);
    //ASND's length
    SubLength = AreaSound.length()+1; /* length of area sound +1 byte for NUL-termination */
    output.write((const char*) &SubLength, 4);
    //write ASND
    output.write(AreaSound.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool MGEF_Data::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
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
    ITEX = Effect Icon string (optional)
    PTEX = Particle texture string (optional)
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

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return false;
  }

  Index = -1;
  in_File.read((char*) &Index, 4); //read the index
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord INDX of MGEF.\n";
    return false;
  }

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
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
  BytesRead += 36;

  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MEDT of MGEF.\n";
    return false;
  }

  const size_t BufferSize = 1024;
  char Buffer [BufferSize];
  memset(Buffer, '\0', BufferSize);

  //effect icon tex and particle texture, both rarely optional
  EffectIcon.clear();
  ParticleTexture.clear();
  bool hasITEX = false;
  bool hasPTEX = false;
  //visual strings, partially optional
  CastingVisual.clear();
  BoltVisual.clear();
  HitVisual.clear();
  AreaVisual.clear();
  bool hasCVFX = false;
  bool hasBVFX = false;
  bool hasHVFX = false;
  bool hasAVFX = false;
  //description, optional
  Description.clear();
  bool hasDESC = false;
  //effect sounds, all optional
  CastSound.clear();
  BoltSound.clear();
  HitSound.clear();
  AreaSound.clear();
  bool hasCSND = false;
  bool hasBSND = false;
  bool hasHSND = false;
  bool hasASND = false;
  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cITEX: //rarely optional, might not be present in some plugins
           if (hasITEX)
           {
             std::cout << "Error: record MGEF seems to have two ITEX subrecords.\n";
             return false;
           }
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ITEX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read tex name
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of MGEF.\n";
             return false;
           }
           EffectIcon = std::string(Buffer);
           hasITEX = true;
           break;
      case cPTEX: //rarely optional, might not be present in some plugins
           if (hasPTEX)
           {
             std::cout << "Error: record MGEF seems to have two PTEX subrecords.\n";
             return false;
           }
           //PTEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord PTEX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read particle tex name
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PTEX of MGEF.\n";
             return false;
           }
           ParticleTexture = std::string(Buffer);
           hasPTEX = true;
           break;
      case cCVFX: //read optional CVFX
           if (hasCVFX)
           {
             std::cout << "Error: record MGEF seems to have two CVFX subrecords.\n";
             return false;
           }
           //CVFX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CVFX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read effect string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CVFX of MGEF.\n";
             return false;
           }
           CastingVisual = std::string(Buffer);
           hasCVFX = true;
           break;
      case cBVFX: //read optional BVFX
           if (hasBVFX)
           {
             std::cout << "Error: record MGEF seems to have two BVFX subrecords.\n";
             return false;
           }
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord BVFX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read effect string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BVFX of MGEF.\n";
             return false;
           }
           BoltVisual = std::string(Buffer);
           hasBVFX = true;
           break;
      case cHVFX: //read optional HVFX
           if (hasHVFX)
           {
             std::cout << "Error: record MGEF seems to have two HVFX subrecords.\n";
             return false;
           }
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord HVFX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read effect string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord HVFX of MGEF.\n";
             return false;
           }
           HitVisual = std::string(Buffer);
           hasHVFX = true;
           break;
      case cAVFX: //read optional AVFX
           if (hasAVFX)
           {
             std::cout << "Error: record MGEF seems to have two AVFX subrecords.\n";
             return false;
           }
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord AVFX of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read effect string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AVFX of MGEF.\n";
             return false;
           }
           AreaVisual = std::string(Buffer);
           hasAVFX = true;
           break;
      case cDESC: //read optional DESC
           if (hasDESC)
           {
             std::cout << "Error: record MGEF seems to have two DESC subrecords.\n";
             return false;
           }
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>=BufferSize)
           {
             std::cout << "Error: subrecord DESC of MGEF is longer than "
                       << BufferSize-1 <<" characters.\n";
             return false;
           }
           //read effect description
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DESC of MGEF.\n";
             return false;
           }
           Description = std::string(Buffer);
           hasDESC = true;
           break;
      case cCSND:
           if (hasCSND)
           {
             std::cout << "Error: record MGEF seems to have two CSND subrecords.\n";
             return false;
           }
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CSND of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read sound string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CSND of MGEF.\n";
             return false;
           }
           CastSound = std::string(Buffer);
           hasCSND = true;
           break;
      case cBSND:
           if (hasBSND)
           {
             std::cout << "Error: record MGEF seems to have two BSND subrecords.\n";
             return false;
           }
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord BSND of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read sound string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BSND of MGEF.\n";
             return false;
           }
           BoltSound = std::string(Buffer);
           hasBSND = true;
           break;
      case cHSND:
           if (hasHSND)
           {
             std::cout << "Error: record MGEF seems to have two HSND subrecords.\n";
             return false;
           }
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord HSND of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read sound string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord HSND of MGEF.\n";
             return false;
           }
           HitSound = std::string(Buffer);
           hasHSND = true;
           break;
      case cASND:
           if (hasASND)
           {
             std::cout << "Error: record MGEF seems to have two ASND subrecords.\n";
             return false;
           }
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ASND of MGEF is longer than 255 characters.\n";
             return false;
           }
           //read sound string
           memset(Buffer, '\0', BufferSize);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ASND of MGEF.\n";
             return false;
           }
           AreaSound = std::string(Buffer);
           hasASND = true;
           break;
      default:
           //unknown sub record, thus stop here, some error occured
           std::cout << "Error in MGEF_Data::loadFromStream: unexpected header "
                     << "found: \""<<IntTo4Char(SubRecName)<<"\".\n";
           return false;
    }//switch
  }// while

  return in_File.good();
}

} //namespace
