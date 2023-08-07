/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2023  Dirk Stolle

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

#ifndef SR_DEPENDENCYSOLVER_HPP
#define SR_DEPENDENCYSOLVER_HPP

#include <set>
#include <string>
#include <vector>
#include "records/TES4HeaderRecord.hpp"

namespace SRTP
{

struct DependencyElement
{
  std::string name; /**< name of the ESM file, e. g. "Skyrim.esm" */
  SRTP::Tes4HeaderRecord header; /**< header data of the ESM file */

  DependencyElement();
  DependencyElement(const std::string& file_name);
  DependencyElement(const std::string& file_name, const Tes4HeaderRecord& head);

  bool operator<(const DependencyElement& other) const;
};

/** \brief Determines the load order for the given ESM files and stores them in result.
 *
 * \param esmNames  vector containing the basic .esm file names, i.e. not the
 *                  full path but only "Skyrim.esm", "Cool PlugIn.esm", etc.
 * \param dataDir   path to the Data directory of Skyrim, including trailing
 *                  backslash
 * \param result    the vector that is used to store the resulting load order
 * \return Returns true in case of success; returns false, if an error occurred.
 */
bool getLoadOrder(const std::vector<std::string>& esmNames, const std::string& dataDir, std::vector<std::string>& result);

/** \brief Determines the load order for the given files and stores them in result.
 *
 * \param files   set of .esm file names, including their headers
 * \param result  the vector that is used to store the resulting load order
 * \return Returns true in case of success; returns false, if an error occurred.
 */
bool getLoadOrder(const std::set<DependencyElement>& files, std::vector<std::string>& result);

} // namespace

#endif // SR_DEPENDENCYSOLVER_HPP
