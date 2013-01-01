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

#ifndef SR_CTDADATA_H
#define SR_CTDADATA_H

#include <fstream>
#include <string>
#include <stdint.h>

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

struct CTDA_CIS2_compound
{
  CTDAData unknownCTDA;
  std::string unknownCIS2;

  /* constructor */
  CTDA_CIS2_compound();

  /* alternative constructor */
  CTDA_CIS2_compound(const CTDAData& ctda, const std::string& cis2);

  /* equality operator */
  bool operator==(const CTDA_CIS2_compound& other) const;

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

struct CTDA_CIS1_compound
{
  CTDAData unknownCTDA;
  std::string unknownCIS1;

  /* constructor */
  CTDA_CIS1_compound();

  /* alternative constructor */
  CTDA_CIS1_compound(const CTDAData& ctda, const std::string& cis1);

  /* equality operator */
  bool operator==(const CTDA_CIS1_compound& other) const;

  #ifndef SR_UNSAVEABLE_RECORDS
  /* tries to save the CTDA_CIS1 compound to the given stream and returns true
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

} //namespace

#endif // SR_CTDADATA_H
