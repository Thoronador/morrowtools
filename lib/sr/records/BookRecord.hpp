/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#ifndef SR_BOOKRECORD_HPP
#define SR_BOOKRECORD_HPP

#include "BasicRecord.hpp"
#include <array>
#include <string>
#include <vector>
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a book. */
struct BookRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    BookRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instances contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const BookRecord& other) const;
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
     * \param input      the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& input, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString title; // sub record FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    LocalizedString text; // DESC
    uint32_t pickupSoundFormID; // sub record YNAM
    uint32_t putdownSoundFormID; // sub record ZNAM
    std::vector<uint32_t> keywords;
    // sub record DATA
    uint32_t bookFlags;
    uint32_t spellOrSkillID;
    uint32_t bookValue;
    float weight;
    // end of sub record DATA
    uint32_t inventoryArtFormID; // sub record INAM
    uint32_t unknownCNAM;

    // flag constants
    static const uint32_t cSkillBookFlag;
    static const uint32_t cSpellTomeFlag;
    static const uint32_t cNoteOrScrollTypeFlag;

    /** \brief Check whether this is a skill book.
     *
     * \return Returns true, if the book is a skill book according to its flags.
     */
    bool isSkillBook() const;

    /** \brief Checks whether the book is a spell tome according to its flags.
     *
     * \return Returns true, if the book is a spell tome according to its flags.
     */
    bool isSpellTome() const;
}; // struct

} // namespace

#endif // SR_BOOKRECORD_HPP
