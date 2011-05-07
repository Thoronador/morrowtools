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

#ifndef NPCS_H
#define NPCS_H

#include <string>
#include <map>
#include "records/NPCRecord.h"
#include "ComparisonFunctor.h"

//iterator type for NPC list
typedef std::map<std::string, NPCRecord, ci_less>::const_iterator NPCListIterator;

class NPCs
{
  public:
    /* destructor */
    ~NPCs();

    /* singleton access method */
    static NPCs& getSingleton();

    /* adds a NPC to the list */
    void addNPC(const NPCRecord& record);

    /* returns true, if a NPC with the given ID is present

       parameters:
           ID - the ID of the NPC object
    */
    bool hasNPC(const std::string& ID) const;

    /* returns the number of NPCs in the list */
    unsigned int getNumberOfNPCs() const;

    /* returns a reference to the NPC record of the NPC with the
       given ID

       parameters:
           ID - the ID of the NPC

       remarks:
           If no NPC with the given ID is present, the function will throw
           an exception. Use hasNPC() to determine, if a NPC with the
           desired ID is present.
    */
    const NPCRecord& getNPC(const std::string& ID) const;

    /* tries to read a NPC record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordNPC_(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    NPCListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    NPCListIterator getEnd() const;

    /* tries to save all available NPCs to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    NPCs
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all NPCs from the list */
    void clearAll();
  private:
    /* constructor */
    NPCs();

    /* empty copy constructor */
    NPCs(const NPCs& op) {}

    /* internal data */
    std::map<std::string, NPCRecord, ci_less> m_NPCs;
};//class

#endif // NPCS_H
