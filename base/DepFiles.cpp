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

/* minimalistic version of a function that converts letters in strings into
   their lower case equivalent. Due to the fact that it's only used to check
   file extensions of master and plugin files, we can limit the treated letters
   to E, P, S and M, because possible extensions are:
     .esm (master), .esp (plugin) and maybe .ess (save).
*/
std::string minimalistLowerCase(std::string str1)
{
  unsigned int i;
  for (i=0; i<str1.size(); ++i)
  {
    switch (str1.at(i))
    {
      case 'E':
           str1.at(i) = 'e';
           break;
      case 'S':
           str1.at(i) = 's';
           break;
      case 'M':
           str1.at(i) = 'm';
           break;
      case 'P':
           str1.at(i) = 'p';
           break;
    }//swi
  }//for
  return str1;
}

DepFile::DepFile()
{
  size = -1;
  name = "";
  modified = -1;
}

DepFile::DepFile(const std::string& fileName)
{
  name = fileName;
  size = -1;
  modified = -1;
}

DepFile::DepFile(const char* fileName)
{
  name = fileName;
  size = -1;
  modified = -1;
}

bool DepFile::isMasterFile() const
{
  const size_t len = name.length();
  if (len>3)
  {
    return (minimalistLowerCase(name.substr(len-4,4))==".esm");
  }
  //too short for .esm extension
  return false;
}

bool operator<(const DepFile& a, const DepFile& b)
{
  //master files go before plugin files
  if (a.isMasterFile()!=b.isMasterFile())
  {
    return a.isMasterFile();
  }
  //now files are either both master or both plugin file
  // So file time has to decide
  return a.modified<b.modified;
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

void DepFileList::sort()
{
  unsigned int i, j, smallest;
  DepFile temp;
  for (i=0; i<m_List.size(); ++i)
  {
    smallest = i;
    for (j=i+1; j<m_List.size(); ++j)
    {
      if (m_List.at(j)<m_List.at(smallest))
      {
        smallest = j;
      }//if
    }//for j
    //now swap, if neccessary
    if (smallest!=i)
    {
      temp = m_List.at(i);
      m_List.at(i) = m_List.at(smallest);
      m_List.at(smallest) = temp;
    }
  }//for i
}

unsigned int DepFileList::removeDuplicates()
{
  unsigned int i, ret;
  ret = 0;
  i = 0;
  while(i+1<m_List.size())
  {
    if (m_List.at(i).name==m_List.at(i+1).name)
    {
      m_List.erase(m_List.begin()+i);
      ++ret;
    }//if same name
    else
    {
      ++i;
    }
  }//while
  return ret;
}

void DepFileList::clear()
{
  m_List.clear();
}
