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

#ifndef SR_BSADIRECTORYBLOCK_HPP
#define SR_BSADIRECTORYBLOCK_HPP

#include <string>
#include <vector>
#include "BSAFileRecord.hpp"

namespace SRTP
{

/** Holds name of a directory inside a BSA and metadata of files in that directory. */
struct BSADirectoryBlock
{
  public:
    BSADirectoryBlock();

    std::string name; /**< name of the directory, may contain several hierarchy levels */
    std::vector<BSAFileRecord> files; /**< metadata of files in the directory */

    /** \brief Tries to read the directory block from the given stream.
     *
     * \param input  the input stream to read from
     * \return Returns true in case of success, false in case of failure.
     */
    bool loadFromStream(std::istream& input, const uint32_t fileCount);
}; // struct

} // namespace

#endif // SR_BSADIRECTORYBLOCK_HPP
