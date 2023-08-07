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

#ifndef SR_IMPACTDATARECORD_HPP
#define SR_IMPACTDATARECORD_HPP

#include "BasicRecord.hpp"
#include <string>
#include "BinarySubRecord.hpp"

namespace SRTP
{

struct ImpactDataRecord: public BasicRecord
{
  public:
    /* constructor */
    ImpactDataRecord();

    /* destructor */
    virtual ~ImpactDataRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ImpactDataRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output stream
    */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File   - the input stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    /* struct for DODT subrecord */
    struct SubrecordDODT
    {
      public:
        float    minWidth;
        float    maxWidth;
        float    minHeight;
        float    maxHeight;
        float    depth;
        float    shininess;
        float    parallaxScale;
        uint32_t parallaxPasses;
        uint8_t  colorRed;
        uint8_t  colorGreen;
        uint8_t  colorBlue;
        uint8_t  zeroes;

        /* constructor */

        /* equality operator */
        bool operator==(const SubrecordDODT& other) const;

        /* returns true, if this subrecord is present */
        inline bool isPresent() const
        {
          return m_Presence;
        }

        /* sets the new presence status of the subrecord */
        void setPresence(const bool newPresence);
      private:
        bool m_Presence;
    };//struct

    /* Sound Level constants */
    static const uint32_t cSoundLevelLoud;
    static const uint32_t cSoundLevelNormal;
    static const uint32_t cSoundLevelSilent;
    static const uint32_t cSoundLevelVeryLoud;

    std::string editorID;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    //subrecord DATA
    float effectDuration;
    uint32_t unknownDATA02;
    float angleTreshold;
    float placementRadius;
    uint32_t soundLevel;
    uint32_t unknownDATA06;
    //end of subrecord DATA
    SubrecordDODT decalData; //subrecord DODT
    uint32_t textureSetFormID; //subrecord DNAM
    uint32_t secondaryTextureSetFormID; //subrecord ENAM
    uint32_t impactSoundOneFormID; //subrecord SNAM
    uint32_t impactSoundTwoFormID; //subrecord NAM1
    uint32_t hazardFormID; //subrecord NAM2
}; //struct


} //namespace

#endif // SR_IMPACTDATARECORD_HPP
