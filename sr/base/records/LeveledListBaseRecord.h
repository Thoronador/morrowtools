/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_LEVELEDLISTBASERECORD_H
#define SR_LEVELEDLISTBASERECORD_H

#include <vector>
#include "BasicRecord.h"

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

    //uint8_t entryCount; //subrecord LLCT
    std::vector<EntryWithCOED> entries; //subrecords LVLO (+ optional COED)
}; //struct

} //namespace

#endif // SR_LEVELEDLISTBASERECORD_H
