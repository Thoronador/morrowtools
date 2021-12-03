/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Thoronador

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

#include "LightRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

LightRecord::LightRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  InventoryIcon(std::string()),
  // light data
  Weight(0.0f),
  Value(0),
  Time(0),
  Radius(0),
  Red(0),
  Green(0),
  Blue(0),
  Zero(0),
  LightFlags(0),
  // end of light data
  ScriptID(std::string()),
  SoundID(std::string())
{}

bool LightRecord::equals(const LightRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (InventoryIcon == other.InventoryIcon)
      && (Weight == other.Weight) && (Value == other.Value) && (Time == other.Time)
      && (Radius == other.Radius) && (Red == other.Red) && (Green == other.Green)
      && (Blue == other.Blue) && (Zero == other.Zero)
      && (LightFlags == other.LightFlags) && (ScriptID == other.ScriptID)
      && (SoundID == other.SoundID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool LightRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cLIGH), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */
        + 4 /* LHDT */ + 4 /* 4 bytes for length */ + 24 /* size of light data */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for length */
          + Name.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (!InventoryIcon.empty())
  {
    Size = Size + 4 /* ITEX */ + 4 /* 4 bytes for length */
          + InventoryIcon.length() + 1 /* length of icon +1 byte for NUL */;
  }
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of script ID +1 byte for NUL */;
  }
  if (!SoundID.empty())
  {
    Size = Size + 4 /* SNAM */ + 4 /* 4 bytes for length */
          + SoundID.length() + 1 /* length of sound ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write light ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  if (!Name.empty())
  {
    // write light's displayed name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }
  if (!InventoryIcon.empty())
  {
    // write inventory icon (ITEX)
    output.write(reinterpret_cast<const char*>(&cITEX), 4);
    SubLength = InventoryIcon.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(InventoryIcon.c_str(), SubLength);
  }

  // write light data (LHDT)
  output.write(reinterpret_cast<const char*>(&cLHDT), 4);
  SubLength = 24;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Time), 4);
  output.write(reinterpret_cast<const char*>(&Radius), 4);
  output.write(reinterpret_cast<const char*>(&Red), 1);
  output.write(reinterpret_cast<const char*>(&Green), 1);
  output.write(reinterpret_cast<const char*>(&Blue), 1);
  output.write(reinterpret_cast<const char*>(&Zero), 1);
  output.write(reinterpret_cast<const char*>(&LightFlags), 4);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  if (!SoundID.empty())
  {
    // write sound ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    SubLength = SoundID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoundID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool LightRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t bytesRead = 0;

  // read record's ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, bytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of LIGH.\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, bytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of LIGH.\n";
    return false;
  }

  // read optional FNAM sub-record
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  bytesRead += 4;
  if (SubRecName == cFNAM)
  {
    // read light's "real" name
    if (!loadString256(input, Name, Buffer, cFNAM, bytesRead))
    {
      std::cerr << "Error while reading subrecord FNAM of LIGH.\n";
      return false;
    }

    // read next (optional) sub-record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    bytesRead += 4;
  }
  else
  {
    Name.clear();
  }

  // read ITEX
  if (SubRecName == cITEX)
  {
    // read light's inventory icon path (ITEX)
    if (!loadString256(input, InventoryIcon, Buffer, cITEX, bytesRead))
    {
      std::cerr << "Error while reading subrecord ITEX of LIGH.\n";
      return false;
    }

    // read next sub-record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    bytesRead += 4;
  }
  else
  {
    InventoryIcon.clear();
  }

  // read LHDT (header was already read before)
  if (SubRecName != cLHDT)
  {
    UnexpectedRecord(cLHDT, SubRecName);
    return false;
  }
  // LHDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  bytesRead += 4;
  if (SubLength != 24)
  {
    std::cerr << "Error: Sub record LHDT of LIGH has invalid length ("
              << SubLength << " bytes). Should be 24 bytes.\n";
    return false;
  }
  // read light data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Time), 4);
  input.read(reinterpret_cast<char*>(&Radius), 4);
  input.read(reinterpret_cast<char*>(&Red), 1);
  input.read(reinterpret_cast<char*>(&Green), 1);
  input.read(reinterpret_cast<char*>(&Blue), 1);
  input.read(reinterpret_cast<char*>(&Zero), 1);
  input.read(reinterpret_cast<char*>(&LightFlags), 4);
  bytesRead += 24;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord LHDT of LIGH.\n";
    return false;
  }

  ScriptID.clear();
  SoundID.clear();
  while (bytesRead < Size)
  {
    // read next subrecord header
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    bytesRead += 4;
    switch (SubRecName)
    {
      case cSCRI:
           // read light's script ID
           if (!loadString256(input, ScriptID, Buffer, cSCRI, bytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of LIGH.\n";
             return false;
           }
           break;
      case cSNAM:
           // read light's sound name
           if (!loadString256(input, SoundID, Buffer, cSNAM, bytesRead))
           {
             std::cerr << "Error while reading subrecord SNAM of LIGH.\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord '"
                     << IntTo4Char(SubRecName)
                     << "', but only SCRI or SNAM are allowed here!\n";
           return false;
    }
  }

  return input.good();
}

} // namespace
