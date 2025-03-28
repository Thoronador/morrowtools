/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#ifndef MW_ESMREADERTRANSLATOR_HPP
#define MW_ESMREADERTRANSLATOR_HPP

#include "../../../lib/mw/ESMReaderGeneric.hpp"

namespace MWTP
{

/** \brief This class is a specialization of ESMReaderGeneric and loads all
 * record types which can potentially contain translatable data into their
 * proper, typed record class while all other records (those where no data has
 * to be translated) are loaded as GenericRecord instead.
 */
class ESMReaderTranslator: public ESMReaderGeneric
{
  public:
    /** \brief Constructs a new reader based on a record vector.
     *
     * \param vec  pointer to the vector that will hold the records that will
     *             be read. This pointer must not be NULL and has to live at
     *             least as long as the ESMReaderTranslator.
     */
    ESMReaderTranslator(VectorType* vec);

    virtual ~ESMReaderTranslator() = default;
  protected:
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

#endif // MW_ESMREADERTRANSLATOR_HPP
