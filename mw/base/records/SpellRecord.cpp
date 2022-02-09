/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "SpellRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

const size_t SpellRecord::cMaximumSpellNameLength = 31;

SpellRecord::SpellRecord()
: BasicRecord(),
  recordID(std::string()),
  Name(std::string()),
  Type(SpellType::Spell),
  Cost(0),
  Flags(0),
  Enchs(std::vector<EnchantmentData>())
{
}

#ifndef MW_UNSAVEABLE_RECORDS
bool SpellRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSPEL), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* SPDT */ + 4 /* SPDT's length */ + 12 /*size of spell data */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for FNAM's length */
         + Name.length() + 1 /*length of name plus one for NUL-termination */;
  }
  // now calculate additional size of enchantment data
  Size = Size + Enchs.size()
       * (4 /*ENAM*/ + 4 /*ENAM's length*/ + 24 /*size of enchantment data*/);
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name (optional (yes, hard to believe))
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */


  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  if (!Name.empty())
  {
    // write spell name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  // write spell data (SPDT)
  output.write(reinterpret_cast<const char*>(&cSPDT), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Type), 4);
  output.write(reinterpret_cast<const char*>(&Cost), 4);
  output.write(reinterpret_cast<const char*>(&Flags), 4);
  if (!output.good())
  {
    std::cerr << "Error while writing sub record SPDT of SPEL.\n";
    return false;
  }

  // write multiple ENAM records
  for (const auto& enchantment: Enchs)
  {
    if (!enchantment.saveToStream(output))
    {
      std::cerr << "Error while writing sub record ENAM of SPEL.\n";
      return false;
    }
  }

  return true;
}
#endif

bool SpellRecord::equals(const SpellRecord& other) const
{
  return (recordID == other.recordID) && (Name == other.Name)
      && (Type == other.Type) && (Cost == other.Cost) && (Flags == other.Flags)
      && (Enchs == other.Enchs);
}

bool SpellRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name (optional (yes, hard to believe))
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read record ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of SPEL from stream.\n";
    return false;
  }

  //read FNAM (or SPDT in some rare cases)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cFNAM)
  {
    // read spell name (FNAM)
    if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
    {
      std::cout << "Error while reading sub record FNAM of SPEL.\n";
      return false;
    }

    // read SPDT
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    // no name is present here
    Name.clear();
  }
  // read SPDT
  //  -> was already read above
  if (SubRecName != cSPDT)
  {
    UnexpectedRecord(cSPDT, SubRecName);
    return false;
  }
  // SPDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record SPDT of SPEL has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  BytesRead += 4;
  // ---- read spell data
  input.read(reinterpret_cast<char*>(&Type), 4);
  input.read(reinterpret_cast<char*>(&Cost), 4);
  input.read(reinterpret_cast<char*>(&Flags), 4);
  if (!input.good())
  {
    std::cerr << "Error while reading sub record SPDT of SPEL.\n";
    return false;
  }
  BytesRead += 12;

  Enchs.clear();
  // read multiple ENAM records
  while (BytesRead < Size)
  {
    // read ENAM
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    if (SubRecName != cENAM)
    {
      UnexpectedRecord(cENAM, SubRecName);
      return false;
    }
    // read enchantment data
    EnchantmentData ench;
    if (!ench.loadFromStream(input))
    {
      std::cerr << "Error while reading sub record ENAM of SPEL.\n";
      return false;
    }
    BytesRead += 28;
    Enchs.push_back(ench);

    if (Enchs.size() > 8)
    {
      /* Although the current record structure allows an arbitrary number of
         enchantments, Morrowind only allows up to eight enchantments for one
         single spell. That's why we issue an error here. */
      std::cerr << "Error: Record SPEL has more than eight ENAM sub records.\n";
      return false;
    }
  }

  return true;
}

bool SpellRecord::autoCalculateCosts() const
{
  return (Flags & sfAutoCalcCosts) != 0;
}

bool SpellRecord::isPCStartSpell() const
{
  return (Flags & sfPCStartSpell) != 0;
}

bool SpellRecord::alwaysSucceeds() const
{
  return (Flags & sfAlwaysSucceeds) != 0;
}

} // namespace
