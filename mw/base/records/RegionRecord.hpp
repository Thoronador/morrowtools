/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_REGIONRECORD_HPP
#define MW_REGIONRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct SoundChanceRecord
{
  std::string Sound;
  uint8_t Chance;
};//struct

struct RegionRecord: public BasicRecord
{
  std::string recordID; //formerly RegionID
  std::string RegionName;
  //weather data
  uint8_t Clear;
  uint8_t Cloudy;
  uint8_t Foggy;
  uint8_t Overcast;
  uint8_t Rain;
  uint8_t Thunder;
  uint8_t Ash;
  uint8_t Blight;
  uint8_t Snow; //BM only
  uint8_t Blizzard; //BM only
  //end of weather data
  std::string SleepCreature;
  //Region Map Colour
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  uint8_t Zero;
  //sounds
  std::vector<SoundChanceRecord> SoundChances;

  /* constructor */
  RegionRecord();

  /* alternative constructor */
  RegionRecord(const std::string& ID);

  /* destructor */
  ~RegionRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const RegionRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output              - the output stream
        forceBloodmoonStyle - if set to true, the written WEAT subrecord will
                              have a size of 10 bytes, even when it wouldn't be
                              necessary
  */
  bool saveToStream(std::ostream& output, const bool forceBloodmoonStyle) const;

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
  bool loadFromStream(std::istream& in_File);
};//struct

//comparison operator for ordered set
bool operator<(const RegionRecord& left, const RegionRecord& right);

} //namespace

#endif // MW_REGIONRECORD_HPP
