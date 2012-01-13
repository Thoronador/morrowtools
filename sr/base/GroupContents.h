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

#ifndef SR_GROUPCONTENTS_H
#define SR_GROUPCONTENTS_H

#include <map>
#include <vector>
#include <stdint.h>
#include "records/BasicRecord.h"
#include "records/GroupData.h"

namespace SRTP
{

class Group;

class GroupContents
{
  public:
    /* constructor */
    GroupContents();

    /* destructor */
    ~GroupContents();

    /* adds another record at the end of the internal list

       parameters:
           rec - pointer to the new record. This pointer must NOT be freed by
                 the application, the class takes care of that. If rec is NULL,
                 the call won't add anything.
    */
    void addRecord(BasicRecord* rec);

    /* returns true, if a record with the given ID is present

       parameters:
           formID   - form ID of the record
           useIndex - if set to true, the function will use the index to find
                      a record (faster). If set to false, the function will
                      search record by record (slower, but definitely finds it,
                      if present).
    */
    bool hasRecord(const uint32_t formID, const bool useIndex = true) const;

    /* returns constant reference to the record with the requested form ID. If
       the requested record is not present, the function will throw an
       exception. Use hasRecord() to determine, whether the desired record is
       present.

       parameters:
           formID   - form ID of the record
           useIndex - if set to true, the function will use the index to find
                      a record (faster).
    */
    const BasicRecord& getRecord(const uint32_t formID, const bool useIndex = true) const;

    /* returns the number of records in the internal list */
    unsigned int getNumberOfRecords() const;

    /* removes all records from the internal list*/
    void removeRecords();

    /* recreates the internal index from scratch */
    void rebuildIndex();

    /* adds another sub group to the end of the sub group list and returns a
       reference to that group.

       parameters:
           g_data - the header data of the sub group
    */
    Group& addSubGroup(const GroupData& g_data);

    /* returns true, if a sub group with the given data is present

       parameters:
           g_data - the header data of the sub group
    */
    bool hasSubGroup(const GroupData& g_data) const;

    /* returns constant reference to the sub group with the requested group
       data. If the group is not present, the function will throw an exception.
       Use hasSubGroup() to determine, whether the desired sub group is
       present.

       parameters:
           g_data - the header data of the sub group
    */
    const Group& getSubGroup(const GroupData& g_data) const;

    /* returns constant reference to the sub group with the requested zero-based
       index. If the group is not present, the function will throw an exception.
       Use getNumberOfSubGroups() to determine how many sub groups are present.

       parameters:
           idx - the zero-based index of the sub group
    */
    const Group& getSubGroupAtIndex(const unsigned int idx) const;

    //non-constant variant of the above function
    Group* getSubGroupAtIndexNC(const unsigned int idx);

    /* returns the number of sub groups in the internal list */
    unsigned int getNumberOfSubGroups() const;

    /* removes all sub groups from the internal list*/
    void removeSubGroups();

    /* removes all contents, i.e. records and sub groups  */
    void removeContents();

    /* writes the group contents to the given output stream and returns true on
       success, false on failure

      parameters:
          output - the output file stream
    */
    bool saveToStream(std::ofstream& output) const;

    /* returns the size in bytes that the records included in that class would
       occupy, if written to a file stream
    */
    uint32_t getContentSize() const;
  private:
    //sub groups
    std::vector<Group> m_SubGroups;

    //internal record list
    std::vector<BasicRecord*> m_Records;

    //index - key is form ID, value is index of entry in m_Records
    std::map<uint32_t, uint32_t> m_Index;
}; //class

} //namespace

#endif // SR_GROUPCONTENTS_H
