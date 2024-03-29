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

#include "ClimateRecord.hpp"
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"
#include <iostream>
#include <cstring>

namespace SRTP
{

/* WeatherEntry's functions */
bool ClimateRecord::WeatherEntry::operator==(const ClimateRecord::WeatherEntry& other) const
{
  return ((weatherFormID==other.weatherFormID) and (chance==other.chance)
      and (globalFormID==other.globalFormID));
}

/* ClimateRecord's functions */

ClimateRecord::ClimateRecord()
: BasicRecord(), editorID(""),
  weatherList(std::vector<WeatherEntry>()),
  sunTexture(""),
  sunGlareTexture(""),
  modelPath("")
{
  unknownMODT.setPresence(false);
  unknownTNAM.setPresence(false);
}

ClimateRecord::~ClimateRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ClimateRecord::equals(const ClimateRecord& other) const
{
  return (equalsBasic(other) and (editorID==other.editorID)
      and (weatherList==other.weatherList) and (sunTexture==other.sunTexture)
      and (sunGlareTexture==other.sunGlareTexture) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (unknownTNAM==other.unknownTNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ClimateRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +2 /* 2 bytes for length */
        +sunTexture.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* GNAM */ +2 /* 2 bytes for length */
        +sunGlareTexture.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  if (!weatherList.empty())
  {
    writeSize = writeSize +4 /* WLST */ +2 /* 2 bytes for length */ +12*weatherList.size() /* fixed size per entry*/;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (unknownTNAM.isPresent())
  {
    writeSize = writeSize + 4 /* TNAM */ + 2 /* 2 bytes for length */ + unknownTNAM.size() /* fixed size of 6 bytes, usually */;
  }
  return writeSize;
}

bool ClimateRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCLMT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!weatherList.empty())
  {
    //write WLST
    output.write((const char*) &cWLST, 4);
    //WLST's length
    const unsigned int wlst_size = weatherList.size();
    subLength = 12*wlst_size; //fixed
    output.write((const char*) &subLength, 2);
    //write WLST's stuff
    unsigned int i;
    for (i=0; i<wlst_size; ++i)
    {
      output.write((const char*) &weatherList[i].weatherFormID, 4);
      output.write((const char*) &weatherList[i].chance, 4);
      output.write((const char*) &weatherList[i].globalFormID, 4);
    }
  }//if WLST

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = sunTexture.length()+1;
  output.write((const char*) &subLength, 2);
  //write Sun texture
  output.write(sunTexture.c_str(), subLength);

  //write GNAM
  output.write((const char*) &cGNAM, 4);
  //GNAM's length
  subLength = sunGlareTexture.length()+1;
  output.write((const char*) &subLength, 2);
  //write Sun Glare texture
  output.write(sunGlareTexture.c_str(), subLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of CLMT!\n";
      return false;
    }
  }//if MODT

  if (unknownTNAM.isPresent())
  {
    //write TNAM
    if (!unknownTNAM.saveToStream(output, cTNAM))
    {
      std::cerr << "Error while writing subrecord TNAM of CLMT!\n";
      return false;
    }
  }//if TNAM

  return output.good();
}
#endif

bool ClimateRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of CLMT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of CLMT!\n";
    return false;
  }
  editorID = std::string(buffer);

  weatherList.clear();
  WeatherEntry temp;
  unsigned int i;
  sunTexture.clear();
  sunGlareTexture.clear();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownTNAM.setPresence(false);

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cWLST:
           if (!weatherList.empty())
           {
             std::cerr << "Error: record CLMT seems to have more than one WLST subrecord!\n";
             return false;
           }
           //WLST's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (((subLength%12)!=0) or (subLength==0))
           {
             std::cerr <<"Error: sub record WLST of CLMT has invalid length ("
                       <<subLength<<" bytes). Should be an integral multiple of 12 bytes.\n";
             return false;
           }
           //read WLST
           for (i=0; i<subLength/12; ++i)
           {
             in_File.read((char*) &temp.weatherFormID, 4);
             in_File.read((char*) &temp.chance, 4);
             in_File.read((char*) &temp.globalFormID, 4);
             bytesRead += 12;
             if (!in_File.good())
             {
               std::cerr << "Error while reading subrecord WLST of CLMT!\n";
               return false;
             }
             weatherList.push_back(temp);
           }//for
           break;
      case cFNAM:
           if (!sunTexture.empty())
           {
             std::cerr << "Error: CLMT seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadString512FromStream(in_File, sunTexture, buffer, cFNAM, false, bytesRead)) return false;
           //length check
           if (sunTexture.empty())
           {
             std::cerr << "Error: subrecord FNAM of CLMT is empty.\n";
             return false;
           }
           break;
      case cGNAM:
           if (!sunGlareTexture.empty())
           {
             std::cerr << "Error: CLMT seems to have more than one GNAM subrecord.\n";
             return false;
           }
           //read GNAM
           if (!loadString512FromStream(in_File, sunGlareTexture, buffer, cGNAM, false, bytesRead)) return false;
           //length check
           if (sunGlareTexture.empty())
           {
             std::cerr << "Error: subrecord GNAM of CLMT is empty.\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: CLMT seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead)) return false;
           //length check
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of CLMT is empty.\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: CLMT seems to have more than one MODT subrecord.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead += (2 + unknownMODT.size());
           break;
      case cTNAM:
           if (unknownTNAM.isPresent())
           {
             std::cerr << "Error: CLMT seems to have more than one TNAM subrecord.\n";
             return false;
           }
           // read TNAM
           if (!unknownTNAM.loadFromStream(in_File, cTNAM, false))
             return false;
           bytesRead += (2 + unknownTNAM.size());
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only WLST, FNAM, GNAM, MODL, MODT, MODS"
                     << " or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(!sunTexture.empty() and !sunGlareTexture.empty()
      and !modelPath.empty() and unknownTNAM.isPresent()))
  {
    std::cerr << "Error: at least one required subrecord of CLMT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ClimateRecord::getRecordType() const
{
  return cCLMT;
}

} //namespace
