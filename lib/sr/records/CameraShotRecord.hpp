/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021, 2025  Dirk Stolle

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

#ifndef SR_CAMERASHOTRECORD_HPP
#define SR_CAMERASHOTRECORD_HPP

#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include <string>

namespace SRTP
{

/** Contains information about a camera shot, e. g. for kill camera. */
struct CameraShotRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    CameraShotRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instances contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const CameraShotRecord& other) const;
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
     * \param input      the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& input, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    /// enumeration type for DATA's length
    enum class DataLengthType: uint16_t { dlt40Byte = 40, dlt44Byte = 44 };

    /// constants for camera action
    static const uint32_t cActionShoot;
    static const uint32_t cActionFly;
    static const uint32_t cActionHit;
    static const uint32_t cActionZoom;

    /// constants for location/target
    static const uint32_t cLocationAttacker;
    static const uint32_t cLocationProjectile;
    static const uint32_t cLocationTarget;
    static const uint32_t cLocationLeadActor;

    std::string editorID;
    std::string modelPath; // sub record MODL
    BinarySubRecord unknownMODT;
    // sub record DATA
    DataLengthType dataLen;
    uint32_t cameraAction;
    uint32_t cameraLocation;
    uint32_t cameraTarget;
    uint32_t cameraFlags;
    float timeMultPlayer;
    float timeMultTarget;
    float timeMultGlobal;
    float maxTime;
    float minTime;
    float targetPercentBetweenActors;
    float nearTargetDistance;
    // end of sub record DATA
    uint32_t imageSpaceModFormID; // sub record MNAM
}; // struct

} // namespace

#endif // SR_CAMERASHOTRECORD_HPP
