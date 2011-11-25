/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef DOORRECORD_H
#define DOORRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

namespace MWTP
{

struct DoorRecord: public BasicRecord
{
  public:
    std::string DoorID;
    std::string Name;
    std::string ModelPath;
    std::string Script;
    std::string SoundOpen;
    std::string SoundClose;

    /* constructor */
    DoorRecord();

    /* destructor */
    ~DoorRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const DoorRecord& other) const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);

  protected:
    /*tries to read a string subrecord and returns true on success

      parameters:
          in_File     - the input file stream
          Buffer      - pointer to a buffer of at least 256 bytes length
          BytesRead   - reference to a variable that counts the number of bytes
                        that were read from the stream
          Destination - the string that will hold the read string after success
    */
    bool readSubRecordString(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead, std::string& Destination);
};//struct

} //namespace

#endif // DOORRECORD_H