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

#ifndef SR_RELATIONSHIPRECORD_H
#define SR_RELATIONSHIPRECORD_H

#include <string>
#include "BasicRecord.h"

namespace SRTP
{

struct RelationshipRecord: public BasicRecord
{
  public:
    /* constructor */
    RelationshipRecord();

    /* destructor */
    virtual ~RelationshipRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const RelationshipRecord& other) const;
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

    /* relationship level constants */
    static const uint16_t cLover;
    static const uint16_t cAlly;
    static const uint16_t cConfidant;
    static const uint16_t cFriend;
    static const uint16_t cAcquaintance;
    static const uint16_t cRival;
    static const uint16_t cFoe;
    static const uint16_t cEnemy;
    static const uint16_t cArchnemesis;

    /* flag constant */
    static const uint16_t cFlagSecret;

    std::string editorID;
    //subrecord DATA
    uint32_t parentNPCFormID;
    uint32_t childNPCFormID;
    uint16_t relationshipLevel;
    uint16_t flags;
    uint32_t associationTypeFormID;
    //end of subrecord DATA

    /* returns true, if the secret flag is set */
    inline bool isSecret() const
    {
      return ((flags & cFlagSecret)!=0);
    }
}; //struct

} //namespace

#endif // SR_RELATIONSHIPRECORD_H
