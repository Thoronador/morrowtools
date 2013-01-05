/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013 Thoronador

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

#ifndef SR_CONTAINERRECORD_H
#define SR_CONTAINERRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "ComponentData.h"

namespace SRTP
{

struct ContainerRecord: public BasicRecord
{
  public:
    /* constructor */
    ContainerRecord();

    /* destructor */
    virtual ~ContainerRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ContainerRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    /* flag constants */
    static const uint8_t cFlagRespawns;
    static const uint8_t cFlagShowOwner;
    // ---- header flags
    static const uint32_t cFlagRandomAnimStart;
    static const uint32_t cFlagObstacle;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    std::vector<ComponentData> contents;
    BinarySubRecord unknownCOED;
    //subrecord DATA
    uint8_t flags;
    float weight;
    //end of subrecord DATA
    uint32_t openSoundFormID; //subrecord SNAM
    uint32_t closeSoundFormID; //subrecord QNAM

    /* returns true, if the respawn flag is set */
    inline bool respawns() const
    {
      return ((flags&cFlagRespawns)!=0);
    }

    /* returns true, if the show owner flag is set */
    inline bool showsOwner() const
    {
      return ((flags&cFlagShowOwner)!=0);
    }

    /* returns true, if the "Random Anim Start" flag is set */
    inline bool hasRandomAnimStart() const
    {
      return ((headerFlags&cFlagRandomAnimStart)!=0);
    }

    /* returns true, if the obstacle flag is set */
    inline bool isObstacle() const
    {
      return ((headerFlags&cFlagObstacle)!=0);
    }
}; //struct

} //namespace

#endif // SR_CONTAINERRECORD_H
