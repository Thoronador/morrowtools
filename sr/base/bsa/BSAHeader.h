/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef SR_BSAHEADER_H
#define SR_BSAHEADER_H

#include <fstream>
#include <stdint.h>

namespace SRTP
{

struct BSAHeader
{
  public:
    /* constructor */
    BSAHeader();

    /* destructor */
    ~BSAHeader();

    //stuff
    int32_t fileID;
    uint32_t version;
    uint32_t offset;
    uint32_t archiveFlags;
    uint32_t folderCount;
    uint32_t fileCount;
    uint32_t totalFolderNameLength;
    uint32_t totalFileNameLength;
    uint32_t fileFlags;

    /* tries to read the header from the given file stream and returns true in
       case of success, false in case of failure

       parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);

    /* flag stuff */
    bool hasNamesForFolders() const;
    bool hasNamesForFiles() const;

    /* returns true, if files are compressed by default, according to flags */
    bool filesCompressedByDefault() const;
}; //struct

} //namespace

#endif // SR_BSAHEADER_H
