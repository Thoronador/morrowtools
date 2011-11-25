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

#ifndef SR_GMSTRECORD_H
#define SR_GMSTRECORD_H

#include "BasicRecord.h"
#include <string>

namespace SRTP
{

enum GMSTType {gtInteger, gtFloat, gtString};

struct GMSTRecord: public BasicRecord
{
  public:
    /* constructor */
    GMSTRecord();

    /* destructor */
    virtual ~GMSTRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const GMSTRecord& other) const;

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

    /* returns the value read from the CNAM field */
    const std::string& getSettingName() const;

    /* returns the type of this setting */
    GMSTType getSettingType() const;

    /* get functions */
    const std::string& getString() const;
    float getFloat() const;
    int32_t getInteger() const;
  protected:
     std::string m_SettingName;
     int32_t m_IntValue;
     float m_FloatValue;
     std::string m_StringValue;
     GMSTType m_Type;
}; //struct

} //namespace

#endif // SR_GMSTRECORD_H