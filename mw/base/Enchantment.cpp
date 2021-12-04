/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#include "Enchantment.hpp"
#include <iostream>
#include "MW_Constants.hpp"

namespace MWTP
{

EnchantmentData::EnchantmentData()
: EffectID(0),
  SkillID(0),
  AttributeID(0),
  Range(RangeType::Self),
  Area(0),
  Duration(0),
  MagnitudeMin(0),
  MagnitudeMax(0)
{
}

bool EnchantmentData::operator==(const EnchantmentData& other) const
{
  return (EffectID == other.EffectID) && (SkillID == other.SkillID)
      && (AttributeID == other.AttributeID) && (Range == other.Range)
      && (Area == other.Area) && (Duration == other.Duration)
      && (MagnitudeMin == other.MagnitudeMin) && (MagnitudeMax == other.MagnitudeMax);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool EnchantmentData::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cENAM), 4);
  const uint32_t SubLength = 24;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write enchantment data
  output.write(reinterpret_cast<const char*>(&EffectID), 2);
  output.write(reinterpret_cast<const char*>(&SkillID), 1);
  output.write(reinterpret_cast<const char*>(&AttributeID), 1);
  output.write(reinterpret_cast<const char*>(&Range), 4);
  output.write(reinterpret_cast<const char*>(&Area), 4);
  output.write(reinterpret_cast<const char*>(&Duration), 4);
  output.write(reinterpret_cast<const char*>(&MagnitudeMin), 4);
  output.write(reinterpret_cast<const char*>(&MagnitudeMax), 4);
  return output.good();
}
#endif

bool EnchantmentData::loadFromStream(std::istream& input)
{
  // ENAM's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 24)
  {
    std::cerr << "Error: Enchanment data (ENAM) has invalid length ("
              << SubLength << " bytes). Should be 24 bytes.\n";
    return false;
  }
  // read enchantment data
  input.read(reinterpret_cast<char*>(&EffectID), 2);
  input.read(reinterpret_cast<char*>(&SkillID), 1);
  input.read(reinterpret_cast<char*>(&AttributeID), 1);
  input.read(reinterpret_cast<char*>(&Range), 4);
  input.read(reinterpret_cast<char*>(&Area), 4);
  input.read(reinterpret_cast<char*>(&Duration), 4);
  input.read(reinterpret_cast<char*>(&MagnitudeMin), 4);
  input.read(reinterpret_cast<char*>(&MagnitudeMax), 4);
  return input.good();
}

} // namespace
