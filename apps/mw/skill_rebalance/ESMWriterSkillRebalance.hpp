/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012, 2021, 2024  Dirk Stolle

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

#ifndef MW_ESMWRITERSKILLREBALANCE_HPP
#define MW_ESMWRITERSKILLREBALANCE_HPP

#include "../../../lib/mw/ESMWriter.hpp"

namespace MWTP
{

/** Specialization of ESMWriter for the skill_rebalance program. */
class ESMWriterSkillRebalance: public ESMWriter
{
  protected:
    /** Gets the number of records that will be written to the stream.
     */
    virtual int32_t getTotalRecords() const override;

    /** \brief Tries to write all data records to the stream.
     *
     * \param output  the output stream that's used to write the records
     * \return Returns true on success. Returns false, if an error occurred.
     */
    virtual bool writeRecords(std::ostream& output) const override;
}; // class

} // namespace

#endif // MW_ESMWRITERSKILLREBALANCE_HPP
