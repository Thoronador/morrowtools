/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2025  Dirk Stolle

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

#ifndef SR_ESMWRITERCONTENTS_HPP
#define SR_ESMWRITERCONTENTS_HPP

#include "ESMWriter.hpp"
#include "ESMFileContents.hpp"

#ifndef SR_UNSAVEABLE_RECORDS
namespace SRTP
{

class ESMWriterContents: public ESMWriter
{
  public:
    /** \brief Constructs a new writer based on a file content structure.
     *
     * \param storage  reference to the structure that will hold the records and
     *                 groups that will be written. This reference has to live
     *                 at least as long as the ESMWriterContents instance.
     * \remarks The application has to fill the storage before calling
     *          writeESM() or the produced ESM won't contain any records besides
     *          the header.
     */
    ESMWriterContents(ESMFileContents& storage);
  protected:
    /** \brief Gets the number of records that will be written to the stream, not
     *         including the header record.
     *
     * \return Returns the number of records that will be written to the stream.
     */
    virtual uint32_t getTotalNumberOfRecords() const;

    /** \brief Gets the number of groups that will be written to the stream.
     */
    virtual uint32_t getTotalNumberOfGroups() const;

    /** \brief Writes all groups and data records to a file stream.
     *
     * \param output  the output file stream that's used to write the data
     * \return Returns true on success. Returns false, if an error occurred.
     */
    virtual bool writeGroups(std::ofstream& output) const;

    /** reference to the structure where the data to be written is stored */
    ESMFileContents& contents;
}; // class

} // namespace
#endif // SR_UNSAVEABLE_RECORDS

#endif // SR_ESMWRITERCONTENTS_HPP
