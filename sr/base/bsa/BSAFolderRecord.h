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

#ifndef SR_BSAFOLDERRECORD_H
#define SR_BSAFOLDERRECORD_H

#include "BSAHash.h"
#include <fstream>

namespace SRTP
{

struct BSAFolderRecord
{
  public:
    /* constructor */
    BSAFolderRecord();

    /* destructor */
    ~BSAFolderRecord();

    //stuff
    BSAHash nameHash;
    uint32_t count;
    uint32_t offset;

    /* tries to read the folder record from the given file stream and returns
       true in case of success, false in case of failure

       parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);
}; //struct

} //namespace

#endif // SR_BSAFOLDERRECORD_H
