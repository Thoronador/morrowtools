/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "ShaderParticleGeometryRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//type constants
const uint32_t ShaderParticleGeometryRecord::cTypeRain = 0x00;
const uint32_t ShaderParticleGeometryRecord::cTypeSnow = 0x01;

ShaderParticleGeometryRecord::ShaderParticleGeometryRecord()
: BasicRecord(), editorID(""),
  //subrecord DATA
  dataLen(dlt40Byte),
  gravityVelocity(0.0f),
  rotationVelocity(0.0f),
  particleSizeX(0.0f),
  particleSizeY(0.0f),
  centerOffsetMin(0.0f),
  centerOffsetMax(0.0f),
  initialRotation(0.0f), //in degrees
  numberOfSubtexturesX(0),
  numberOfSubtexturesY(0),
  shaderType(0),
  boxSize(4096),
  particleDensity(1.0f),
  //end of subrecord DATA
  particleTexture("")
{

}

ShaderParticleGeometryRecord::~ShaderParticleGeometryRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ShaderParticleGeometryRecord::equals(const ShaderParticleGeometryRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (dataLen==other.dataLen)
      and (gravityVelocity==other.gravityVelocity) and (rotationVelocity==other.rotationVelocity)
      and (particleSizeX==other.particleSizeX) and (particleSizeY==other.particleSizeY)
      and (centerOffsetMin==other.centerOffsetMin) and (centerOffsetMax==other.centerOffsetMax)
      and (initialRotation==other.initialRotation) and (numberOfSubtexturesX==other.numberOfSubtexturesX)
      and (numberOfSubtexturesY==other.numberOfSubtexturesY) and (shaderType==other.shaderType)
      and (boxSize==other.boxSize) and (particleDensity==other.particleDensity)
      and (particleTexture==other.particleTexture));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ShaderParticleGeometryRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (dataLen==dlt48Byte)
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +48 /* fixed size of 48 bytes */;
  }
  else
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +40 /* fixed size of 40 bytes */;
  }
  if (!particleTexture.empty())
  {
    writeSize = writeSize +4 /* ICON */ +2 /* 2 bytes for length */
        +particleTexture.length()+1 /* length of path +1 byte for NUL termination */;
  }
  return writeSize;
}

bool ShaderParticleGeometryRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSPGD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = dataLen; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &gravityVelocity, 4);
  output.write((const char*) &rotationVelocity, 4);
  output.write((const char*) &particleSizeX, 4);
  output.write((const char*) &particleSizeY, 4);
  output.write((const char*) &centerOffsetMin, 4);
  output.write((const char*) &centerOffsetMax, 4);
  output.write((const char*) &initialRotation, 4);
  output.write((const char*) &numberOfSubtexturesX, 4);
  output.write((const char*) &numberOfSubtexturesY, 4);
  output.write((const char*) &shaderType, 4);
  if (dataLen==dlt48Byte)
  {
    output.write((const char*) &boxSize, 4);
    output.write((const char*) &particleDensity, 4);
  }

  if (!particleTexture.empty())
  {
    //write ICON
    output.write((const char*) &cICON, 4);
    //ICON's length
    subLength = particleTexture.length()+1;
    output.write((const char*) &subLength, 2);
    //write particle texture path
    output.write(particleTexture.c_str(), subLength);
  }

  return output.good();
}
#endif

bool ShaderParticleGeometryRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of SPGD is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SPGD!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DATA
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if ((subLength!=48) and (subLength!=40))
  {
    std::cout <<"Error: sub record DATA of SPGD has invalid length ("<<subLength
              << " bytes). Should be 48 bytes or 40 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) &gravityVelocity, 4);
  in_File.read((char*) &rotationVelocity, 4);
  in_File.read((char*) &particleSizeX, 4);
  in_File.read((char*) &particleSizeY, 4);
  in_File.read((char*) &centerOffsetMin, 4);
  in_File.read((char*) &centerOffsetMax, 4);
  in_File.read((char*) &initialRotation, 4);
  in_File.read((char*) &numberOfSubtexturesX, 4);
  in_File.read((char*) &numberOfSubtexturesY, 4);
  in_File.read((char*) &shaderType, 4);
  if (subLength==48)
  {
    in_File.read((char*) &boxSize, 4);
    in_File.read((char*) &particleDensity, 4);
    dataLen = dlt48Byte;
  }
  else
  {
    boxSize = 4096;
    particleDensity = 1.0f;
    dataLen = dlt40Byte;
  }
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of SPGD!\n";
    return false;
  }

  if (bytesRead<readSize)
  {
    //read particle texture path
    if (!loadString512FromStream(in_File, particleTexture, buffer, cICON, true, bytesRead)) return false;
    //make sure it's not empty
    if (particleTexture.empty())
    {
      std::cout << "Error while reading subrecord ICON of SPGD: path is empty!\n";
      return false;
    }
  }
  else
  {
    //no particle texture present
    particleTexture.clear();
  }

  return in_File.good();
}

uint32_t ShaderParticleGeometryRecord::getRecordType() const
{
  return cSPGD;
}

} //namespace
