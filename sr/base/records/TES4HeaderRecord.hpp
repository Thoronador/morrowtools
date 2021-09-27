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

#ifndef SR_TES4HEADERRECORD_HPP
#define SR_TES4HEADERRECORD_HPP

#include <optional>
#include <vector>
#include "BasicRecord.hpp"

namespace SRTP
{

/** Holds header information of an ESM / ESP file.  */
struct Tes4HeaderRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    Tes4HeaderRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const Tes4HeaderRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         record header is not included.
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

    /** \brief Checks whether the file is an ESM file, according to its flags.
     *
     * \return Returns true, if the file is an ESM file, according to its flags.
     *         Returns false otherwise.
     */
    bool isMasterFile() const;

    /** \brief Checks whether the file is localized, according to its flags.
     *
     * \return Returns true, if the file is localized, according to its flags.
     *         Returns false otherwise.
     */
    bool isLocalized() const;

    /// flag constants
    static const uint32_t cMasterFlag    = 0x00000001; /**< master file flag */
    static const uint32_t cLocalizedFlag = 0x00000080; /**< localized flag */

    /// structure for file dependencies
    struct MasterFile
    {
      std::string fileName; /**< name of the dependency, e. g. "Skyrim.esm" */
      int64_t data;         /**< data value; seems to be zero in most cases */

      /** Constructor, creates an empty struct. */
      MasterFile();

      /** \brief Equality operator for MasterFile.
       *
       * \param other   the other MasterFile to compare to
       * \return Returns true, if both structs contain the same data.
       *         Returns false otherwise.
       */
      bool operator==(const MasterFile& other) const;
    }; // struct

    // subrecord HEDR
    float version; /**< version; starts with 0.94 for Skyrim.esm, goes up to 1.7 */
    uint32_t numRecordsAndGroups; /**< number of records and groups in the file */
    uint32_t nextObjectID; /**< next free object ID (?) */
    // end of HEDR
    std::string authorName; /**< name of the file author, subrecord CNAM */
    std::string summary; /**< description, subrecord SNAM */
    std::vector<MasterFile> dependencies; /**< dependency files, subrecords MAST + DATA */
    std::vector<uint32_t> unknownONAM; /**< list of form IDs with unknown purpose, subrecord ONAM */
    uint32_t unknownIntValue; /**< unknown integer value, subrecord INTV */
    std::optional<uint32_t> unknownINCC; /**< unknown integer value, subrecord INCC */
}; // struct

} // namespace

#endif // SR_TES4HEADERRECORD_HPP
