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

#ifndef SR_CRAFTABLEOBJECTRECORD_H
#define SR_CRAFTABLEOBJECTRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "CTDAData.h"
#include "ComponentData.h"

namespace SRTP
{

struct CraftableObjectRecord: public BasicRecord
{
  public:
    /* constructor */
    CraftableObjectRecord();

    /* destructor */
    virtual ~CraftableObjectRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CraftableObjectRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

       parameters:
           output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

       parameters:
           in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

    std::string editorID;
    uint32_t componentCount;
    std::vector<ComponentData> components;
    std::vector<CTDAData> unknownCTDAs;
    uint32_t unknownCNAM;
    uint32_t unknownBNAM;
    uint16_t resultCount;
}; //struct

} //namespace

#endif // SR_CRAFTABLEOBJECTRECORD_H
