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

#ifndef FACTIONS_H
#define FACTIONS_H

#include <string>
#include <map>
#include "records/FactionRecord.h"

namespace MWTP
{

//iterator type for faction list
typedef std::map<std::string, FactionRecord>::const_iterator FactionListIterator;

class Factions
{
  public:
    /* destructor */
    ~Factions();

    /* singleton access method */
    static Factions& getSingleton();

    /* adds a faction to the list */
    void addFaction(const FactionRecord& record);

    /* returns true, if a faction with the given ID is present

       parameters:
           ID - the ID of the faction
    */
    bool hasFaction(const std::string& ID) const;

    /* returns the number of factions in the list */
    unsigned int getNumberOfFactions() const;

    /* returns a reference to the faction record of the faction with the
       given ID

       parameters:
           ID - the ID of the faction

       remarks:
           If no faction with the given ID is present, the function will throw
           an exception. Use hasFaction() to determine, if a faction with the
           desired ID is present.
    */
    const FactionRecord& getFaction(const std::string& ID) const;

    /* tries to read a faction record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordFACT(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    FactionListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    FactionListIterator getEnd() const;

    /* tries to save all available factions to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    factions
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all factions from the list */
    void clearAll();
  private:
    /* constructor */
    Factions();

    /* empty copy constructor */
    Factions(const Factions& op) {}

    /* internal data */
    std::map<std::string, FactionRecord> m_Factions;
};//class

} //namespace

#endif // FACTIONS_H
