/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_EYES_H
#define SR_EYES_H

#include <map>
#include <string>
#include "records/EyeRecord.h"

namespace SRTP
{

//iterator type for eye list
typedef std::map<std::string, EyeRecord>::const_iterator EyeListIterator;

class Eyes
{
  public:
    /* destructor */
    ~Eyes();

    /* singleton access method */
    static Eyes& getSingleton();

    /* adds a eye to the list */
    void addEye(const EyeRecord& record);

    /* returns true, if a eye with the given ID is present

       parameters:
           ID - the ID of the eye object
    */
    bool hasEye(const std::string& ID) const;

    /* returns the number of eyes in the list */
    unsigned int getNumberOfEyes() const;

    /* returns a reference to the eye record of the eye with the given ID

       parameters:
           ID - the ID of the eye

       remarks:
           If no eye with the given ID is present, the function will throw
           an exception. Use hasEye() to determine, if a eye with the
           desired ID is present.
    */
    const EyeRecord& getEye(const std::string& ID) const;

    /* tries to read a eye record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readNextRecord(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    EyeListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    EyeListIterator getEnd() const;

    /* tries to save all available eyes to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    eyes
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all eyes from the list */
    void clearAll();
  private:
    /* constructor */
    Eyes();

    /* empty copy constructor */
    Eyes(const Eyes& op) {}

    /* internal data */
    std::map<std::string, EyeRecord> m_Eyes;
};//class

} //namespace

#endif // SR_EYES_H
