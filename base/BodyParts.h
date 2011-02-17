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

#ifndef BODYPARTS_H
#define BODYPARTS_H

#include <string>
#include <set>
#include "records/BodyPartRecord.h"

//iterator type for body part list
typedef std::set<BodyPartRecord>::const_iterator BodyPartListIterator;

class BodyParts
{
  public:
    /* destructor */
    ~BodyParts();

    /* singleton access method */
    static BodyParts& getSingleton();

    /* adds a body part to the list */
    void addBodyPart(const BodyPartRecord& record);

    /* returns true, if a body part with the given ID is present

       parameters:
           ID - the ID of the body part object
    */
    bool hasBodyPart(const std::string& ID) const;

    /* returns the number of body parts in the list */
    unsigned int getNumberOfBodyParts() const;

    /* returns a reference to the body part record of the landscape
       texture with the given ID

       parameters:
           ID - the ID of the body part

       remarks:
           If no body part with the given ID is present, the function
           will throw an exception. Use hasBodyPart() to determine, if
           a body part with the desired ID is present.
    */
    const BodyPartRecord& getBodyPart(const std::string& ID) const;

    /* tries to read a body part record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordBODY(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    BodyPartListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    BodyPartListIterator getEnd() const;

    /* tries to save all available body parts to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    body parts
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all body parts from the list */
    void clearAll();
  private:
    /* constructor */
    BodyParts();

    /* empty copy constructor */
    BodyParts(const BodyParts& op) {}

    /* internal data */
    std::set<BodyPartRecord> m_BodyParts;
};//class

#endif // BODYPARTS_H
