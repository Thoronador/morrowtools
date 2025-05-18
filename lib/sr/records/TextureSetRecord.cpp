/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2025  Dirk Stolle

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

#include "TextureSetRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

TextureSetRecord::TextureSetRecord()
: BasicRecord(),
  editorID(""),
  unknownOBND(std::array<uint8_t, 12>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })),
  texture00(""),
  texture01(""),
  texture02(""),
  texture03(""),
  texture04(""),
  texture05(""),
  texture07(""),
  unknownDODT(std::nullopt),
  unknownDNAM(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool TextureSetRecord::equals(const TextureSetRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (texture00 == other.texture00) && (texture01 == other.texture01)
      && (texture02 == other.texture02) && (texture03 == other.texture03)
      && (texture04 == other.texture04) && (texture05 == other.texture05)
      && (texture07 == other.texture07) && (unknownDNAM == other.unknownDNAM)
      && (unknownOBND == other.unknownOBND) && (unknownDODT == other.unknownDODT);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t TextureSetRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* TX00 */ + 2 /* 2 bytes for length */
        + texture00.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 2 /* fixed size */;
  if (!texture01.empty())
  {
    writeSize = writeSize + 4 /* TX01 */ + 2 /* 2 bytes for length */
        + texture01.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture02.empty())
  {
    writeSize = writeSize + 4 /* TX02 */ + 2 /* 2 bytes for length */
        + texture02.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture03.empty())
  {
    writeSize = writeSize + 4 /* TX03 */ + 2 /* 2 bytes for length */
        + texture03.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture04.empty())
  {
    writeSize = writeSize + 4 /* TX04 */ + 2 /* 2 bytes for length */
        + texture04.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture05.empty())
  {
    writeSize = writeSize + 4 /* TX05 */ + 2 /* 2 bytes for length */
        + texture05.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!texture07.empty())
  {
    writeSize = writeSize + 4 /* TX07 */ + 2 /* 2 bytes for length */
        + texture07.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownDODT.has_value())
  {
    writeSize = writeSize + 4 /* DODT */ + 2 /* 2 bytes for length */ + 36 /* fixed size */;
  }
  return writeSize;
}

bool TextureSetRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cTXST), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  // write OBND
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12; /* fixed size */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write zeroeth texture (TX00)
  output.write((const char*) &cTX00, 4);
  subLength = texture00.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  //write zeroeth texture
  output.write(texture00.c_str(), subLength);

  if (!texture01.empty())
  {
    // write first texture (TX01)
    output.write(reinterpret_cast<const char*>(&cTX01), 4);
    subLength = texture01.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture01.c_str(), subLength);
  }

  if (!texture02.empty())
  {
    // write second texture (TX02)
    output.write(reinterpret_cast<const char*>(&cTX02), 4);
    subLength = texture02.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture02.c_str(), subLength);
  }

  if (!texture03.empty())
  {
    // write third texture (TX03)
    output.write(reinterpret_cast<const char*>(&cTX03), 4);
    subLength = texture03.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture03.c_str(), subLength);
  }

  if (!texture04.empty())
  {
    // write 4th texture (TX04)
    output.write(reinterpret_cast<const char*>(&cTX04), 4);
    subLength = texture04.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture04.c_str(), subLength);
  }

  if (!texture05.empty())
  {
    // write fifth texture (TX05)
    output.write(reinterpret_cast<const char*>(&cTX05), 4);
    subLength = texture05.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture05.c_str(), subLength);
  }

  if (!texture07.empty())
  {
    // write seventh texture (TX07)
    output.write(reinterpret_cast<const char*>(&cTX07), 4);
    subLength = texture07.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(texture07.c_str(), subLength);
  }

  if (unknownDODT.has_value())
  {
    // write DODT
    output.write(reinterpret_cast<const char*>(&cDODT), 4);
    subLength = 36; /* fixed size */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(unknownDODT.value().data()), 36);
  }

  // write DNAM
  output.write(reinterpret_cast<const char*>(&cDNAM), 4);
  subLength = 2; /* fixed size */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownDNAM), 2);

  return output.good();
}
#endif

