/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_COMBATSTYLERECORD_H
#define SR_COMBATSTYLERECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"

namespace SRTP
{

struct CombatStyleRecord: public BasicRecord
{
  public:
    /* constructor */
    CombatStyleRecord();

    /* destructor */
    virtual ~CombatStyleRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CombatStyleRecord& other) const;
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

    //enumeration type for length of CSGD subrecord
    enum CSGDLength {gdl8Bytes = 8, gdl32Bytes = 32, gdl40Bytes = 40};

    //enumeration type for length of CSCR subrecord
    enum CSCRLength {crl0Bytes = 0, crl8Bytes = 8, crl16Bytes = 16};

    std::string editorID;
    //subrecord CSGD
    CSGDLength csgdLen;
    float offensiveMult;
    float defensiveMult;
    float groupOffensiveMult;
    float melee; //Equipment Score Mult
    float magic; //Equipment Score Mult
    float ranged; //Equipment Score Mult
    float shout; //Equipment Score Mult
    float unarmed; //Equipment Score Mult
    float staff; //Equipment Score Mult
    float avoidThreatChance;
    //end of subrecord CSGD
    //subrecord CSMD
    bool hasCSMD;
    float unknownCSMDOne;
    float unknownCSMDTwo;
    //end of subrecord CSMD
    std::vector<float> unknownCSME;
    //subrecord CSCR
    CSCRLength cscrLen;
    float circleMult; //Close Range, Dueling
    float fallbackMult; //Close Range, Dueling
    float flankDistance; //Close Range, Flanking
    float stalkTime; //Close Range, Flanking
    //end of subrecord CSCR
    bool hasCSLR;
    float longRangeStrafeMult; //subrecord CSLR
    std::vector<float> unknownCSFL;
    bool hasDATA;
    uint32_t unknownDATA;
}; //struct

} //namespace

#endif // SR_COMBATSTYLERECORD_H
