/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2025  Dirk Stolle

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

#ifndef MW_ESMREADERCONTENTS_HPP
#define MW_ESMREADERCONTENTS_HPP

#include "ESMReader.hpp"

namespace MWTP
{

/** This descendant of the ESMReader class tries to read all records from the
 * given .esm/.esp file and stores the read data records in a vector.
 */
class ESMReaderContents: public ESMReader
{
  public:
    /// shortcut for the type of the used vector
    typedef std::vector<BasicRecord*> VectorType;


    /** \brief Constructs a new reader based on a record vector.
     *
     * \param storage  reference to the vector that will hold the records that
     *                 will be read. This reference has to live as long as the
     *                 ESMReaderContents instance, at least.
     */
    ESMReaderContents(VectorType& storage);
  protected:
    /** \brief Tries to read the next record from a stream.
     *
     * \param input  the input stream the record shall be read from
     * \return Returns the number of relevant records that were read (usually
     *         one). If an error occurred, -1 is returned. If the record was
     *         skipped or contained no relevant data, zero is returned.
     */
    virtual int processNextRecord(std::istream& input) override;

    /** reference to the vector where the read data will be stored */
    VectorType& contents;
}; // class

} // namespace

#endif // MW_ESMREADERCONTENTS_HPP
