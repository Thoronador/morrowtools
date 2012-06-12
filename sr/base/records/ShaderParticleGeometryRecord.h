/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_SHADERPARTICLEGEOMETRYRECORD_H
#define SR_SHADERPARTICLEGEOMETRYRECORD_H

#include <string>
#include "BasicRecord.h"

namespace SRTP
{

struct ShaderParticleGeometryRecord: public BasicRecord
{
  public:
    /* constructor */
    ShaderParticleGeometryRecord();

    /* destructor */
    virtual ~ShaderParticleGeometryRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const ShaderParticleGeometryRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    //enumeration type for DATA's length
    enum DataLengthType {dlt40Byte=40, dlt48Byte=48};

    std::string editorID;
    //subrecord DATA
    DataLengthType dataLen;
    float gravityVelocity;
    float rotationVelocity;
    float particleSizeX;
    float particleSizeY;
    float centerOffsetMin;
    float centerOffsetMax;
    float initialRotation; //in degrees
    uint32_t numberOfSubtexturesX;
    uint32_t numberOfSubtexturesY;
    uint32_t shaderType;
    uint32_t boxSize;
    float particleDensity;
    //end of subrecord DATA
    std::string particleTexture;

    //type constants
    static const uint32_t cTypeRain;
    static const uint32_t cTypeSnow;
}; //struct

} //namespace

#endif // SR_SHADERPARTICLEGEOMETRYRECORD_H
