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

#ifndef SR_MATERIALTYPERECORD_HPP
#define SR_MATERIALTYPERECORD_HPP

#include "BasicRecord.hpp"
#include <string>

namespace SRTP
{

struct MaterialTypeRecord: public BasicRecord
{
  public:
    /* constructor */
    MaterialTypeRecord();

    /* destructor */
    virtual ~MaterialTypeRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const MaterialTypeRecord& other) const;
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

    /* returns true, if arrows stick to this material (according to flags) */
    bool arrowsStick() const;

    /* returns true, if this material is stair material (according to flags) */
    bool isStairMaterial() const;

    //constants for flags
    static const uint32_t cFlagStairMaterial;
    static const uint32_t cFlagArrowsStick;

    std::string editorID;
    std::string materialName; //subrecord MNAM
    uint32_t parentMaterialFormID; //subrecord PNAM
    //subrecord CNAM
    float havokDisplayColorR;
    float havokDisplayColorG;
    float havokDisplayColorB;
    //end of subrecord CNAM
    float bouyancy; //subrecord BNAM
    uint32_t flags; //subrecord FNAM
    uint32_t havokImpactDataSetID; //subrecord HNAM
}; //struct

} //namespace

#endif // SR_MATERIALTYPERECORD_HPP
