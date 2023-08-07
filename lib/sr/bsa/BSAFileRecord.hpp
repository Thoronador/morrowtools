/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021, 2023  Dirk Stolle

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

#ifndef SR_BSAFILERECORD_HPP
#define SR_BSAFILERECORD_HPP

#include "BSAHash.hpp"
#include <fstream>
#include <string>

namespace SRTP
{

/** Holds hash, size, flag and offset data of a file as well as its name. */
struct BSAFileRecord
{
  public:
    BSAFileRecord();

    BSAHash nameHash;
    uint32_t fileBlockSize; /**< size of the file block in bytes, plus flag bit */
    uint32_t offset;

    // not directly part of record
    std::string fileName;

    /** \brief Tries to read the file record from the given stream.
     *
     * \param input  the input stream to read from
     * \return Returns true in case of success, false in case of failure.
     */
    bool loadFromStream(std::istream& input);

    /** \brief Checks whether the file's compression deviates from the archive's
     *         default.
     *
     * \return Returns true, if the file's compression deviates from the
     *         archive's default. Returns false otherwise.
     */
    bool isCompressionToggled() const;

    /** \brief Gets the actual file block size.
     *
     * \return Returns the file block's size, i.e. fileBlockSize without the
     *         flag bit.
     */
    uint32_t getRealFileBlockSize() const;
}; // struct

} // namespace

#endif // SR_BSAFILERECORD_HPP
