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

#ifndef ACTIVATORS_H
#define ACTIVATORS_H

#include <string>
#include <map>
#include "records/ActivatorRecord.h"
#include "ComparisonFunctor.h"

namespace MWTP
{

//iterator type for activator list
typedef std::map<std::string, ActivatorRecord, ci_less>::const_iterator ActivatorListIterator;

class Activators
{
  public:
    /* destructor */
    ~Activators();

    /* singleton access method */
    static Activators& getSingleton();

    /* adds an activator to the list */
    void addActivator(const ActivatorRecord& record);

    /* returns true, if an activator with the given ID is present

       parameters:
           ID - the ID of the activator object
    */
    bool hasActivator(const std::string& ID) const;

    /* returns the number of activators in the list */
    unsigned int getNumberOfActivators() const;

    /* returns a reference to the activator record of the activator with the
       given ID

       parameters:
           ID - the ID of the activator

       remarks:
           If no activator with the given ID is present, the function will throw
           an exception. Use hasActivator() to determine, if an activator with the
           desired ID is present.
    */
    const ActivatorRecord& getActivator(const std::string& ID) const;

    /* tries to read a activator record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordACTI(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ActivatorListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ActivatorListIterator getEnd() const;

    /* tries to save all available activators to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    activators
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all activators from the list */
    void clearAll();
  private:
    /* constructor */
    Activators();

    /* empty copy constructor */
    Activators(const Activators& op) {}

    /* internal data */
    std::map<std::string, ActivatorRecord, ci_less> m_Activators;
};//class

} //namespace

#endif // ACTIVATORS_H
