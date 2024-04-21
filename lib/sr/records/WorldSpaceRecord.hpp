/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021, 2024  Dirk Stolle

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

#ifndef SR_WORLDSPACERECORD_HPP
#define SR_WORLDSPACERECORD_HPP

#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "BinarySubRecordExtended.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a world space in the game. */
struct WorldSpaceRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    WorldSpaceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const WorldSpaceRecord& other) const;
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

    std::string editorID; /**< ID of the record in the editor */
    std::vector<BinarySubRecord> unknownRNAMs;
    BinarySubRecord unknownMHDT;
    LocalizedString name; // sub record FULL
    // sub record WCTR
    bool hasWCTR;
    int16_t centerCellX;
    int16_t centerCellY;
    // end of sub record WCTR
    uint32_t interiorLightingFormID; // sub record LTMP
    uint32_t encounterZoneFormID; // sub record XEZN
    uint32_t climateFormID; // sub record CNAM
    uint32_t locationFormID; // sub record XLCN
    uint32_t parentWorldSpaceFormID; // sub record WNAM
    std::optional<uint16_t> unknownPNAM;
    uint32_t waterFormID; // sub record NAM2
    uint32_t LODWaterTypeFormID; // sub record NAM3
    std::optional<float> LODWaterHeight; // sub record NAM4
    std::optional<uint64_t> unknownDNAM;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMNAM;
    BinarySubRecord unknownONAM;
    float distantLODMultiplier; // sub record NAMA
    std::optional<uint8_t> unknownDATA;
    std::optional<uint64_t> unknownNAM0;
    std::optional<uint64_t> unknownNAM9;
    uint32_t musicFormID; // sub record ZNAM
    std::string HD_LOD_DiffuseTexture; // sub record TNAN
    std::string HD_LOD_NormalTexture; // sub record UNAM
    std::string unknownXWEM; // sub record XWEM
    BinarySubRecordExtended unknownOFST;
}; // struct

} // namespace

#endif // SR_WORLDSPACERECORD_HPP
