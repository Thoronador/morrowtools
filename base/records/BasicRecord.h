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

#ifndef BASICRECORD_H
#define BASICRECORD_H

#include <fstream>

struct BasicRecord
{
  public:
    /* constructor */
    BasicRecord();

    /* destructor */
    virtual ~BasicRecord();

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const = 0;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File) = 0;

    //returns the first header part of this record
    int32_t getHeaderOne() const;

    //returns the header flags of this record
    int32_t getHeaderFlags() const;

    //returns true, if the blocked flag for this record is set
    bool isBlocked() const;

    //returns true, if the "References Persist" flag for this record is set
    bool isPersistent() const;
  protected:
     int32_t HeaderOne;
     int32_t HeaderFlags;
}; //struct

#endif // BASICRECORD_H
