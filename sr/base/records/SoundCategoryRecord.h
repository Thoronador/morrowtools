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

#ifndef SR_SOUNDCATEGORYRECORD_H
#define SR_SOUNDCATEGORYRECORD_H

#include <string>
#include "BasicRecord.h"

namespace SRTP
{

struct SoundCategoryRecord: public BasicRecord
{
  public:
    /* constructor */
    SoundCategoryRecord();

    /* destructor */
    virtual ~SoundCategoryRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const SoundCategoryRecord& other) const;

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

    /* returns true, if the "Mute When submerged" flag is set */
    bool isMuteWhenSubmerged() const;

    /* returns true, if the "Should Appear on Menu" flag is set */
    bool shouldAppearOnMenu() const;

    /* returns the actual static volume multiplier as a float value */
    float getStaticVolumeMultiplier() const;

    /* returns the default menu value as float */
    float getDefaultMenuValue() const;

    //constants for FNAM
    static const uint32_t cMuteWhenSubmerged;
    static const uint32_t cShouldAppearOnMenu;

    std::string editorID;
    uint32_t unknownFULL;
    uint32_t unknownFNAM; //flags
    bool hasParent;
    uint32_t parentFormID; //subrecord PNAM, ID of parent category
    uint16_t staticVolumeMultUint16; //surecord VNAM
    bool hasUNAM;
    uint16_t defaultMenuValueUint16; //subrecord UNAM
}; //struct

} //namespace

#endif // SR_SOUNDCATEGORYRECORD_H