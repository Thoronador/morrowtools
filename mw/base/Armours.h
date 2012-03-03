/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef MW_ARMOURS_H
#define MW_ARMOURS_H

#include <string>
#include <map>
#include "records/ArmourRecord.h"
#include "../../base/ComparisonFunctor.h"

namespace MWTP
{

//iterator type for armour list
typedef std::map<std::string, ArmourRecord, ci_less>::const_iterator ArmourListIterator;

class Armours
{
  public:
    /* destructor */
    ~Armours();

    /* singleton access method */
    static Armours& getSingleton();

    /* adds an armour to the list */
    void addArmour(const ArmourRecord& record);

    /* returns true, if an armour with the given ID is present

       parameters:
           ID - the ID of the armour object
    */
    bool hasArmour(const std::string& ID) const;

    /* returns the number of armours in the list */
    unsigned int getNumberOfArmours() const;

    /* returns a reference to the armour record of the armour with the
       given ID

       parameters:
           ID - the ID of the armour

       remarks:
           If no armour with the given ID is present, the function will throw
           an exception. Use hasArmour() to determine, if an armour with the
           desired ID is present.
    */
    const ArmourRecord& getArmour(const std::string& ID) const;

    /* tries to read a armour record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordARMO(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ArmourListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ArmourListIterator getEnd() const;

    /* tries to save all available armours to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    armours
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all armours from the list */
    void clearAll();
  private:
    /* constructor */
    Armours();

    /* empty copy constructor */
    Armours(const Armours& op) {}

    /* internal data */
    std::map<std::string, ArmourRecord, ci_less> m_Armours;
};//class

} //namespace

#endif // MW_ARMOURS_H
