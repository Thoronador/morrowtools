/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2026  Dirk Stolle

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

#include "ProjectileRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* equality operator for DSTD_DSTF_record */
bool ProjectileRecord::DSTD_DSTF_record::operator==(const ProjectileRecord::DSTD_DSTF_record& other) const
{
  return (unknownDSTD == other.unknownDSTD);
}

/* ProjectileRecord's functions */

ProjectileRecord::ProjectileRecord()
: BasicRecord(), editorID(""),
  unknownOBND(std::array<uint8_t, 12>{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  name(LocalizedString()),
  modelPath(""),
  unknownDEST(std::nullopt),
  unknownDSTD_DSTFs(std::vector<DSTD_DSTF_record>()),
  unknownNAM1(""),
  unknownVNAM(0)
{
  unknownMODT.setPresence(false);
  unknownDATA.setPresence(false);
  unknownNAM2.setPresence(false);
}

#ifndef SR_NO_RECORD_EQUALITY
bool ProjectileRecord::equals(const ProjectileRecord& other) const
{
  return ((equalsBasic(other)) && (editorID == other.editorID)
      && (unknownOBND == other.unknownOBND) && (name == other.name)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      && (unknownDEST == other.unknownDEST)
      && (unknownDSTD_DSTFs == other.unknownDSTD_DSTFs) && (unknownDATA == other.unknownDATA)
      && (unknownNAM1 == other.unknownNAM1) && (unknownNAM2 == other.unknownNAM2)
      && (unknownVNAM == other.unknownVNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ProjectileRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of string +1 byte for NUL-termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + unknownDATA.size() /* size */
        + 4 /* NAM1 */ + 2 /* 2 bytes for length */
        + unknownNAM1.length() + 1 /* length of string +1 byte for NUL-termination */
        + 4 /* VNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of string +1 byte for NUL-termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */
               + unknownMODT.size();
  }
  if (unknownDEST.has_value())
  {
    writeSize = writeSize + 4 /* DEST */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  }
  if (!unknownDSTD_DSTFs.empty())
  {
    writeSize = writeSize + unknownDSTD_DSTFs.size() * (4 /* DSTD */ + 2 /* 2 bytes for length */ + 20 /* fixed size */
               + 4 /* DSTF */ + 2 /* 2 bytes for length */ + 0 /* fixed size */);
  }
  if (unknownNAM2.isPresent())
  {
    writeSize = writeSize + 4 /* NAM2 */ + 2 /* 2 bytes for length */
               + unknownNAM2.size();
  }
  return writeSize;
}

bool ProjectileRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cPROJ), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
  {
    return false;
  }

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  if (name.isPresent())
  {
    // write FULL
    if (!name.saveToStream(output, cFULL))
    {
      return false;
    }
  }

  if (!modelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing sub record MODT of PROJ!\n";
      return false;
    }
  }

  if (unknownDEST.has_value())
  {
    // write DEST
    output.write(reinterpret_cast<const char*>(&cDEST), 4);
    subLength = 8; // fixed size
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownDEST.value()), 4);
  }

  if (!unknownDSTD_DSTFs.empty())
  {
    const auto count = unknownDSTD_DSTFs.size();
    for (decltype(unknownDSTD_DSTFs)::size_type i = 0; i < count; ++i)
    {
      // write DSTD
      output.write(reinterpret_cast<const char*>(&cDSTD), 4);
      subLength = 20; // fixed size
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      //write DSTD's stuff
      output.write(reinterpret_cast<const char*>(unknownDSTD_DSTFs[i].unknownDSTD.data()), 20);

      // write DSTF
      output.write(reinterpret_cast<const char*>(&cDSTF), 4);
      subLength = 0; // always zero, DSTF is an end marker
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      // zero length, nothing to write
    }
  }

  // write DATA
  if (unknownDATA.isPresent())
  {
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cerr << "Error while writing sub record DATA of PROJ!\n";
      return false;
    }
  }
  else
  {
    std::cerr << "Error while writing sub record DATA of PROJ: no DATA sub record present!\n";
    return false;
  }

  // write NAM1 path (NAM1)
  output.write(reinterpret_cast<const char*>(&cNAM1), 4);
  subLength = unknownNAM1.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(unknownNAM1.c_str(), subLength);

  if (unknownNAM2.isPresent())
  {
    if (!unknownNAM2.saveToStream(output, cNAM2))
    {
      std::cerr << "Error while writing sub record NAM2 of PROJ!\n";
      return false;
    }
  }

  // write VNAM
  output.write(reinterpret_cast<const char*>(&cVNAM), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write((const char*) &unknownVNAM, 4);

  return output.good();
}
#endif

