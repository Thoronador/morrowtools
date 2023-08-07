/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_INGREDIENTRECORD_HPP
#define SR_INGREDIENTRECORD_HPP

#include <array>
#include <string>
#include "BasicRecord.hpp"
#include "EffectBlock.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about an ingredient for potions. */
struct IngredientRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    IngredientRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const IngredientRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     */
    virtual uint32_t getWriteSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param in_File    the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    /* flag constants */
    static const uint32_t cFlagNoAutoCalc;
    static const uint32_t cFlagFoodItem;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString name; // subrecord FULL
    std::vector<uint32_t> keywords;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    uint32_t pickupSoundFormID; // subrecord YNAM
    uint32_t putdownSoundFormID; // subrecord ZNAM
    // subrecord DATA
    uint32_t value;
    float weight;
    // end of DATA
    // subrecord ENIT
    uint32_t baseCost;
    uint32_t flags;
    // end of ENIT
    std::vector<EffectBlock> effects;

    /** \brief Checks whether the Food Item flag is set.
     *
     * \return Returns true, if the Food Item flag is set.
     */
    inline bool isFoodItem() const
    {
      return (flags & cFlagFoodItem) != 0;
    }

    /** \brief Checks whether the Auto Calc option is active.
     *
     * \brief Returns true, if the Auto Calc option is active.
     */
    inline bool doesAutoCalc() const
    {
      return (flags & cFlagNoAutoCalc) == 0;
    }
}; // struct

} // namespace

#endif // SR_INGREDIENTRECORD_HPP
