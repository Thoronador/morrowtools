/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_ESMFILECONTENTS_HPP
#define SR_ESMFILECONTENTS_HPP

#include "Group.hpp"
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

    /* returns the number of (top level) groups in the internal list */
    unsigned int getNumberOfGroups() const;

    /* removes all contents */
    void removeContents();

    /* removes all empty groups and returns the number of groups that were
       removed during the process
    */
    unsigned int purgeEmptyGroups();

    //function type for following traverseGroups() function
    typedef bool (*traverseFunction) (const Group& current, const Group* parent);

    /* traverses through all groups and subgroups within the file contents and
       calls func for each group, passing the current group in the first
       parameter and its parent group in the second parameter of func. Note
       that the later one will be NULL, if the current group is at top level.

       parameters:
           func - the function that will be called for each group
    */
    void traverseGroups(traverseFunction func) const;
  //private:
    //internal group list
    std::vector<Group> m_Groups;

    /* determines the latest group according to the given group nesting level
       and returns a pointer to it. If NULL is returned, the level was invalid
       or no group exists.
     */
    Group * determineLatestGroup(const unsigned int level);
  private:
    bool traverseSubGroups(const Group& grp, traverseFunction func) const;
}; //class

} //namespace

#endif // SR_ESMFILECONTENTS_HPP
