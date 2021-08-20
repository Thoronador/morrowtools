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

#include "EnchantmentRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

EnchantmentRecord::EnchantmentRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  effects(std::vector<EffectBlock>())
{
  memset(unknownOBND, 0, 12);
  unknownENIT.setPresence(false);
}

EnchantmentRecord::~EnchantmentRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool EnchantmentRecord::equals(const EnchantmentRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name)
      and (unknownENIT==other.unknownENIT) and (effects==other.effects));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t EnchantmentRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        + 4 /* ENIT */ + 2 /* 2 bytes for length */ + unknownENIT.size() /* fixed length of 36 or 32 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /*FULL*/;
  }
  unsigned int i;
  if (!effects.empty())
  {
    for (i=0; i<effects.size(); ++i)
    {
      writeSize = writeSize +effects[i].getWriteSize();
    }//for
  }//if effects
  return writeSize;
}

bool EnchantmentRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cENCH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if has FULL

  if (unknownENIT.isPresent())
  {
    //write ENIT
    if (!unknownENIT.saveToStream(output, cENIT))
    {
      std::cerr << "Error while writing subrecord ENIT of ENCH!\n";
      return false;
    }
  }
  else
  {
    //no ENIT
    std::cerr << "Error while writing ENCH: no ENIT subrecord found!\n";
    return false;
  }

  if (!effects.empty())
  {
    unsigned int i;
    for (i=0; i<effects.size(); ++i)
    {
      if (!effects[i].saveToStream(output))
      {
        std::cerr << "Error while writing effects of ENCH record.\n";
        return false;
      }
    }//for i
  }//if effects

  return output.good();
}
#endif

bool EnchantmentRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of ENCH is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of ENCH!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cerr <<"Error: subrecord OBND of ENCH has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord OBND of ENCH!\n";
    return false;
  }

  name.reset();
  unknownENIT.setPresence(false);
  effects.clear();
  EffectBlock tempEffect;
  CTDAData tempCTDA;
  bool hasNonPushedEffect = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: ENCH seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cENIT:
           if (unknownENIT.isPresent())
           {
             std::cerr << "Error: ENCH seems to have more than one ENIT subrecord!\n";
             return false;
           }
           if (!unknownENIT.loadFromStream(in_File, cENIT, false))
           {
             std::cerr << "Error while reading subrecord ENIT of SPEL!\n";
             return false;
           }
           //check ENIT's length
           subLength = unknownENIT.size();
           bytesRead += 2;
           if ((subLength != 36) && (subLength != 32))
           {
             std::cerr << "Error: subrecord ENIT of ENCH has invalid length ("
                       << subLength << " bytes). Should be 36 bytes or 32 bytes!\n";
             return false;
           }
           bytesRead += (2 + subLength);
           break;
      case cEFID:
           //check for old effect block
           if (hasNonPushedEffect)
           {
             //need to push
             effects.push_back(tempEffect);
             hasNonPushedEffect = false;
           }
           //new effect block
           tempEffect.unknownCTDA_CIS2s.clear();
           //EFID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: subrecord EFID of ENCH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EFID's stuff
           in_File.read((char*) &(tempEffect.effectFormID), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFID of ENCH!\n";
             return false;
           }

           //read EFIT
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cEFIT)
           {
             UnexpectedRecord(cEFIT, subRecName);
             return false;
           }
           //EFIT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: subrecord EFIT of ENCH has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read EFIT's stuff
           in_File.read((char*) &(tempEffect.magnitude), 4);
           in_File.read((char*) &(tempEffect.areaOfEffect), 4);
           in_File.read((char*) &(tempEffect.duration), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EFIT of ENCH!\n";
             return false;
           }
           hasNonPushedEffect = true;
           break;
      case cCTDA:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading ENCH: CTDA found, but there was no EFID/EFIT block.\n";
             return false;
           }
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cerr <<"Error: subrecord CTDA of ENCH has invalid length ("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord CTDA of ENCH!\n";
             return false;
           }
           tempEffect.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (!hasNonPushedEffect)
           {
             std::cerr << "Error while reading ENCH: CIS2 found, but there was no EFID/EFIT block!\n";
             return false;
           }
           if (tempEffect.unknownCTDA_CIS2s.empty())
           {
             std::cerr << "Error while reading ENCH: CIS2 found, but there was no CTDA before it!\n";
             return false;
           }
           if (!tempEffect.unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cerr << "Error: ENCH seems to have more than one CIS2 subrecord per CTDA!\n";
             return false;
           }
           //read CIS2
           if (!loadString512FromStream(in_File, tempEffect.unknownCTDA_CIS2s.back().unknownCISx,
                                        buffer, cCIS2, false, bytesRead))
             return false;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, ENIT, EFID, CTDA or CIS2 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check possibly not yet pushed effect block
  if (hasNonPushedEffect)
  {
    effects.push_back(tempEffect);
  }

  //check presence
  if ((!unknownENIT.isPresent()) or (effects.empty()))
  {
    std::cerr << "Error while reading record ENCH: required "
              << "subrecords ENIT or EFID/EFIT are missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t EnchantmentRecord::getRecordType() const
{
  return cENCH;
}

} //namespace
