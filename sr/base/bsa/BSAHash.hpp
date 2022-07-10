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

#ifndef SR_BSAHASH_HPP
#define SR_BSAHASH_HPP

#include <cstdint>
#include <filesystem>

namespace SRTP
{

// define hash type
using BSAHash = uint64_t;

/** \brief Calculates the hash value for a file name.
 *
 * \param path   path containing the file name, e. g. "foo.dds"
 * \return Returns the corresponding hash value.
 * \remarks This is currently not tested for directories, only for files.
 */
BSAHash calculateHash(const std::filesystem::path& path);

} // namespace

#endif // SR_BSAHASH_HPP
