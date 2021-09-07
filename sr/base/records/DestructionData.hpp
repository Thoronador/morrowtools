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

#ifndef SR_DESTRUCTIONDATA_HPP
#define SR_DESTRUCTIONDATA_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "BinarySubRecord.hpp"

namespace SRTP
{

/** Contains data about one destruction stage. */
struct DestructionStage
{
  public:
    /** Constructor, creates an empty stage. */
    DestructionStage();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other stage to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool operator==(const DestructionStage& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the DestructionStage's data would
     *         occupy in a file stream.
     *
     * \return Returns the size in bytes that the stage would need.
     */
    uint32_t getWriteSize() const;

    /** \brief Writes the DestructionStage to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (stage was written to stream).
     *         Returns false, if an error occurred.
     */
    bool saveToStream(std::ostream& output) const;
    #endif

    /** Clears all data members. */
    void reset();

    /** Checks whether all data is reset to the initial state.
     *
     * \return Returns true, if the data is reset to the initial state.
     *         Returns false otherwise.
     */
    inline bool isReset() const
    {
      return !unknownDSTD.isPresent() && replacementModel.empty() && !unknownDMDT.isPresent();
    }

    BinarySubRecord unknownDSTD;
    std::string replacementModel;
    BinarySubRecord unknownDMDT;
}; // struct

struct DestructionData
{
  public:
    /* constructor */
    DestructionData();

    /* destructor */
    ~DestructionData() {}

    #ifndef SR_NO_RECORD_EQUALITY
    /* comparison operator */
    bool operator==(const DestructionData& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the DestructionData would occupy in a
       file stream
    */
    uint32_t getWriteSize() const;

    /* writes the DestructionData to the given output stream and returns true
       on success

       parameters:
           output - the output stream
    */
    bool saveToStream(std::ostream& output) const;
    #endif

    /* loads the DestructionData from the given input stream and returns true
       on success

       parameters:
          in_File    - the input stream
          recordType - type of the record the destruction data belongs to
          buffer     - a pre-allocated array of char that can hold at least 512 bytes
          bytesRead  - the variable that holds the number of bytes read so far
    */
    bool loadFromStream(std::istream& in_File, const uint32_t recordType, char * buffer, uint32_t& bytesRead);

    /* clears all data members and sets the status to "not present" */
    void clear();

    //subrecord DEST
    bool isPresent;
    uint32_t health;
    uint8_t  stageCount;
    uint8_t  unknownTwo;
    uint16_t unknownThreeFour;
    //end of subrecord DEST
    std::vector<DestructionStage> stages;
};//struct

} //namespace

#endif // SR_DESTRUCTIONDATA_HPP
