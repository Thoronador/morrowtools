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

#ifndef MW_BIRTHSIGNS_H
#define MW_BIRTHSIGNS_H

#include <string>
#include <set>
#include "records/BirthSignRecord.h"

namespace MWTP
{

//iterator type for birth sign list
typedef std::set<BirthSignRecord>::const_iterator BirthSignListIterator;

class BirthSigns
{
  public:
    /* destructor */
    ~BirthSigns();

    /* singleton access method */
    static BirthSigns& getSingleton();

    /* adds a birth sign to the list */
    void addBirthSign(const BirthSignRecord& record);

    /* returns true, if a birth sign with the given ID is present

       parameters:
           ID - the ID of the birth sign object
    */
    bool hasBirthSign(const std::string& ID) const;

    /* returns the number of birth signs in the list */
    unsigned int getNumberOfBirthSigns() const;

    /* returns a reference to the birth sign record of the birth sign with the
       given ID

       parameters:
           ID - the ID of the birth sign

       remarks:
           If no birth sign with the given ID is present, the function will
           throw an exception. Use hasBirthSign() to determine, if a birth sign
           with the desired ID is present.
    */
    const BirthSignRecord& getBirthSign(const std::string& ID) const;

    /* tries to read a birth sign record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordBSGN(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    BirthSignListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    BirthSignListIterator getEnd() const;

    /* tries to save all available birth signs to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    birth signs
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all birth signs from the list */
    void clearAll();
  private:
    /* constructor */
    BirthSigns();

    /* empty copy constructor */
    BirthSigns(const BirthSigns& op) {}

    /* internal data */
    std::set<BirthSignRecord> m_BirthSigns;
};//class

} //namespace

#endif // MW_BIRTHSIGNS_H
