/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#ifndef SR_CONTAINERRECORD_HPP
#define SR_CONTAINERRECORD_HPP

#include <array>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a container and its contents. */
struct ContainerRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    ContainerRecord();

    /** Destructor. */
    virtual ~ContainerRecord() = default;

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const ContainerRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     */
    virtual uint32_t getWriteSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param in_File    the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    /* flag constants */
    static const uint8_t cFlagRespawns;
    static const uint8_t cFlagShowOwner;
    // ---- header flags
    static const uint32_t cFlagRandomAnimStart;
    static const uint32_t cFlagObstacle;

    std::string editorID; /**< ID of the record in the editor */
    BinarySubRecord unknownVMAD;
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString name; // subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    std::vector<ComponentData> contents;
    BinarySubRecord unknownCOED;
    // subrecord DATA
    uint8_t flags;
    float weight;
    // end of subrecord DATA
    uint32_t openSoundFormID; // subrecord SNAM
    uint32_t closeSoundFormID; // subrecord QNAM

    /** \brief Checks whether the respawn flag is set.
     *
     * \return Returns true, if the respawn flag is set.
     */
    inline bool respawns() const
    {
      return (flags & cFlagRespawns) != 0;
    }

    /** \brief Checks whether the show owner flag is set.
     *
     * \return Returns true, if the show owner flag is set.
     */
    inline bool showsOwner() const
    {
      return (flags & cFlagShowOwner) != 0;
    }

    /** \brief Checks whether the "Random Anim Start" flag is set.
     *
     * \return Returns true, if the "Random Anim Start" flag is set.
     */
    inline bool hasRandomAnimStart() const
    {
      return (headerFlags & cFlagRandomAnimStart) != 0;
    }

    /** \brief Checks whether the obstacle flag is set.
     *
     * \return Returns true, if the obstacle flag is set.
     */
    inline bool isObstacle() const
    {
      return (headerFlags & cFlagObstacle) != 0;
    }
}; // struct

} // namespace

#endif // SR_CONTAINERRECORD_HPP
