/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022  Dirk Stolle

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

#ifndef SR_RACERECORD_HPP
#define SR_RACERECORD_HPP

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"
#include "SubBlock.hpp"

namespace SRTP
{

struct RaceRecord: public BasicRecord
{
  public:
    /** Creates an empty record. */
    RaceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as this instance.
     *         Returns false otherwise.
     */
    bool equals(const RaceRecord& other) const;
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
     * \param input    the input stream
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

    /** \brief Gets the length of the DATA record in bytes.
     *
     * \return Returns the length of the DATA record in bytes.
     */
    uint32_t getDataLength() const;

    /// type for data sub-record
    struct RaceData
    {
      std::array<std::uint8_t, 16> unknown16;
      float heightMale;
      float heightFemale;
      float weightMale;
      float weightFemale;
      std::array<uint8_t, 96> unknown96;
      std::optional<std::array<uint8_t, 36> > unknown36;

      RaceData();

      bool operator==(const RaceData& other) const;

      /** \brief Resets all data. */
      void clear();
    };

    std::string editorID;
    LocalizedString name; // subrecord FULL
    LocalizedString description; // subrecord DESC
    std::vector<uint32_t> spellFormIDs;
    std::optional<uint32_t> unknownWNAM;
    BinarySubRecord unknownBODT;
    BinarySubRecord unknownBOD2;
    std::vector<uint32_t> keywords;
    RaceData data;
    std::vector<SubBlock> subBlocks;
}; // struct

} // namespace

#endif // SR_RACERECORD_HPP
