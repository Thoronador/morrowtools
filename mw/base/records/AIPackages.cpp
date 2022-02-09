/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "AIPackages.hpp"
#include <cstring>
#include "../MW_Constants.hpp"

namespace MWTP
{

/* **** BasicAIPackage's functions ****/

NPC_BasicAIPackage::~NPC_BasicAIPackage()
{
}

/* **** AIActivate's functions ****/

NPC_AIActivate::NPC_AIActivate()
: NPC_BasicAIPackage(),
  TargetID(std::string()),
  Reset(0)
{
}

bool NPC_AIActivate::equals(const NPC_AIActivate& other) const
{
  return (TargetID == other.TargetID) && (Reset == other.Reset);
}

PackageType NPC_AIActivate::getPackageType() const
{
  return PackageType::ptActivate;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIActivate::saveToStream(std::ostream& output) const
{
  // write AI_A
  output.write(reinterpret_cast<const char*>(&cAI_A), 4);
  const uint32_t SubLength = 33;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write AI activate data
  // ---- write target ID
  auto len = TargetID.length();
  if (len > 31)
  {
    len = 31;
  }
  output.write(TargetID.c_str(), len);
  // ---- fill rest with NUL
  output.write(NULof32, 32 - len);
  // ---- reset flag
  output.write(reinterpret_cast<const char*>(&Reset), 1);

  return output.good();
}
#endif

/* **** AIEscortFollow's functions ****/

NPC_AIEscortFollow::NPC_AIEscortFollow()
: NPC_BasicAIPackage(),
  X(0.0f), Y(0.0f), Z(0.0f),
  Duration(0),
  TargetID(std::string()),
  Reset(0),
  CellName(std::string())
{
}

void NPC_AIEscortFollow::clear()
{
  X = Y = Z = 0.0f;
  Duration = 0;
  TargetID.clear();
  Reset = 0;
  CellName.clear();
}

bool NPC_AIEscortFollow::equals(const NPC_AIEscortFollow& other) const
{
  /* Note: Usually it should be enough to check X, Y and Z for equality with
           their counterparts in 'other', but if one of them is a NaN, we have
           a problem, because one NaN is (by definition of IEEE 754) never equal
           to another NaN, even if their internal bit representation is exactly
           the same. That's why we have expressions like (X!=X) down there, they
           catch the NaNs. */
  return ((X == other.X) || ((X != X) && (other.X != other.X)))
      && ((Y == other.Y) || ((Y != Y) && (other.Y != other.Y)))
      && ((Z == other.Z) || ((Z != Z) && (other.Z != other.Z)))
      && (Duration == other.Duration) && (TargetID == other.TargetID)
      && (Reset == other.Reset) && (CellName == other.CellName);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIEscortFollow::saveToStream(std::ostream& output) const
{
  const uint32_t typeHeader = (getPackageType() == PackageType::ptEscort) ? cAI_E : cAI_F;
  // write AI_E or AI_F
  output.write(reinterpret_cast<const char*>(&typeHeader), 4);
  uint32_t SubLength = 48;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write AI escort/follow data
  output.write(reinterpret_cast<const char*>(&X), 4);
  output.write(reinterpret_cast<const char*>(&Y), 4);
  output.write(reinterpret_cast<const char*>(&Z), 4);
  output.write(reinterpret_cast<const char*>(&Duration), 2);
  // ---- write target ID
  unsigned int len = TargetID.length();
  if (len > 31)
  {
    len = 31;
  }
  output.write(TargetID.c_str(), len);
  // ---- fill rest with NUL
  output.write(NULof32, 32 - len);
  // ---- reset flag
  output.write(reinterpret_cast<const char*>(&Reset), 2);

  if (!CellName.empty())
  {
    // write AI escort's/follow's cell name (CNDT)
    output.write(reinterpret_cast<const char*>(&cCNDT), 4);
    SubLength = CellName.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(CellName.c_str(), SubLength);
  }

  return output.good();
}
#endif

/* **** AIEscort functions ****/

PackageType NPC_AIEscort::getPackageType() const
{
  return PackageType::ptEscort;
}

/* **** AIFollow functions ****/

PackageType NPC_AIFollow::getPackageType() const
{
  return PackageType::ptFollow;
}

/* **** AITravel's functions ****/

NPC_AITravel::NPC_AITravel()
: NPC_BasicAIPackage(),
  X(0.0f), Y(0.0f), Z(0.0f),
  Reset(0)
{
}

void NPC_AITravel::clear()
{
  X = Y = Z = 0.0f;
  Reset = 0;
}

bool NPC_AITravel::equals(const NPC_AITravel& other) const
{
  /* Note: Usually it should be enough to check X, Y and Z for equality with
           their counterparts in 'other', but if one of them is a NaN, we have
           a problem, because one NaN is (by definition of IEEE 754) never equal
           to another NaN, even if their internal bit representation is exactly
           the same. That's why we have expressions like (X!=X) down there, they
           catch the NaNs. */
  return ((X == other.X) || ((X != X) && (other.X != other.X)))
      && ((Y==other.Y) || ((Y != Y) && (other.Y != other.Y)))
      && ((Z == other.Z) || ((Z != Z) && (other.Z != other.Z)))
      && (Reset == other.Reset);
}

PackageType NPC_AITravel::getPackageType() const
{
  return PackageType::ptTravel;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AITravel::saveToStream(std::ostream& output) const
{
  // write AI_T
  output.write(reinterpret_cast<const char*>(&cAI_T), 4);
  const uint32_t SubLength = 16;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write AI travel data
  output.write(reinterpret_cast<const char*>(&X), 4);
  output.write(reinterpret_cast<const char*>(&Y), 4);
  output.write(reinterpret_cast<const char*>(&Z), 4);
  output.write(reinterpret_cast<const char*>(&Reset), 4);

  return output.good();
}
#endif

/* **** AIWander's functions ****/

NPC_AIWander::NPC_AIWander()
: NPC_BasicAIPackage(),
  Distance(0),
  Duration(0),
  Time(0),
  Idle({ 0, 0, 0, 0, 0, 0, 0, 0 }),
  Reset(0)
{
}

void NPC_AIWander::clear()
{
  Distance = 0;
  Duration = 0;
  Time = 0;
  Idle.fill(0);
  Reset = 0;
}

bool NPC_AIWander::equals(const NPC_AIWander& other) const
{
  return (Distance == other.Distance) && (Duration == other.Duration)
      && (Time == other.Time) && (Reset == other.Reset)
      && (Idle == other.Idle);
}

PackageType NPC_AIWander::getPackageType() const
{
  return PackageType::ptWander;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIWander::saveToStream(std::ostream& output) const
{
  // write AI_W
  output.write(reinterpret_cast<const char*>(&cAI_W), 4);
  const uint32_t SubLength = 14;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write AI wander data
  output.write(reinterpret_cast<const char*>(&Distance), 2);
  output.write(reinterpret_cast<const char*>(&Duration), 2);
  output.write(reinterpret_cast<const char*>(&Time), 1);
  output.write(reinterpret_cast<const char*>(Idle.data()), 8);
  output.write(reinterpret_cast<const char*>(&Reset), 1);

  return output.good();
}
#endif

} // namespace
