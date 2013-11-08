/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013  Thoronador

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

#include "LightRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

LightRecord::LightRecord()
: BasicRecord(),
  recordID(""),
  ModelPath(""),
  Name(""),
  InventoryIcon(""),
  //light data
  Weight(0.0f),
  Value(0),
  Time(0),
  Radius(0),
  Red(0),
  Green(0),
  Blue(0),
  Zero(0),
  LightFlags(0),
  //end of light data
  ScriptID(""),
  SoundID("")
{}

bool LightRecord::equals(const LightRecord& other) const
{
  return ((recordID==other.recordID) and (ModelPath==other.ModelPath)
      and (Name==other.Name) and (InventoryIcon==other.InventoryIcon)
      and (Weight==other.Weight) and (Value==other.Value) and (Time==other.Time)
      and (Radius==other.Radius) and (Red==other.Red) and (Green==other.Green)
      and (Blue==other.Blue) and (Zero==other.Zero)
      and (LightFlags==other.LightFlags) and (ScriptID==other.ScriptID)
      and (SoundID==other.SoundID));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool LightRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cLIGH, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* LHDT */ +4 /* 4 bytes for length */ +24 /* size of light data */;
  if (!Name.empty())
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
          +Name.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (!InventoryIcon.empty())
  {
    Size = Size +4 /* ITEX */ +4 /* 4 bytes for length */
          +InventoryIcon.length()+1 /* length of icon +1 byte for NUL termination */;
  }
  if (!ScriptID.empty())
  {
    Size = Size +4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */;
  }
  if (!SoundID.empty())
  {
    Size = Size +4 /* SNAM */ +4 /* 4 bytes for length */
          +SoundID.length()+1 /* length of sound ID +1 byte for NUL termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Lights:
    NAME = ID string
    MODL = NIF model name
    FNAM = Item name (optional)
    ITEX = Inventory icon (optional)
    LHDT = Light data (24 bytes)
        float Weight
        long  Value
        long  Time
        long  Radius
        byte  Red	}
        byte  Green	}
        byte  Blue	} long ColorRef?
        byte  Null	}
        long  Flags (0x0001 = Dynamic,0x0002 = Can Carry,0x0004 = Negative,
                     0x0008 = Flicker,0x0010 = Fire,0x0020 = Off Default,
                     0x0040 = Flicker Slow,0x0080 = Pulse,0x0100 = Pulse Slow)
    SCRI = Script name (optional)
    SNAM = Sound name (optional?) */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength = recordID.length()+1;//length of ID plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write light's ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  SubLength = ModelPath.length()+1;//length of path plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write light's model path
  output.write(ModelPath.c_str(), SubLength);

  if (!Name.empty())
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    SubLength = Name.length()+1;//length of name plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write light's "real" name
    output.write(Name.c_str(), SubLength);
  }
  if (!InventoryIcon.empty())
  {
    //write ITEX
    output.write((const char*) &cITEX, 4);
    //ITEX's length
    SubLength = InventoryIcon.length()+1;//length of icon path plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write inventory icon
    output.write(InventoryIcon.c_str(), SubLength);
  }

  //write LHDT
  output.write((const char*) &cLHDT, 4);
  //LHDT's length
  SubLength = 24;//fixed length of 24 bytes, always
  output.write((const char*) &SubLength, 4);
  //write light data
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &Time, 4);
  output.write((const char*) &Radius, 4);
  output.write((const char*) &Red, 1);
  output.write((const char*) &Green, 1);
  output.write((const char*) &Blue, 1);
  output.write((const char*) &Zero, 1);
  output.write((const char*) &LightFlags, 4);

  if (!ScriptID.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    //SCRI's length
    SubLength = ScriptID.length()+1;//length of script ID + one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }
  if (!SoundID.empty())
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    SubLength = SoundID.length()+1;//length of sound ID + one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write sound ID
    output.write(SoundID.c_str(), SubLength);
  }
  return output.good();
}
#endif

