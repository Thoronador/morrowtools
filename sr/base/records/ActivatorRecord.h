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

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ActivatorRecord& other) const;
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
    virtual uint32_t getRecordType() const;

    struct destStruct
    {
      bool hasDSTD;
      uint8_t unknownDSTD[20];
      std::string destroyedModelPath;
      BinarySubRecord unknownDMDT;
      BinarySubRecord unknownDMDS;

      /* equality operator */
      bool operator==(const destStruct& other) const;

      /* resets/clears the internal values */
      void reset();
    }; //struct

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID;
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    bool hasDEST;
    uint8_t unknownDEST[8];
    std::vector<destStruct> destructionStructures;
    std::vector<uint32_t> keywordArray;
    //subrecord PNAM
    bool hasPNAM;
    uint8_t defaultPrimitiveColourRed;
    uint8_t defaultPrimitiveColourGreen;
    uint8_t defaultPrimitiveColourBlue;
    //end of subrecord PNAM
    uint32_t loopingSoundFormID; //subrecord SNAM
    uint32_t activateSoundFormID; //subrecord VNAM
    uint32_t waterTypeFormID; //subrecord WNAM
    bool hasRNAM;
    uint32_t unknownRNAM;
    bool hasFNAM;
    uint16_t unknownFNAM;
    uint32_t interactionKeywordFormID; //subrecord KNAM
}; //struct

} //namespace

#endif // SR_ACTIVATORRECORD_H
