/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022, 2023, 2025  Dirk Stolle

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

#include "NPCRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

/* **** functions for NPCRecord ****/

const uint32_t nfFemale = 0x00000001;
const uint32_t nfEssential = 0x00000002;
const uint32_t nfRespawn = 0x00000004;
const uint32_t nfAutoCalcStats = 0x00000010;
const uint32_t nfSkeletonWhiteBloodTex = 0x00000400;
const uint32_t nfMetalGoldBloodTex = 0x00000800;

/*FLAG = NPC Flags (4 bytes, long)
        0x0001 = Female, 0x0002 = Essential, 0x0004 = Respawn, 0x0008 = None? (seems to be set always)
        0x0010 = Autocalc, 0x0400 = Blood Skel, 0x0800 = Blood Metal*/

NPCRecord::NPCRecord()
: PreNPCRecord(),
  recordID(""), Name(""), ModelPath(""), RaceID(""), FactionID(""), HeadModel(""), ClassID(""),
  HairModel(""), ScriptID(""),
  // NPC data
  Level(0),
  Strength(0), Intelligence(0), Willpower(0), Agility(0), Speed(0), Endurance(0),
  Personality(0), Luck(0),
  Skills({ 0, 0, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  Reputation(0),
  Health(0), SpellPoints(0), Fatigue(0),
  Disposition(0), Data_FactionID(0),
  Rank(0),
  Unknown1(0), Unknown2(0), Unknown3(0),
  Gold(0),
  NPCDataType(NPDT_Type::Small12Bytes),
  // end of NPC data
  NPC_Flag(0)
{
}

NPCRecord::~NPCRecord()
{
  removeAIPackages();
}

NPCRecord& NPCRecord::operator=(const NPCRecord& source)
{
  if (this == &source)
    return *this;

  recordID = source.recordID;
  Name = source.Name;
  ModelPath = source.ModelPath;
  RaceID = source.RaceID;
  FactionID = source.FactionID;
  HeadModel = source.HeadModel;
  ClassID = source.ClassID;
  HairModel = source.HairModel;
  ScriptID = source.ScriptID;
  //NPC data
  Level = source.Level;
  Strength = source.Strength;
  Intelligence = source.Intelligence;
  Willpower =source.Willpower;
  Agility = source.Agility;
  Speed = source.Speed;
  Endurance = source.Endurance;
  Personality =source.Personality;
  Luck = source.Luck;
  Skills = source.Skills;
  Reputation = source.Reputation;
  Health = source.Health;
  SpellPoints = source.SpellPoints;
  Fatigue = source.Fatigue;
  Disposition = source.Disposition;
  Data_FactionID = source.Data_FactionID;
  Rank = source.Rank;
  Unknown1 = source.Unknown1;
  Unknown2 = source.Unknown2;
  Unknown3 = source.Unknown3;
  Gold = source.Gold;
  NPCDataType = source.NPCDataType;
  //end of NPC data
  NPC_Flag = source.NPC_Flag;
  Items = source.Items;
  NPC_Spells = source.NPC_Spells;
  AIData = source.AIData;
  removeAIPackages();
  copyAIPackages(source);
  Destinations = source.Destinations;
  return *this;
}

bool NPCRecord::equals(const NPCRecord& other) const
{
  return (recordID == other.recordID) && (Name == other.Name)
      && (ModelPath == other.ModelPath) && (RaceID == other.RaceID)
      && (FactionID == other.FactionID) && (HeadModel == other.HeadModel)
      && (ClassID == other.ClassID) && (HairModel == other.HairModel)
      && (ScriptID == other.ScriptID)
      && (Level == other.Level) && (Strength == other.Strength)
      && (Intelligence == other.Intelligence) && (Willpower == other.Willpower)
      && (Agility == other.Agility) && (Speed == other.Speed)
      && (Endurance == other.Endurance) && (Personality == other.Personality)
      && (Luck == other.Luck) && (Skills == other.Skills)
      && (Reputation == other.Reputation) && (Health == other.Health)
      && (SpellPoints == other.SpellPoints) && (Fatigue == other.Fatigue)
      && (Disposition == other.Disposition) && (Data_FactionID == other.Data_FactionID)
      && (Rank == other.Rank) && (Unknown1 == other.Unknown1)
      && (Unknown2 == other.Unknown2) && (Unknown3 == other.Unknown3)
      && (Gold == other.Gold) && (NPCDataType == other.NPCDataType)
      && (NPC_Flag == other.NPC_Flag) && (Items == other.Items)
      && (NPC_Spells == other.NPC_Spells) && (AIData == other.AIData)
      && (hasEqualAIPackages(other))
      && (Destinations == other.Destinations);
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t NPCRecord::getWriteSize() const
{
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL terminator */
        + 4 /* RNAM */ + 4 /* 4 bytes for length */
        + RaceID.length() + 1 /* length of race ID +1 byte for NUL */
        + 4 /* ANAM */ + 4 /* 4 bytes for length */
        + FactionID.length() + 1 /* length of faction ID +1 byte for NUL */
        + 4 /* BNAM */ + 4 /* 4 bytes for length */
        + HeadModel.length() + 1 /* length of head model +1 byte for NUL */
        + 4 /* KNAM */ + 4 /* 4 bytes for length */
        + HairModel.length() + 1 /* length of hair model +1 byte for NUL */
        + 4 /* NPDT */ + 4 /* 4 bytes for length */;
  switch (NPCDataType)
  {
    case NPDT_Type::Small12Bytes:
         Size += 12; //fixed length of 12 bytes
         break;
    case NPDT_Type::Large52Bytes:
         Size += 52; //fixed length of 52 bytes
         break;
  }
  Size += 4 /* FLAG */ + 4 /* 4 bytes for length */ + 4 /* fixed length of four bytes */
        + Items.size() * (4 /* NPCO */ + 4 /* 4 bytes for length */ + 36 /* fixed length of 36 bytes */)
        + NPC_Spells.size() * (4 /* NPCS */ + 4 /* 4 bytes for length */ + 32 /* fixed length of 32 bytes */);

  // add size of the optional stuff
  if (!Name.empty())
  {
    Size += 4 /* FNAM */ + 4 /* 4 bytes for length */
          + Name.length() + 1 /* length of name +1 byte for NUL terminator */;
  }
  if (!ModelPath.empty())
  {
    Size += 4 /* MODL */ + 4 /* 4 bytes for length */
          + ModelPath.length() + 1 /* length of path +1 byte for NUL */;
  }
  if (!ClassID.empty())
  {
    Size += 4 /* CNAM */ + 4 /* 4 bytes for length */
          + ClassID.length() + 1 /* length of class ID +1 byte for NUL */;
  }
  if (!ScriptID.empty())
  {
    Size += 4 /* SCRI */ + 4 /* 4 bytes for length */
          + ScriptID.length() + 1 /* length of script+1 byte for NUL */;
  }
  if (AIData.has_value())
  {
    Size += 4 /* AIDT */ + 4 /* 4 bytes for length */ + 12 /* fixed: 12 bytes */;
  }

  // AI packages
  for (const auto& package: AIPackages)
  {
    Size += package->getStreamSize();
  }

  // travel service destinations
  for (const auto& dest: Destinations)
  {
    Size += dest.getStreamSize();
  }
  return Size;
}

bool NPCRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cNPC_), 4);
  const uint32_t Size = getWriteSize();
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);
  /*NPCs:
    NAME = NPC ID string
    FNAM = NPC name (optional, e.g. todwendy in DV)
    MODL = Animation file (optional)
    RNAM = Race Name    }
    ANAM = Faction name } Required, even if empty
    BNAM = Head model   }
    CNAM = Class name (optional)
    KNAM = Hair model   }
    SCRI = Script ID (optional)
    NPDT = NPC Data (12 bytes or 52 bytes?)
        short Level
        byte  Strength
		byte  Intelligence
		byte  Willpower
		byte  Agility
		byte  Speed
		byte  Endurance
		byte  Personality
		byte  Luck
		byte  Skills[27]  } According to the skillID (0-26)
		byte  Reputation
		short Health
		short SpellPts
		short Fatigue
		byte  Disposition
		byte  FactionID
		byte  Rank
		byte  Unknown1
		long  Gold

		12 byte Version
		short Level
		byte  Disposition
		byte  FactionID?
		byte  Rank
		byte  Unknown1
		byte  Unknown2
		byte  Unknown3
		long  Gold?
    FLAG = NPC Flags (4 bytes, long)
        0x0001 = Female, 0x0002 = Essential, 0x0004 = Respawn, 0x0008 = None?
        0x0010 = Autocalc, 0x0400 = Blood Skel, 0x0800 = Blood Metal
    NPCO = NPC item (36 bytes, occurs 0+ times)
        long    Count     Number of the item
        char    Name[32]  The ID of the item
    NPCS = NPC spell (32 bytes, occurs 0+ times)
        char    Name[32]  The ID of the item
    AIDT = AI data (12 bytes)
        byte Hello
        byte Unknown1
        byte Fight
        byte Flee
        byte Alarm
        byte Unknown2
        byte Unknown3
        byte Unknown4
        long Flags
            0x00001 = Weapon, 0x00002 = Armor, 0x00004 = Clothing,
            0x00008 = Books, 0x00010 = Ingredients, 0x00020 = Picks,
            0x00040 = Probes, 0x00080 = Lights, 0x00100 = Apparatus,
            0x00200 = Repair, 0x00400 = Misc, 0x00800 = Spells,
            0x01000 = Magic Items, 0x02000 = Potions, 0x04000 = Training,
            0x08000 = Spellmaking, 0x10000 = Enchanting, 0x20000 = Repair Item
	AI_W = AI bytes (14 bytes)
		short Distance
		short Duration
		byte  TimeOfDay
		byte  Idle[8]
		byte  Unknown (1?)
	AI_T = AI Travel (16 bytes)
		float X
		float Y
		float Z
		long  Unknown (1?)
    AI_F = AI Follow (48 bytes)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_E = AI Escort (48 bytes)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
	CNDT = Cell escort/follow to string (optional)
	AI_A = AI Activate (33 bytes)
		char Name[32]
		byte Unknown (1?)
	DODT = Cell Travel Destination
		float XPos
		float YPos
		float ZPos
		float XRot
		float YRot
		float ZRot
	DNAM = Cell name for previous DODT, if interior
	XSCL = Scale (4 bytes, float, optional)
		Only present if the scale is not 1.0 */

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  if (!ModelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    SubLength = ModelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ModelPath.c_str(), SubLength);
  }

  if (!Name.empty())
  {
    // write NPC's name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  // write race ID (RNAM)
  output.write(reinterpret_cast<const char*>(&cRNAM), 4);
  SubLength = RaceID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(RaceID.c_str(), SubLength);

  if (!ClassID.empty())
  {
    // write class ID (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    SubLength = ClassID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ClassID.c_str(), SubLength);
  }

  // write faction ID (ANAM)
  output.write(reinterpret_cast<const char*>(&cANAM), 4);
  SubLength = FactionID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(FactionID.c_str(), SubLength);

  // write head model (BNAM)
  output.write(reinterpret_cast<const char*>(&cBNAM), 4);
  SubLength = HeadModel.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(HeadModel.c_str(), SubLength);

  // write hair model (KNAM)
  output.write(reinterpret_cast<const char*>(&cKNAM), 4);
  SubLength = HairModel.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(HairModel.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write NPC data (NPDT)
  output.write(reinterpret_cast<const char*>(&cNPDT), 4);
  switch (NPCDataType)
  {
    case NPDT_Type::Small12Bytes:
         SubLength = 12;
         break;
    case NPDT_Type::Large52Bytes:
         SubLength = 52;
         break;
  }
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write NPC data
  if (SubLength == 52)
  {
    // ---- level
    output.write(reinterpret_cast<const char*>(&Level), 2);
    // ---- attributes
    output.write(reinterpret_cast<const char*>(&Strength), 1);
    output.write(reinterpret_cast<const char*>(&Intelligence), 1);
    output.write(reinterpret_cast<const char*>(&Willpower), 1);
    output.write(reinterpret_cast<const char*>(&Agility), 1);
    output.write(reinterpret_cast<const char*>(&Speed), 1);
    output.write(reinterpret_cast<const char*>(&Endurance), 1);
    output.write(reinterpret_cast<const char*>(&Personality), 1);
    output.write(reinterpret_cast<const char*>(&Luck), 1);
    // ---- skills
    output.write(reinterpret_cast<const char*>(Skills.data()), 27);
    // ---- reputations
    output.write(reinterpret_cast<const char*>(&Reputation), 1);
    // ---- secondary attributes
    output.write(reinterpret_cast<const char*>(&Health), 2);
    output.write(reinterpret_cast<const char*>(&SpellPoints), 2);
    output.write(reinterpret_cast<const char*>(&Fatigue), 2);
    // ---- disposition
    output.write(reinterpret_cast<const char*>(&Disposition), 1);
    // ---- faction stuff
    output.write(reinterpret_cast<const char*>(&Data_FactionID), 1);
    output.write(reinterpret_cast<const char*>(&Rank), 1);
    // ---- others
    output.write(reinterpret_cast<const char*>(&Unknown1), 1);
    output.write(reinterpret_cast<const char*>(&Gold), 4);
  }
  else
  {
    // 12 byte version
    // ---- level
    output.write(reinterpret_cast<const char*>(&Level), 2);
    // ---- disposition
    output.write(reinterpret_cast<const char*>(&Disposition), 1);
    // ---- faction stuff
    output.write(reinterpret_cast<const char*>(&Data_FactionID), 1);
    output.write(reinterpret_cast<const char*>(&Rank), 1);
    // ---- others
    output.write(reinterpret_cast<const char*>(&Unknown1), 1);
    output.write(reinterpret_cast<const char*>(&Unknown2), 1);
    output.write(reinterpret_cast<const char*>(&Unknown3), 1);
    output.write(reinterpret_cast<const char*>(&Gold), 4);
  }

  // write flags (FLAG)
  output.write(reinterpret_cast<const char*>(&cFLAG), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&NPC_Flag), 4);

  // items and spells, AI data, AI packages, travel service destinations
  writeItemsSpellsAIDataDestinations(output);

  return output.good();
}
#endif

