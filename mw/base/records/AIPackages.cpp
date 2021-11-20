/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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
  TargetID(""),
  Reset(0),
  CellName("")
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
  return (((X==other.X) or ((X!=X) and (other.X!=other.X)))
      and ((Y==other.Y) or ((Y!=Y) and (other.Y!=other.Y)))
      and ((Z==other.Z) or ((Z!=Z) and (other.Z!=other.Z)))
      and (Duration==other.Duration) and (TargetID==other.TargetID)
      and (Reset==other.Reset) and (CellName==other.CellName));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIEscortFollow::saveToStream(std::ostream& output) const
{
  if (getPackageType() == PackageType::ptEscort)
  {
    // write AI_E
    output.write((const char*) &cAI_E, 4);
  }
  else
  {
    // write AI_F
    output.write((const char*) &cAI_F, 4);
  }
  uint32_t SubLength = 48; //fixed length of 48 bytes
  //write AI_E's/AI_F's length
  output.write((const char*) &SubLength, 4);
  //write AI escort/follow data
  output.write((const char*) &X, 4);
  output.write((const char*) &Y, 4);
  output.write((const char*) &Z, 4);
  output.write((const char*) &Duration, 2);
  // ---- write target ID
  unsigned int len = TargetID.length();
  if (len>31)
  {
    len = 31;
  }
  output.write(TargetID.c_str(), len);
  // ---- fill rest with NUL
  output.write(NULof32, 32-len);
  // ---- reset flag
  output.write((const char*) &Reset, 2);

  //check for presence of cell
  if (!CellName.empty())
  {
    //write CNDT
    output.write((const char*) &cCNDT, 4);
    SubLength = CellName.length()+1; //length of cell name +1 byte for NUL
    //write CNDT's length
    output.write((const char*) &SubLength, 4);
    //write AI escort's/follow's cell name
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
  return (((X==other.X) or ((X!=X) and (other.X!=other.X)))
      and ((Y==other.Y) or ((Y!=Y) and (other.Y!=other.Y)))
      and ((Z==other.Z) or ((Z!=Z) and (other.Z!=other.Z)))
      and (Reset==other.Reset));
}

PackageType NPC_AITravel::getPackageType() const
{
  return PackageType::ptTravel;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AITravel::saveToStream(std::ostream& output) const
{
  //write AI_T
  output.write((const char*) &cAI_T, 4);
  uint32_t SubLength = 16; //fixed length of 16 bytes
  //write AI_T's length
  output.write((const char*) &SubLength, 4);
  //write AI travel data
  output.write((const char*) &X, 4);
  output.write((const char*) &Y, 4);
  output.write((const char*) &Z, 4);
  output.write((const char*) &Reset, 4);

  return output.good();
}
#endif

/* **** AIWander's functions ****/

NPC_AIWander::NPC_AIWander()
: NPC_BasicAIPackage(),
  Distance(0),
  Duration(0),
  Time(0),
  Reset(0)
{
  Idle[0] = Idle[1] = Idle[2] = Idle[3] =
  Idle[4] = Idle[5] = Idle[6] = Idle[7] = 0;
}

void NPC_AIWander::clear()
{
  Distance = Duration = 0;
  Time = 0;
  Idle[0] = Idle[1] = Idle[2] = Idle[3] = Idle[4] = Idle[5] = Idle[6] =
    Idle[7] = 0;
  Reset = 0;
}

bool NPC_AIWander::equals(const NPC_AIWander& other) const
{
  return ((Distance==other.Distance) and (Duration==other.Duration)
      and (Time==other.Time) and (Reset==other.Reset)
      and (memcmp(Idle, other.Idle, 8)==0));
}

PackageType NPC_AIWander::getPackageType() const
{
  return PackageType::ptWander;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool NPC_AIWander::saveToStream(std::ostream& output) const
{
  //write AI_W
  output.write((const char*) &cAI_W, 4);
  uint32_t SubLength = 14; //fixed length of 14 bytes
  //write AI_W's length
  output.write((const char*) &SubLength, 4);
  //write AI wander data
  output.write((const char*) &Distance, 2);
  output.write((const char*) &Duration, 2);
  output.write((const char*) &Time, 1);
  output.write((const char*) &Idle, 8);
  output.write((const char*) &Reset, 1);

  return output.good();
}
#endif

} // namespace
