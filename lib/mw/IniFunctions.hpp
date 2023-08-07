/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#ifndef MW_INIFUNCTIONS_HPP
#define MW_INIFUNCTIONS_HPP

#include <string>
#include "DepFiles.hpp"

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

#endif // MW_INIFUNCTIONS_HPP
