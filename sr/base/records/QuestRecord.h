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

#ifndef SR_QUESTRECORD_H
#define SR_QUESTRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct QuestRecord: public BasicRecord
{
  public:
    /* constructor */
    QuestRecord();

    /* destructor */
    virtual ~QuestRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const QuestRecord& other) const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

    /* type for holding quest stages / indices*/
    struct IndexEntry
    {
      uint32_t index; //stage index
      bool isFinisher; //true, if entry finishes the quest
      uint32_t CNAM; //CNAM - string ID of journal text?

      /* constructor */
      IndexEntry();

      /* equality operator */
      bool operator==(const IndexEntry& other) const;
    };//struct

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint32_t unknownFULL;
    uint8_t unknownDNAM[12];
    std::string filter;
    std::vector<IndexEntry> indices;
};//struct

} //namespace

#endif // SR_QUESTRECORD_H