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

#ifndef SR_TES4HEADERRECORD_H
#define SR_TES4HEADERRECORD_H

#include "BasicRecord.h"
#include <vector>

namespace SRTP
{

struct Tes4HeaderRecord: public BasicRecord
{
  public:
    /* constructor */
    Tes4HeaderRecord();

    /* destructor */
    virtual ~Tes4HeaderRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const Tes4HeaderRecord& other) const;

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

    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* returns true, if the file is an ESM file, accodring to the set flags */
    bool isMasterFile() const;

    /* returns true, if the file is localized, accodring to the set flags */
    bool isLocalized() const;

    //flag constants
    static const uint32_t cMasterFlag    = 0x00000001;
    static const uint32_t cLocalizedFlag = 0x00000080;

    //structure for file dependencies
    struct MasterFile
    {
      std::string fileName;
      int64_t data;

      /* equality operator */
      bool operator==(const MasterFile& other) const;
    };//struct

    float version;
    uint32_t numRecordsAndGroups;
    uint32_t nextObjectID;
    std::string authorName;
    std::vector<MasterFile> dependencies;
    uint32_t unknownIntValue;
}; //struct

} //namespace

#endif // SR_TES4HEADERRECORD_H