bool ProjectileRecord::loadFromStream(std::istream& input, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
  {
    return false;
  }

  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
  {
    return false;
  }

  // read OBND
  uint32_t subRecName = 0;
  input.read(reinterpret_cast<char*>(&subRecName), 4);
  bytesRead += 4;
  if (subRecName != cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of PROJ has invalid length("
              << subLength << " bytes). Should be 12 bytes!\n";
    return false;
  }
  input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record OBND of PROJ!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownDEST.reset();
  unknownDSTD_DSTFs.clear();
  DSTD_DSTF_record tempDSTD_DSTF;
  bool hasReadNAM1 = false;
  unknownNAM1.clear();
  unknownNAM2.setPresence(false);
  bool hasReadVNAM = false; unknownVNAM = 0;

  while (bytesRead < readSize)
  {
    // read next sub record name
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: Record PROJ seems to have more than one FULL sub record!\n";
             return false;
           }
           // read FULL
           if (!name.loadFromStream(input, cFULL, false, bytesRead, localized, table, buffer))
           {
             std::cerr << "Error while reading sub record FULL of PROJ!\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: Record PROJ seems to have more than one MODL sub record!\n";
             return false;
           }
           // read model path
           if (!loadString512FromStream(input, modelPath, buffer, cMODL, false, bytesRead))
           {
             return false;
           }
           // check content
           if (modelPath.empty())
           {
             std::cerr << "Error: MODL sub record of PROJ is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: Record PROJ seems to have more than one MODT sub record!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(input, cMODT, false))
           {
             std::cerr << "Error while reading sub record MODT of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cDEST:
           if (unknownDEST.has_value())
           {
             std::cerr << "Error: Record PROJ seems to have more than one DEST sub record!\n";
             return false;
           }
           // DEST's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record DEST of PROJ has invalid length("
                       << subLength << " bytes). Should be eight bytes!\n";
             return false;
           }
           // read DEST's stuff
           unknownDEST = 0;
           input.read(reinterpret_cast<char*>(&unknownDEST.value()), 8);
           bytesRead += 8;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DEST of PROJ!\n";
             unknownDEST.reset();
             return false;
           }
           break;
      case cDSTD:
           if (!unknownDEST.has_value())
           {
             std::cerr << "Error: Record PROJ seems to have a DSTF sub record but no DEST sub record!\n";
             return false;
           }
           // DSTD's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 20)
           {
             std::cerr << "Error: Sub record DSTD of PROJ has invalid length("
                       << subLength << " bytes). Should be 20 bytes!\n";
             return false;
           }
           // read DSTD's stuff
           input.read(reinterpret_cast<char*>(tempDSTD_DSTF.unknownDSTD.data()), 20);
           bytesRead += 20;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DSTD of PROJ!\n";
             return false;
           }

           // read DSTF
           input.read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cDSTF)
           {
             UnexpectedRecord(cDSTF, subRecName);
             return false;
           }
           // DSTF's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 0)
           {
             std::cerr << "Error: Sub record DSTF of PROJ has invalid length("
                       << subLength << " bytes). Should be zero bytes!\n";
             return false;
           }
           unknownDSTD_DSTFs.push_back(tempDSTD_DSTF);
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cerr << "Error: Record PROJ seems to have more than one DATA sub record!\n";
             return false;
           }
           // read DATA
           if (!unknownDATA.loadFromStream(input, cDATA, false))
           {
             std::cerr << "Error while reading sub record DATA of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownDATA.size();
           // check DATA's length
           subLength = unknownDATA.size();
           if ((subLength != 92) && (subLength != 88) && (subLength != 84))
           {
             std::cerr << "Error: sub record DATA of PROJ has invalid length("
                       << subLength << " bytes). Should be 92 or 88 or 84 bytes!\n";
             return false;
           }
           break;
      case cNAM1:
           if (hasReadNAM1)
           {
             std::cerr << "Error: Record PROJ seems to have more than one NAM1 sub record!\n";
             return false;
           }
           // read NAM1's stuff
           if (!loadString512FromStream(input, unknownNAM1, buffer, cNAM1, false, bytesRead))
           {
             return false;
           }
           hasReadNAM1 = true;
           break;
      case cNAM2:
           if (!hasReadNAM1)
           {
             std::cerr << "Error: Record PROJ seems to have a NAM2 sub record but no NAM1 sub record!\n";
             return false;
           }
           if (unknownNAM2.isPresent())
           {
             std::cerr << "Error: Record PROJ seems to have more than one NAM2 sub record!\n";
             return false;
           }
           // read NAM2
           if (!unknownNAM2.loadFromStream(input, cNAM2, false))
           {
             std::cerr << "Error while reading sub record NAM2 of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownNAM2.size();
           break;
      case cVNAM:
           if (hasReadVNAM)
           {
             std::cerr << "Error: Record PROJ seems to have more than one VNAM sub record!\n";
             return false;
           }
           // read VNAM's content
           if (!loadUint32SubRecordFromStream(input, cVNAM, unknownVNAM, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadVNAM = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, DEST, DSTD, DATA, NAM1, NAM2 or VNAM are allowed!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!(hasReadNAM1 && unknownDATA.isPresent() && hasReadVNAM))
  {
    std::cerr << "Error: At least one of the required sub records of PROJ is missing!\n";
    return false;
  }

  return input.good();
}

uint32_t ProjectileRecord::getRecordType() const
{
  return cPROJ;
}

} // namespace
