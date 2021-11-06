/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_DOORRECORD_HPP
#define MW_DOORRECORD_HPP

#include <string>
#include <fstream>
#include "BasicRecord.hpp"

namespace MWTP
{

struct DoorRecord: public BasicRecord
{
  public:
    std::string recordID; //formerly DoorID
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

    #ifndef MW_UNSAVEABLE_RECORDS
    /* writes the record to the given output stream and returns true on success

      parameters:
          output - the output stream
    */
    bool saveToStream(std::ostream& output) const override;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input stream
    */
    bool loadFromStream(std::istream& in_File) override;

  protected:
    /*tries to read a string subrecord and returns true on success

      parameters:
          in_File     - the input stream
          Buffer      - pointer to a buffer of at least 256 bytes length
          BytesRead   - reference to a variable that counts the number of bytes
                        that were read from the stream
          Destination - the string that will hold the read string after success
    */
    bool readSubRecordString(std::istream& in_File, char* Buffer, uint32_t& BytesRead, std::string& Destination);
};//struct

} //namespace

#endif // MW_DOORRECORD_HPP
