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

#ifndef SR_CAMERASHOTRECORD_H
#define SR_CAMERASHOTRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>

namespace SRTP
{

struct CameraShotRecord: public BasicRecord
{
  public:
    /* constructor */
    CameraShotRecord();

    /* destructor */
    virtual ~CameraShotRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CameraShotRecord& other) const;
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

    //enumeration type for DATA's length
    enum DataLengthType {dlt40Byte=40, dlt44Byte=44};

    //constants for camera action
    static const uint32_t cActionShoot;
    static const uint32_t cActionFly;
    static const uint32_t cActionHit;
    static const uint32_t cActionZoom;

    //constants for location/target
    static const uint32_t cLocationAttacker;
    static const uint32_t cLocationProjectile;
    static const uint32_t cLocationTarget;
    static const uint32_t cLocationLeadActor;

    std::string editorID;
    std::string modelPath; //subrecord MODL
    BinarySubRecord unknownMODT;
    //subrecord DATA
    DataLengthType dataLen;
    uint32_t cameraAction;
    uint32_t cameraLocation;
    uint32_t cameraTarget;
    uint32_t cameraFlags;
    float timeMultPlayer;
    float timeMultTarget;
    float timeMultGlobal;
    float maxTime;
    float minTime;
    float targetPercentBetweenActors;
    float nearTargetDistance;
    //end of subrecord DATA
    uint32_t imageSpaceModFormID; //subrecord MNAM
}; //struct

} //namespace

#endif // SR_CAMERASHOTRECORD_H
