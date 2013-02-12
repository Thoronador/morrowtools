/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_IDLEANIMATIONRECORD_H
#define SR_IDLEANIMATIONRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "CTDAData.h"

namespace SRTP
{

struct IdleAnimationRecord: public BasicRecord
{
  public:
    /* constructor */
    IdleAnimationRecord();

    /* destructor */
    virtual ~IdleAnimationRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const IdleAnimationRecord& other) const;
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

    /* flag constants */
    static const uint8_t cFlagSequence;
    static const uint8_t cFlagNotAttacking;
    static const uint8_t cFlagBlocking;

    std::string editorID;
    std::vector<CTDA_CIS1_compound> conditions; //subrecords CTDA + CIS1
    std::string unknownDNAM;
    std::string animEventName; //subrecord ENAM
    //subrecord ANAM
    uint32_t parentFormID;
    uint32_t previousFormID;
    //end of subrecord ANAM
    //subrecord DATA
    uint8_t minLoopingSeconds;
    uint8_t maxLoopingSeconds;
    uint8_t flags;
    uint8_t unknownDATAFour;
    uint16_t replayDelay;
    //end of subrecord DATA

    /* returns true, if the Sequence flag is set */
    inline bool isSequence() const
    {
      return ((flags & cFlagSequence)!=0);
    }

    /* returns true, if the 'Not attacking' flag is set */
    inline bool notAttacking() const
    {
      return ((flags & cFlagNotAttacking)!=0);
    }

    /* returns true, if the 'Blocking' flag is set */
    inline bool isBlocking() const
    {
      return ((flags & cFlagBlocking)!=0);
    }

    /* returns true, if the loop forever state is set */
    inline bool loopForever() const
    {
      return ((minLoopingSeconds==255) and (maxLoopingSeconds==255));
    }
}; //struct

} //namespace

#endif // SR_IDLEANIMATIONRECORD_H
