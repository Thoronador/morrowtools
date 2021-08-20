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

#include "CameraPathRecord.hpp"
#include "../SR_Constants.hpp"
#include <iostream>
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* SubrecordANAM's functions */

bool CameraPathRecord::SubrecordANAM::operator==(const CameraPathRecord::SubrecordANAM& other) const
{
  return ((parentFormID==other.parentFormID) and (nextFormID==other.nextFormID));
}

/* data flag constants */
const uint8_t CameraPathRecord::cFlagInverseMustHaveCameraShots = 0x80;
const uint8_t CameraPathRecord::cCameraZoomDefault              = 0x00;
const uint8_t CameraPathRecord::cCameraZoomDisable              = 0x01;
const uint8_t CameraPathRecord::cCameraZoomShotList             = 0x02;

/* CameraPathRecord's functions */

CameraPathRecord::CameraPathRecord()
: BasicRecord(), editorID(""),
  conditions(std::vector<CTDA_CIS1_compound>()),
  unknownDATA(0),
  camShotList(std::vector<uint32_t>())
{
  cameraPathLinks.parentFormID = 0;
  cameraPathLinks.nextFormID = 0;
}

CameraPathRecord::~CameraPathRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CameraPathRecord::equals(const CameraPathRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (conditions==other.conditions) and (cameraPathLinks==other.cameraPathLinks)
      and (unknownDATA==other.unknownDATA) and (camShotList==other.camShotList));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CameraPathRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize = 0;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
         +editorID.length() +1 /* length of ID +1 byte for NUL terminations */
         +4 /* ANAM */ +2 /* 2 bytes for length */ +8 /* fixed size of eight bytes*/
         +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
         +camShotList.size()*(4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes*/);
  unsigned int i;
  for (i=0; i<conditions.size(); ++i)
  {
    writeSize += conditions[i].getWriteSize();
  }//for
  return writeSize;
}

bool CameraPathRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCPTH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write EDID
  output.write(editorID.c_str(), subLength);

  //write CTDA / CIS1
  unsigned int i;
  for (i=0; i<conditions.size(); ++i)
  {
    if (!conditions[i].saveToStream(output))
    {
      std::cerr << "Error while writing subrecord CTDA of CPTH!\n";
      return false;
    }
  }//for

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = 8; //fixed size
  output.write((const char*) &subLength, 2);
  //write ANAM
  output.write((const char*) &cameraPathLinks.parentFormID, 4);
  output.write((const char*) &cameraPathLinks.nextFormID, 4);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 1; //fixed size
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &unknownDATA, 1);

  for (i=0; i<camShotList.size(); ++i)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SNAM
    output.write((const char*) &camShotList[i], 4);
  }//for

  return output.good();
}
#endif

bool CameraPathRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  conditions.clear();
  CTDA_CIS1_compound tempCC;
  bool hasCTDA = false;
  bool hasReadANAM = false;
  bool hasReadDATA = false;
  camShotList.clear();
  uint32_t tempUint32;

  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cCTDA:
           if (hasCTDA)
           {
             conditions.push_back(tempCC);
           }
           //load CTDA
           if (!tempCC.unknownCTDA.loadFromStream(in_File, bytesRead)) return false;
           hasCTDA = true;
           tempCC.unknownCISx.clear();
           break;
      case cCIS1:
           if (!hasCTDA)
           {
             std::cerr << "Error: CPTH has CIS1 subrecord without prior CTDA!\n";
             return false;
           }
           if (!tempCC.unknownCISx.empty())
           {
             std::cerr << "Error: CPTH seems to have more than one CIS1 per CTDA!\n";
             return false;
           }
           //load CIS1
           if (!loadString512FromStream(in_File, tempCC.unknownCISx, buffer, cCIS1, false, bytesRead))
             return false;
           //check content
           if (tempCC.unknownCISx.empty())
           {
             std::cerr << "Error: subrecord CIS1 of CPTH is empty!\n";
             return false;
           }
           conditions.push_back(tempCC);
           hasCTDA = false;
           break;
      case cANAM:
           if (hasReadANAM)
           {
             std::cerr << "Error: CPTH seems to have more than one ANAM subrecord!\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cerr <<"Error: subrecord ANAM of CPTH has invalid length ("
                       <<subLength <<" bytes). Should be eight bytes!\n";
             return false;
           }
           //read ANAM's stuff
           in_File.read((char*) &cameraPathLinks.parentFormID, 4);
           in_File.read((char*) &cameraPathLinks.nextFormID, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ANAM of CPTH!\n";
             return false;
           }//if
           hasReadANAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: CPTH seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: subrecord DATA of CPTH has invalid length ("
                       <<subLength <<" bytes). Should be one byte!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &unknownDATA, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of CPTH!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      case cSNAM:
           //load SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, tempUint32, false))
             return false;
           bytesRead += 6;
           //check content
           if (tempUint32==0)
           {
             std::cerr << "Error: subrecord SNAM of CPTH is zero!\n";
             return false;
           }
           camShotList.push_back(tempUint32);
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only CTDA, CIS1, ANAM, DATA or SNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //push leftover CTDA/CIS1
  if (hasCTDA)
  {
    conditions.push_back(tempCC);
  }

  //presence checks
  if (!(hasReadANAM and hasReadDATA))
  {
    std::cerr << "Error: At least one subrecord of CPTH is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t CameraPathRecord::getRecordType() const
{
  return cCPTH;
}

bool CameraPathRecord::mustHaveCameraShots() const
{
  return ((unknownDATA & cFlagInverseMustHaveCameraShots)==0);
}

CameraPathRecord::CameraZoomType CameraPathRecord::getCameraZoom() const
{
  switch (unknownDATA & 0x0F)
  {
    case cCameraZoomDefault:
         return czDefault;
    case cCameraZoomDisable:
         return czDisable;
    case cCameraZoomShotList:
    default:
         return czShotList;
  }//swi
}

} //namespace
