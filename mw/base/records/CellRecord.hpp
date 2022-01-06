/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Dirk Stolle

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

#ifndef MW_CELLRECORD_HPP
#define MW_CELLRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "ReferencedObject.hpp"

namespace MWTP
{

struct AmbientLight
{
  bool isPresent;
  uint32_t AmbientColour;
  uint32_t SunlightColour;
  uint32_t FogColour;
  float FogDensity;

  bool operator==(const AmbientLight& other) const;
};//struct

struct CellRecord: public BasicRecord
{
  std::string CellID;
  //cell data
  uint32_t CellFlags;
  int32_t GridX;
  int32_t GridY;
  //end of cell data
  std::string RegionID;
  uint32_t NumReferences;
  uint32_t ColourRef;

  //WHGT
  bool hasWHGT;
  float WaterHeight;
  //AMBI
  AmbientLight Ambience;

  std::vector<ReferencedObject> References;

  CellRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const CellRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input stream
  */
  bool loadFromStream(std::istream& in_File) override;

  /* returns true, if the interior cell flag is set */
  bool isInterior() const;

  /* returns true, if the water flag is set */
  bool hasWater() const;
};//struct

} //namespace

#endif // MW_CELLRECORD_HPP
