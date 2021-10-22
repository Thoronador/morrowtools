/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#ifndef SR_BSAHEADER_HPP
#define SR_BSAHEADER_HPP

#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds header data of an *.bsa archive. */
struct BSAHeader
{
  public:
    /** Constructor, sets all data to zero. */
    BSAHeader();

    int32_t fileID;
    uint32_t version;
    uint32_t offset;
    uint32_t archiveFlags;
    uint32_t folderCount;
    uint32_t fileCount;
    uint32_t totalFolderNameLength;
    uint32_t totalFileNameLength;
    uint32_t fileFlags;

    /** \brief Tries to read the header from the given stream.
     *
     * \param input  the input stream
     * \return Returns true in case of success, false in case of failure.
     */
    bool loadFromStream(std::istream& input);

    /* flag stuff */
    bool hasNamesForFolders() const;
    bool hasNamesForFiles() const;

    /** \brief Checks whether files are compressed by default, according to flags.
     *
     * \return Returns true, if files are compressed by default.
     */
    bool filesCompressedByDefault() const;
}; // struct

} // namespace

#endif // SR_BSAHEADER_HPP
