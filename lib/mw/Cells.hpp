/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011  Dirk Stolle

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

#ifndef MW_CELLS_HPP
#define MW_CELLS_HPP

#include <string>
#include <map>
#include "records/CellRecord.hpp"

namespace MWTP
{

struct CellRef
{
  int32_t X;
  int32_t Y;
  std::string Cell;

  /* constructor */
  CellRef(const CellRecord& rec);

  /* constructor */
  CellRef(const int32_t coordX, const int32_t coordY, const std::string& theCell);

  /* compare operator */
  bool operator<(const CellRef& rhs) const;

  /* function to ease output of CellRef */
  std::string toString() const;
};//struct

//iterator type for cell list
typedef std::map<CellRef, CellRecord>::const_iterator CellListIterator;

class Cells
{
  public:
    /* singleton access method */
    static Cells& get();

    /* adds a cell to the list */
    void addCell(const CellRecord& record);

    /* returns true, if a cell with the given ref is present

       parameters:
           ref - the reference for the cell
    */
    bool hasCell(const CellRef& ref) const;

    /* returns the number of cells in the list */
    unsigned int getNumberOfCells() const;

    /* returns a reference to the cell record of the cell with the
       given ref

       parameters:
           ref - the ref of the cell

       remarks:
           If no cell with the given params is present, the function will throw
           an exception. Use hasCell() to determine, if a cell with the
           desired ref is present.
    */
    const CellRecord& getCell(const CellRef& ref) const;

    /* tries to read a cell record from the given input stream.

       return value:
           If an error occurred, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           input  - the input stream that is used to read the record
    */
    int readRecordCELL(std::istream& input);

    /* returns constant iterator to the beginning of the internal list */
    CellListIterator begin() const;

    /* returns constant iterator to the end of the internal list */
    CellListIterator end() const;

    /* tries to save all available cells to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output stream that shall be used to save the cells
    */
    bool saveAllToStream(std::ostream& output) const;

    /* removes all cells from the list */
    void clear();
  private:
    /* constructor */
    Cells();

    /* empty copy constructor */
    Cells(const Cells& op) {}

    /* internal data */
    std::map<CellRef, CellRecord> m_Cells;
};//class

} //namespace

#endif // MW_CELLS_HPP
