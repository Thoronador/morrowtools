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

#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <map>
#include "records/LandscapeRecord.h"

struct LandscapeCoords
{
  int32_t X;
  int32_t Y;

  /* constructor */
  LandscapeCoords(const LandscapeRecord& rec);

  /* constructor */
  LandscapeCoords(const int32_t cellX, const int32_t cellY);

  /* comparison operator */
  bool operator<(const LandscapeCoords& rhs) const;
};//struct

//iterator type for landscape record list
typedef std::map<LandscapeCoords, LandscapeRecord>::const_iterator LandscapeListIterator;

class Landscape
{
  public:
    /* destructor */
    ~Landscape();

    /* singleton access method */
    static Landscape& getSingleton();

    /* adds a landscape record to the list */
    void addLandscapeRecord(const LandscapeRecord& record);

    /* returns true, if a landscape record for the given exterior cell is
       present

       parameters:
           coords - the coordinates of the cell
    */
    bool hasLandscapeRecord(const LandscapeCoords& coords) const;

    /* returns the number of landscape records in the list */
    unsigned int getNumberOfLandscapeRecords() const;

    /* returns a reference to the landscape record of the cell with the given
       coordinates
       given ID

       parameters:
           coords - the coordinates of the cell

       remarks:
           If no landscape record for the given cell is present, the function
           will throw an exception. Use hasLandscapeRecord() to determine, if a
           landscape record for a certain cell is present.
    */
    const LandscapeRecord& getLandscapeRecord(const LandscapeCoords& coords) const;

    /* tries to read a landscape record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordLAND(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    LandscapeListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    LandscapeListIterator getEnd() const;

    /* tries to save all available landscape records to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    landscape records
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all landscape records from the list */
    void clearAll();
  private:
    /* constructor */
    Landscape();

    /* empty copy constructor */
    Landscape(const Landscape& op) {}

    /* internal data */
    std::map<LandscapeCoords, LandscapeRecord> m_Landscape;
};//class

#endif // LANDSCAPE_H
