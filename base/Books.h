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

#ifndef BOOKS_H
#define BOOKS_H

#include <string>
#include <map>
#include "records/BookRecord.h"

//iterator type for book list
typedef std::map<std::string, BookRecord>::const_iterator BookListIterator;

class Books
{
  public:
    /* destructor */
    ~Books();

    /* singleton access method */
    static Books& getSingleton();

    /* adds a book to the list */
    void addBook(const BookRecord& record);

    /* returns true, if a book with the given ID is present

       parameters:
           ID - the ID of the book object
    */
    bool hasBook(const std::string& ID) const;

    /* returns the number of books in the list */
    unsigned int getNumberOfBooks() const;

    /* returns a reference to the book record of the book with the
       given ID

       parameters:
           ID - the ID of the book

       remarks:
           If no book with the given ID is present, the function will throw
           an exception. Use hasBook() to determine, if a book with the
           desired ID is present.
    */
    const BookRecord& getBook(const std::string& ID) const;

    /* tries to read a book record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordBOOK(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    BookListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    BookListIterator getEnd() const;

    /* tries to save all available books to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    books
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all books from the list */
    void clearAll();
  private:
    /* constructor */
    Books();

    /* empty copy constructor */
    Books(const Books& op) {}

    /* internal data */
    std::map<std::string, BookRecord> m_Books;
};//class

#endif // BOOKS_H
