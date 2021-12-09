/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021  Dirk Stolle

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

#ifndef MW_ESMREADER_HPP
#define MW_ESMREADER_HPP

#include <string>
#include <fstream>
#include "DepFiles.hpp"
#include "records/TES3Record.hpp"

namespace MWTP
{

/** ESMReader class
 *
 * This is the basic class for reading master (.esm) and plugin (.esp) files of
 * Morrowind. The ESMReader class reads the header of the file; all subsequent
 * records will be read by processNextRecord() function. In this class, the
 * function just skips all records and reads no real data.
 * To change that behaviour and actually read some data, you have to derive a
 * new class from ESMReader and re-implement the processNextRecord() function,
 * which has to take care that the required data is read and not skipped.
 */
class ESMReader
{
  public:
    /** Virtual destructor to ensure destructors of derived classes get called.
     */
    virtual ~ESMReader() = default;

    /** \brief Skips a record.
     *
     * \param input  the input stream
     * \return Returns zero on success, and -1 on error.
     */
    static int skipRecord(std::istream& input);

    /** \brief Tries to read an .esm/.esp file.
     *
     * \param FileName  name of the .esm/.esp file
     * \param theHead   the TES3Record that will be used to store the header
     *                  data
     * \return Returns the number of relevant records that were read.
     *         If an error occurred, -1 is returned.
     */
    int readESM(const std::string& FileName, TES3Record& theHead);

    /** \brief Tries to read the TES3 record of an .esm/.esp file.
     *
     * \param FileName  name of the .esm/.esp file
     * \param theHead   the TES3Record that will be used to store the header
     *                  data
     * \return Returns true in case of success, false on failure.
     */
    static bool peekESMHeader(const std::string& FileName, TES3Record& theHead);

    /** \brief Tries to read the next record from a stream.
     *
     * \param input  the input stream the record shall be read from
     * \return Returns the number of relevant records that were read (usually
     *         one). If an error occurred, -1 is returned. If the record was
     *         skipped or contained no relevant data, zero is returned.
     * \remarks
     * If you actually want to read some data, you have to derive a class from
     * ESMReader and set its processNextRecord() function up in a way that does
     * not just skip all data records - because that is what the implementation
     * here in ESMReader basically does.
     * So naturally, this function will never return values larger than zero.
     */
    virtual int processNextRecord(std::istream& input);
}; // class

} // namespace

#endif // MW_ESMREADER_HPP
