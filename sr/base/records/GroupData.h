/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef SR_GROUPDATA_H
#define SR_GROUPDATA_H

#include <fstream>
#include <stdint.h>

namespace SRTP
{

struct GroupData
{
  public:
    /* constructor */
    GroupData();

    /* destructor */
    virtual ~GroupData();

    /* writes the group data to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;

    /* loads the group data from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the group "name" */
    uint32_t getGroupLabel() const;

    /* returns the size of the group read from the stream */
    uint32_t getGroupSize() const;

    /* returns the type of the group read from the stream */
    uint32_t getGroupType() const;

    //type constants
    static const uint32_t cTopLevelGroup;
    static const uint32_t cCellChildren;
    static const uint32_t cTopicChildren;
    static const uint32_t cCellPersistentChildren;
    static const uint32_t cCellTemporaryChildren;

    /* sets a new group label

       parameters:
           newLabel - the new name value that shall be set
    */
    void setGroupLabel(const uint32_t newLabel);

    /* sets a new group size value

       parameters:
           newSize - the new size value that shall be set
    */
    void setGroupSize(const uint32_t newSize);

    /* sets a new group type value

       parameters:
           newType - the new type value that shall be set
    */
    void setGroupType(const uint32_t newType);

    /* equality operator */
    bool operator==(const GroupData& other) const;
  protected:
    uint32_t m_GroupSize;
    uint32_t m_GroupLabel;
    uint32_t m_GroupType;
    int32_t UnknownGroupDataTwo[2];
}; //struct

} //namespace

#endif // SR_GROUPDATA_H
