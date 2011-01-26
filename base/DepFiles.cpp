/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "DepFiles.h"
#include <iostream>

DepFile::DepFile()
{
  size = -1;
  name = "";
}

DepFile::DepFile(const std::string& fileName)
{
  name = fileName;
  size = -1;
}

DepFile::DepFile(const char* fileName)
{
  name = fileName;
  size = -1;
}

// ******* DepFileList methods *******

DepFileList::DepFileList()
{
  //empty
}

DepFileList::~DepFileList()
{
  m_List.clear();
}

size_t DepFileList::getSize() const
{
  return m_List.size();
}

bool DepFileList::isEmpty() const
{
  return m_List.empty();
}

const DepFile& DepFileList::at(size_t index) const
{
  return m_List.at(index);
}

DepFile& DepFileList::at(size_t index)
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
  unsigned int i;
  for (i=0; i<m_List.size(); ++i)
  {
    if (m_List.at(i).name == fileName)
    {
      return true;
    }
  }//for
  //file not found, return false
  return false;
}

void DepFileList::writeDeps() const
{
  unsigned int i;
  for (i=0; i<m_List.size(); ++i)
  {
    std::cout << "  "<<m_List.at(i).name<<"\n";
  }//for
}

void DepFileList::clear()
{
  m_List.clear();
}
