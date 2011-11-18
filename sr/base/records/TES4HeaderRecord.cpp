

#include "Tes4HeaderRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

Tes4HeaderRecord::Tes4HeaderRecord()
: BasicRecord()
{

}

Tes4HeaderRecord::~Tes4HeaderRecord()
{
  //empty
}

bool Tes4HeaderRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cTES4, 4);
  uint32_t writeSize;
  writeSize = 4 /* HEDR */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* CNAM */ +2 /* 2 bytes for length */
        +m_Name.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;
  //write HEDR
  output.write((char*) &cHEDR, 4);
  //HEDR's length
  uint16_t SubLength = 12;
  output.write((char*) &SubLength, 2);
  //write HEDR's stuff
  // ---- write version
  output.write((char*) &version, 4);
  // ---- write unknown stuff
  output.write((char*) &(HeaderUnknownTwo[0]), 4);
  output.write((char*) &(HeaderUnknownTwo[1]), 4);
  if (!output.good())
  {
    std::cout << "Error while writing subrecord HEDR of TES4!\n";
    return false;
  }

  //write CNAM
  output.write((char*) &cCNAM, 4);
  //CNAM's length
  SubLength = m_Name.length()+1;
  output.write((char*) &SubLength, 2);
  //write name
  output.write(m_Name.c_str(), SubLength);

  return output.good();
}

bool Tes4HeaderRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t SubRecName;
  uint16_t SubLength;
  uint32_t BytesRead;
  SubRecName = SubLength = 0;

  //read HEDR
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cHEDR)
  {
    UnexpectedRecord(cHEDR, SubRecName);
    return false;
  }
  //HEDR's length
  in_File.read((char*) &SubLength, 2);
  BytesRead += 2;
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record HEDR of TES4 has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read HEDR's stuff
  // ---- read version number
  in_File.read((char*) &version, 4);
  // ---- read unknown values
  in_File.read((char*) &(HeaderUnknownTwo[0]), 4);
  in_File.read((char*) &(HeaderUnknownTwo[1]), 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES4!\n";
    return false;
  }

  //read CNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cCNAM)
  {
    UnexpectedRecord(cCNAM, SubRecName);
    return false;
  }
  //CNAM's length
  in_File.read((char*) &SubLength, 2);
  BytesRead += 2;
  if (SubLength>511)
  {
    std::cout << "Error: subrecord CNAM of TES4 is longer than 511 characters.\n";
    return false;
  }
  //read armour ID
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CNAM of TES4!\n";
    return false;
  }
  m_Name = std::string(Buffer);

  //read INTV
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cINTV)
  {
    UnexpectedRecord(cINTV, SubRecName);
    return false;
  }
  //INTV's length
  in_File.read((char*) &SubLength, 2);
  BytesRead += 2;
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record INTV of TES4 has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  //read value
  in_File.read((char*) &m_IntValue, 4);

  return in_File.good();
}

const std::string& Tes4HeaderRecord::getName() const
{
  return m_Name;
}

uint32_t Tes4HeaderRecord::getIntValue() const
{
  return m_IntValue;
}

} //namespace
