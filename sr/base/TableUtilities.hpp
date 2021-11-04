/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2015, 2021  Thoronador

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

#ifndef SR_TABLEUTILITIES_HPP
#define SR_TABLEUTILITIES_HPP

#include <string>
#include <vector>
#include "StringTable.hpp"

namespace SRTP
{

/** \brief Tries to find the language component of the string file names for a given
 * plugin.
 *
 * \param dataDir           - path to the data directory of Skyrim
 * \param pluginName        - name of the plugin (without extension),
                              e.g. "Skyrim" for Skyrim.esm
 * \param languageComponent - string that will be used to return the language
                              component of the string file names
 * \param stringTableFiles  - vector that will be used to returns the found file
                              names of the string table files
 * \return Returns an error code not equal to zero, if an error occurred.
 *         Returns zero in case of success.
 * \remarks The value of languageComponent is changed by this function, even
 *          if an error occurs. However, the value is undefined in that case
 *          and does not represent a proper value.
 */
int getLanguageComponent(const std::string& dataDir, const std::string& pluginName, std::string& languageComponent, std::vector<std::string>& stringTableFiles);

/* retrieves the paths of the associated string table files for a given
   .esm/.esp file and returns zero in case of success or non-zero return code
   in case of error

   parameters:
       fileName - name of the .esm/.esp file
       files    - vector that will hold the string table files
*/
int getAssociatedTableFiles(const std::string& fileName, std::vector<std::string>& files);

/** \brief Loads all associated string tables for an ESM / ESP file.
 *
 * \param esmFileName  path of the master / plugin file
 * \param table        the table to load the data into
 * \return Returns true in case of success.
 *         Returns false in case of failure.
 */
bool loadStringTables(const std::string& esmFileName, StringTable& table);

/** \brief Loads all associated string tables for an ESM / ESP file from the BSA.
 *
 * \param esmFileName  path of the master / plugin file
 * \param table        the table to load the data into
 * \return Returns true in case of success.
 *         Returns false in case of failure.
 */
bool loadStringTablesFromBSA(const std::string& esmFileName, StringTable& table);

} // namespace

#endif // SR_TABLEUTILITIES_HPP
