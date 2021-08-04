/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_GROUPDATA_HPP
#define SR_GROUPDATA_HPP

#include <cstdint>
#include <fstream>

namespace SRTP
{

struct GroupData
{
  public:
    /* constructor */
    GroupData();

    /* destructor */
    virtual ~GroupData();

    #ifndef SR_UNSAVEABLE_RECORDS
    /* writes the group data to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

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
    static const uint32_t cWorldspaceChildren;
    static const uint32_t cCellChildren;
    static const uint32_t cTopicChildren;
    static const uint32_t cCellPersistentChildren;
    static const uint32_t cCellTemporaryChildren;
    static const uint32_t cCellVisibleDistantChildren;

    /* returns true, if the group label is a cell form ID according to the
       group type
    */
    bool labelIsCellID() const;

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

#endif // SR_GROUPDATA_HPP
