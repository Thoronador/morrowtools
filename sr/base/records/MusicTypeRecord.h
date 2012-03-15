/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_MUSICTYPERECORD_H
#define SR_MUSICTYPERECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>

namespace SRTP
{

struct MusicTypeRecord: public BasicRecord
{
  public:
    /* constructor */
    MusicTypeRecord();

    /* destructor */
    virtual ~MusicTypeRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const MusicTypeRecord& other) const;
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
    virtual int32_t getRecordType() const;

    /* flag constants */
    static const uint32_t FlagPlaysOneTransition;
    static const uint32_t FlagAbruptTransition;
    static const uint32_t FlagCycleTracks;
    static const uint32_t FlagMaintainTrackOrder;
    static const uint32_t FlagDucksCurrentTrack;

    /* returns true, if the "Play One Transition" flag is set */
    bool playsOneTransition() const;

    /* returns true, if the "Abrupt Transition" flag is set */
    bool hasAbruptTransition() const;

    /* returns true, if the "Cycle Tracks" flag is set */
    bool cyclesTracks() const;

    /* returns true, if the "Maintrain Track Order" flag is set */
    bool maintainsTrackOrder() const;

    /* returns true, if the "Ducks Current Track" flag is set */
    bool ducksCurrentTrack() const;

    /* returns the actual Ducking dB value as a float */
    float getDucking_dB() const;

    std::string editorID;
    uint32_t flags; //subrecord FNAM
    //subrecord PNAM
    uint16_t priority;
    uint16_t ducking_dB_uint16;
    //end of PNAM
    float fadeDuration; //subrecord WNAM
    std::vector<uint32_t> musicTrackFormIDs; //subrecord TNAM
}; //struct

} //namespace

#endif // SR_MUSICTYPERECORD_H
