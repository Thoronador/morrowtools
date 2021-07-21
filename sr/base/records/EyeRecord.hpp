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

#ifndef SR_EYERECORD_HPP
#define SR_EYERECORD_HPP

#include <string>
#include "BasicRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct EyeRecord: public BasicRecord
{
  public:
    /* constructor */
    EyeRecord();

    /* destructor */
    virtual ~EyeRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const EyeRecord& other) const;
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

    /* flag constants */
    static const uint8_t FlagPlayable;
    static const uint8_t FlagFemale;
    static const uint8_t FlagMale;

    /* returns true, if the playable flag is set */
    bool isPlayable() const;

    /* returns true, if the "Can Be Male" flag is set */
    bool canBeMale() const;

    /* returns true, if the "Can Be Female" flag is set */
    bool canBeFemale() const;

    std::string editorID;
    LocalizedString name; //subrecord FULL
    std::string iconPath;
    uint8_t flags; //subrecord DATA
}; //struct

} //namespace

#endif // SR_EYERECORD_HPP
