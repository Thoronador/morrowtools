/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021 Thoronador

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

#ifndef SR_STRINGTABLE_HPP
#define SR_STRINGTABLE_HPP

#include <cstdint>
#include <unordered_map>
#include <string>

namespace SRTP
{

/** Holds string data for an ESM / ESP file. */
class StringTable
{
  public:
    /** Constructor, creates an empty table. */
    StringTable();

    /** enumeration type for table data type */
    enum DataType {sdUnknown, sdNULterminated, sdPascalStyle};

    /** \brief Adds a string with the given ID and content to the string table.
     *
     * If a string with the same ID is already present, that string will be
     * replaced. If the string ID is zero, no string will be added or changed.
     *
     * \param stringID  the ID of the string
     * \param content   the (new) content of the string
     */
    void addString(const uint32_t stringID, const std::string& content);

    /** \brief Checks whether the table contains a string with the given ID.
     *
     * \param stringID    the ID of the string
     * \return Returns true, if the table contains a string with the given ID.
     */
    bool hasString(const uint32_t stringID) const;

    /** \brief Gets the string for a given ID.
     *
     * If no string with that ID is present, the function will throw an
     * exception. Use hasString() to check the presence of a string first.
     * \param stringID  the ID of the string
     * \return Returns the string for a given ID, if it exists.
     * \throws If no string with the ID is present, the function will throw an
     *         exception.
     */
    const std::string& getString(const uint32_t stringID) const;

    /** \brief Tries to remove the string with the given ID from the table.
     *
     * \param stringID    the ID of the string
     * \return Returns true, if a string was removed. Returns false otherwise.
     */
    bool deleteString(const uint32_t stringID);

    /** \brief Removes all entries from the string table. */
    void tabulaRasa();

    /** \brief Tries to read a string table from the given file.
     *
     * \param FileName  name of the file containing the string table
     * \param stringType  expected type of string data in table
     *        (Setting this to unknown will let the method guess the type based
     *         on the file extension.)
     * \return Returns true in case of success, false on failure.
     */
    bool readTable(const std::string& FileName, DataType stringType);

    /** \brief Tries to write a string table to a file.
     *
     * \param FileName  name of the output file for the string table
     * \param stringType  desired type of string data in table
     * \return Returns true in case of success, false on failure.
     */
    bool writeTable(const std::string& FileName, DataType stringType) const;

    /** \brief Returns the number of strings in the table. */
    uint32_t getNumberOfTableEntries() const;
  private:
    struct DirectoryEntry
    {
      uint32_t stringID;
      uint32_t offset;
    }; // struct

    std::unordered_map<uint32_t, std::string> m_Strings; /**< holds the string */
}; // class

} // namespace

#endif // SR_STRINGTABLE_HPP
