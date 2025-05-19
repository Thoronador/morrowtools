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

#ifndef SR_ACTIVATORRECORD_HPP
#define SR_ACTIVATORRECORD_HPP

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about an activator. */
struct ActivatorRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    ActivatorRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const ActivatorRecord& other) const;
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

    struct destStruct
    {
      bool hasDSTD;
      std::array<uint8_t, 20> unknownDSTD;
      std::string destroyedModelPath;
      BinarySubRecord unknownDMDT;
      BinarySubRecord unknownDMDS;

      /* constructor */
      destStruct();

      /* equality operator */
      bool operator==(const destStruct& other) const;

      /* resets/clears the internal values */
      void reset();
    };

    struct Colour
    {
      uint8_t red;
      uint8_t green;
      uint8_t blue;

      Colour();

      bool operator==(const Colour& other) const;
    };

    std::string editorID; /**< ID of the record in the editor */
    BinarySubRecord unknownVMAD;
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString name; // sub record FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    std::optional<uint64_t> unknownDEST;
    std::vector<destStruct> destructionStructures;
    std::vector<uint32_t> keywords;
    // sub record PNAM
    std::optional<Colour> defaultPrimitiveColour;
    // end of sub record PNAM
    uint32_t loopingSoundFormID; // sub record SNAM
    uint32_t activateSoundFormID; // sub record VNAM
    uint32_t waterTypeFormID; // sub record WNAM
    LocalizedString activateTextOverride; // sub record RNAM
    std::optional<uint16_t> unknownFNAM;
    uint32_t interactionKeywordFormID; // sub record KNAM
}; // struct

} // namespace

#endif // SR_ACTIVATORRECORD_HPP
