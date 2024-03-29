/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_LEVELEDLISTBASERECORD_HPP
#define SR_LEVELEDLISTBASERECORD_HPP

#include <vector>
#include "BasicRecord.hpp"

namespace SRTP
{

struct LeveledListBaseRecord: public BasicRecord
{
  public:
    /* constructor */
    LeveledListBaseRecord();

    /* destructor */
    virtual ~LeveledListBaseRecord();

    //structure for leveled list entries
    struct LeveledListEntry
    {
      uint32_t level;
      uint32_t formID;
      uint32_t count;

      /* comparison operator */
      bool operator==(const LeveledListEntry& other) const;
    };//struct

    //structure for additional data to the entry
    struct COEDData
    {
      uint32_t factionFormID;
      uint32_t requiredRank;
      float    unknownFloat;

      /* comparison operator */
      bool operator==(const COEDData& other) const;

      /* initialises structure with default values */
      void init();

      /* returns true, if the structure contains the default values */
      bool isInit() const;
    };//struct

    struct EntryWithCOED
    {
      LeveledListEntry entry;
      COEDData coed;

      /* comparison operator */
      bool operator==(const EntryWithCOED& other) const;
    }; //struct

    //flag constants
    static const uint8_t cFlagCalcFromAllLevels;
    static const uint8_t cFlagCalcForEach;

    /* returns true, if the "calculate from all levels <= PC" flag is set */
    bool calculateFromAllLevels() const;

    /* returns true, if the "calculate for each item in count" flag is set */
    bool calculateForEachItem() const;

    uint8_t chanceNone; //subrecord LVLD
    uint8_t flags; //subrecord LVLF
    //uint8_t entryCount; //subrecord LLCT
    std::vector<EntryWithCOED> entries; //subrecords LVLO (+ optional COED)
}; //struct

} //namespace

#endif // SR_LEVELEDLISTBASERECORD_HPP
