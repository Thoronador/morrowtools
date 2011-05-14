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
#include <cstring>
#include "../MW_Constants.h"

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

bool NPC_AIActivate::saveToStream(std::ofstream& output) const
{
  //write AI_A
  output.write((char*) &cAI_A, 4);
  uint32_t SubLength = 33; //fixed length of 33 bytes
  //write AI_A's length
  output.write((char*) &SubLength, 4);
  //write AI activate data
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
  output.write((char*) &Reset, 1);

  return output.good();
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

bool NPC_AIEscortFollow::saveToStream(std::ofstream& output) const
{
  if (getPackageType()==ptEscort)
  {
    //write AI_E
    output.write((char*) &cAI_E, 4);
  }
  else
  {
    //write AI_F
    output.write((char*) &cAI_F, 4);
  }
  uint32_t SubLength = 48; //fixed length of 48 bytes
  //write AI_E's/AI_F's length
  output.write((char*) &SubLength, 4);
  //write AI escort/follow data
  output.write((char*) &X, 4);
  output.write((char*) &Y, 4);
  output.write((char*) &Z, 4);
  output.write((char*) &Duration, 2);
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
  output.write((char*) &Reset, 2);

  //check for presence of cell
  if (CellName!="")
  {
    //write CNDT
    output.write((char*) &cCNDT, 4);
    SubLength = CellName.length()+1; //length of cell name +1 byte for NUL
    //write CNDT's length
    output.write((char*) &SubLength, 4);
    //write AI escort's/follow's cell name
    output.write(CellName.c_str(), SubLength);
  }

  return output.good();
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

bool NPC_AITravel::saveToStream(std::ofstream& output) const
{
  //write AI_T
  output.write((char*) &cAI_T, 4);
  uint32_t SubLength = 16; //fixed length of 16 bytes
  //write AI_T's length
  output.write((char*) &SubLength, 4);
  //write AI travel data
  output.write((char*) &X, 4);
  output.write((char*) &Y, 4);
  output.write((char*) &Z, 4);
  output.write((char*) &Reset, 4);

  return output.good();
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

bool NPC_AIWander::saveToStream(std::ofstream& output) const
{
  //write AI_W
  output.write((char*) &cAI_W, 4);
  uint32_t SubLength = 14; //fixed length of 14 bytes
  //write AI_W's length
  output.write((char*) &SubLength, 4);
  //write AI wander data
  output.write((char*) &Distance, 2);
  output.write((char*) &Duration, 2);
  output.write((char*) &Time, 1);
  output.write((char*) &Idle, 8);
  output.write((char*) &Reset, 1);

  return output.good();
}
