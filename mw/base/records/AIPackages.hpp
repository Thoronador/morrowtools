/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#ifndef MW_AIPACKAGES_HPP
#define MW_AIPACKAGES_HPP

#include <cstdint>
#include <string>
#include <fstream>

namespace MWTP
{

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

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the package subrecord to the given output stream and returns true
     on success

     parameters:
         output - the output stream
  */
  virtual bool saveToStream(std::ostream& output) const = 0;
  #endif
};//struct

/* structure to hold the AI activate data of an NPC */
struct NPC_AIActivate: public NPC_BasicAIPackage
{
  std::string TargetID;
  int8_t Reset;

  /* constructor */
  NPC_AIActivate();

  /* "comparison operator" */
  bool equals(const NPC_AIActivate& other) const;

  /* returns enum for type */
  PackageType getPackageType() const override;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the package subrecord to the given output stream and returns true
     on success

     parameters:
         output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif
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

  /* constructor */
  NPC_AIEscortFollow();

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AIEscortFollow& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the package subrecord to the given output stream and returns true
     on success

     parameters:
         output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif
};//struct

struct NPC_AIEscort: public NPC_AIEscortFollow
{
  /* returns enum for type */
  PackageType getPackageType() const override;
};//struct

struct NPC_AIFollow: public NPC_AIEscortFollow
{
  /* returns enum for type */
  PackageType getPackageType() const override;
};//struct

/* structure to hold the travel data of an NPC */
struct NPC_AITravel: public NPC_BasicAIPackage
{
  float X, Y, Z;
  int32_t Reset;

  /* constructor */
  NPC_AITravel();

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AITravel& other) const;

  /* returns enum for type */
  PackageType getPackageType() const override;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the package subrecord to the given output stream and returns true
     on success

     parameters:
         output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif
};//struct

/* structure to hold the wandering data of an NPC */
struct NPC_AIWander: public NPC_BasicAIPackage
{
  int16_t Distance;
  int16_t Duration;
  int8_t Time;
  uint8_t Idle[8];
  uint8_t Reset;

  /* constructor */
  NPC_AIWander();

  /* sets all members to zero */
  void clear();

  /* "comparison operator" */
  bool equals(const NPC_AIWander& other) const;

  /* returns enum indicating the type */
  PackageType getPackageType() const override;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the package subrecord to the given output stream and returns true
     on success

     parameters:
         output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif
};//struct

} //namespace

#endif // MW_AIPACKAGES_HPP