bool NPCRecord::loadFromStream(std::istream& input)
{
  /* TODO #1: Not completely implemented yet!
              The XSCL subrecord is still missing.
     TODO #2: This function is a bit messy and could need a partial rewrite.
  */
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);
  /*NPCs:
    NAME = NPC ID string
    FNAM = NPC name (optional, e.g. todwendy in DV)
    MODL = Animation file (optional)
    RNAM = Race Name    }
    ANAM = Faction name } Required, even if empty
    BNAM = Head model   }
    CNAM = Class name (optional)
    KNAM = Hair model   }
    SCRI = Script ID (optional)
    NPDT = NPC Data (12 bytes or 52 bytes?)
        short Level
        byte  Strength
        byte  Intelligence
        byte  Willpower
        byte  Agility
        byte  Speed
        byte  Endurance
        byte  Personality
        byte  Luck
        byte  Skills[27]  } According to the skillID (0-26)
        byte  Reputation
        short Health
        short SpellPts
        short Fatigue
        byte  Disposition
        byte  FactionID
        byte  Rank
        byte  Unknown1
        long  Gold

        12 byte Version
        short Level
        byte  Disposition
        byte  FactionID?
        byte  Rank
        byte  Unknown1
        byte  Unknown2
        byte  Unknown3
        long  Gold?
    FLAG = NPC Flags (4 bytes, long)
        0x0001 = Female, 0x0002 = Essential, 0x0004 = Respawn, 0x0008 = None?
        0x0010 = Autocalc, 0x0400 = Blood Skel, 0x0800 = Blood Metal
    NPCO = NPC item (36 bytes, occurs 0+ times)
        long    Count     Number of the item
        char    Name[32]  The ID of the item
    NPCS = NPC spell (32 bytes, occurs 0+ times)
        char    Name[32]  The ID of the item
    AIDT = AI data (12 bytes)
        byte Hello
        byte Unknown1
        byte Fight
        byte Flee
        byte Alarm
        byte Unknown2
        byte Unknown3
        byte Unknown4
        long Flags
            0x00001 = Weapon, 0x00002 = Armor, 0x00004 = Clothing,
            0x00008 = Books, 0x00010 = Ingredients, 0x00020 = Picks,
            0x00040 = Probes, 0x00080 = Lights, 0x00100 = Apparatus,
            0x00200 = Repair, 0x00400 = Misc, 0x00800 = Spells,
            0x01000 = Magic Items, 0x02000 = Potions, 0x04000 = Training,
            0x08000 = Spellmaking, 0x10000 = Enchanting, 0x20000 = Repair Item
    AI_W = AI bytes (14 bytes, occurs 0+ times)
        short Distance
        short Duration
        byte  TimeOfDay
        byte  Idle[8]
        byte  Unknown (1?)
    AI_T = AI Travel (16 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        long  Unknown (1?)
    AI_F = AI Follow (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_E = AI Escort (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    CNDT = Cell escort/follow to string (optional)
    AI_A = AI Activate (33 bytes, occurs 0+ times)
        char Name[32]
        byte Unknown (1?)
    DODT = Cell Travel Destination
        float XPos
        float YPos
        float ZPos
        float XRot
        float YRot
        float ZRot
    DNAM = Cell name for previous DODT, if interior
    XSCL = Scale (4 bytes, float, optional)
        Only present if the scale is not 1.0 */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of NPC_!\n";
    return false;
  }

  bool hasANAM = false;
  bool hasBNAM = false;
  bool hasCNAM = false;
  bool hasFNAM = false;
  bool hasKNAM = false;
  bool hasMODL = false;
  bool hasNPDT = false;
  bool hasRNAM = false;
  bool hasSCRI = false;

  ScriptID.clear();
  ClassID.clear();
  RaceID.clear();
  Name.clear();
  ModelPath.clear();

  while (!(hasANAM && hasBNAM && hasCNAM && hasKNAM && hasNPDT && hasRNAM))
  {
    // read next sub record
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cANAM:
           if (hasANAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two ANAM sub records.\n";
             return false;
           }
           // read faction name (ANAM)
           if (!loadString256(input, FactionID, Buffer, cANAM, BytesRead))
           {
             std::cerr << "Error while reading sub record ANAM of NPC_!\n";
             return false;
           }
           hasANAM = true;
           break;
      case cBNAM:
           if (hasBNAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two BNAM sub records.\n";
             return false;
           }
           // read head model (BNAM)
           if (!loadString256(input, HeadModel, Buffer, cBNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record BNAM of NPC_!\n";
             return false;
           }
           hasBNAM = true;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two CNAM sub records.\n";
             return false;
           }
           // read class ID (CNAM)
           if (!loadString256(input, ClassID, Buffer, cCNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record CNAM of NPC_!\n";
             return false;
           }
           hasCNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two FNAM sub records.\n";
             return false;
           }
           // NPC's name (FNAM)
           if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record FNAM of NPC_!\n";
             return false;
           }
           hasFNAM = true;
           break;
      case cKNAM:
           if (hasKNAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two KNAM sub records.\n";
             return false;
           }
           // read hair model (KNAM)
           if (!loadString256(input, HairModel, Buffer, cKNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record KNAM of NPC_!\n";
             return false;
           }
           hasKNAM = true;
           break;
      case cMODL:
           if (hasMODL)
           {
             std::cerr << "Error: Record NPC_ seems to have two MODL sub records.\n";
             return false;
           }
           // read model (MODL)
           if (!loadString256(input, ModelPath, Buffer, cMODL, BytesRead))
           {
             std::cerr << "Error while reading sub record MODL of NPC_!\n";
             return false;
           }
           hasMODL = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cerr << "Error: Record NPC_ seems to have two RNAM sub records.\n";
             return false;
           }
           // read race name (RNAM)
           if (!loadString256(input, RaceID, Buffer, cRNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record RNAM of NPC_!\n";
             return false;
           }
           hasRNAM = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cerr << "Error: Record NPC_ seems to have two SCRI sub records.\n";
             return false;
           }
           // read script ID (SCRI)
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading sub record SCRI of NPC_!\n";
             return false;
           }
           hasSCRI = true;
           break;
      case cNPDT:
           if (hasNPDT)
           {
             std::cerr << "Error: Record NPC_ seems to have two NPDT sub records.\n";
             return false;
           }
           // NPDT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if ((SubLength != 52) && (SubLength != 12))
           {
             std::cerr << "Error: Sub record NPDT of NPC_ has invalid length ("
                       << SubLength << " bytes). Should be 12 or 52 bytes.\n";
             return false;
           }
           // read NPC data
           if (SubLength == 52)
           {
             // ---- level
             input.read(reinterpret_cast<char*>(&Level), 2);
             // ---- attributes
             input.read(reinterpret_cast<char*>(&Strength), 1);
             input.read(reinterpret_cast<char*>(&Intelligence), 1);
             input.read(reinterpret_cast<char*>(&Willpower), 1);
             input.read(reinterpret_cast<char*>(&Agility), 1);
             input.read(reinterpret_cast<char*>(&Speed), 1);
             input.read(reinterpret_cast<char*>(&Endurance), 1);
             input.read(reinterpret_cast<char*>(&Personality), 1);
             input.read(reinterpret_cast<char*>(&Luck), 1);
             // ---- skills
             input.read(reinterpret_cast<char*>(Skills.data()), 27);
             // ---- reputations
             input.read(reinterpret_cast<char*>(&Reputation), 1);
             // ---- secondary attributes
             input.read(reinterpret_cast<char*>(&Health), 2);
             input.read(reinterpret_cast<char*>(&SpellPoints), 2);
             input.read(reinterpret_cast<char*>(&Fatigue), 2);
             // ---- disposition
             input.read(reinterpret_cast<char*>(&Disposition), 1);
             // ---- faction stuff
             input.read(reinterpret_cast<char*>(&Data_FactionID), 1);
             input.read(reinterpret_cast<char*>(&Rank), 1);
             // ---- others
             input.read(reinterpret_cast<char*>(&Unknown1), 1);
             input.read(reinterpret_cast<char*>(&Gold), 4);
             NPCDataType = NPDT_Type::Large52Bytes;
           }
           else
           {
             // 12 byte version
             // ---- level
             input.read(reinterpret_cast<char*>(&Level), 2);
             // ---- disposition
             input.read(reinterpret_cast<char*>(&Disposition), 1);
             // ---- faction stuff
             input.read(reinterpret_cast<char*>(&Data_FactionID), 1);
             input.read(reinterpret_cast<char*>(&Rank), 1);
             // ---- others
             input.read(reinterpret_cast<char*>(&Unknown1), 1);
             input.read(reinterpret_cast<char*>(&Unknown2), 1);
             input.read(reinterpret_cast<char*>(&Unknown3), 1);
             input.read(reinterpret_cast<char*>(&Gold), 4);
             NPCDataType = NPDT_Type::Small12Bytes;
             // Reset attributes and skills to zero, because we have no data.
             // TODO: Maybe the auto-calculation feature can be implemented to
             //       fill those with the same data that Morrowind sets them to.
             Strength = 0;
             Intelligence = 0;
             Willpower = 0;
             Agility = 0;
             Speed = 0;
             Endurance = 0;
             Personality = 0;
             Luck = 0;
             Skills.fill(0);
           }
           BytesRead += SubLength; // should be 12 or 52 bytes
           if (!input.good())
           {
             std::cerr << "Error while reading sub record NPDT of NPC_!\n";
             return false;
           }
           hasNPDT = true;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected ANAM, BNAM, CNAM, FNAM or KNAM.\n";
           return false;
    }
  }

  // read flag bits (FLAG)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  // FLAG's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record FLAG of NPC_ has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  // read NPC flag
  input.read(reinterpret_cast<char*>(&NPC_Flag), 4);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record FLAG of NPC_!\n";
    return false;
  }

  // items, spells and AI data
  Items.clear();
  ItemRecord temp;
  NPC_Spells.clear();
  AIData.reset();
  removeAIPackages();
  Destinations.clear();

  bool hasReadDestination = false;
  TravelDestination tempDest;

  NPC_AIActivate* activatePointer = nullptr;
  NPC_AIEscortFollow* escortFollowPointer = nullptr;
  NPC_AITravel* travelPointer = nullptr;
  NPC_AIWander* wanderPointer = nullptr;
  uint32_t previousSubRecord = 0;

  while (BytesRead < Size)
  {
    // read next sub record
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cNPCO:
           if (!temp.loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record NPCO of NPC_!\n";
             return false;
           }
           Items.emplace_back(temp);
           previousSubRecord = cNPCO;
           break;
      case cNPCS:
           // NPCS's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 32)
           {
             std::cerr << "Error: Sub record NPCS of NPC_ has invalid length ("
                       << SubLength << " bytes). Should be 32 bytes.\n";
             return false;
           }
           // read spell ID
           memset(Buffer, '\0', 33);
           input.read(Buffer, 32);
           BytesRead += 32;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record NPCS of NPC_!\n";
             return false;
           }
           NPC_Spells.push_back(std::string(Buffer));
           previousSubRecord = cNPCS;
           break;
      case cAIDT:
           if (AIData.has_value())
           {
             std::cerr << "Error: Record NPC_ seems to have two AIDT sub records.\n";
             return false;
           }
           // read AI data
           AIData = NPC_AIData();
           if (!AIData.value().loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AIDT of NPC_!\n";
             return false;
           }
           previousSubRecord = cAIDT;
           break;
      case cAI_A:
           // read AI activate data (AI_A)
           activatePointer = new NPC_AIActivate;
           if (!activatePointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_A of NPC_!\n";
             delete activatePointer;
             return false;
           }
           AIPackages.push_back(activatePointer);
           activatePointer = nullptr;
           previousSubRecord = cAI_A;
           break;
      case cAI_E:
           // read AI escort data (AI_E)
           escortFollowPointer = new NPC_AIEscort;
           if (!escortFollowPointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_E of NPC_!\n";
             delete escortFollowPointer;
             return false;
           }
           AIPackages.push_back(escortFollowPointer);
           escortFollowPointer = nullptr; // just to be safe
           previousSubRecord = cAI_E;
           break;
      case cAI_F:
           // read AI follow data (AI_F)
           escortFollowPointer = new NPC_AIFollow;
           if (!escortFollowPointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_F of NPC_!\n";
             delete escortFollowPointer;
             return false;
           }
           AIPackages.push_back(escortFollowPointer);
           escortFollowPointer = nullptr; // just to be safe
           previousSubRecord = cAI_F;
           break;
      case cAI_T:
           // read AI travel data (AI_T)
           travelPointer = new NPC_AITravel;
           if (!travelPointer->loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_T of NPC_!\n";
             delete travelPointer;
             return false;
           }
           AIPackages.push_back(travelPointer);
           travelPointer = nullptr; // just to be safe later
           previousSubRecord = cAI_T;
           break;
      case cAI_W:
           // read AI wander data (AI_W)
           wanderPointer = new NPC_AIWander;
           if (!wanderPointer->loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_W of NPC_!\n";
             delete wanderPointer;
             return false;
           }
           AIPackages.push_back(wanderPointer);
           wanderPointer = nullptr; // just to be safe later
           previousSubRecord = cAI_W;
           break;
      case cCNDT:
           if ((previousSubRecord != cAI_E) && (previousSubRecord != cAI_F))
           {
             std::cerr << "Error: Sub record before CNDT of NPC_ was neither "
                       << "AI_E nor AI_F, but \"" << IntTo4Char(previousSubRecord)
                       << "\".\n";
             return false;
           }
           // previous record was AI escort or follow, so set it's cell name
           if (!loadString256(input, static_cast<NPC_AIEscortFollow*>(AIPackages.back())->CellName, Buffer, cCNDT, BytesRead))
           {
             std::cerr << "Error while reading sub record CNDT of NPC_!\n";
             return false;
           }
           previousSubRecord = cCNDT;
           break;
      case cDODT:
           if (hasReadDestination)
           {
             Destinations.push_back(tempDest);
             hasReadDestination = false;
           }
           // read destination data
           if (!tempDest.partialLoadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record DODT of NPC_!\n";
             return false;
           }
           hasReadDestination = true;
           previousSubRecord = cDODT;
           break;
      case cDNAM:
           if (!hasReadDestination)
           {
             std::cerr << "Error while reading NPC record: DNAM sub record "
                       << "without previous DODT sub record.\n";
             return false;
           }
           // read destination cell name (DNAM)
           if (!loadString256(input, tempDest.CellName, Buffer, cDNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record DNAM of NPC_!\n";
             return false;
           }
           // push record
           Destinations.push_back(tempDest);
           hasReadDestination = false;
           previousSubRecord = cDNAM;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected NPCO, NPCS, AIDT, AI_E, AI_F, AI_T,"
                     << " AI_W, DODT or DNAM.\n";
           return false;
    }
  }
  // check for destination record
  if (hasReadDestination)
  {
    Destinations.push_back(tempDest);
    hasReadDestination = false;
  }

  return input.good();
}

bool NPCRecord::isFemale() const
{
  return (nfFemale & NPC_Flag) != 0;
}

bool NPCRecord::isEssential() const
{
  return (nfEssential & NPC_Flag) != 0;
}

bool NPCRecord::doesRespawn() const
{
  return (nfRespawn & NPC_Flag) != 0;
}

bool NPCRecord::hasAutoCalcStats() const
{
  return (nfAutoCalcStats & NPC_Flag) != 0;
}

bool NPCRecord::hasWhiteBloodTex() const
{
  return (nfSkeletonWhiteBloodTex & NPC_Flag) != 0;
}

bool NPCRecord::hasGoldBloodTex() const
{
  return (nfMetalGoldBloodTex & NPC_Flag) != 0;
}

} // namespace
