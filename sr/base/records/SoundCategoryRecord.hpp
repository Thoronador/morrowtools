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

#ifndef SR_SOUNDCATEGORYRECORD_HPP
#define SR_SOUNDCATEGORYRECORD_HPP

#include <string>
#include "BasicRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct SoundCategoryRecord: public BasicRecord
{
  public:
    /* constructor */
    SoundCategoryRecord();

    /* destructor */
    virtual ~SoundCategoryRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const SoundCategoryRecord& other) const;
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
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

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
    LocalizedString name;
    uint32_t unknownFNAM; //flags
    bool hasParent;
    uint32_t parentFormID; //subrecord PNAM, ID of parent category
    uint16_t staticVolumeMultUint16; //surecord VNAM
    bool hasUNAM;
    uint16_t defaultMenuValueUint16; //subrecord UNAM
}; //struct

} //namespace

#endif // SR_SOUNDCATEGORYRECORD_HPP
