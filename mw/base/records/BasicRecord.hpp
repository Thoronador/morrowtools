/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#ifndef MW_BASICRECORD_HPP
#define MW_BASICRECORD_HPP

#include <cstdint>
#include <fstream>

namespace MWTP
{

/** Abstract base class for all records in Morrowind's ESM files. */
struct BasicRecord
{
  public:
    BasicRecord();

    virtual ~BasicRecord();

    #ifndef MW_UNSAVEABLE_RECORDS
    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const = 0;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param input    the input stream
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& input) = 0;

    /** \brief Gets the first header part of this record. */
    int32_t getHeaderOne() const;

    /** \brief Gets the header flags of this record. */
    int32_t getHeaderFlags() const;

    /** \brief Sets the first header part of the record.
     *
     * \param newValue  the new value for this header part
     */
    void setHeaderOne(const int32_t newValue);

    /** \brief Sets the header flags of the record.
     *
     * \param newValue  the new flag value
     */
    void setHeaderFlags(const int32_t newValue);

    /** \brief Checks whether the blocked flag is set.
     *
     * \return Returns true, if the blocked flag is set.
     */
    bool isBlocked() const;

    /** \brief Checks whether the "References Persist" flag is set.
     *
     * \return Returns true, if the "References Persist" flag is set.
     */
    bool isPersistent() const;
  protected:
     int32_t HeaderOne;
     int32_t HeaderFlags;
}; // struct

} // namespace

#endif // MW_BASICRECORD_HPP
