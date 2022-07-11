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

#ifndef SR_BSAHEADER_HPP
#define SR_BSAHEADER_HPP

#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds header data of a *.bsa archive. */
struct BSAHeader
{
  public:
    /** Constructor, sets all data to zero. */
    BSAHeader();

    int32_t fileID;
    uint32_t version; /**< BSA version, usually 104 or 105 for Skyrim */
    uint32_t offset;
    uint32_t archiveFlags; /**< contains flag bits for the archive */
    uint32_t directoryCount; /**< number of directories in the archive */
    uint32_t fileCount; /**< number of files in the archive */
    uint32_t totalDirectoryNameLength;
    uint32_t totalFileNameLength;
    uint32_t fileFlags; /**< content type flags */

    /** \brief Tries to read the header from the given stream.
     *
     * \param input  the input stream
     * \return Returns true in case of success, false in case of failure.
     */
    bool loadFromStream(std::istream& input);

    /* flag stuff */
    /** \brief Checks whether the archive has directory names, according to flags.
     *
     * \return Returns true, if the archive has directory names.
     */
    bool hasNamesForDirectories() const;

    /** \brief Checks whether the archive has file names, according to flags.
     *
     * \return Returns true, if the archive has file names.
     */
    bool hasNamesForFiles() const;

    /** \brief Checks whether files are compressed by default, according to flags.
     *
     * \return Returns true, if files are compressed by default.
     */
    bool filesCompressedByDefault() const;

    /** \brief Checks whether the file is an Xbox archive, according to flags.
     *
     * \return Returns true, the file is an Xbox archive.
     */
    bool isXboxArchive() const;

    /** \brief Checks whether file names are embedded, according to flags.
     *
     * \return Returns true, if file names are embedded.
     */
    bool hasEmbeddedFileNames() const;

    /** \brief Checks whether the file is an Xbox archive, according to flags.
     *
     * \return Returns true, the file is an Xbox archive.
     */
    bool usesXMemCodec() const;


    enum class ContentType : std::uint32_t
    {
      Meshes = 0x00000001,
      Textures = 0x00000002,
      Menus = 0x00000004,
      Sounds = 0x00000008,
      Voices = 0x00000010,
      Shaders = 0x00000020,
      Trees = 0x00000040,
      Fonts = 0x00000080,
      Misc = 0x00000100
    };

    /** \brief Checks whether the file contains certain types of content, according to flags.
     *
     * \param content   the content type to test for
     * \return Returns true, the contains the given content type.
     */
    bool contains(const ContentType content) const;
}; // struct

using ContentType = BSAHeader::ContentType;

} // namespace

#endif // SR_BSAHEADER_HPP
