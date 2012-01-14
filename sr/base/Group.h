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

#ifndef SR_GROUP_H
#define SR_GROUP_H

#include "records/GroupData.h"
#include "GroupContents.h"

namespace SRTP
{

class Group
{
  public:
    /* constructor */
    Group();

    /* destructor */
    ~Group();

    //the group's data
    GroupData headerData;
    GroupContents contents;

    /* writes the group to the given output stream and returns true on success

      parameters:
          output - the output file stream
    */
    bool saveToStream(std::ofstream& output) const;

    /* sets the group's size value in the group header to match the actual size
       of the group and its records */
    void updateGroupSize();

    /* determines the latest group according to the given group nesting level
       and returns a pointer to it. If NULL is returned, the level was invalid
       or no group exists.
     */
    Group * determineLatestGroup(const unsigned int level);
}; //class

} //namespace

#endif // SR_GROUP_H
