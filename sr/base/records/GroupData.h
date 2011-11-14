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
    int32_t getGroupName() const;

    /* returns the size of the group read from the stream */
    uint32_t getGroupSize() const;
  //protected: //TODO: should be protected later on, as soon as they are known
     int32_t UnknownGroupData[3];
  protected:
    uint32_t m_GroupSize;
    int32_t m_GroupName;
}; //struct

} //namespace

#endif // SR_GROUPDATA_H
