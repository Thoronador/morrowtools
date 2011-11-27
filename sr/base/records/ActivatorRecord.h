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

#ifndef SR_ACTIVATORRECORD_H
#define SR_ACTIVATORRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"

namespace SRTP
{

struct ActivatorRecord: public BasicRecord
{
  public:
    /* constructor */
    ActivatorRecord();

    /* destructor */
    virtual ~ActivatorRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const ActivatorRecord& other) const;

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

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t unknownFULL;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    bool hasDEST;
    uint8_t unknownDEST[8];
    bool hasDSTD;
    uint8_t unknownDSTD[20];
    std::string destroyedModelPath;
    BinarySubRecord unknownDMDT;
    BinarySubRecord unknownDMDS;
    BinarySubRecord unknownDSTF;
    uint32_t keywordSize;
    std::vector<uint32_t> keywordArray;
    uint32_t unknownPNAM;
    bool hasVNAM;
    uint32_t unknownVNAM;
    bool hasWNAM;
    uint32_t unknownWNAM;
    bool hasRNAM;
    uint32_t unknownRNAM;
    uint16_t unknownFNAM;
    bool hasKNAM;
    uint32_t unknownKNAM;
}; //struct

} //namespace

#endif // SR_ACTIVATORRECORD_H
