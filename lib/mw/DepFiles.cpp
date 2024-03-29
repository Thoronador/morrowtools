/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "DepFiles.hpp"
#include <iostream>

namespace MWTP
{

/* minimalistic version of a function that converts letters in strings into
   their lower case equivalent. Due to the fact that it's only used to check
   file extensions of master and plugin files, we can limit the treated letters
   to E, P, S and M, because possible extensions are:
     .esm (master), .esp (plugin) and maybe .ess (save).
*/
std::string minimalistLowerCase(std::string str1)
{
  for (auto& c: str1)
  {
    switch (c)
    {
      case 'E':
           c = 'e';
           break;
      case 'S':
           c = 's';
           break;
      case 'M':
           c = 'm';
           break;
      case 'P':
           c = 'p';
           break;
    }
  }
  return str1;
}

DepFile::DepFile(const std::string& fileName)
: name(fileName),
  size(-1),
  modified(-1)
{
}

bool DepFile::isMasterFile() const
{
  const auto len = name.length();
  if (len > 3)
  {
    return minimalistLowerCase(name.substr(len -4, 4)) == ".esm";
  }
  // too short for .esm extension
  return false;
}

bool DepFile::operator==(const DepFile& other) const
{
  return (name == other.name) && (size == other.size) && (modified == other.modified);
}

bool operator<(const DepFile& a, const DepFile& b)
{
  // Master files go before plugin files.
  const bool a_is_esm = a.isMasterFile();
  if (a_is_esm != b.isMasterFile())
  {
    return a_is_esm;
  }
  // Now files are either both master or both plugin file.
  // So file time has to decide.
  return a.modified < b.modified;
}


// ******* DepFileList methods *******

DepFileList::DepFileList()
: m_List(std::vector<DepFile> ())
{
}

size_t DepFileList::getSize() const
{
  return m_List.size();
}

bool DepFileList::isEmpty() const
{
  return m_List.empty();
}

const DepFile& DepFileList::at(const size_t index) const
{
  return m_List.at(index);
}

DepFile& DepFileList::at(const size_t index)
{
  return m_List.at(index);
}

void DepFileList::push_back(const DepFile& val)
{
  m_List.push_back(val);
}

void DepFileList::push_front(const DepFile& val)
{
  m_List.insert(m_List.begin(), val);
}

bool DepFileList::hasDepFile(const std::string& fileName) const
{
  for (const auto& element: m_List)
  {
    if (element.name == fileName)
    {
      return true;
    }
  }
  // file not found
  return false;
}

void DepFileList::writeDeps() const
{
  for (const auto& element: m_List)
  {
    std::cout << "  " << element.name << "\n";
  }
}

void DepFileList::sort()
{
  decltype(m_List)::size_type smallest;
  DepFile temp;
  for (decltype(smallest) i = 0; i < m_List.size(); ++i)
  {
    smallest = i;
    for (decltype(i) j = i + 1; j < m_List.size(); ++j)
    {
      if (m_List[j] < m_List[smallest])
      {
        smallest = j;
      }
    } // for j
    // now swap, if necessary
    if (smallest != i)
    {
      temp = m_List[i];
      m_List[i] = m_List[smallest];
      m_List[smallest] = temp;
    }
  } // for i
}

unsigned int DepFileList::removeDuplicates()
{
  unsigned int i = 0;
  unsigned int ret = 0;
  while(i + 1 < m_List.size())
  {
    if (m_List[i].name == m_List[i+1].name)
    {
      m_List.erase(m_List.begin() + i);
      ++ret;
    }
    else
    {
      ++i;
    }
  }
  return ret;
}

bool DepFileList::removeEntry(const size_t index)
{
  if (index >= m_List.size())
  {
    // index is out of range, we can't remove an entry
    return false;
  }
  m_List.erase(m_List.begin() + index);
  return true;
}

void DepFileList::clear()
{
  m_List.clear();
}

bool DepFileList::operator==(const DepFileList& other) const
{
  return m_List == other.m_List;
}

} // namespace
