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

#ifndef SR_TEXTURESETRECORD_H
#define SR_TEXTURESETRECORD_H

#include <string>
#include "BasicRecord.h"

namespace SRTP
{

struct TextureSetRecord: public BasicRecord
{
  public:
    /* constructor */
    TextureSetRecord();

    /* destructor */
    virtual ~TextureSetRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const TextureSetRecord& other) const;

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
    uint8_t unknownOBND[12];
    std::string texture00;
    std::string texture01;
    std::string texture02;
    std::string texture03;
    std::string texture04;
    std::string texture05;
    std::string texture07;
    bool hasDODT;
    uint8_t unknownDODT[36];
    uint16_t unknownDNAM;
}; //struct

} //namespace

#endif // SR_TEXTURESETRECORD_H
