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

#ifndef ALCHEMYPOTIONS_H
#define ALCHEMYPOTIONS_H

#include <string>
#include <map>
#include "records/AlchemyPotionRecord.h"

namespace MWTP
{

//iterator type for potion list
typedef std::map<std::string, AlchemyPotionRecord>::const_iterator AlchemyPotionListIterator;

class AlchemyPotions
{
  public:
    /* destructor */
    ~AlchemyPotions();

    /* singleton access method */
    static AlchemyPotions& getSingleton();

    /* adds a potion to the list */
    void addAlchemyPotion(const AlchemyPotionRecord& record);

    /* returns true, if a potion with the given ID is present

       parameters:
           ID - the ID of the potion object
    */
    bool hasAlchemyPotion(const std::string& ID) const;

    /* returns the number of potions in the list */
    unsigned int getNumberOfAlchemyPotions() const;

    /* returns a reference to the potion record of the potion with the
       given ID

       parameters:
           ID - the ID of the potion

       remarks:
           If no potion with the given ID is present, the function will throw
           an exception. Use hasAlchemyPotion() to determine, if a potion with the
           desired ID is present.
    */
    const AlchemyPotionRecord& getAlchemyPotion(const std::string& ID) const;

    /* tries to read a potion record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordALCH(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    AlchemyPotionListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    AlchemyPotionListIterator getEnd() const;

    /* tries to save all available potions to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    potions
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all potions from the list */
    void clearAll();
  private:
    /* constructor */
    AlchemyPotions();

    /* empty copy constructor */
    AlchemyPotions(const AlchemyPotions& op) {}

    /* internal data */
    std::map<std::string, AlchemyPotionRecord> m_AlchemyPotions;
};//class

} //namespace

#endif // ALCHEMYPOTIONS_H
