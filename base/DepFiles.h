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

#ifndef DEPFILES_H
#define DEPFILES_H

#include <string>
#include <vector>

struct DepFile
{
  /* standard constructor */
  DepFile();

  /* constructor for file-only data */
  DepFile(const std::string& fileName);

  /* constructor for file-only data via old C string*/
  DepFile(const char* fileName);

  /* returns true, if the specified file name has the .esm extension */
  bool isMasterFile() const;

  /* data members */
  std::string name;
  int64_t size;
  time_t modified;
};//struct

//typedef std::vector<DepFile> DepFileList;

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
    const DepFile& at(size_t index) const;

    /* returns a reference to the index-th entry in the list

       parameters:
           index - zero-based index of the requested entry

       remarks:
           The value of index must not be less than zero, and is has to be
           less than getSize(). Otherwise the function will throw an exception.
           The function will always throw an exception for empty lists.
    */
    DepFile& at(size_t index);

    /* adds an entry to the end of the list */
    void push_back(const DepFile& val);

    /* adds an entry to the beginning of the list */
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
    bool removeEntry(size_t index);

    /* removes all files from the list */
    void clear();
  private:
    std::vector<DepFile> m_List;
}; //class

#endif // DEPFILES_H
