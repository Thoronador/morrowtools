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
#include <string>
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

    /* returns the size of the MODT subrecord */
    uint8_t getSizeOfMODT() const;

    /* returns the pointer to MODT's data */
    const uint8_t * getMODTPointer() const;

    std::string editorID;
    uint8_t unknownOBND[12];
    uint32_t unknownFULL;
    std::string modelPath;
  protected:
    uint8_t m_sizeOfMODT;
    uint8_t * m_pointerToMODT;
  public:
    uint32_t unknownEITM;
    uint16_t unknownEAMT;
    uint32_t unknownETYP;
    uint32_t unknownBIDS;
    uint32_t unknownBAMT;
    uint32_t unknownKSIZ;
    uint8_t  unknownKWDA[12];
    uint32_t unknownDESC;
    uint32_t unknownINAM;
    uint32_t unknownWNAM;
    uint32_t unknownTNAM;
    uint32_t unknownNAM9;
    uint32_t unknownNAM8;
    uint8_t  unknownDATA[10];
    uint8_t  unknownDNAM[100];
    uint8_t  unknownCRDT[16];
    uint32_t unknownVNAM;
}; //struct

} //namespace

#endif // SR_WEAPONRECORD_H
