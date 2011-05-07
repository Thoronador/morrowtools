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

#ifndef CONTAINERS_H
#define CONTAINERS_H


#include <string>
#include <map>
#include "records/ContainerRecord.h"
#include "ComparisonFunctor.h"

//iterator type for container list
typedef std::map<std::string, ContainerRecord, ci_less>::const_iterator ContainerListIterator;

class Containers
{
  public:
    /* destructor */
    ~Containers();

    /* singleton access method */
    static Containers& getSingleton();

    /* adds a container to the list */
    void addContainer(const ContainerRecord& record);

    /* returns true, if a container with the given ID is present

       parameters:
           ID - the ID of the container object
    */
    bool hasContainer(const std::string& ID) const;

    /* returns the number of containers in the list */
    unsigned int getNumberOfContainers() const;

    /* returns a reference to the container record of the container with the
       given ID

       parameters:
           ID - the ID of the container

       remarks:
           If no container with the given ID is present, the function will throw
           an exception. Use hasContainer() to determine, if a container with the
           desired ID is present.
    */
    const ContainerRecord& getContainer(const std::string& ID) const;

    /* tries to read a container record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordCONT(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ContainerListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ContainerListIterator getEnd() const;

    /* tries to save all available containers to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    containers
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all containers from the list */
    void clearAll();
  private:
    /* constructor */
    Containers();

    /* empty and private copy constructor (singleton) */
    Containers(const Containers& op) {}

    /* internal data */
    std::map<std::string, ContainerRecord, ci_less> m_Containers;
};//class

#endif // CONTAINERS_H
