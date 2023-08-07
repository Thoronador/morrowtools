/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2021  Dirk Stolle

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

namespace SRTP
{

/** Holds contents of an ESM file. */
class ESMFileContents
{
  public:
    ESMFileContents();

    /** \brief Adds a new group to the internal list.
     *
     * \return Returns a reference to the new group.
     */
    Group& addNewGroup();

    /** \brief Gets the number of (top level) groups in the internal list.
     */
    unsigned int getNumberOfGroups() const;

    /** Removes all contents. */
    void removeContents();

    /** \brief Removes all empty groups.
     *
     * \return Returns the number of groups that were removed.
     */
    unsigned int purgeEmptyGroups();

    /// function type for following traverseGroups() function
    typedef bool (*traverseFunction) (const Group& current, const Group* parent);

    /** Traverses through all groups and subgroups within the file contents and
     *  calls func for each group, passing the current group in the first
     *  parameter and its parent group in the second parameter of func. Note
     *  that the later one will be NULL, if the current group is at top level.
     *
     * \param func  function that will be called for each group
    */
    void traverseGroups(traverseFunction func) const;
  //private:
    std::vector<Group> m_Groups; /**< internal group list */

    /** \brief Determines the latest group according to the given group nesting level.
     *
     * \param level   the one-based nesting level; top-level group is level one,
     *                the next sub-group is level one, its sub groups level 2,
     *                and so on.
     * \return Returns a pointer to the latest group. If nullptr is returned,
     *         the level was invalid or no group exists.
     */
    Group * determineLatestGroup(const unsigned int level);
  private:
    bool traverseSubGroups(const Group& grp, traverseFunction func) const;
}; // class

} // namespace

#endif // SR_ESMFILECONTENTS_HPP