bool TextureSetRecord::loadFromStream(std::istream& input,
                                      [[maybe_unused]] const bool localized,
                                      [[maybe_unused]] const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(input, readSize))
    return false;

  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(input, editorID, buffer, cEDID, true, bytesRead))
  {
    std::cerr << "Error while reading sub record EDID of TXST!\n";
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
  // OBND's length
  uint16_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 12)
  {
    std::cerr << "Error: Sub record OBND of TXST has invalid length ("
              << subLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read OBND's stuff
  input.read(reinterpret_cast<char*>(unknownOBND.data()), 12);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record OBND of TXST!\n";
    return false;
  }

  // read TX00
  if (!loadString512FromStream(input, texture00, buffer, cTX00, true, bytesRead))
    return false;

  // DODT or DNAM are next
  bool hasReadDNAM = false;
  unknownDODT.reset();
  texture01.clear();
  texture02.clear();
  texture03.clear();
  texture04.clear();
  texture05.clear();
  texture07.clear();
  while (bytesRead<readSize)
  {
    // read next header
    input.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cerr << "Error: Record TXST seems to have more than one DNAM sub record!\n";
             return false;
           }
           // DNAM's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record DNAM of TXST has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read DNAM's data
           input.read(reinterpret_cast<char*>(&unknownDNAM), 2);
           bytesRead += 2;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DNAM of TXST!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cDODT:
           if (unknownDODT.has_value())
           {
             std::cerr << "Error: Record TXST seems to have more than one DODT sub record!\n";
             return false;
           }
           // DODT's length
           input.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 36)
           {
             std::cerr << "Error: Sub record DODT of TXST has invalid length ("
                       << subLength << " bytes). Should be 36 bytes.\n";
             return false;
           }
           // read DODT's stuff
           unknownDODT = std::array<uint8_t, 36>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
           input.read(reinterpret_cast<char*>(unknownDODT.value().data()), 36);
           bytesRead += 36;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DODT of TXST!\n";
             return false;
           }
           break;
      case cTX01:
           if (!texture01.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX01 sub record!\n";
             return false;
           }
           // read TX01
           if (!loadString512FromStream(input, texture01, buffer, cTX01, false, bytesRead))
             return false;
           break;
      case cTX02:
           if (!texture02.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX02 sub record!\n";
             return false;
           }
           // read TX02
           if (!loadString512FromStream(input, texture02, buffer, cTX02, false, bytesRead))
             return false;
           break;
      case cTX03:
           if (!texture03.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX03 sub record!\n";
             return false;
           }
           // read TX03
           if (!loadString512FromStream(input, texture03, buffer, cTX03, false, bytesRead))
             return false;
           break;
      case cTX04:
           if (!texture04.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX04 sub record!\n";
             return false;
           }
           // read TX04
           if (!loadString512FromStream(input, texture04, buffer, cTX04, false, bytesRead))
             return false;
           break;
      case cTX05:
           if (!texture05.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX05 sub record!\n";
             return false;
           }
           // read TX05
           if (!loadString512FromStream(input, texture05, buffer, cTX05, false, bytesRead))
             return false;
           break;
      case cTX07:
           if (!texture07.empty())
           {
             std::cerr << "Error: Record TXST seems to have more than one TX07 sub record!\n";
             return false;
           }
           // read TX07
           if (!loadString512FromStream(input, texture07, buffer, cTX07, false, bytesRead))
             return false;
           break;
      default:
           std::cerr << "Error: Expected record name DNAM, DODT, TX01, TX02, "
                     << "TX03, TX04, TX05 or TX07 was not found. Instead, \""
                     << IntTo4Char(subRecName) << "\" was found.\n";
           return false;
    }
  }

  if (!hasReadDNAM)
  {
    std::cerr << "Error: Sub record DNAM of TXST is missing!\n";
    return false;
  }

  return input.good();
}

uint32_t TextureSetRecord::getRecordType() const
{
  return cTXST;
}

} // namespace
