/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2024  Dirk Stolle

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

#ifndef SR_CTDADATA_HPP
#define SR_CTDADATA_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include "../SR_Constants.hpp"

namespace SRTP
{

/** Holds condition data (for a magic effect, etc.). */
struct CTDAData
{
  /** Constructor, creates a record with all content set to zero. */
  CTDAData();

  std::array<uint8_t, 32> content; /**< the sub record's content */

  /** \brief Loads the CTDA data from the given input stream.
   *
   * \param input      the input stream to read from
   * \param bytesRead  reference to the variable that counts the number of read bytes
   * \return Returns true on success, or false on failure.
   */
  bool loadFromStream(std::istream& input, uint32_t& bytesRead);

  #ifndef SR_UNSAVEABLE_RECORDS
  /** \brief Writes the CTDA data to the given output stream.
   *
   * \param output the output stream
   * \return Returns true on success, or false on failure.
   */
  bool saveToStream(std::ostream& output) const;
  #endif

  #ifndef SR_NO_RECORD_EQUALITY
  /** equality operator */
  bool operator==(const CTDAData& other) const;
  #endif

  /** Clears content by setting it all to zero. */
  void clear();
}; // struct



template<uint32_t cisRecName>
struct CTDA_CISx_compound
{
  CTDAData unknownCTDA; /**< condition data */
  std::string unknownCISx; /**< variable name for x-th parameter */

  /** constant to allow easy access to CISx 'type' */
  static const uint32_t cCISx;

  /** Constructor, creates empty compound. */
  CTDA_CISx_compound();

  /** Alternative constructor to create compound with preset data.
   *
   * \param ctda  value for CTDA sub record
   * \param cisx  value for CISx sub record
   */
  CTDA_CISx_compound(const CTDAData& ctda, const std::string& cisx);

  #ifndef SR_NO_RECORD_EQUALITY
  /** equality operator */
  bool operator==(const CTDA_CISx_compound& other) const;
  #endif

  #ifndef SR_UNSAVEABLE_RECORDS
  /** Tries to save the CTDA_CIS2 compound to the given stream.
   *
   * \param output    the output stream
   * \return Returns true in case of success, false on failure.
   */
  bool saveToStream(std::ostream& output) const;

  /** Gets the size in bytes that the CTDA_CIS2 compound's data would occupy.
   *
   * \return Returns the size in bytes that the CTDA_CIS2 compound's data would
   *         occupy in a file stream.
   */
  uint32_t getWriteSize() const;
  #endif
}; // struct

/** constant to allow easy access to CISx 'type' */
template<uint32_t cisRecName>
const uint32_t CTDA_CISx_compound<cisRecName>::cCISx = cisRecName;

template<uint32_t cisRecName>
CTDA_CISx_compound<cisRecName>::CTDA_CISx_compound()
: unknownCTDA(CTDAData()),
  unknownCISx("")
{
}

template<uint32_t cisRecName>
CTDA_CISx_compound<cisRecName>::CTDA_CISx_compound(const CTDAData& ctda, const std::string& cisx)
: unknownCTDA(ctda), unknownCISx(cisx)
{
}

#ifndef SR_NO_RECORD_EQUALITY
template<uint32_t cisRecName>
bool CTDA_CISx_compound<cisRecName>::operator==(const CTDA_CISx_compound& other) const
{
  return (unknownCTDA == other.unknownCTDA) && (unknownCISx == other.unknownCISx);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
template<uint32_t cisRecName>
bool CTDA_CISx_compound<cisRecName>::saveToStream(std::ostream& output) const
{
  if (!unknownCTDA.saveToStream(output))
    return false;

  if (!unknownCISx.empty())
  {
    // write CISx
    output.write(reinterpret_cast<const char*>(&cCISx), 4);
    // CIS1's/CIS2's length
    const uint16_t subLength = unknownCISx.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write CIS1's/CIS2's stuff
    output.write(unknownCISx.c_str(), subLength);
  }

  return output.good();
}

template<uint32_t cisRecName>
uint32_t CTDA_CISx_compound<cisRecName>::getWriteSize() const
{
  uint32_t writeSize = 4 /* CTDA */ + 2 /* 2 bytes for length */ + 32 /* fixed length */;
  if (!unknownCISx.empty())
  {
    writeSize = writeSize + 4 /* CIS1/CIS2 */ + 2 /* 2 bytes for length */
               + unknownCISx.length() + 1 /* length of string +1 byte for NUL termination */;
  }

  return writeSize;
}
#endif

/** alias for compound with CIS1 sub record for string */
using CTDA_CIS1_compound = CTDA_CISx_compound<cCIS1>;

/** alias for compound with CIS2 sub record for string */
using CTDA_CIS2_compound = CTDA_CISx_compound<cCIS2>;

} // namespace

#endif // SR_CTDADATA_HPP
