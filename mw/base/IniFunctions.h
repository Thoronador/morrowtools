/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef INIFUNCTIONS_H
#define INIFUNCTIONS_H

#include <string>
#include "DepFiles.h"

namespace MWTP
{

/* tries to add all game files listed in the Morrowind.ini to 'files ' and
   returns zero on success or the non-zero error code on failure.

   parameters:
       baseDir - the path of the Morrowind Data Files directory, including the
                 trailing backslash
       files   - the list that will hold the file names from the Morrowind.ini
*/
int getFilesFromMorrowindIni(const std::string& baseDir, DepFileList& files);

} //namespace

#endif // INIFUNCTIONS_H
