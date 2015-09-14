/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef MW_PATHGRIDS_H
#define MW_PATHGRIDS_H

#include <string>
#include <map>
#include "records/PathGridRecord.h"

namespace MWTP
{

struct PathGridRef
{
  int32_t X;
  int32_t Y;
  std::string Cell;

  /* constructor */
  PathGridRef(const PathGridRecord& rec);

  /* constructor */
  PathGridRef(const int32_t coordX, const int32_t coordY, const std::string& theCell);

  /* compare operator */
  bool operator<(const PathGridRef& rhs) const;

  /* function to ease output of PathGridRef */
  std::string toString() const;
};//struc

//iterator type for path grid list
typedef std::map<PathGridRef, PathGridRecord>::const_iterator PathGridListIterator;

class PathGrids
{
  public:
    /* destructor */
    ~PathGrids();

    /* singleton access method */
    static PathGrids& getSingleton();

    /* adds a path grid to the list */
    void addPathGrid(const PathGridRecord& record);

    /* returns true, if a path grid with the given ref is present

       parameters:
           ref - the reference for the path grid
    */
    bool hasPathGrid(const PathGridRef& ref) const;

    /* returns the number of path grids in the list */
    unsigned int getNumberOfPathGrids() const;

    /* returns a reference to the path grid record of the path grid with the
       given ref

       parameters:
           ref - the ref of the path grid

       remarks:
           If no path grid with the given parameters is present, the function
           will throw an exception. Use hasPathGrid() to determine, if a path
           grid with the desired ref is present.
    */
    const PathGridRecord& getPathGrid(const PathGridRef& ref) const;

    /* tries to read a path grid record from the given input file stream.

       return value:
           If an error occurred, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordPGRD(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    PathGridListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    PathGridListIterator getEnd() const;

    /* tries to save all available path grids to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    path grids
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all path grids from the list */
    void clearAll();
  private:
    /* constructor */
    PathGrids();

    /* empty copy constructor */
    PathGrids(const PathGrids& op) {}

    /* internal data */
    std::map<PathGridRef, PathGridRecord> m_PathGrids;
};//class

} //namespace

#endif // MW_PATHGRIDS_H