bool LightRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Lights:
    NAME = ID string
    MODL = NIF model name
    FNAM = Item name (optional)
    ITEX = Inventory icon (optional)
    LHDT = Light data (24 bytes)
        float Weight
        long  Value
        long  Time
        long  Radius
        byte  Red	}
        byte  Green	}
        byte  Blue	} long ColorRef?
        byte  Null	}
        long  Flags (0x0001 = Dynamic,0x0002 = Can Carry,0x0004 = Negative,
                     0x0008 = Flicker,0x0010 = Fire,0x0020 = Off Default,
                     0x0040 = Flicker Slow,0x0080 = Pulse,0x0100 = Pulse Slow)
    SCRI = Script name (optional)
    SNAM = Sound name (optional?) */

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of LIGH is longer than 255 bytes.\n";
    return false;
  }
  //read light's ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of LIGH.\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord MODL of LIGH is longer than 255 bytes.\n";
    return false;
  }
  //read light's model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of LIGH.\n";
    return false;
  }
  ModelPath = std::string(Buffer);

  //read optional FNAM sub-record
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cFNAM)
  {
    //FNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord FNAM of LIGH is longer than 255 bytes.\n";
      return false;
    }
    //read light's "real" name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FNAM of LIGH.\n";
      return false;
    }
    Name = std::string(Buffer);

    //read next (optional) sub-record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    Name.clear();
  }

  //read ITEX
  if (SubRecName==cITEX)
  {
    //ITEX's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord ITEX of LIGH is longer than 255 bytes.\n";
      return false;
    }
    //read light's inventory icon path
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord ITEX of LIGH.\n";
      return false;
    }
    InventoryIcon = std::string(Buffer);

    //read next sub-record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    InventoryIcon.clear();
  }

  //read LHDT (already read before)
  if (SubRecName!=cLHDT)
  {
    UnexpectedRecord(cLHDT, SubRecName);
    return false;
  }
  //LHDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=24)
  {
    std::cout <<"Error: sub record LHDT of LIGH has invalid length ("<<SubLength
              <<" bytes). Should be 24 bytes.\n";
    return false;
  }
  //read light data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Time, 4);
  in_File.read((char*) &Radius, 4);
  in_File.read((char*) &Red, 1);
  in_File.read((char*) &Green, 1);
  in_File.read((char*) &Blue, 1);
  in_File.read((char*) &Zero, 1);
  in_File.read((char*) &LightFlags, 4);
  BytesRead += 24;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LHDT of LIGH.\n";
    return false;
  }

  ScriptID.clear();
  SoundID.clear();
  if (BytesRead<Size)
  {
    //read optional SCRI
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName==cSCRI)
    {
      //SCRI's length
      in_File.read((char*) &SubLength, 4);
      BytesRead += 4;
      if (SubLength>255)
      {
        std::cout << "Error: subrecord SCRI of LIGH is longer than 255 bytes.\n";
        return false;
      }
      //read light's script ID
      memset(Buffer, '\0', 256);
      in_File.read(Buffer, SubLength);
      BytesRead += SubLength;
      if (!in_File.good())
      {
        std::cout << "Error while reading subrecord SCRI of LIGH.\n";
        return false;
      }
      ScriptID = std::string(Buffer);

      //check for next optional record
      if (BytesRead<Size)
      {
        //read SNAM
        in_File.read((char*) &SubRecName, 4);
        BytesRead += 4;
      }
    }

    if (BytesRead<Size)
    {
      //read next (optional) record SNAM
      //already read above
      if (SubRecName!=cSNAM)
      {
        UnexpectedRecord(cSNAM, SubRecName);
        return false;
      }
      //SNAM's length
      in_File.read((char*) &SubLength, 4);
      BytesRead += 4;
      if (SubLength>255)
      {
        std::cout << "Error: subrecord SNAM of LIGH is longer than 255 bytes.\n";
        return false;
      }
      //read light's sound name
      memset(Buffer, '\0', 256);
      in_File.read(Buffer, SubLength);
      BytesRead += SubLength;
      if (!in_File.good())
      {
        std::cout << "Error while reading subrecord SNAM of LIGH.\n";
        return false;
      }
      SoundID = std::string(Buffer);
    }
  }
  return in_File.good();
}

} //namespace
