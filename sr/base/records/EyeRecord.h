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

#ifndef SR_EYERECORD_H
#define SR_EYERECORD_H

#include "BasicRecord.h"
#include <string>
#include <stdint.h>

namespace SRTP
{

struct EyeRecord: public BasicRecord
{
  public:
    /* constructor */
    EyeRecord();

    /* destructor */
    virtual ~EyeRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const EyeRecord& other) const;

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

    std::string editorID;
    uint32_t unknownFULL;
    std::string iconPath;
    uint8_t unknownDATA;
}; //struct

} //namespace


#endif // SR_EYERECORD_H
