/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "AIPackages.h"

/* **** BasicAIPackage's functions ****/

NPC_BasicAIPackage::~NPC_BasicAIPackage()
{
  //empty - just here to get rid of compiler warnings
}

/* **** AIActivate's functions ****/

bool NPC_AIActivate::equals(const NPC_AIActivate& other) const
{
  return ((TargetID==other.TargetID) and (Reset==other.Reset));
}

PackageType NPC_AIActivate::getPackageType() const
{
  return ptActivate;
}

/* **** AIEscortFollow's functions ****/

void NPC_AIEscortFollow::clear()
{
  X = Y = Z = 0.0f;
  Duration = 0;
  TargetID = "";
  Reset = 0;
  CellName = "";
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

/* **** AIEscort functions ****/

PackageType NPC_AIEscort::getPackageType() const
{
  return ptEscort;
}

/* **** AIFollow functions ****/

PackageType NPC_AIFollow::getPackageType() const
{
  return ptFollow;
}

/* **** AITravel's functions ****/

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
  return ptTravel;
}

/* **** AIWander's functions ****/

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
  return ptWander;
}
