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

#ifndef SR_ESMFILECONTENTS_H
#define SR_ESMFILECONTENTS_H

#include "Group.h"
#include <map>
#include <vector>
#include <stdint.h>

namespace SRTP
{

class ESMFileContents
{
  public:
    /* constructor */
    ESMFileContents();

    /* destructor */
    ~ESMFileContents();

    /* adds a new group to the internal list and returns a reference to that
       group
    */
    Group& addNewGroup();

    /* returns the number of records in the internal list */
    unsigned int getNumberOfGroups() const;

    /* removes all contents */
    void removeContents();
  //private:
    //internal group list
    std::vector<Group> m_Groups;

    /* determines the latest group according to the given group nesting level
       and returns a pointer to it. If NULL is returned, the level was invalid
       or no group exists.
     */
    Group * determineLatestGroup(const unsigned int level);
}; //class

} //namespace

#endif // SR_ESMFILECONTENTS_H
