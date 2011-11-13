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

#ifndef APPARATUSES_H
#define APPARATUSES_H

#include <string>
#include <map>
#include <fstream>
#include "records/ApparatusRecord.h"

namespace MWTP
{

//iterator type for apparatus list
typedef std::map<std::string, ApparatusRecord>::const_iterator ApparatusListIterator;

class Apparatuses
{
  public:
    /* destructor */
    ~Apparatuses();

    /* singleton access method */
    static Apparatuses& getSingleton();

    /* adds an apparatus to the list */
    void addApparatus(const ApparatusRecord& record);

    /* returns true, if an apparatus with the given ID is present

       parameters:
           ID - the ID of the apparatus object
    */
    bool hasApparatus(const std::string& ID) const;

    /* returns the number of apparatuses in the list */
    unsigned int getNumberOfApparatuses() const;

    /* returns a reference to the apparatus record of the apparatus with the
       given ID

       parameters:
           ID - the ID of the apparatus

       remarks:
           If no apparatus with the given ID is present, the function will throw
           an exception. Use hasApparatus() to determine, if an apparatus with the
           desired ID is present.
    */
    const ApparatusRecord& getApparatus(const std::string& ID) const;

    /* tries to read a apparatus record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordAPPA(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ApparatusListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ApparatusListIterator getEnd() const;

    /* tries to save all available apparatuses to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    apparatuses
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all apparatuses from the list */
    void clearAll();
  private:
    /* constructor */
    Apparatuses();

    /* empty copy constructor due to singleton pattern */
    Apparatuses(const Apparatuses& op) {}

    /* internal data */
    std::map<std::string, ApparatusRecord> m_Apparatuses;
};//class

} //namespace

#endif // APPARATUSES_H
