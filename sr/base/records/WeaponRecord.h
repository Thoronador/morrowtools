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

#ifndef SR_WEAPONRECORD_H
#define SR_WEAPONRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct WeaponRecord: public BasicRecord
{
  public:
    /* constructor */
    WeaponRecord();

    /* copy constructor */
    WeaponRecord(const WeaponRecord& other);

    /* assignment operator */
    WeaponRecord& operator=(const WeaponRecord& other);

    /* destructor */
    virtual ~WeaponRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const WeaponRecord& other) const;

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
    uint32_t nameStringID; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    bool hasEITM;
    uint32_t unknownEITM;
    bool hasEAMT;
    uint16_t unknownEAMT;
    bool hasETYP;
    uint32_t unknownETYP;
    bool hasBIDS;
    uint32_t unknownBIDS;
    bool hasBAMT;
    uint32_t unknownBAMT;
    std::vector<uint32_t> keywordArray;
    uint32_t descriptionStringID; //subrecord DESC
    std::string unknownNNAM;
    bool hasINAM;
    uint32_t unknownINAM;
    bool hasWNAM;
    uint32_t unknownWNAM;
    bool hasTNAM;
    uint32_t unknownTNAM;
    bool hasUNAM;
    uint32_t unknownUNAM;
    bool hasNAM9;
    uint32_t unknownNAM9;
    bool hasNAM8;
    uint32_t unknownNAM8;
    bool hasSNAM;
    uint32_t unknownSNAM;
    uint8_t  unknownDATA[10];
    uint8_t  unknownDNAM[100];
    uint8_t  unknownCRDT[16];
    uint32_t unknownVNAM;
    bool hasCNAM;
    uint32_t unknownCNAM;
}; //struct

} //namespace

#endif // SR_WEAPONRECORD_H
