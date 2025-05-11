/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "MagicEffectRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

MagicEffectRecord::MagicEffectRecord()
: Index(EffectIndex::WaterBreathing),
  SpellSchool(MagicSchool::Alteration),
  BaseCost(0.0f),
  Flags(0),
  Red(0), Green(0), Blue(0),
  SpeedX(0.0f), SizeX(0.0f), SizeCap(0.0f),
  EffectIcon(std::string()),
  ParticleTexture(std::string()),
  BoltSound(std::string()),
  CastSound(std::string()),
  HitSound(std::string()),
  AreaSound(std::string()),
  CastingVisual(std::string()),
  BoltVisual(std::string()),
  HitVisual(std::string()),
  AreaVisual(std::string()),
  Description(std::string())
{
}

bool MagicEffectRecord::equals(const MagicEffectRecord& other) const
{
  return (Index == other.Index)
      && (SpellSchool == other.SpellSchool) && (BaseCost == other.BaseCost)
      && (Flags == other.Flags) && (Red == other.Red) && (Green == other.Green)
      && (Blue == other.Blue) && (SpeedX == other.SpeedX)
      && (SizeX == other.SizeX)
      && (SizeCap == other.SizeCap) && (EffectIcon == other.EffectIcon)
      && (ParticleTexture == other.ParticleTexture)
      && (CastingVisual == other.CastingVisual)
      && (BoltVisual == other.BoltVisual) && (HitVisual == other.HitVisual)
      && (AreaVisual == other.AreaVisual) && (Description == other.Description)
      && (CastSound == other.CastSound) && (BoltSound == other.BoltSound)
      && (HitSound == other.HitSound) && (AreaSound == other.AreaSound);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool MagicEffectRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cMGEF), 4);
  uint32_t Size = 4 /* INDX */ + 4 /* four bytes for length */ + 4 /* length of index */
        + 4 /* MEDT */ + 4 /* four bytes for length */ + 36 /* length of effect data */;
  if (!EffectIcon.empty())
  {
    Size += 4 /* ITEX */ + 4 /* four bytes for length */
          + EffectIcon.length() + 1 /* length of path +1 byte for NUL */;
  }
  if (!ParticleTexture.empty())
  {
    Size += 4 /* PTEX */ + 4 /* four bytes for length */
          + ParticleTexture.length() + 1 /* length of path +1 byte for NUL */;
  }
  if (!BoltSound.empty())
  {
    Size += 4 /* BSND */ + 4 /* length */
          + BoltSound.length() + 1 /* length of bolt sound +1 byte for NUL */;
  }
  if (!CastSound.empty())
  {
    Size += 4 /* CSND */ + 4 /* four bytes for length */
         + CastSound.length() + 1 /* length of casting sound +1 byte for NUL */;
  }
  if (!HitSound.empty())
  {
    Size += 4 /* HSND */ + 4 /* four bytes for length */
          + HitSound.length() + 1 /* length of hit sound +1 byte for NUL */;
  }
  if (!AreaSound.empty())
  {
    Size += 4 /* ASND */ + 4 /* four bytes for length */
          + AreaSound.length() + 1 /* length of area sound +1 byte for NUL */;
  }
  if (!CastingVisual.empty())
  {
    Size += 4 /* CVFX */ + 4 /* four bytes for length */
          + CastingVisual.length() + 1 /* length of visual +1 byte for NUL */;
  }
  if (!BoltVisual.empty())
  {
    Size += 4 /* BVFX */ + 4 /* four bytes for length */
          + BoltVisual.length() + 1 /* length of bolt visual +1 byte for NUL */;
  }
  if (!HitVisual.empty())
  {
    Size += 4 /* HVFX */ + 4 /* four bytes for length */
          + HitVisual.length() + 1 /* length of hit visual +1 byte for NUL */;
  }
  if (!AreaVisual.empty())
  {
    Size += 4 /* AVFX */ + 4 /* four bytes for length */
          + AreaVisual.length() + 1 /* length of area visual +1 byte for NUL */;
  }
  if (!Description.empty())
  {
    Size += 4 /* DESC */ + 4 /* four bytes for length */
          + Description.length() /* length of description (no NUL-termination) */;
  }

  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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
    BSND = Bolt sound (optional)
    CSND = Cast sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?)
  */

  // write effect index (INDX)
  output.write(reinterpret_cast<const char*>(&cINDX), 4);
  uint32_t SubLength;
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Index), 4);

  // write magic effect data (MEDT)
  output.write(reinterpret_cast<const char*>(&cMEDT), 4);
  SubLength = 36;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write data
  output.write(reinterpret_cast<const char*>(&SpellSchool), 4);
  output.write(reinterpret_cast<const char*>(&BaseCost), 4);
  output.write(reinterpret_cast<const char*>(&Flags), 4);
  // ---- Colour
  output.write(reinterpret_cast<const char*>(&Red), 4);
  output.write(reinterpret_cast<const char*>(&Green), 4);
  output.write(reinterpret_cast<const char*>(&Blue), 4);
  output.write(reinterpret_cast<const char*>(&SpeedX), 4);
  output.write(reinterpret_cast<const char*>(&SizeX), 4);
  output.write(reinterpret_cast<const char*>(&SizeCap), 4);

  if (!EffectIcon.empty())
  {
    // write icon texture path (ITEX)
    output.write(reinterpret_cast<const char*>(&cITEX), 4);
    SubLength = EffectIcon.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(EffectIcon.c_str(), SubLength);
  }

  if (!ParticleTexture.empty())
  {
    // write particle texture path (PTEX)
    output.write(reinterpret_cast<const char*>(&cPTEX), 4);
    SubLength = ParticleTexture.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ParticleTexture.c_str(), SubLength);
  }

  if (!BoltSound.empty())
  {
    // write bolt sound (BSND)
    output.write(reinterpret_cast<const char*>(&cBSND), 4);
    SubLength = BoltSound.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(BoltSound.c_str(), SubLength);
  }

  if (!CastSound.empty())
  {
    // write casting sound (CSND)
    output.write(reinterpret_cast<const char*>(&cCSND), 4);
    SubLength = CastSound.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(CastSound.c_str(), SubLength);
  }

  if (!HitSound.empty())
  {
    // write hit sound (HSND)
    output.write(reinterpret_cast<const char*>(&cHSND), 4);
    SubLength = HitSound.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(HitSound.c_str(), SubLength);
  }

  if (!AreaSound.empty())
  {
    // write area sound (ASND)
    output.write(reinterpret_cast<const char*>(&cASND), 4);
    SubLength = AreaSound.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(AreaSound.c_str(), SubLength);
  }

  if (!CastingVisual.empty())
  {
    // write casting visual (CVFX)
    output.write(reinterpret_cast<const char*>(&cCVFX), 4);
    SubLength = CastingVisual.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(CastingVisual.c_str(), SubLength);
  }

  if (!BoltVisual.empty())
  {
    // write bolt visual (BVFX)
    output.write(reinterpret_cast<const char*>(&cBVFX), 4);
    SubLength = BoltVisual.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(BoltVisual.c_str(), SubLength);
  }

  if (!HitVisual.empty())
  {
    // write hit visual (HVFX)
    output.write(reinterpret_cast<const char*>(&cHVFX), 4);
    SubLength = HitVisual.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(HitVisual.c_str(), SubLength);
  }

  if (!AreaVisual.empty())
  {
    // write area visual (AVFX)
    output.write(reinterpret_cast<const char*>(&cAVFX), 4);
    SubLength = AreaVisual.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(AreaVisual.c_str(), SubLength);
  }

  if (!Description.empty())
  {
    // write description (DESC)
    output.write(reinterpret_cast<const char*>(&cDESC), 4);
    // length of description (no NUL-termination)
    SubLength = Description.length();
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Description.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool MagicEffectRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Green
        long  Blue
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string (optional)
    PTEX = Particle texture string (optional)
    BSND = Bolt sound (optional)
    CSND = Cast sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?)
  */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;

  // read INDX
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t BytesRead = 4;
  if (SubRecName != cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  // INDX's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record INDX of MGEF has invalid length ("
              << SubLength << " bytes). Should be 4 bytes.\n";
    return false;
  }

  Index = EffectIndex::WaterBreathing; // index zero
  input.read(reinterpret_cast<char*>(&Index), 4);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord INDX of MGEF.\n";
    return false;
  }

  // read MEDT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  // MEDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 36)
  {
    std::cerr << "Error: Sub record MEDT of MGEF has invalid length ("
              << SubLength << " bytes). Should be 36 bytes.\n";
    return false;
  }

  // read the data
  input.read(reinterpret_cast<char*>(&SpellSchool), 4);
  input.read(reinterpret_cast<char*>(&BaseCost), 4);
  input.read(reinterpret_cast<char*>(&Flags), 4);
  input.read(reinterpret_cast<char*>(&Red), 4);
  input.read(reinterpret_cast<char*>(&Green), 4);
  input.read(reinterpret_cast<char*>(&Blue), 4);
  input.read(reinterpret_cast<char*>(&SpeedX), 4);
  input.read(reinterpret_cast<char*>(&SizeX), 4);
  input.read(reinterpret_cast<char*>(&SizeCap), 4);
  BytesRead += 36;

  if (!input.good())
  {
    std::cerr << "Error while reading subrecord MEDT of MGEF.\n";
    return false;
  }

  const size_t BufferSize = 1024;
  char Buffer [BufferSize];

  EffectIcon.clear();
  ParticleTexture.clear();
  bool hasITEX = false;
  bool hasPTEX = false;
  // effect sounds, all optional
  BoltSound.clear();
  CastSound.clear();
  HitSound.clear();
  AreaSound.clear();
  bool hasCSND = false;
  bool hasBSND = false;
  bool hasHSND = false;
  bool hasASND = false;
  // visual strings, partially optional
  CastingVisual.clear();
  BoltVisual.clear();
  HitVisual.clear();
  AreaVisual.clear();
  bool hasCVFX = false;
  bool hasBVFX = false;
  bool hasHVFX = false;
  bool hasAVFX = false;
  // description, optional
  Description.clear();
  bool hasDESC = false;

  while (BytesRead < Size)
  {
    // read next subrecord
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cITEX: // rarely optional, might not be present in some plugins
           if (hasITEX)
           {
             std::cerr << "Error: Record MGEF seems to have two ITEX subrecords.\n";
             return false;
           }
           // read texture path
           if (!loadString256(input, EffectIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of MGEF.\n";
             return false;
           }
           hasITEX = true;
           break;
      case cPTEX: // rarely optional, might not be present in some plugins
           if (hasPTEX)
           {
             std::cerr << "Error: Record MGEF seems to have two PTEX subrecords.\n";
             return false;
           }
           // read particle texture name
           if (!loadString256(input, ParticleTexture, Buffer, cPTEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord PTEX of MGEF.\n";
             return false;
           }
           hasPTEX = true;
           break;
      case cCVFX: // read optional CVFX
           if (hasCVFX)
           {
             std::cerr << "Error: Record MGEF seems to have two CVFX subrecords.\n";
             return false;
           }
           if (!loadString256(input, CastingVisual, Buffer, cCVFX, BytesRead))
           {
             std::cerr << "Error while reading subrecord CVFX of MGEF.\n";
             return false;
           }
           hasCVFX = true;
           break;
      case cBVFX: // read optional BVFX
           if (hasBVFX)
           {
             std::cerr << "Error: Record MGEF seems to have two BVFX subrecords.\n";
             return false;
           }
           if (!loadString256(input, BoltVisual, Buffer, cBVFX, BytesRead))
           {
             std::cerr << "Error while reading subrecord BVFX of MGEF.\n";
             return false;
           }
           hasBVFX = true;
           break;
      case cHVFX:
           if (hasHVFX)
           {
             std::cerr << "Error: Record MGEF seems to have two HVFX subrecords.\n";
             return false;
           }
           if (!loadString256(input, HitVisual, Buffer, cHVFX, BytesRead))
           {
             std::cerr << "Error while reading subrecord HVFX of MGEF.\n";
             return false;
           }
           hasHVFX = true;
           break;
      case cAVFX:
           if (hasAVFX)
           {
             std::cerr << "Error: Record MGEF seems to have two AVFX subrecords.\n";
             return false;
           }
           if (!loadString256(input, AreaVisual, Buffer, cAVFX, BytesRead))
           {
             std::cerr << "Error while reading subrecord AVFX of MGEF.\n";
             return false;
           }
           hasAVFX = true;
           break;
      case cDESC:
           if (hasDESC)
           {
             std::cerr << "Error: Record MGEF seems to have two DESC subrecords.\n";
             return false;
           }
           // DESC's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength >= BufferSize)
           {
             std::cerr << "Error: Subrecord DESC of MGEF is longer than "
                       << BufferSize - 1 << " characters.\n";
             return false;
           }
           // read effect description
           memset(Buffer, '\0', BufferSize);
           input.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord DESC of MGEF.\n";
             return false;
           }
           Description = std::string(Buffer);
           hasDESC = true;
           break;
      case cBSND:
           if (hasBSND)
           {
             std::cerr << "Error: Record MGEF seems to have two BSND subrecords.\n";
             return false;
           }
           if (!loadString256(input, BoltSound, Buffer, cBSND, BytesRead))
           {
             std::cerr << "Error while reading subrecord BSND of MGEF.\n";
             return false;
           }
           hasBSND = true;
           break;
      case cCSND:
           if (hasCSND)
           {
             std::cerr << "Error: Record MGEF seems to have two CSND subrecords.\n";
             return false;
           }
           if (!loadString256(input, CastSound, Buffer, cCSND, BytesRead))
           {
             std::cerr << "Error while reading subrecord CSND of MGEF.\n";
             return false;
           }
           hasCSND = true;
           break;
      case cHSND:
           if (hasHSND)
           {
             std::cerr << "Error: Record MGEF seems to have two HSND subrecords.\n";
             return false;
           }
           if (!loadString256(input, HitSound, Buffer, cHSND, BytesRead))
           {
             std::cerr << "Error while reading subrecord HSND of MGEF.\n";
             return false;
           }
           hasHSND = true;
           break;
      case cASND:
           if (hasASND)
           {
             std::cerr << "Error: Record MGEF seems to have two ASND subrecords.\n";
             return false;
           }
           if (!loadString256(input, AreaSound, Buffer, cASND, BytesRead))
           {
             std::cerr << "Error while reading subrecord ASND of MGEF.\n";
             return false;
           }
           hasASND = true;
           break;
      default:
           // unknown sub record, thus stop here, some error occurred
           std::cerr << "Error in MagicEffectRecord::loadFromStream: Unexpected"
                     << " header found: \"" << IntTo4Char(SubRecName) << "\".\n";
           return false;
    }
  }

  return true;
}

} // namespace
