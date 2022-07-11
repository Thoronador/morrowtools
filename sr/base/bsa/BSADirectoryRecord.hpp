/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021, 2022  Dirk Stolle

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

#ifndef SR_BSADIRECTORYRECORD_HPP
#define SR_BSADIRECTORYRECORD_HPP

#include "BSAHash.hpp"
#include <fstream>

namespace SRTP
{

struct BSADirectoryRecord
{
  public:
    BSADirectoryRecord();

    BSAHash nameHash;
    uint32_t count;
    uint32_t offset;

    /** \brief Tries to read the directory record from the given stream.
     *
     * \param  input       the input stream
     * \param  bsaVersion  version from the BSA header
     * \return Returns true in case of success, false in case of failure.
     */
    bool loadFromStream(std::istream& input, const uint32_t bsaVersion);
}; // struct

} // namespace

#endif // SR_BSADIRECTORYRECORD_HPP
