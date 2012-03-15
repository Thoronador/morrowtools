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
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

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
