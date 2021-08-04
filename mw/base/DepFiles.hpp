/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef MW_DEPFILES_HPP
#define MW_DEPFILES_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace MWTP
{

struct DepFile
{
  /* standard constructor */
  DepFile();

  /* constructor for file-only data

     parameters:
         fileName - name of the file
  */
  DepFile(const std::string& fileName);

  /* constructor for file-only data via old C string

     parameters:
         fileName - pointer to a character string containing the name of the
                    file
  */
  DepFile(const char* fileName);

  /* returns true, if the specified file name has the .esm extension */
  bool isMasterFile() const;

  /* equality operator */
  bool operator==(const DepFile& other) const;

  /* data members */
  std::string name;
  int64_t size;
  time_t modified;
};//struct


class DepFileList
{
  public:
    /* constructor */
    DepFileList();

    /* destructor */
    ~DepFileList();

    /* returns the number of elements within the list */
    size_t getSize() const;

    /* returns true, if there are no elements in the list */
    bool isEmpty() const;

    /* returns a constant reference to the index-th entry in the list

       parameters:
           index - zero-based index of the requested entry

       remarks:
           The value of index must not be less than zero, and is has to be
           less than getSize(). Otherwise the function will throw an exception.
           The function will always throw an exception for empty lists.
    */
    const DepFile& at(const size_t index) const;

    /* returns a reference to the index-th entry in the list

       parameters:
           index - zero-based index of the requested entry

       remarks:
           The value of index must not be less than zero, and is has to be
           less than getSize(). Otherwise the function will throw an exception.
           The function will always throw an exception for empty lists.
    */
    DepFile& at(const size_t index);

    /* adds an entry to the end of the list

       parameters:
           val - the entry that will be added to the end of the list
    */
    void push_back(const DepFile& val);

    /* adds an entry to the beginning of the list

       parameters:
           val - the entry that will be added to the beginning of the list
    */
    void push_front(const DepFile& val);

    /* returns true, if the file fileName is in the list

       parameters:
           fileName - name of the file to be searched for in the list
    */
    bool hasDepFile(const std::string& fileName) const;

    /* writes the names of the files in the list to the standard output */
    void writeDeps() const;

    /* sorts the list of files - master files first, plugin files second */
    void sort();

    /* removes duplicate files from the list. This only works properly, if the
       list has been sorted previous to the call of removeDuplicates(), i.e. by
       calling sort(), and the contents of the list haven't changed in between.
       The function returns the number of elements that have been removed.
    */
    unsigned int removeDuplicates();

    /* removes an element at the specified index from the list and returns
       true, if an element was removed - false otherwise

       parameters:
           index - zero-based index of the element
    */
    bool removeEntry(const size_t index);

    /* removes all files from the list */
    void clear();

    /* equality operator */
    bool operator==(const DepFileList& other) const;
  private:
    std::vector<DepFile> m_List;
}; //class

} //namespace

#endif // MW_DEPFILES_HPP
