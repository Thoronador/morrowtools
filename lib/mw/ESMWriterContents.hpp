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

#ifndef MW_ESMWRITERCONTENTS_HPP
#define MW_ESMWRITERCONTENTS_HPP

#include "ESMWriter.hpp"

namespace MWTP
{

class ESMWriterContents: public ESMWriter
{
  public:
    /// shortcut for the type of the used vector
    typedef std::vector<BasicRecord*> VectorType;


    /** \brief Constructs a new writer based on a record vector.
     *
     * \param storage  reference to the vector that will hold the records that
     *                 will be written. This reference has to live as long as
     *                 the ESMWriterContents instance, at least.
     */
    ESMWriterContents(VectorType& storage);
  protected:
    /** Gets the number of records that will be written to the stream. */
    virtual int32_t getTotalRecords() const override;
    /** \brief Writes all data records to the given stream.
     *
     * \param output  the output stream that's used to write the records
     * \return Returns true, if all records have been written successfully.
     *         Returns false otherwise.
     */
    virtual bool writeRecords(std::ostream& output) const override;

    /** reference to the vector where the data to be written is stored */
    VectorType& contents;
};

} // namespace

#endif // MW_ESMWRITERCONTENTS_HPP
