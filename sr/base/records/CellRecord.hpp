/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#ifndef SR_CELLRECORD_HPP
#define SR_CELLRECORD_HPP

#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct CellRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    CellRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const CellRecord& other) const;
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

    /// type for XCLC subrecord
    struct SubrecordXCLC
    {
      bool presence;
      int32_t locationX;
      int32_t locationY;
      uint32_t unknownThird;

      SubrecordXCLC();

      bool operator==(const SubrecordXCLC& other) const;
    };

    std::string editorID; /**< ID of the record in the editor */
    LocalizedString name; // subrecord FULL
    BinarySubRecord unknownDATA;
    SubrecordXCLC gridLocation;
    BinarySubRecord unknownTVDT;
    BinarySubRecord unknownMHDT;
    BinarySubRecord unknownXCLL;
    uint32_t lightingTemplateFormID; // subrecord LTMP
    std::optional<uint32_t> unknownLNAM;
    float    unknownXCLW;
    std::vector<uint32_t> unknownXCLR;
    std::optional<uint8_t> unknownXNAM;
    std::optional<uint32_t> unknownXWCN;
    std::optional<uint32_t> unknownXWCS;
    BinarySubRecord unknownXWCU;
    uint32_t imageSpaceFormID; // subrecord XCIM
    uint32_t locationFormID; // subrecord XLCN
    uint32_t encounterZoneFormID; // subrecord XEZN
    std::optional<uint32_t> unknownXCWT;
    uint32_t ownerFactionFormID; // subrecord XOWN
    uint32_t lockListFormID; // subrecord XILL
    uint32_t musicTypeFormID; // subrecord XCMO
    uint32_t regionFormID; // subrecord XCCM
    std::string environmentMap; // subrecord XWEM
    uint32_t defaultAcousticSpaceFormID; // subrecord CAS
}; // struct

} // namespace

#endif // SR_CELLRECORD_HPP
