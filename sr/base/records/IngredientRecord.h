/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013 Thoronador

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

#ifndef SR_INGREDIENTRECORD_H
#define SR_INGREDIENTRECORD_H

#include <string>
#include "BasicRecord.h"
#include "EffectBlock.h"
#include "BinarySubRecord.h"
#include "LocalizedString.h"

namespace SRTP
{

struct IngredientRecord: public BasicRecord
{
  public:
    /* constructor */
    IngredientRecord();

    /* destructor */
    virtual ~IngredientRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const IngredientRecord& other) const;
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
    static const uint32_t cFlagNoAutoCalc;
    static const uint32_t cFlagFoodItem;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    LocalizedString name; //subrecord FULL
    std::vector<uint32_t> keywordArray;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    uint32_t pickupSoundFormID; //subrecord YNAM
    uint32_t putdownSoundFormID; //subrecord ZNAM
    //subreord DATA
    uint32_t value;
    float weight;
    //end of DATA
    //subrecord ENIT
    uint32_t baseCost;
    uint32_t flags;
    //end of ENIT
    std::vector<EffectBlock> effects;

    /* returns true, if the Food Item flag is set */
    inline bool isFoodItem() const
    {
      return ((flags&cFlagFoodItem)!=0);
    }

    /* returns true, if the Auto Calc option is active */
    inline bool doesAutoCalc() const
    {
      return ((flags&cFlagNoAutoCalc)==0);
    }
}; //struct

} //namespace

#endif // SR_INGREDIENTRECORD_H
