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

#ifndef SR_LIGHTRECORD_H
#define SR_LIGHTRECORD_H

#include <string>
#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "LocalizedString.h"

namespace SRTP
{

struct LightRecord: public BasicRecord
{
  public:
    /* constructor */
    LightRecord();

    /* destructor */
    virtual ~LightRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const LightRecord& other) const;
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
          in_File   - the input file stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    /* returns true, if the "can be carried" flag is set */
    bool canBeCarried() const;

    /* returns true, if the light is off by default

       remarks:
           This is only meaningful, if the "can be carried" flag is set.
    */
    bool offByDefault() const;

    //enumeration values for flicker effect type
    enum FlickerEffectType {fetNone, fetFlicker, fetPulse};

    /* returns the light's flicker effect type */
    FlickerEffectType getFlickerType() const;

    //enumeration values for light type
    enum LightType {ltOmnidirectional, ltShadowSpot, ltShadowHemisphere, ltShadowOmnidirectional};

    /* returns the light's type */
    LightType getLightType() const;

    //flag constants
    static const uint32_t cFlagCanBeCarried;
    static const uint32_t cFlagOffByDefault;
    static const uint32_t cFlagShadowSpot;
    static const uint32_t cFlagShadowHemisphere;
    static const uint32_t cFlagShadowOmnidirectional;
    static const uint32_t cFlagFlicker;
    static const uint32_t cFlagPulse;

    std::string editorID;
    uint8_t unknownOBND[12];
    std::string modelPath;
    BinarySubRecord unknownMODT;
    LocalizedString name; //subrecord FULL
    //subrecord DATA
    int32_t  time;
    uint32_t radius;
    uint32_t colour; //R, G, B one byte each, last byte is zero
    uint32_t flags;
    float    falloffExponent;
    float    FOV;
    float    nearClip;
    uint32_t unknownDATA_08;
    float    intensityAmplitude;
    float    movementAmplitude;
    uint32_t value;
    float    weight;
    //end of subrecord DATA
    float fadeTime; //subrecord FNAM
    uint32_t soundFormID; //subrecord SNAM
}; //struct

} //namespace

#endif // SR_LIGHTRECORD_H
