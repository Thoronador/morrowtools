/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2021  Thoronador

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

#ifndef SR_ALCHEMYPOTIONRECORD_HPP
#define SR_ALCHEMYPOTIONRECORD_HPP

#include <array>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "EffectBlock.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a potion. */
struct AlchemyPotionRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    AlchemyPotionRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const AlchemyPotionRecord& other) const;
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
    static const uint32_t cFlagMedicine;
    static const uint32_t cFlagPoison;

    std::string editorID;
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString name; // subrecord FULL
    std::vector<uint32_t> keywords;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    uint32_t pickupSoundFormID; // subrecord YNAM
    uint32_t putdownSoundFormID; // subrecord ZNAM
    uint32_t equipTypeFormID; // subrecord ETYP
    float weight; // subrecord DATA
    // subrecord ENIT
    uint32_t value;
    uint32_t flags;
    uint32_t unknownThirdENIT;
    float    addictionChance;
    uint32_t useSoundFormID;
    // end of subrecord ENIT
    std::vector<EffectBlock> effects;

    /** \brief Checks whether the cost etc. are calculated automatically.
     *
     * \return Returns true, if the cost etc. are calculated automatically.
     */
    bool doesAutoCalc() const;

    /** \brief Checks whether the potion is a food item.
     *
     * \return Returns true, if the potion is a food item, according to flags.
     */
    inline bool isFoodItem() const
    {
      return (flags & cFlagFoodItem) != 0;
    }

    /** \brief Checks whether the potion is a medicine.
     *
     * \return Returns true, if the potion is a medicine, according to flags.
     */
    inline bool isMedicine() const
    {
      return (flags & cFlagMedicine) != 0;
    }

    /** \brief Checks whether the potion is a poison.
     *
     * \return Returns true, if the potion is a poison, according to flags.
     */
    inline bool isPoison() const
    {
      return (flags & cFlagPoison) != 0;
    }
}; // struct

} // namespace

#endif // SR_ALCHEMYPOTIONRECORD_HPP
