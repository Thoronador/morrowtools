/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2021 Thoronador

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
  /** Constructs a new instance with the given file name.
   *
   * \param fileName  name of the file
   */
  DepFile(const std::string& fileName = "");

  /** Checks whether the file is a master file (ESM).
   *
   * \return Returns true, if the specified file name has the .esm extension.
   *         Returns false otherwise.
   */
  bool isMasterFile() const;

  bool operator==(const DepFile& other) const;

  std::string name; /**< name, e. g. "Morrowind.esm" */
  int64_t size;     /**< size of the file in bytes */
  time_t modified;  /**< time of last file modification */
}; // struct


class DepFileList
{
  public:
    /** Constructs an empty list. */
    DepFileList();

    /** Gets the number of elements within the list.
     *
     * \return Returns the number of elements within the list.
     */
    size_t getSize() const;

    /** Checks whether the list is empty.
     *
     * \return Returns true, if the list is empty. Returns false otherwise.
     */
    bool isEmpty() const;

    /** Gets a constant reference to the index-th entry in the list.
     *
     * \param index  zero-based index of the requested entry
     * \return Returns a constant reference to the index-th entry in the list.
     * \remarks The value of index must not be less than zero, and is has to be
     *          less than getSize(). Otherwise the function will throw an exception.
     * \throws Throws an exception, if the index is out of bounds.
     */
    const DepFile& at(const size_t index) const;

    /** Gets a reference to the index-th entry in the list.
     *
     * \param index  zero-based index of the requested entry
     * \return Returns a reference to the index-th entry in the list.
     * \remarks The value of index must not be less than zero, and is has to be
     *          less than getSize(). Otherwise the function will throw an exception.
     * \throws Throws an exception, if the index is out of bounds.
     */
    DepFile& at(const size_t index);

    /** Adds an entry to the end of the list.
     *
     * \param val  the entry that will be added to the end of the list
     */
    void push_back(const DepFile& val);

    /** Adds an entry to the beginning of the list.
     *
     * \param val  the entry that will be added to the beginning of the list
     */
    void push_front(const DepFile& val);

    /** Checks whether a file is in the list.
     *
     * \param fileName  name of the file to be searched for in the list
     * \return Returns true, if the file @fileName is in the list.
     */
    bool hasDepFile(const std::string& fileName) const;

    /** Writes the names of the files in the list to the standard output. */
    void writeDeps() const;

    /** Sorts the list of files - master files first, plugin files second. */
    void sort();

    /** Removes duplicate files from the list.
     *
     * \remark This only works properly, if the list has been sorted previous
     *         to the call of removeDuplicates(), i.e. by calling sort(), and
     *         the contents of the list haven't changed in between.
     * \return The function returns the number of elements that were removed.
     */
    unsigned int removeDuplicates();

    /** Removes an element at the specified index from the list.
     *
     * \param index  zero-based index of the element to remove
     * \return Returns true, if an element was removed - false otherwise.
     */
    bool removeEntry(const size_t index);

    /** Removes all files from the list. */
    void clear();

    bool operator==(const DepFileList& other) const;
  private:
    std::vector<DepFile> m_List;
}; // class

} // namespace

#endif // MW_DEPFILES_HPP
