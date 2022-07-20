/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#ifndef SR_ESMREADER_HPP
#define SR_ESMREADER_HPP

#include <fstream>
#include "Localization.hpp"
#include "StringTable.hpp"
#include "records/TES4HeaderRecord.hpp"
#include "records/GroupData.hpp"

namespace SRTP
{

class ESMReader
{
  public:
    ESMReader();

    /** Virtual destructor to ensure destructors of derived classes get called.
     */
    virtual ~ESMReader() = default;

    /** \brief Skips a record.
     *
     * \param input  the input stream
     * \return Returns zero on success, and -1 on error.
     */
    static int skipRecord(std::istream& input);

    /** \brief Skips a group.
     *
     * \param input  the input stream
     * \param g_data the data of the group to skip
     * \return Returns zero on success, and -1 on error.
     */
    static int skipGroup(std::istream& input, const GroupData& g_data);

    /** \brief Tries to read an .esm/.esp file of Skyrim.
     *
     * \param FileName  name of the .esm/.esp file
     * \param head      the record that will be used to store the header
     *                  information
     * \param l10n      the preferred localization for string tables, if any
     * \return Returns the number of relevant records that were read.
     *         If an error occurred, -1 is returned.
     */
    int readESM(const std::string& FileName, Tes4HeaderRecord& head, const std::optional<Localization>& l10n);

    /** \brief Tries to read the TES4 record of an .esm/.esp file.
     *
     * \param FileName name of the .esm/.esp file
     * \param theHead  the Tes4HeaderRecord that will be used to store the
     *                 header data
     * \return Returns true in case of success.
     *         Returns false, if an error occurred.
     */
    static bool peekESMHeader(const std::string& FileName, Tes4HeaderRecord& theHead);

  protected:
    /** \brief Tries to read the next group from a stream.
     *
     * \param input       the input stream the group shall be read from
     * \param withHeader  if set to true, the starting four bytes (GRUP) will be
     *                    read, too. If set to false, the functions expects the
     *                    data directly.
     * \param localized   true, if the data in the stream is localized
     * \param table       in case of localized data: the string table
     * \return Returns the number of relevant groups that were read (usually
     *         one). If an error occurred, -1 is returned. If the group was
     *         skipped or contained no relevant data, zero is returned.
     */
    int processGroup(std::istream& input, const bool withHeader, const bool localized, const StringTable& table);

    /** \brief Indicates whether the reader implementation wants to read a group.
     *
     * This has to return true for any group data that shall be read.
     * All groups where false is returned will be skipped by the reader.
     * \param g_data  the group header data
     * \return If you actually want to read some data, you have to derive a
     *         class from ESMReader and set its needGroup() function up in a way
     *         that it does not just always return false and causes processGroup()
     *         to skip all groups and data records.
     */
    virtual bool needGroup(const GroupData& g_data) const = 0;

    /** \brief This function's sole purpose is to "notify" the reader that a
     *         new group was encountered and give the classes derived from
     *         ESMReader the possibility to update their internal state
     *         accordingly. Note that this function will only be called for
     *         groups where needGroup() returns true.
     *         Skipped groups will not trigger that function.
     *         Shall return false only if the read process should be aborted.
     * \param g_data  the group header data
     * \param sub     if set to true, the new group is a sub-group of another
     *                group
     * \return Shall returns true, if reading from the stream shall continue.
     *         Shall return false, only if the read process shall be aborted.
     */
    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub) = 0;

    /** \brief This function's sole purpose is to "notify" the reader that a
     *         started group has been read (or skipped) completely and give the
     *         classes derived from ESMReader the possibility to update their
     *         internal state accordingly.
     * \remarks
     * Note that this function will only be called for groups where needGroup()
     * returns true. Skipped groups will not trigger that function, but it might
     * be possible that all records and subgroups in such a group have been
     * skipped, depending on the implementation of readNextRecord() and
     * readGroup().
     * Shall return false only if the read process should be aborted.
     * \param g_data  the group header data
     * \return Shall returns true, if reading from the stream shall continue.
     *         Shall return false, only if the read process shall be aborted.
     */
    virtual bool groupFinished(const GroupData& g_data) = 0;

    /** \brief Tries to read the next group from a stream.
     *
     * \param input      the input stream used to read the group
     * \param g_data     group's data header
     * \param localized  true, if the data in the stream is localized
     * \param table      in case of localized data: the string table
     * \return Returns the number of relevant groups that were read (usually
     *         one). If an error occurred, -1 is returned. If the group was
     *         skipped or contained no relevant data, zero is returned.
     * \remarks This function is guaranteed to be only called for needed groups
     *          (see the function needGroup() for details).
     *
     *          If you actually want to read some data, you have to derive a
     *          class from ESMReader and set its readGroup() function up in a
     *          way that does not just skip all groups - because that is what
     *          the implementation here in ESMReader does.
     *          So naturally, this function will never return values larger than
     *          zero.
     */
    virtual int readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table);

    /** \brief Tries to read the next record from a stream.
     * \param input      the input stream the record shall be read from
     * \param recName    name (header) of the next record
     * \param localized  true, if the data in the stream is localized
     * \param table      in case of localized data: the string table
     * \return Returns the number of relevant records that were read (usually
     *         one). If an error occurred, -1 is returned. If the record was
     *         skipped or contained no relevant data, zero is returned.
     * \remark If you actually want to read some data, you have to derive a
     *         class from ESMReader and set its readNextRecord() function up
     *         in a way that does read the data records you want to have.
     */
    virtual int readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table) = 0;

    /** \brief Holds the current/last TES4 header record that was read by the reader.
     *
     * \remark This is only valid, if readESM() has been called. Otherwise its
     *         content is undefined.
     */
    Tes4HeaderRecord currentHead;
}; // class

} // namespace

#endif // SR_ESMREADER_HPP
