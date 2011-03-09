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

#ifndef PROBES_H
#define PROBES_H

#include <string>
#include <set>
#include "records/ProbeRecord.h"

//iterator type for probe list
typedef std::set<ProbeRecord>::const_iterator ProbeListIterator;

class Probes
{
  public:
    /* destructor */
    ~Probes();

    /* singleton access method */
    static Probes& getSingleton();

    /* adds a probe to the list */
    void addProbe(const ProbeRecord& record);

    /* returns true, if a probe with the given ID is present

       parameters:
           ID - the ID of the probe object
    */
    bool hasProbe(const std::string& ID) const;

    /* returns the number of probes in the list */
    unsigned int getNumberOfProbes() const;

    /* returns a reference to the probe record of the probe with the
       given ID

       parameters:
           ID - the ID of the probe

       remarks:
           If no probe with the given ID is present, the function will throw
           an exception. Use hasProbe() to determine, if a probe with the
           desired ID is present.
    */
    const ProbeRecord& getProbe(const std::string& ID) const;

    /* tries to read a probe record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordPROB(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ProbeListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ProbeListIterator getEnd() const;

    /* tries to save all available probes to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    probes
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all probes from the list */
    void clearAll();
  private:
    /* constructor */
    Probes();

    /* empty copy constructor */
    Probes(const Probes& op) {}

    /* internal data */
    std::set<ProbeRecord> m_Probes;
};//class

#endif // PROBES_H
