/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013, 2021  Thoronador

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

#ifndef SR_FORMIDFUNCTIONS_HPP
#define SR_FORMIDFUNCTIONS_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace SRTP
{

/** \brief Extracts the mod index from a form ID.
 *
 * \param fID  the form ID
 * \return Returns the form ID's mod index.
 */
uint8_t extractModIndex(const uint32_t fID);


/** \brief Extracts the object index from a form ID.
 *
 * \param fID  the form ID
 * \return Returns the object index part of the form ID.
 */
uint32_t extractObjectIndex(const uint32_t fID);

/** \brief Changes the mod index of a form ID to @newModIndex.
 *
 * \param fID  the form ID to adjust
 * \param  newModIndex  the new mod index to set
 * \remarks This function changes the mod index directly in the given form ID.
 *
 */
inline void changeModIndexInSitu(uint32_t& fID, const uint8_t newModIndex)
{
  fID = ((fID & 0x00FFFFFF) | (newModIndex << 24));
}

/** \brief Gets a string representation of the form ID.
 *
 * \param fID  the form ID
 * \return Returns the string representation of the form ID.
 */
std::string getFormIDAsString(uint32_t fID);

/** \brief Gets a string representation of the form ID with its mod index part replaced by 'xx' if it is not zero.
 *
 * \param fID  the form ID
 * \return Returns a string representation of the form ID. If the mod index of
 *         the form ID is non-zero, it will show 'xx' instead of the mod index
 *         part.
 */
std::string getFormIDAsStringXX(const uint32_t fID);

/** \brief Gets a string representation of the form ID including file information.
 *
 * \param fID         the form ID
 * \param loadOrder   list of master and plugin files in the order they were loaded
 * \param withFiles   if set to true, the file name is included in the result,
 *                    otherwise just the form ID is shown.
 * \return Returns a string representation of the form ID. If the mod index of
 *         the form ID is non-zero, it will show 'xx' instead of the mod index
 *         part.
 *         Additionally, it shows the file that the form ID came from, based on
 *         the mod index, if @withFiles is true.
 */
std::string getFormIDAsStringWithFile(const uint32_t fID, const std::vector<std::string>& loadOrder, const bool withFiles);

} // namespace

#endif // SR_FORMIDFUNCTIONS_HPP
