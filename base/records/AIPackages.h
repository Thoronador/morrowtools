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

#ifndef AIPACKAGES_H
#define AIPACKAGES_H

#include <string>

/* enumeration type for types of AI packages */
enum PackageType {ptActivate, ptEscort, ptFollow, ptTravel, ptWander};

//dummy structure for use as ancestor of real AI packages like NPC_AIWander or NPC_AITravel
struct NPC_BasicAIPackage
{
  /*destructor */
  virtual ~NPC_BasicAIPackage();

  /* returns enum indicating the type

     remarks:
         Each derived class has to implement its own version of that function.
  */
  virtual PackageType getPackageType() const = 0;
};//struct

/* structure to hold tha AI activate data of an NPC */
struct NPC_AIActivate: public NPC_BasicAIPackage
{
  std::string TargetID;
  int8_t Reset;

  /* "comparison operator" */
  bool equals(const NPC_AIActivate& other) const;

  /* returns enum for type */
  virtual PackageType getPackageType() const;
};//struct

/* structure to hold the escort/follow data of an NPC (both escort and follow
     have the same data members, so we need only one structure for both) */
struct NPC_AIEscortFollow: public NPC_BasicAIPackage
{
  float X, Y, Z;
  int16_t Duration;
  std::string TargetID;
  int16_t Reset;
  std::string CellName;

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AIEscortFollow& other) const;
};//struct

struct NPC_AIEscort: public NPC_AIEscortFollow
{
  /* returns enum for type */
  virtual PackageType getPackageType() const;
};//struct

struct NPC_AIFollow: public NPC_AIEscortFollow
{
  /* returns enum for type */
  virtual PackageType getPackageType() const;
};//struct

/* structure to hold the travel data of an NPC */
struct NPC_AITravel: public NPC_BasicAIPackage
{
  float X, Y, Z;
  int32_t Reset;

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AITravel& other) const;

  /* returns enum for type */
  virtual PackageType getPackageType() const;
};//struct

/* structure to hold the wandering data of an NPC */
struct NPC_AIWander: public NPC_BasicAIPackage
{
  int16_t Distance;
  int16_t Duration;
  int8_t Time;
  uint8_t Idle[8];
  uint8_t Reset;

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AIWander& other) const;

  /* returns enum indicating the type */
  virtual PackageType getPackageType() const;
};//struct

#endif // AIPACKAGES_H
