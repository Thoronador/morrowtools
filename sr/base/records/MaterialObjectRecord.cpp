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

#include "MaterialObjectRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

MaterialObjectRecord::MaterialObjectRecord()
: BasicRecord()
{
  editorID = "";
  std::string modelPath = "";
  unknownDNAMs.clear();
  //subrecord DATA
  falloffScale = 0.0f;
  falloffBias = 0.0f;
  noise_UV_Scale = 0.0f;
  material_UV_Scale = 0.0f;
  directionalProjectionVectorX = 0.0f;
  directionalProjectionVectorY = 0.0f;
  directionalProjectionVectorZ = 0.0f;
  normalDampener = 1.0f;
  singlePassColorR = 0.0f; //(*1/255, i.e. "real" value in CK is 255 times that)
  singlePassColorG = 0.0f; //(*1/255, i.e. "real" value in CK  is 255 times that)
  singlePassColorB = 0.0f; //(*1/255, i.e. "real" value in CK  is 255 times that)
  singlePass = false; // 0=false, 1=true
  //end of DATA
}

MaterialObjectRecord::~MaterialObjectRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MaterialObjectRecord::equals(const MaterialObjectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (modelPath==other.modelPath) and (unknownDNAMs==other.unknownDNAMs)
    //subrecord DATA
    and (falloffScale==other.falloffScale) and (falloffBias==other.falloffBias)
    and (noise_UV_Scale==other.noise_UV_Scale) and (material_UV_Scale==other.material_UV_Scale)
    and (directionalProjectionVectorX==other.directionalProjectionVectorX)
    and (directionalProjectionVectorY==other.directionalProjectionVectorY)
    and (directionalProjectionVectorZ==other.directionalProjectionVectorZ)
    and (normalDampener==other.normalDampener)
    and (singlePassColorR==other.singlePassColorR)
    and (singlePassColorG==other.singlePassColorG)
    and (singlePassColorB==other.singlePassColorB)
    and (singlePass==other.singlePass));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MaterialObjectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ + getExpectedDataLength();

  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }//if MODL

  unsigned int i;
  for (i=0; i<unknownDNAMs.size(); ++i)
  {
    if (unknownDNAMs[i].isPresent())
    {
      writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */
                 +unknownDNAMs[i].getSize();
    }
  }//for

  return writeSize;
}

bool MaterialObjectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMATO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //path's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  unsigned int i;
  for (i=0; i<unknownDNAMs.size(); ++i)
  {
    if (unknownDNAMs[i].isPresent())
    {
      if (!unknownDNAMs[i].saveToStream(output, cDNAM))
      {
        std::cout << "Error while saving subrecord DNAM of MATO!\n";
        return false;
      }
    }//if DNAM
  }//for

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = getExpectedDataLength();
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &falloffScale, 4);
  output.write((const char*) &falloffBias, 4);
  output.write((const char*) &noise_UV_Scale, 4);
  output.write((const char*) &material_UV_Scale, 4);
  output.write((const char*) &directionalProjectionVectorX, 4);
  output.write((const char*) &directionalProjectionVectorY, 4);
  output.write((const char*) &directionalProjectionVectorZ, 4);
  if (subLength>=32)
    output.write((const char*) &normalDampener, 4);
  if (subLength==48)
  {
    output.write((const char*) &singlePassColorR, 4);
    output.write((const char*) &singlePassColorG, 4);
    output.write((const char*) &singlePassColorB, 4);
    const uint32_t uint_singlePass = singlePass ? 1 : 0;
    output.write((const char*) &uint_singlePass, 4);
  }
  return output.good();
}
#endif

bool MaterialObjectRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
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
    std::cout <<"Error: sub record EDID of MATO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MATO!\n";
    return false;
  }
  editorID = std::string(buffer);

  modelPath.clear();
  bool hasReadDATA = false;
  uint32_t tempUint32;
  unknownDNAMs.clear();
  BinarySubRecord tempBinary;
  while (bytesRead<readSize)
  {
      //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: MATO seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
           {
             std::cout << "Error while reading subrecord MODL of MATO!\n";
             return false;
           }

           if (modelPath.empty())
           {
             std::cout << "Error: model path of MATO is empty!\n";
             return false;
           }
           break;
      case cDNAM:
           //read DNAM's stuff
           if (!tempBinary.loadFromStream(in_File, cDNAM, false))
           {
             std::cout << "Error while reading subrecord DNAM of MATO!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempBinary.getSize();
           unknownDNAMs.push_back(tempBinary);
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: MATO seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=getExpectedDataLength())
           {
             std::cout <<"Error: subrecord DATA of MATO has invalid length ("
                       <<subLength<<"bytes). Should be "<<getExpectedDataLength()
                       <<" bytes!\nInfo: Header version is "<<headerVersion<<".\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &falloffScale, 4);
           in_File.read((char*) &falloffBias, 4);
           in_File.read((char*) &noise_UV_Scale, 4);
           in_File.read((char*) &material_UV_Scale, 4);
           in_File.read((char*) &directionalProjectionVectorX, 4);
           in_File.read((char*) &directionalProjectionVectorY, 4);
           in_File.read((char*) &directionalProjectionVectorZ, 4);
           if (subLength>=32)
           {
             in_File.read((char*) &normalDampener, 4);
           }
           else
           {
             normalDampener = 1.0f;
           }
           if (subLength==48)
           {
             in_File.read((char*) &singlePassColorR, 4);
             in_File.read((char*) &singlePassColorG, 4);
             in_File.read((char*) &singlePassColorB, 4);
             in_File.read((char*) &tempUint32, 4);
             singlePass = (tempUint32!=0);
           }
           else
           {
             singlePassColorR = 0.0f;
             singlePassColorG = 0.0f;
             singlePassColorB = 0.0f;
             singlePass = false;
           }
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of MATO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only DNAM or DATA are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cout << "Error: subrecord DATA of MATO is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MaterialObjectRecord::getRecordType() const
{
  return cMATO;
}

uint16_t MaterialObjectRecord::getExpectedDataLength() const
{
  if (headerVersion<=18) return 28;
  if (headerVersion<=19) return 32;
  /* Versions 25 and up have 48 bytes.
     I assume that versions 20 to 24 have 48 bytes, too, although I don't have
     any data on such versions.
  */
  return 48;
}

} //namespace
