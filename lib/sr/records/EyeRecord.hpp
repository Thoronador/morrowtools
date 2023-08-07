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

#ifndef SR_EYERECORD_HPP
#define SR_EYERECORD_HPP

#include <string>
#include "BasicRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct EyeRecord: public BasicRecord
{
  public:
    EyeRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as this instance.
     *         Returns false otherwise.
     */
    bool equals(const EyeRecord& other) const;
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

    /* flag constants */
    static const uint8_t FlagPlayable;
    static const uint8_t FlagFemale;
    static const uint8_t FlagMale;

    /** \brief Checks whether the playable flag is set.
     *
     * \return Returns true, if the playable flag is set.
     */
    bool isPlayable() const;

    /** \brief Checks whether the "Can Be Male" flag is set.
     *
     * \return Returns true, if the "Can Be Male" flag is set.
     */
    bool canBeMale() const;

    /** \brief Checks whether the "Can Be Female" flag is set.
     *
     * \return Returns true, if the "Can Be Female" flag is set.
     */
    bool canBeFemale() const;

    std::string editorID;
    LocalizedString name;
    std::string iconPath;
    uint8_t flags; // sub record DATA
}; // struct

} // namespace

#endif // SR_EYERECORD_HPP
