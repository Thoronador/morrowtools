/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

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

#include "SpellItem.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

// flag constants
const uint32_t SpellItem::cFlagNoAutoCalc              = 0x00000001;
const uint32_t SpellItem::cFlagPCStartSpell            = 0x00020000;
const uint32_t SpellItem::cFlagAreaEffectIgnoresLOS    = 0x00080000;
const uint32_t SpellItem::cFlagIgnoreResistance        = 0x00100000;
const uint32_t SpellItem::cFlagDisallowAbsorbReflect   = 0x00200000;
const uint32_t SpellItem::cFlagNoDualCastModifications = 0x00800000;

// type constants
const uint32_t SpellItem::cSpell       = 0x00000000;
const uint32_t SpellItem::cDisease     = 0x00000001;
const uint32_t SpellItem::cPower       = 0x00000002;
const uint32_t SpellItem::cLesserPower = 0x00000003;
const uint32_t SpellItem::cAbility     = 0x00000004;
const uint32_t SpellItem::cPoison      = 0x00000005;
const uint32_t SpellItem::cAddiction   = 0x0000000A;
const uint32_t SpellItem::cVoicePower  = 0x0000000B;

// casting type constants
const uint32_t SpellItem::cConstantEffect = 0x00000000;
const uint32_t SpellItem::cFireAndForget  = 0x00000001;
const uint32_t SpellItem::cConcentration  = 0x00000002;

// delivery constants
const uint32_t SpellItem::cSelf           = 0x00000000;
const uint32_t SpellItem::cContact        = 0x00000001;
const uint32_t SpellItem::cAimed          = 0x00000002;
const uint32_t SpellItem::cTargetActor    = 0x00000003;
const uint32_t SpellItem::cTargetLocation = 0x00000004;


SpellItem::SpellItem()
: castingCost(0),
  flags(0),
  type(0),
  chargeTime(0.0f),
  castingType(0),
  delivery(0),
  castDuration(0.0f),
  range(0.0f),
  castingPerkFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool SpellItem::operator==(const SpellItem& other) const
{
  return (castingCost == other.castingCost) && (flags == other.flags)
      && (type == other.type) && (chargeTime == other.chargeTime)
      && (castingType == other.castingType) && (delivery == other.delivery)
      && (castDuration == other.castDuration) && (range == other.range)
      && (castingPerkFormID == other.castingPerkFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
bool SpellItem::saveToStream(std::ostream& output) const
{
  // write spell item (SPIT)
  output.write(reinterpret_cast<const char*>(&cSPIT), 4);
  constexpr uint16_t subLength = 36; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write SPIT's content
  output.write(reinterpret_cast<const char*>(&castingCost), 4);
  output.write(reinterpret_cast<const char*>(&flags), 4);
  output.write(reinterpret_cast<const char*>(&type), 4);
  output.write(reinterpret_cast<const char*>(&chargeTime), 4);
  output.write(reinterpret_cast<const char*>(&castingType), 4);
  output.write(reinterpret_cast<const char*>(&delivery), 4);
  output.write(reinterpret_cast<const char*>(&castDuration), 4);
  output.write(reinterpret_cast<const char*>(&range), 4);
  output.write(reinterpret_cast<const char*>(&castingPerkFormID), 4);

  return output.good();
}
#endif

bool SpellItem::loadFromStream(std::istream& input, const uint32_t recordName, uint32_t& bytesRead)
{
  // SPIT's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 36)
  {
    std::cerr << "Error: Subrecord SPIT of " << IntTo4Char(recordName)
              << " has invalid length (" << subLength
              << " bytes). Should be 36 bytes!\n";
    return false;
  }
  // read SPIT's stuff
  input.read(reinterpret_cast<char*>(&castingCost), 4);
  input.read(reinterpret_cast<char*>(&flags), 4);
  input.read(reinterpret_cast<char*>(&type), 4);
  input.read(reinterpret_cast<char*>(&chargeTime), 4);
  input.read(reinterpret_cast<char*>(&castingType), 4);
  input.read(reinterpret_cast<char*>(&delivery), 4);
  input.read(reinterpret_cast<char*>(&castDuration), 4);
  input.read(reinterpret_cast<char*>(&range), 4);
  input.read(reinterpret_cast<char*>(&castingPerkFormID), 4);
  bytesRead += 36;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord SPIT of "
              << IntTo4Char(recordName) << "!\n";
    return false;
  }
  return true;
}

bool SpellItem::doesAutoCalc() const
{
  return (flags & cFlagNoAutoCalc) == 0;
}

bool SpellItem::isPCStartSpell() const
{
  return (flags & cFlagPCStartSpell) != 0;
}

bool SpellItem::areaEffectIgnoresLOS() const
{
  return (flags & cFlagAreaEffectIgnoresLOS) != 0;
}

bool SpellItem::ignoresResistance() const
{
  return (flags & cFlagIgnoreResistance) != 0;
}

bool SpellItem::disallowsAbsorbAndReflect() const
{
  return (flags & cFlagDisallowAbsorbReflect) != 0;
}

bool SpellItem::noDualCastModifications() const
{
  return (flags & cFlagNoDualCastModifications) != 0;
}

} // namespace
