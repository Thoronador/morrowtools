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

#ifndef CLOTHINGS_H
#define CLOTHINGS_H

#include <string>
#include <map>
#include "records/ClothingRecord.h"
#include "ComparisonFunctor.h"

//iterator type for clothing list
typedef std::map<std::string, ClothingRecord>::const_iterator ClothingListIterator;

class Clothings
{
  public:
    /* destructor */
    ~Clothings();

    /* singleton access method */
    static Clothings& getSingleton();

    /* adds a clothing record to the list */
    void addClothing(const ClothingRecord& record);

    /* returns true, if a clothing with the given ID is present

       parameters:
           ID - the ID of the clothing object
    */
    bool hasClothing(const std::string& ID) const;

    /* returns the number of clothings in the list */
    unsigned int getNumberOfClothings() const;

    /* returns a reference to the clothing record of the clothing with the
       given ID

       parameters:
           ID - the ID of the clothing

       remarks:
           If no clothing with the given ID is present, the function will throw
           an exception. Use hasClothing() to determine, if a clothing with the
           desired ID is present.
    */
    const ClothingRecord& getClothing(const std::string& ID) const;

    /* tries to read a clothing record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordCLOT(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ClothingListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ClothingListIterator getEnd() const;

    /* tries to save all available clothings to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    clothings
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all clothings from the list */
    void clearAll();
  private:
    /* constructor */
    Clothings();

    /* empty copy constructor */
    Clothings(const Clothings& op) {}

    /* internal data */
    std::map<std::string, ClothingRecord, ci_less> m_Clothings;
};//class

#endif // CLOTHINGS_H