/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef MW_DIRECTORYTRAVERSAL_H
#define MW_DIRECTORYTRAVERSAL_H

#include <set>
#include <string>
#include "../base/DepFiles.h"
#include "../../base/ComparisonFunctor.h"

namespace MWTP
{

/* tries to get all master and plugin files in the specified directory and adds
   them to the file list 'files'

   parameters:
       dir   - the directory that is searched for master and plugin files
       files - the list that will hold the file names
*/
void getAllDataFiles(const std::string& dir, DepFileList& files);

void getDeletableMeshes(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables);

void getDeletableIcons(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables);

} //namespace

#endif // MW_DIRECTORYTRAVERSAL_H
