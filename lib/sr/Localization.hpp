/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#ifndef SR_LOCALIZATION_HPP
#define SR_LOCALIZATION_HPP

#include <string>

namespace SRTP
{

/** Enumeration type for different localizations of Skyrim. */
enum class Localization
{
  /** English, available since original Skyrim of 2011 */
  English,

  /** French, available since original Skyrim of 2011 */
  French,

  /** German, available since original Skyrim of 2011 */
  German,

  /** Italian, available since original Skyrim of 2011 */
  Italian,

  /** Polish, available since Skyrim Special Edition */
  Polish,

  /** Russian, available since Skyrim Special Edition */
  Russian,

  /** Spanish, available since original Skyrim of 2011 */
  Spanish
};

/** \brief Gets a name suffix for localized string tables in BSA files.
 *
 * \param loc   the localization for which to get the suffi
 * \return Returns the matching suffix, e.g. the "english" in
 *         "skyrim_english.dlstrings" when selecting English.
 */
std::string stringTableSuffix(const Localization loc);

} // namespace

#endif // SR_LOCALIZATION_HPP
