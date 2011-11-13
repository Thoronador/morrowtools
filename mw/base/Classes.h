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

#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <map>
#include "records/ClassRecord.h"

namespace MWTP
{

//iterator type for class list
typedef std::map<std::string, ClassRecord>::const_iterator ClassListIterator;

class Classes
{
  public:
    /* destructor */
    ~Classes();

    /* singleton access method */
    static Classes& getSingleton();

    /* adds a class to the list */
    void addClass(const ClassRecord& record);

    /* returns true, if a class with the given ID is present

       parameters:
           ID - the ID of the class
    */
    bool hasClass(const std::string& ID) const;

    /* returns the number of classes in the list */
    unsigned int getNumberOfClasses() const;

    /* returns a reference to the class record of the class with the given ID

       parameters:
           ID - the ID of the class

       remarks:
           If no class with the given ID is present, the function will throw
           an exception. Use hasClass() to determine, if a class with the
           desired ID is present.
    */
    const ClassRecord& getClass(const std::string& ID) const;

    /* tries to read a class record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordCLAS(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    ClassListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ClassListIterator getEnd() const;

    /* tries to save all available classes to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    classes
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all classes from the list */
    void clearAll();
  private:
    /* constructor */
    Classes();

    /* empty copy constructor */
    Classes(const Classes& op) {}

    /* internal data */
    std::map<std::string, ClassRecord> m_Classes;
};//class

} //namespace

#endif // CLASSES_H
