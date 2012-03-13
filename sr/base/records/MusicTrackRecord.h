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

#ifndef SR_MUSICTRACKRECORD_H
#define SR_MUSICTRACKRECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>
#include "CTDAData.h"

namespace SRTP
{

struct MusicTrackRecord: public BasicRecord
{
  public:
    /* constructor */
    MusicTrackRecord();

    /* destructor */
    virtual ~MusicTrackRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const MusicTrackRecord& other) const;

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

    //structure for loop-related data
    struct LoopData
    {
      bool hasLNAM;
      float loopBegins; //seconds
      float loopEnds; //seconds
      uint32_t loopCount;

      /* equality operator */
      bool operator==(const LoopData& other) const;
    };//struct

    //constant values for track types
    static const uint32_t cTypeSingle;
    static const uint32_t cTypeSilent;
    static const uint32_t cTypePalette;

    /* enumeration type for track types */
    enum TrackType {ttSingleTrack, ttSilentTrack, ttPalette};

    /* returns the track's type */
    TrackType getTrackType() const;

    std::string editorID;
    uint32_t typeIdentifier; //subrecor CNAM
    //subrecord FLTV
    bool hasFLTV;
    float duration;
    //end of FLTV
    //subrecord DNAM
    bool hasDNAM;
    float fadeOut;
    //end of DNAM
    std::vector<uint32_t> trackList; //subrecord SNAM
    std::string unknownANAM; //subrecord ANAM
    std::string finalePath; //subrecord BNAM
    std::vector<float> cuePoints; //subrecord FNAM
    //subrecord LNAM
    LoopData loop;
    //end of LNAM
    std::vector<CTDAData> conditions; //subrecord(s) CTDA (with prior CITC)
}; //struct

} //namespace

#endif // SR_MUSICTRACKRECORD_H
