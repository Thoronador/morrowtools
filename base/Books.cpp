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

#include "Books.h"
#include <iostream>

Books::Books()
{
  //empty
}

Books::~Books()
{
  //empty
}

Books& Books::getSingleton()
{
  static Books Instance;
  return Instance;
}

void Books::addBook(const BookRecord& record)
{
  if (record.BookID!="")
  {
    m_Books[record.BookID] = record;
  }
}

bool Books::hasBook(const std::string& ID) const
{
  return (m_Books.find(ID)!=m_Books.end());
}

unsigned int Books::getNumberOfBooks() const
{
  return m_Books.size();
}

const BookRecord& Books::getBook(const std::string& ID) const
{
  std::map<std::string, BookRecord>::const_iterator iter = m_Books.find(ID);
  if (iter!=m_Books.end())
  {
    return iter->second;
  }
  std::cout << "No book with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

BookListIterator Books::getBegin() const
{
  return m_Books.begin();
}

BookListIterator Books::getEnd() const
{
  return m_Books.end();
}

bool Books::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Books::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  BookListIterator iter = m_Books.begin();
  const BookListIterator end_iter = m_Books.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Books::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Books::clearAll()
{
  m_Books.clear();
}

int Books::readRecordBOOK(std::ifstream& in_File)
{
  BookRecord temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "readRecordBOOK: Error while reading book record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasBook(temp.BookID))
  {
    if (getBook(temp.BookID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if book present
  addBook(temp);
  return 1;
} //readRecordBOOK
