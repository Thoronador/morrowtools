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

#ifndef SR_DUALCASTDATARECORD_H
#define SR_DUALCASTDATARECORD_H

#include "BasicRecord.h"
#include <string>

namespace SRTP
{

struct DualCastDataRecord: public BasicRecord
{
  public:
    /* constructor */
    DualCastDataRecord();

    /* destructor */
    virtual ~DualCastDataRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const DualCastDataRecord& other) const;
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

    /* returns true, if the hit effect art inherits the scale (according to flags) */
    bool hitEffectArtInheritsScale() const;

    /* returns true, if the projectile inherits the scale (according to flags) */
    bool projectileInheritsScale() const;

    /* returns true, if the explosion inherits the scale (according to flags) */
    bool explosionInheritsScale() const;

    /* constants for flag values */
    static const uint32_t cFlagHitEffectArtInherits;
    static const uint32_t cFlagProjectileInherits;
    static const uint32_t cFlagExplosionInherits;

    std::string editorID;
    uint8_t unknownOBND[12];
    //subrecord DATA
    uint32_t projectileFormID;
    uint32_t explosionFormID;
    uint32_t effectShaderFormID;
    uint32_t hitEffectArtFormID;
    uint32_t impactDataSetFormID;
    uint32_t inheritFlags;
    //end of DATA
}; //struct

} //namespace

#endif // SR_DUALCASTDATARECORD_H
