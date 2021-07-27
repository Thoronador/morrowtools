/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013 Thoronador

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

#ifndef SR_CONTAINERRECORD_HPP
#define SR_CONTAINERRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "LocalizedString.hpp"

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
          output   - the output stream
    */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File   - the input stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

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
    LocalizedString name; //subrecord FULL
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

#endif // SR_CONTAINERRECORD_HPP
