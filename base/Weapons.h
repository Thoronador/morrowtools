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

#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>
#include <map>
#include "records/WeaponRecord.h"

namespace MWTP
{

//iterator type for weapon list
typedef std::map<std::string, WeaponRecord>::const_iterator WeaponListIterator;

class Weapons
{
  public:
    /* destructor */
    ~Weapons();

    /* singleton access method */
    static Weapons& getSingleton();

    /* adds a weapon to the list */
    void addWeapon(const WeaponRecord& record);

    /* returns true, if a weapon with the given ID is present

       parameters:
           ID - the ID of the weapon object
    */
    bool hasWeapon(const std::string& ID) const;

    /* returns the number of weapons in the list */
    unsigned int getNumberOfWeapons() const;

    /* returns a reference to the weapon record of the weapon with the given ID

       parameters:
           ID - the ID of the weapon

       remarks:
           If no weapon with the given ID is present, the function will throw
           an exception. Use hasWeapon() to determine, if a weapon with the
           desired ID is present.
    */
    const WeaponRecord& getWeapon(const std::string& ID) const;

    /* tries to read a weapon record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordWEAP(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    WeaponListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    WeaponListIterator getEnd() const;

    /* tries to save all available weapons to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    weapons
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all weapons from the list */
    void clearAll();
  private:
    /* constructor */
    Weapons();

    /* empty copy constructor */
    Weapons(const Weapons& op) {}

    /* internal data */
    std::map<std::string, WeaponRecord> m_Weapons;
};//class

} //namespace

#endif // WEAPONS_H
