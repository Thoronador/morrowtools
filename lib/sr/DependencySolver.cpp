/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2021, 2023  Dirk Stolle

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

#include "DependencySolver.hpp"
#include <iostream>
#include "../base/UtilityFunctions.hpp"
#include "../base/ComparisonFunctor.hpp"
#include "ESMReader.hpp"

namespace SRTP
{

DependencyElement::DependencyElement()
: name(""), header(Tes4HeaderRecord())
{
}

DependencyElement::DependencyElement(const std::string& file_name)
: name(file_name), header(Tes4HeaderRecord())
{
}

DependencyElement::DependencyElement(const std::string& file_name, const Tes4HeaderRecord& head)
: name(file_name), header(head)
{
}

/* comparison operator */
bool DependencyElement::operator<(const DependencyElement& other) const
{
  return lowerCaseCompare(name, other.name) < 0;
}

/* function for traversing the dependency "graph" */
bool depPostOrderTraverse(const std::string& fileName, const std::set<DependencyElement>& files,
                          std::vector<std::string>& result, std::set<std::string, MWTP::ci_less>& presence)
{
  const std::set<DependencyElement>::const_iterator iter = files.find(fileName);
  if (iter == files.end())
  {
    std::cerr << "Error: SRTP::dpot: No entry for \"" << fileName << "\" found in file list!\n";
    return false;
  }
  // check for size limit
  if (presence.size() > 255)
  {
    std::cerr << "Error: SRTP::dpot: Limit of 255 entries exceeded!\n";
    return false;
  }
  // add child nodes
  for (const auto& master_file: iter->header.dependencies)
  {
    if (presence.find(master_file.fileName) == presence.end())
    {
      // file not found, iterate over children
      if (!depPostOrderTraverse(master_file.fileName, files, result, presence))
        return false;
    }
  }
  // add file itself
  if (presence.find(fileName) == presence.end())
  {
    presence.insert(fileName);
    result.push_back(fileName);
  }
  return true;
}

bool getLoadOrder(const std::vector<std::string>& esmNames, const std::string& dataDir, std::vector<std::string>& result)
{
  std::set<SRTP::DependencyElement> filesWithHeaders;

  for (const auto& esm_name: esmNames)
  {
    DependencyElement depElem;
    depElem.name = esm_name;
    if (!ESMReader::peekESMHeader(dataDir + esm_name, depElem.header))
    {
      std::cerr << "Error: Could not read header of " << dataDir + esm_name << "!\n";
      return false;
    }
    filesWithHeaders.insert(depElem);
  }
  return getLoadOrder(filesWithHeaders, result);
}

bool getLoadOrder(const std::set<DependencyElement>& files, std::vector<std::string>& result)
{
  result.clear();
  if (files.empty())
  {
    return true;
  }

  std::set<std::string, MWTP::ci_less> presence;
  for (const auto& dependency: files)
  {
    if (!depPostOrderTraverse(dependency.name, files, result, presence))
      return false;
  }
  return true;
}

} // namespace
