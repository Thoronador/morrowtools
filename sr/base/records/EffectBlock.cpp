/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#include "EffectBlock.hpp"
#include "../SR_Constants.hpp"

namespace SRTP
{

EffectBlock::EffectBlock()
: effectFormID(0),
  magnitude(0.0f),
  areaOfEffect(0),
  duration(0),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool EffectBlock::operator==(const EffectBlock& other) const
{
  return (effectFormID == other.effectFormID) && (magnitude == other.magnitude)
      && (areaOfEffect == other.areaOfEffect) && (duration == other.duration)
      && (unknownCTDA_CIS2s == other.unknownCTDA_CIS2s);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
bool EffectBlock::saveToStream(std::ostream& output) const
{
  // write EFID
  output.write(reinterpret_cast<const char*>(&cEFID), 4);
  uint16_t subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&effectFormID), 4);

  // write EFIT
  output.write(reinterpret_cast<const char*>(&cEFIT), 4);
  subLength = 12; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write EFIT's stuff
  output.write(reinterpret_cast<const char*>(&magnitude), 4);
  output.write(reinterpret_cast<const char*>(&areaOfEffect), 4);
  output.write(reinterpret_cast<const char*>(&duration), 4);

  // write CTDA-CIS2 compounds
  for (const auto& elem: unknownCTDA_CIS2s)
  {
    if (!elem.saveToStream(output))
      return false;
  }

  return output.good();
}

uint32_t EffectBlock::getWriteSize() const
{
  uint32_t writeSize = 4 /* EFID */ + 2 /* 2 bytes for length */ + 4 /* fixed length */
                     + 4 /* EFIT */ + 2 /* 2 bytes for length */ + 12 /* fixed length */;
  for (const auto& compound: unknownCTDA_CIS2s)
  {
    writeSize += compound.getWriteSize();
  }
  return writeSize;
}
#endif

} // namespace
