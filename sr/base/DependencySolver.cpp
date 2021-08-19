/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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
#include "../../base/UtilityFunctions.hpp"
#include "../../base/ComparisonFunctor.hpp"
#include "ESMReader.hpp"

namespace SRTP
{

/* comparison operator */
bool DependencyElement::operator<(const DependencyElement& other) const
{
  return (lowerCaseCompare(name, other.name)<0);
}

/* function for traversing the dependency "graph" */
bool depPostOrderTraverse(const std::string& fileName, const std::set<DependencyElement>& files,
                          std::vector<std::string>& result, std::set<std::string, MWTP::ci_less>& presence)
{
  std::set<DependencyElement>::const_iterator iter = files.find(fileName);
  if (iter==files.end())
  {
    std::cerr << "Error: SRTP::dpot: no entry for \""<<fileName<<"\" found in file list!\n";
    return false;
  }
  //check for size limit
  if (presence.size()>255)
  {
    std::cerr << "Error: SRTP::dpot: limit of 255 entries exceeded!\n";
    return false;
  }
  //add child nodes
  std::vector<Tes4HeaderRecord::MasterFile>::const_iterator mf_iter = iter->header.dependencies.begin();
  while (mf_iter!=iter->header.dependencies.end())
  {
    if (presence.find(mf_iter->fileName)==presence.end())
    {
      //file not found, iterate over children
      if (!depPostOrderTraverse(mf_iter->fileName, files, result, presence))
        return false;
    }
    ++mf_iter;
  }//while
  //add file itself
  if (presence.find(fileName)==presence.end())
  {
    presence.insert(fileName);
    result.push_back(fileName);
  }
  return true;
}//function

bool getLoadOrder(const std::vector<std::string>& esmNames, const std::string& dataDir, std::vector<std::string>& result)
{
  std::set<SRTP::DependencyElement> filesWithHeaders;

  unsigned int i;
  for (i=0; i<esmNames.size(); ++i)
  {
    DependencyElement depElem;
    depElem.name = esmNames[i];
    if (!ESMReader::peekESMHeader(dataDir+esmNames[i], depElem.header))
    {
      std::cerr << "Error: could not read header of "<<dataDir+esmNames[i]<<"!\n";
      return false;
    }
    filesWithHeaders.insert(depElem);
  }//for
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
  std::set<DependencyElement>::const_iterator set_iter = files.begin();
  while (set_iter!=files.end())
  {
    if (!depPostOrderTraverse(set_iter->name, files, result, presence))
      return false;
    ++set_iter;
  }//while
  return true;
}//function getLoadOrder

} //namespace
