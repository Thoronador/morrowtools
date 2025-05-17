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

#ifndef SR_ESMREADERCONTENTSALL_HPP
#define SR_ESMREADERCONTENTSALL_HPP

#include "ESMReaderContentsBase.hpp"

namespace SRTP
{

class ESMReaderContentsAll: public ESMReaderContentsBase
{
  public:
    /** \brief Constructs a new reader based on a file contents structure.
     *
     * \param storage  reference to the structure that will hold the records and
     *                 groups that will be read. This reference has to live at
     *                 least as long as the ESMReaderContentsAll instance.
     */
    ESMReaderContentsAll(ESMFileContents& storage);
  protected:
    /** \brief Indicates whether the reader implementation wants to read a group.
     *
     * \param g_data  the group header data
     * \return Always returns true.
     * \remarks Returns true for all groups, because this reader wants to read
     *          all groups.
     */
    virtual bool needGroup(const GroupData& g_data) const override;

    /** \brief Tries to read the next record from a stream.
     * \param input      the input stream the record shall be read from
     * \param recName    name (header) of the next record
     * \param localized  true, if the data in the stream is localized
     * \param table      in case of localized data: the string table
     * \return Returns the number of relevant records that were read (usually
     *         one). If an error occurred, -1 is returned. If the record was
     *         skipped or contained no relevant data, zero is returned.
     */
    virtual int readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table) override;
}; // class

} // namespace

#endif // SR_ESMREADERCONTENTSALL_HPP
