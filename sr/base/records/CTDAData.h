/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_CTDADATA_H
#define SR_CTDADATA_H

#include <fstream>
#include <cstring>
#include <string>
#include <stdint.h>
#include "../SR_Constants.h"

namespace SRTP
{

struct CTDAData
{
  uint8_t content[32];

  /* loads the CTDA data from the given input stream and returns true on success

     parameters:
         in_Stream - the input stream
         bytesRead - reference to the variable that counts the number of read bytes
  */
  bool loadFromStream(std::istream& in_Stream, uint32_t& bytesRead);

  #ifndef SR_UNSAVEABLE_RECORDS
  /* writes the CTDA data to the given output stream and returns true on success

     parameters:
         output - the output stream
  */
  bool saveToStream(std::ostream& output) const;
  #endif

  /* equality operator */
  bool operator==(const CTDAData& other) const;

  /* clears content */
  void clear();
}; //struct



template<uint32_t cisRecName>
struct CTDA_CISx_compound
{
  CTDAData unknownCTDA;
  std::string unknownCISx;

  /* constant to allow easy access to CISx 'type' */
  static const uint32_t cCISx;

  /* constructor */
  CTDA_CISx_compound();

  /* alternative constructor */
  CTDA_CISx_compound(const CTDAData& ctda, const std::string& cisx);

  /* equality operator */
  bool operator==(const CTDA_CISx_compound& other) const;

  #ifndef SR_UNSAVEABLE_RECORDS
  /* tries to save the CTDA_CIS2 compound to the given stream and returns true
     in case of success, false on failure

     parameters:
          output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* returns the size in bytes that the CTDA_CIS2 compound's data would occupy
     in a file stream
  */
  uint32_t getWriteSize() const;
  #endif
}; //struct

/* constant to allow easy access to CISx 'type' */
template<uint32_t cisRecName>
const uint32_t CTDA_CISx_compound<cisRecName>::cCISx = cisRecName;

template<uint32_t cisRecName>
CTDA_CISx_compound<cisRecName>::CTDA_CISx_compound()
: unknownCISx("")
{
  memset(unknownCTDA.content, 0, 32);
}

template<uint32_t cisRecName>
CTDA_CISx_compound<cisRecName>::CTDA_CISx_compound(const CTDAData& ctda, const std::string& cisx)
: unknownCTDA(ctda), unknownCISx(cisx)
{

}

template<uint32_t cisRecName>
bool CTDA_CISx_compound<cisRecName>::operator==(const CTDA_CISx_compound& other) const
{
  return ((unknownCTDA==other.unknownCTDA) and (unknownCISx==other.unknownCISx));
}

#ifndef SR_UNSAVEABLE_RECORDS
template<uint32_t cisRecName>
bool CTDA_CISx_compound<cisRecName>::saveToStream(std::ofstream& output) const
{
  if (!unknownCTDA.saveToStream(output)) return false;

  if (!unknownCISx.empty())
  {
    //write CISx
    output.write((const char*) &cCISx, 4);
    //CIS1's/CIS2's length
    const uint16_t subLength = unknownCISx.length()+1;
    output.write((const char*) &subLength, 2);
    //write CIS1's/CIS2's stuff
    output.write(unknownCISx.c_str(), subLength);
  }//if CIS1/CIS2

  return output.good();
}

template<uint32_t cisRecName>
uint32_t CTDA_CISx_compound<cisRecName>::getWriteSize() const
{
  uint32_t writeSize = 4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed length */;
  if (!unknownCISx.empty())
  {
    writeSize = writeSize +4 /* CIS1/CIS2 */ +2 /* 2 bytes for length */
               +unknownCISx.length()+1 /* length of string +1 byte for NUL termination */;
  }//if CIS2

  return writeSize;
}
#endif

typedef CTDA_CISx_compound<cCIS1> CTDA_CIS1_compound;
typedef CTDA_CISx_compound<cCIS2> CTDA_CIS2_compound;

} //namespace

#endif // SR_CTDADATA_H
