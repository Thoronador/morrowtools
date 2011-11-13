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

#ifndef ENCHANTINGS_H
#define ENCHANTINGS_H

#include <string>
#include <map>
#include "records/EnchantingRecord.h"

namespace MWTP
{

//iterator type for enchanting list
typedef std::map<std::string, EnchantingRecord>::const_iterator EnchantingListIterator;

class Enchantings
{
  public:
    /* destructor */
    ~Enchantings();

    /* singleton access method */
    static Enchantings& getSingleton();

    /* adds an enchanting to the list */
    void addEnchanting(const EnchantingRecord& record);

    /* returns true, if an enchanting with the given ID is present

       parameters:
           ID - the ID of the enchanting object
    */
    bool hasEnchanting(const std::string& ID) const;

    /* returns the number of enchantings in the list */
    unsigned int getNumberOfEnchantings() const;

    /* returns a reference to the enchanting record of the enchanting with the
       given ID

       parameters:
           ID - the ID of the enchanting

       remarks:
           If no enchanting with the given ID is present, the function will throw
           an exception. Use hasEnchanting() to determine, if an enchanting with the
           desired ID is present.
    */
    const EnchantingRecord& getEnchanting(const std::string& ID) const;

    /* tries to read a enchanting record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordENCH(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    EnchantingListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    EnchantingListIterator getEnd() const;

    /* tries to save all available enchantings to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    enchantings
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all enchanting effects from the list */
    void clearAll();
  private:
    /* constructor */
    Enchantings();

    /* empty, private copy constructor due to singleton pattern */
    Enchantings(const Enchantings& op) {}

    /* internal data */
    std::map<std::string, EnchantingRecord> m_Enchantings;
};//class

} //namespace

#endif // ENCHANTINGS_H
