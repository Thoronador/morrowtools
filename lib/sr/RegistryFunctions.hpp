/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2021, 2022  Dirk Stolle

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

#ifndef SR_REGISTRYFUNCTIONS_HPP
#define SR_REGISTRYFUNCTIONS_HPP

#include <optional>
#include <string>
#include "Edition.hpp"
#include "../base/RegistryFunctions.hpp"

namespace SRTP
{

/** Tries to read Skyrim's install path from the Windows registry.
 *
 * \param pathData  reference to a string that shall hold the read path
 * \param edition      the expected edition of Skyrim
 * \return In case of success, the function will return true and the retrieved
 *         path will be stored in the string referenced by pathData. In case of
 *         failure, the function will return false and the string referenced by
 *         pathData will not be changed.
 *
 * \remarks This function will always return false on non-Windows-OSes.
 */
inline bool getSkryrimPathFromRegistry(std::string& pathData, [[maybe_unused]] const std::optional<Edition> edition)
{
  #if defined(_WIN64)
  if (edition.has_value())
  {
    switch (edition.value())
    {
      case Edition::SpecialEdition:
           return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim Special Edition", "Installed Path");
      case Edition::Skyrim2011:
      default:
           return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim", "Installed Path");
    }
  }

  // No edition has been specified, so try all of them.
  // Try old Skyrim first, because that's the behaviour in earlier versions.
  if (MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim", "Installed Path"))
    return true;
  // Try Special Edition next.
  return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim Special Edition", "Installed Path");
  #elif defined(_WIN32)
  if (edition.has_value())
  {
    switch (edition.value())
    {
      case Edition::SpecialEdition:
           return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", "Installed Path");
      case Edition::Skyrim2011:
      default:
           return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Skyrim", "Installed Path");
    }
  }

  // No edition has been specified, so try all of them.
  // Try old Skyrim first, because that's the behaviour in earlier versions.
  if (MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Skyrim", "Installed Path"))
    return true;
  // Try Special Edition next.
  return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", "Installed Path");
  #else
  // Assume Win32-type registry for non-Windows systems. Does not really matter,
  // because there is no registry on non-Windows systems anyway.
  return MWTP::getRegistryStringValueHKLM(pathData, "SOFTWARE\\Bethesda Softworks\\Skyrim", "Installed Path");
  #endif // _WIN64 or _WIN32 or else
}

} // namespace

#endif // SR_REGISTRYFUNCTIONS_HPP
