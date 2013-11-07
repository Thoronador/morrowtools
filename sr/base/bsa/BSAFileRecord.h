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

#ifndef SR_BSAFILERECORD_H
#define SR_BSAFILERECORD_H

#include "BSAHash.h"
#include <fstream>

namespace SRTP
{

struct BSAFileRecord
{
  public:
    /* constructor */
    BSAFileRecord();

    /* destructor */
    ~BSAFileRecord();

    //stuff
    BSAHash nameHash;
    uint32_t fileSize;
    uint32_t offset;

    // not directly part of record
    std::string fileName;

    /* tries to read the file record from the given file stream and returns
       true in case of success, false in case of failure

       parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);

    /* returns true, if the file's compression deviates from the archive's default */
    bool isCompressionToggled() const;

    /* returns the file's size, i.e. fileSize without the flag bit */
    uint32_t getRealFileSize() const;
}; //struct

} //namespace

#endif // SR_BSAFILERECORD_H
