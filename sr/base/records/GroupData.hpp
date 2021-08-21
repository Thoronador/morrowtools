/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_GROUPDATA_HPP
#define SR_GROUPDATA_HPP

#include <array>
#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds information about a group header. */
struct GroupData
{
  public:
    /** Constructor, creates empty group data. */
    GroupData();

    /** Destructor. */
    ~GroupData() = default;

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Writes the group data to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (data was written to stream).
     *         Returns false, if an error occurred.
     */
    bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the group data from the given input stream.
     *
     * \param in_File    the input stream
     * \return Returns true on success (data was loaded from stream).
     *         Returns false, if an error occurred.
     */
    bool loadFromStream(std::istream& in_File);

    /** \brief Gets the group "name". Meaning depends on type.
     *
     * \return Returns the group "name".
     */
    uint32_t label() const;

    /** \brief Gets the size of the group read from the stream in bytes.
     *
     * \return Returns the size of the group read from the stream in bytes.
     */
    uint32_t size() const;

    /** \brief Gets the type of the group.
     *
     * \return Returns type of the group.
     * \remarks This is usually one of the predefined type constants.
     */
    uint32_t type() const;

    /// type constants
    static const uint32_t cTopLevelGroup;
    static const uint32_t cWorldspaceChildren;
    static const uint32_t cCellChildren;
    static const uint32_t cTopicChildren;
    static const uint32_t cCellPersistentChildren;
    static const uint32_t cCellTemporaryChildren;
    static const uint32_t cCellVisibleDistantChildren;

    /** \brief Checks whether the label is a cell form ID according to the group type.
     *
     * \return Returns true, if the group label is a cell form ID according to
     *         the group type. Returns false otherwise.
     */
    bool labelIsCellID() const;

    /** \brief Sets a new group label.
     *
     * \param newLabel    the new name / label that shall be set
     */
    void setLabel(const uint32_t newLabel);

    /** \brief Sets a new group size value.
     *
     * \param newSize    the new size that shall be set
     */
    void setSize(const uint32_t newSize);

    /** \brief Sets a new group type.
     *
     * \param newType    the new type that shall be set
     */
    void setType(const uint32_t newType);

    /** \brief Equality operator for GroupData.
     *
     * \param other  the other GroupData instance to compare to
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool operator==(const GroupData& other) const;
  private:
    uint32_t m_Size; /**< size of the group */
    uint32_t m_Label; /**< label of the group */
    uint32_t m_Type; /**< type of the group */
    std::array<int32_t, 2> UnknownDataTwo; /**< unknown data */
}; // struct

} // namespace

#endif // SR_GROUPDATA_HPP
