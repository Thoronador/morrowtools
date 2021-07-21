/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

#ifndef SR_LOCALIZEDSTRING_HPP
#define SR_LOCALIZEDSTRING_HPP

#include <exception>
#include <string>
#include <stdint.h>
#include "../StringTable.hpp"

namespace SRTP
{

/* class WrongStringType:
     This exception gets thrown, if an invalid operation is performed,
     i.e. requesting a string index for a non-indexed string.
*/
class WrongStringType: public std::exception
{
  public:
    WrongStringType() { }

    virtual const char* what() const throw() //_GLIBCXX_USE_NOEXCEPT
    {
      return "LocalizedString: Invalid string type for requested operation!";
    }
}; //class


struct LocalizedString
{
  public:
    /* constructor */
    LocalizedString();

    /* returns true, if the string is present in any form */
    bool isPresent() const;

    /* enumeration type type of string */
    enum Type {lsNone, lsIndex, lsString};

    inline Type getType() const
    {
      return m_Type;
    }

    /* returns the index of the string, if present. If there is no index, the
       function throws an exception
    */
    uint32_t getIndex() const;

    /* returns the stored string value */
    const std::string& getString() const;

    /* loads the String from the given input stream and returns true on success

      parameters:
          in_Stream  - the input stream
          subHeader  - the expected header of that subrecord
          withHeader - if set to true, the record header will be read, too
          bytesRead  - the variable that holds the number of bytes read so far
          localized  - indicates whether a localized string index (true) or a
                       plain text string (false) is to be loaded
          table      - the current string table (only matters if localized is true)
          buffer     - pointer to a pre-allocated buffer that can hold at least
                       512 bytes
    */
    bool loadFromStream(std::istream& in_Stream, const uint32_t subHeader,
                        const bool withHeader, uint32_t& bytesRead,
                        const bool localized, const StringTable& table,
                        char* buffer);

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the LocalizedString record's data would
       occupy in a file stream
    */
    uint32_t getWriteSize() const;

    /* writes the LocalizedString to the given output stream and returns true
       on success

      parameters:
          output    - the output file stream
          subHeader - the record header that should be used for saving the string
    */
    bool saveToStream(std::ostream& output, const uint32_t subHeader) const;
    #endif

    /* resets the internal state of the localized string */
    void reset();

    /* comparison operator */
    bool operator==(const LocalizedString& other) const;

    inline bool operator!=(const LocalizedString& other) const
    {
      return (!(*this==other));
    }
  private:
    Type m_Type;
    uint32_t m_Index;
    std::string m_String;
}; //struct

} //namespace

#endif // SR_LOCALIZEDSTRING_HPP
