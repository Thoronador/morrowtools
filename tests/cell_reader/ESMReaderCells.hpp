/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#ifndef MW_TEST_ESMREADERCELLS_HPP
#define MW_TEST_ESMREADERCELLS_HPP

#include "../../lib/mw/ESMReader.hpp"

namespace MWTP
{

/** ESMReaderCells class
 *
 * Reads all cell records from master (.esm) and plugin (.esp) files of
 * Morrowind.
 */
class ESMReaderCells: public ESMReader
{
  public:
    /** \brief Tries to read the next record from a stream.
     *
     * \param input  the input stream the record shall be read from
     * \return Returns the number of relevant records that were read (usually
     *         one). If an error occurred, -1 is returned. If the record was
     *         skipped or contained no relevant data, zero is returned.
     */
    virtual int processNextRecord(std::istream& input) override;
}; // class

} // namespace

#endif // MW_TEST_ESMREADERCELLS_HPP
