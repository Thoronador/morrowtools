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

#ifndef SR_TES4HEADERRECORD_H
#define SR_TES4HEADERRECORD_H

#include "BasicRecord.h"

namespace SRTP
{

struct Tes4HeaderRecord: public BasicRecord
{
  public:
    /* constructor */
    Tes4HeaderRecord();

    /* destructor */
    virtual ~Tes4HeaderRecord();

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

    /* returns the value read from the CNAM field */
    const std::string& getName() const;

    /* returns the internal integer value */
    uint32_t getIntValue() const;

  //protected:
     float version;
     uint32_t HeaderUnknownTwo[3];
  protected:
     std::string m_Name;
     uint32_t m_IntValue;
}; //struct

} //namespace

#endif // SR_TES4HEADERRECORD_H
