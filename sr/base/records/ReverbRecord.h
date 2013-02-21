/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_REVERBRECORD_H
#define SR_REVERBRECORD_H

#include <string>
#include "BasicRecord.h"

namespace SRTP
{

struct ReverbRecord: public BasicRecord
{
  public:
    /* constructor */
    ReverbRecord();

    /* destructor */
    virtual ~ReverbRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ReverbRecord& other) const;
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

    /* returns the floating point value for decay HF ratio */
    float getDecayHFRatio() const;

    /* returns the real, unscaled reflect delay value */
    uint16_t getReflectDelay() const;

    static const float cReflectDelayScale;

    std::string editorID;
    //start of data subrecord
    uint16_t decayTime; //in ms
    uint16_t HF_reference; //in Hz
    int8_t   roomFilter;
    int8_t   roomHF_filter;
    int8_t   reflections;
    int8_t   reverbAmp;
    uint8_t  decayHF_ratio100; // x100, real value is one hundreth of that
    uint8_t  reflectDelayScaled; //scaled by approx. 0.83 - value 0x00 maps to 0, 0xF9 maps to 300
    uint8_t  reverbDelay; //in ms
    uint8_t  diffusionPercentage; //in %
    uint8_t  densitiyPercentage; //in %
    uint8_t  unknownDATA_unused; //probably unused?
    //end of DATA
}; //struct

} //namespace

#endif // SR_REVERBRECORD_H
