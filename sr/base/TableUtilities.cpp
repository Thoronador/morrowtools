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

#include "TableUtilities.hpp"
#include <filesystem>
#include <iostream>
#include <set>
#include "../../base/DirectoryFunctions.hpp" // for path delimiter
#include "../../base/FileFunctions.hpp"      // for getDirectoryFileList()
#include "../../base/UtilityFunctions.hpp"   // for lowerCase()
#include "bsa/BSA.hpp"
#include "ReturnCodes.hpp"

namespace SRTP
{

int getLanguageComponent(const std::string& dataDir, const std::string& pluginName, std::string& languageComponent, std::vector<std::string>& stringTableFiles)
{
  const std::string lcPluginName = lowerCase(pluginName);
  const std::string::size_type piNameLength = pluginName.length();

  languageComponent = "";
  std::string part_path, part_name, part_ext;

  const auto files = getDirectoryFileList(dataDir + "Strings" + MWTP::pathDelimiter);
  if (!files.has_value() || files.value().size() < 3)
  {
    // Not enough files.
    return SRTP::rcFileError;
  }

  std::set<std::string> presentStuff;

  for (const auto& entry: files.value())
  {
    if ((!entry.isDirectory) && (lowerCase(entry.fileName.substr(0, piNameLength + 1)) == lcPluginName + "_"))
    {
      splitPathFileExtension(entry.fileName, MWTP::pathDelimiter, part_path, part_name, part_ext);
      if ((lowerCaseCompare(part_ext, "dlstrings") == 0) || (lowerCaseCompare(part_ext, "strings") == 0)
        || (lowerCaseCompare(part_ext, "ilstrings") == 0))
      {
        // Do we have a language component yet?
        if (languageComponent.empty())
        {
          languageComponent = part_name.substr(piNameLength + 1);
          presentStuff.insert(dataDir + "Strings" + MWTP::pathDelimiter + entry.fileName);
        }
        else
        {
          if (part_name.substr(piNameLength + 1) == languageComponent)
          {
            presentStuff.insert(dataDir + "Strings" + MWTP::pathDelimiter + entry.fileName);
          }
        }
      } // if string file extension
    } // if file name starts with plugin name
  }

  if (presentStuff.size() < 3)
  {
    std::cerr << "Error: Could not find all three string table files!\n";
    return SRTP::rcFileError;
  }

  if (presentStuff.size() > 3)
  {
    // Could possibly happen on file systems with case-insensitive file names.
    std::cerr << "Error: Found more than three string table files!\n";
    return SRTP::rcFileError;
  }

  stringTableFiles.clear();
  for (const auto& fn: presentStuff)
  {
    stringTableFiles.push_back(fn);
  }

  return 0;
}

int getAssociatedTableFiles(const std::string& fileName, std::vector<std::string>& files)
{
  std::string part_path, part_name, part_ext;
  splitPathFileExtension(fileName, MWTP::pathDelimiter, part_path, part_name, part_ext);

  int lc_return = getLanguageComponent(part_path, part_name, part_ext, files);
  // If return code is not zero, an error occurred! We should return in that case.
  if (lc_return != 0)
    return lc_return;
  else
    return 0;
}

bool loadStringTables(const std::string& esmFileName, StringTable& table, const std::optional<Localization>& l10n)
{
  std::vector<std::string> files;
  const int rc = getAssociatedTableFiles(esmFileName, files);
  if (rc != 0)
  {
    return loadStringTablesFromBSA(esmFileName, table, l10n);
  }
  for (const auto& fileName: files)
  {
    if (!table.readTable(fileName, StringTable::sdUnknown))
    {
      std::cerr << "Error while reading string tables for " << esmFileName << "!\n";
      return false;
    }
  }

  return true;
}

bool loadStringTablesFromBSA(const std::string& esmFileName, StringTable& table, const std::optional<Localization>& l10n)
{
  std::string part_path, part_name, part_ext;
  splitPathFileExtension(esmFileName, MWTP::pathDelimiter, part_path, part_name, part_ext);
  const std::string bsaFileName = part_path + "Skyrim - Interface.bsa";

  BSA bsa;
  if (!bsa.open(bsaFileName) || !bsa.grabAllStructureData())
    return false;
  part_name = lowerCase(part_name);

  const auto tempPath = std::filesystem::temp_directory_path();
  const std::string language = stringTableSuffix(l10n.value_or(Localization::German));

  for (const auto& extension: { ".strings", ".dlstrings", ".ilstrings" })
  {
    const auto fn = part_name + "_" + language + extension;
    const auto destination = (tempPath / fn).string();
    if (!bsa.extractFile("strings\\" + fn, destination))
      return false;
    const bool success = table.readTable(destination, StringTable::sdUnknown);
    // Delete extracted file.
    std::error_code error;
    std::filesystem::remove(destination, error);
    if (error)
      std::cerr << "Error: Failed to delete temporary file " << destination
                << ": " << error.message() << "\n";
    if (!success)
    {
      return false;
    }
  }

  return true;
}

} // namespace
