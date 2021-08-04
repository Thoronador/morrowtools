/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2021  Thoronador

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

/** \brief This exception gets thrown, if an invalid operation is performed on
 *         a LocalizedString, i.e. requesting a string index for a non-indexed
 *         string.
 */
class WrongStringType: public std::exception
{
  public:
    WrongStringType() = default;

    virtual const char* what() const throw() //_GLIBCXX_USE_NOEXCEPT
    {
      return "LocalizedString: Invalid string type for requested operation!";
    }
}; // class


/** Holds a (possibly) localized string value. */
struct LocalizedString
{
  public:
    /** enumeration type indicating the type of string */
    enum class Type { None, Index, String };

    /** \brief Constructor to create an empty string. */
    LocalizedString();

    /** \brief Constructor with initial values.
     *
     * \param t   the string type to use
     * \param idx the index of the string
     * \param str the actual string data
     */
    LocalizedString(const Type t, const uint32_t idx, const std::string& str);

    /** \brief Checks whether the string data is present in any form.
     *
     * \return Returns true, if the string contains data in any form.
     */
    bool isPresent() const noexcept;

    /** \brief Gets the type of the string: none, indexed, or plain string.
     *
     * \return Returns the type of the string.
     */
    inline Type getType() const noexcept
    {
      return m_Type;
    }

    /** \brief Gets the index of the string, if present.
     *
     * \return Returns the index of the string, if present. If there is no
     *         index, the function throws an exception
     * \exception WrongStringType   the string is not an indexed string
     */
    uint32_t getIndex() const;

    /** \brief Gets the stored string value.
     *
     * \exception WrongStringType  The string is of type None.
     * \return Returns the stored string value.
     */
    const std::string& getString() const;

    /** \brief Loads the string from the given input stream.
     *
     * \param in_Stream    the input stream
     * \param subHeader    the expected header of that subrecord
     * \param withHeader   if set to true, the record header will be read, too
     * \param bytesRead    the variable that holds the number of bytes read so far
     * \param localized    indicates whether a localized string index (true) or
     *                     a plain text string (false) is to be loaded
     * \param table        the current string table (only matters if @localized is true)
     * \param buffer       pointer to a pre-allocated buffer that can hold at
     *                     least 512 bytes
     * \return Returns true on success. Returns false, if loading failed.
     */
    bool loadFromStream(std::istream& in_Stream, const uint32_t subHeader,
                        const bool withHeader, uint32_t& bytesRead,
                        const bool localized, const StringTable& table,
                        char* buffer);

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Get the size in bytes that the LocalizedString record's data
     *         would occupy in a (file) stream.
     *
     * \return Returns the size in bytes that the LocalizedString would need
     *         were it written to a file.
     */
    uint32_t getWriteSize() const noexcept;

    /** \brief Writes the LocalizedString to the given output stream.
     *
     * \param output     the output stream
     * \param subHeader  the record header that should be used for saving the string
     * \return Returns true on success. Returns false if the operation failed.
     */
    bool saveToStream(std::ostream& output, const uint32_t subHeader) const;
    #endif

    /** Resets the internal state of the localized string to None / empty. */
    void reset();

    /** \brief Comparison operator for LocalizedString instances.
     *
     * \param other   the other LocalizedString to compare with
     * \return Returns true, if @other is equal to this instance.
     *         Returns false otherwise.
     */
    bool operator==(const LocalizedString& other) const;

    /** inequality operator */
    inline bool operator!=(const LocalizedString& other) const
    {
      return (!(*this==other));
    }
  private:
    Type m_Type; /**< type of the string: none, indexed, plain string */
    uint32_t m_Index; /**< index of the string (only valid when type is Type::Index) */
    std::string m_String; /**< actual string data (only valid when type != None) */
}; // struct

} // namespace

#endif // SR_LOCALIZEDSTRING_HPP
