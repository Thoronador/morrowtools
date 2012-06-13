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

#ifndef MW_STARTSCRIPTS_H
#define MW_STARTSCRIPTS_H

#include <string>
#include <map>
#include "records/StartScriptRecord.h"

namespace MWTP
{

//iterator type for StartScript list
typedef std::map<std::string, StartScriptRecord>::const_iterator StartScriptListIterator;

class StartScripts
{
  public:
    /* destructor */
    ~StartScripts();

    /* singleton access method */
    static StartScripts& getSingleton();

    /* adds a StartScript to the list */
    void addStartScript(const StartScriptRecord& record);

    /* returns true, if a StartScript with the given ID is present

       parameters:
           ID - the ID of the StartScript object
    */
    bool hasStartScript(const std::string& ID) const;

    /* returns the number of StartScripts in the list */
    unsigned int getNumberOfStartScripts() const;

    /* returns a reference to the StartScript record of the StartScript with the
       given ID

       parameters:
           ID - the ID of the StartScript

       remarks:
           If no StartScript with the given ID is present, the function will throw
           an exception. Use hasStartScript() to determine, if a StartScript with the
           desired ID is present.
    */
    const StartScriptRecord& getStartScript(const std::string& ID) const;

    /* tries to read a StartScript record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordSSCR(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    StartScriptListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    StartScriptListIterator getEnd() const;

    /* tries to save all available StartScripts to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    StartScripts
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all StartScripts from the list */
    void clearAll();
  private:
    /* constructor */
    StartScripts();

    /* empty copy constructor */
    StartScripts(const StartScripts& op) {}

    /* internal data */
    std::map<std::string, StartScriptRecord> m_StartScripts;
};//class

} //namespace

#endif // MW_STARTSCRIPTS_H
