/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

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

#ifndef SR_CAMERAPATHRECORD_H
#define SR_CAMERAPATHRECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>
#include "CTDAData.h"

namespace SRTP
{

struct CameraPathRecord: public BasicRecord
{
  public:
    /* constructor */
    CameraPathRecord();

    /* destructor */
    virtual ~CameraPathRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CameraPathRecord& other) const;
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

    /* data flag constants */
    static const uint8_t cFlagInverseMustHaveCameraShots;
    static const uint8_t cCameraZoomDefault;
    static const uint8_t cCameraZoomDisable;
    static const uint8_t cCameraZoomShotList;

    /* struct for ANAM subrecords */
    struct SubrecordANAM
    {
      uint32_t parentFormID;
      uint32_t nextFormID;

      /* comparison operator */
      bool operator==(const SubrecordANAM& other) const;
    };//struct

    std::string editorID;
    std::vector<CTDA_CIS1_compound> conditions;
    SubrecordANAM cameraPathLinks;
    uint8_t unknownDATA;
    std::vector<uint32_t> camShotList;

    /* returns true, if the "Must have Camera Shots" option is set */
    bool mustHaveCameraShots() const;

    /* emumeration type for camera zoom */
    enum CameraZoomType {czDefault, czDisable, czShotList};

    /* returns the camera zoom type */
    CameraZoomType getCameraZoom() const;
}; //struct

} //namespace

#endif // SR_CAMERAPATHRECORD_H
