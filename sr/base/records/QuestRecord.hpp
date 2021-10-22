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

#ifndef SR_QUESTRECORD_HPP
#define SR_QUESTRECORD_HPP

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "CTDAData.hpp"
#include "LocalizedString.hpp"
#include "quest/AliasEntry.hpp"
#include "quest/IndexEntry.hpp"
#include "quest/QOBJEntry.hpp"

namespace SRTP
{

/** Holds information about a quest and its stages. */
struct QuestRecord: public BasicRecord
{
  public:
    QuestRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const QuestRecord& other) const;
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

    /** \brief Checks whether an QOBJ record with the given index is present.
     *
     * \param idx  the requested index
     * \return Returns true, if an QOBJ record with the given index is present.
     */
    bool hasQOBJForIndex(const uint16_t idx) const;

    /** \brief Returns the QOBJ record with the given index, if it is present.
     *         If no such QOBJ is present, the method will throw an exception.
     *         Use hasQOBJForIndex() to check for presence first.
     *
     * \param idx  the requested index
     * \return Returns the QOBJ record with the given index, if it is present.
     * \throws If no QOBJ with matching index is present, the method will throw
     *         an exception.
     */
    const QOBJEntry& getQOBJForIndex(const uint16_t idx) const;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    LocalizedString name; // subrecord FULL
    std::array<uint8_t, 12> unknownDNAM;
    std::optional<uint32_t> unknownENAM;
    std::vector<uint32_t> unknownQTGLs;
    std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
    std::string filter;
    std::vector<IndexEntry> indices;
    std::vector<QOBJEntry> theQOBJs;
    uint32_t unknownANAM;
    std::vector<AliasEntry> aliases;
};// struct

} // namespace

#endif // SR_QUESTRECORD_HPP
