/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "CreatureRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

CreatureRecord::CreatureRecord()
: PreNPCRecord(),
  recordID(""), ModelPath(""), Name(""), SoundGenCreature(""),
  // creature data
  CreatureType(0),
  Level(0),
  Strength(0), Intelligence(0), Willpower(0), Agility(0),
  Speed(0), Endurance(0), Personality(0), Luck(0),
  Health(0), SpellPoints(0), Fatigue(0),
  Soul(0),
  Combat(0), Magic(0), Stealth(0),
  AttackMin1(0), AttackMax1(0), AttackMin2(0), AttackMax2(0), AttackMin3(0), AttackMax3(0),
  Gold(0),
  // end of creature data
  CreatureFlag(0),
  ScriptID(""),
  Scale(1.0f)
{
}

CreatureRecord::~CreatureRecord()
{
  removeAIPackages();
}

CreatureRecord& CreatureRecord::operator=(const CreatureRecord& source)
{
  if (this == &source)
    return *this;
  recordID = source.recordID;
  ModelPath = source.ModelPath;
  Name = source.Name;
  SoundGenCreature = source.SoundGenCreature;
  // creature data
  CreatureType = source.CreatureType;
  Level = source.Level;
  Strength = source.Strength;
  Intelligence = source.Intelligence;
  Willpower = source.Willpower;
  Agility = source.Agility;
  Speed = source.Speed;
  Endurance = source.Endurance;
  Personality = source.Personality;
  Luck = source.Luck;
  Health = source.Health;
  SpellPoints = source.SpellPoints;
  Fatigue = source.Fatigue;
  Soul = source.Soul;
  Combat = source.Combat;
  Magic = source.Magic;
  Stealth = source.Stealth;
  AttackMin1 = source.AttackMin1;
  AttackMax1 = source.AttackMax1;
  AttackMin2 = source.AttackMin2;
  AttackMax2 = source.AttackMax2;
  AttackMin3 = source.AttackMin3;
  AttackMax3 = source.AttackMax3;
  Gold = source.Gold;
  // end of creature data
  CreatureFlag = source.CreatureFlag;
  Items = source.Items;
  NPC_Spells = source.NPC_Spells;
  AIData = source.AIData;
  // AI packages
  removeAIPackages();
  copyAIPackages(source);
  Destinations = source.Destinations;
  ScriptID = source.ScriptID;
  Scale = source.Scale;
  return *this;
}

bool CreatureRecord::equals(const CreatureRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (SoundGenCreature == other.SoundGenCreature)
      && (CreatureType == other.CreatureType) && (Level == other.Level)
      && (Strength == other.Strength) && (Intelligence == other.Intelligence)
      && (Willpower == other.Willpower) && (Agility == other.Agility)
      && (Speed == other.Speed) && (Endurance == other.Endurance)
      && (Personality == other.Personality) && (Luck == other.Luck)
      && (Health == other.Health) && (SpellPoints == other.SpellPoints)
      && (Fatigue == other.Fatigue) && (Soul == other.Soul)
      && (Combat == other.Combat) && (Magic == other.Magic)
      && (Stealth == other.Stealth) && (AttackMin1 == other.AttackMin1)
      && (AttackMax1 == other.AttackMax1) && (AttackMin2 == other.AttackMin2)
      && (AttackMax2 == other.AttackMax2) && (AttackMin3 == other.AttackMin3)
      && (AttackMax3 == other.AttackMax3) && (Gold == other.Gold)
      && (CreatureFlag == other.CreatureFlag) && (Items == other.Items)
      && (NPC_Spells == other.NPC_Spells) && (AIData == other.AIData)
      && (hasEqualAIPackages(other)) && (Destinations == other.Destinations)
      && (ScriptID == other.ScriptID) && (Scale == other.Scale);
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t CreatureRecord::getWriteSize() const
{
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ +4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of model path +1 byte for NUL */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* NPDT */ + 4 /* 4 bytes for length */ + 96 /* fixed length of 96 bytes */
        + 4 /* FLAG */ + 4 /* 4 bytes for length */ + 4 /* fixed length of 96 bytes */
        + Items.size() * (4 /* NPCO */ + 4 /* 4 bytes for length */ + 36 /* fixed length of 36 bytes */)
        + NPC_Spells.size()*(4 /* NPCS */ + 4 /* 4 bytes for length */ + 32 /* fixed length of 32 bytes */);
  if (!SoundGenCreature.empty())
  {
    Size += 4 /* CNAM */ + 4 /* 4 bytes for length */
          + SoundGenCreature.length() + 1 /* length of ID +1 byte for NUL */;
  }
  if (AIData.has_value())
  {
    Size += 4 /* AIDT */ + 4 /* 4 bytes for length */ + 12 /* fixed length of 12 bytes */;
  }
  if (!ScriptID.empty())
  {
    Size += 4 /* SCRI */ + 4 /* 4 bytes for length */
          + ScriptID.length() + 1 /* length of ID +1 byte for NUL terminator */;
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
  // scale?
  if (Scale != 1.0f)
  {
    Size += 4 /* XSCL */ + 4 /* 4 bytes for length */ + 4 /* fixed: 4 bytes */;
  }
  return Size;
}
bool CreatureRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCREA), 4);
  const uint32_t Size = getWriteSize();
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Creature:
    NAME = ID
	MODL = NIF Model
	CNAM = Sound Gen Creature ID (optional)
	FNAM = Creature name
	SCRI = Script (optional)
	NPDT = Creature data, 96 bytes
		long Type
			0 = Creature
			1 = Daedra
			2 = Undead
			3 = Humanoid
		long Level
		long Strength
		long Intelligence
		long Willpower
		long Agility
		long Speed
		long Endurance
		long Personality
		long Luck
		long Health
		long SpellPts
		long Fatigue
		long Soul
		long Combat
		long Magic
		long Stealth
		long AttackMin1
		long AttackMax1
		long AttackMin2
		long AttackMax2
		long AttackMin3
		long AttackMax3
		long Gold
	FLAG = Creature Flags (4 bytes, bit field)
        0x0001 = Biped, 0x0002 = Respawn, 0x0004 = Weapon and shield,
        0x0008 = None, 0x0010 = Swims, 0x0020 = Flies, 0x0040 = Walks
        0x0048 = Default flags, 0x0080 = Essential, 0x0400 = Skeleton Blood
        0x0800 = Metal Blood
    NPCO = Item record (36 bytes, 0+ times)
        long    Count     Number of the item
        char    Name[32]  The ID of the item
    NPCS = creature spell (32 bytes, occurs 0+ times)
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
    AI_E = AI Escort (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_F = AI Follow (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_T = AI Travel (16 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        long  Unknown (1?)
    AI_W = AI Wander (14 bytes)
        short Distance
        byte  Duration
        byte  TimeOfDay
        byte  Idle[10]
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
        Only present if the scale is not 1.0
  */

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  if (!SoundGenCreature.empty())
  {
    // write sound gen creature's ID (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    SubLength = SoundGenCreature.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoundGenCreature.c_str(), SubLength);
  }

  // write creature's displayed name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write creature data (NPDT)
  output.write(reinterpret_cast<const char*>(&cNPDT), 4);
  SubLength = 96;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write creature data
  output.write(reinterpret_cast<const char*>(&CreatureType), 4);
  output.write(reinterpret_cast<const char*>(&Level), 4);
  output.write(reinterpret_cast<const char*>(&Strength), 4);
  output.write(reinterpret_cast<const char*>(&Intelligence), 4);
  output.write(reinterpret_cast<const char*>(&Willpower), 4);
  output.write(reinterpret_cast<const char*>(&Agility), 4);
  output.write(reinterpret_cast<const char*>(&Speed), 4);
  output.write(reinterpret_cast<const char*>(&Endurance), 4);
  output.write(reinterpret_cast<const char*>(&Personality), 4);
  output.write(reinterpret_cast<const char*>(&Luck), 4);
  output.write(reinterpret_cast<const char*>(&Health), 4);
  output.write(reinterpret_cast<const char*>(&SpellPoints), 4);
  output.write(reinterpret_cast<const char*>(&Fatigue), 4);
  output.write(reinterpret_cast<const char*>(&Soul), 4);
  output.write(reinterpret_cast<const char*>(&Combat), 4);
  output.write(reinterpret_cast<const char*>(&Magic), 4);
  output.write(reinterpret_cast<const char*>(&Stealth), 4);
  output.write(reinterpret_cast<const char*>(&AttackMin1), 4);
  output.write(reinterpret_cast<const char*>(&AttackMax1), 4);
  output.write(reinterpret_cast<const char*>(&AttackMin2), 4);
  output.write(reinterpret_cast<const char*>(&AttackMax2), 4);
  output.write(reinterpret_cast<const char*>(&AttackMin3), 4);
  output.write(reinterpret_cast<const char*>(&AttackMax3), 4);
  output.write(reinterpret_cast<const char*>(&Gold), 4);

  // write flag (FLAG)
  output.write(reinterpret_cast<const char*>(&cFLAG), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&CreatureFlag), SubLength);

  if (Scale != 1.0f)
  {
    // write scale (XSCL)
    output.write(reinterpret_cast<const char*>(&cXSCL), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&Scale), SubLength);
  }

  // items and spells, AI data, AI packages, travel service destinations
  writeItemsSpellsAIDataDestinations(output);

  return output.good();
}
#endif

bool CreatureRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Creature:
    NAME = ID
	MODL = NIF Model
	CNAM = Sound Gen Creature ID (optional)
	FNAM = Creature name
	SCRI = Script (optional)
	NPDT = Creature data, 96 bytes
		long Type
			0 = Creature
			1 = Daedra
			2 = Undead
			3 = Humanoid
		long Level
		long Strength
		long Intelligence
		long Willpower
		long Agility
		long Speed
		long Endurance
		long Personality
		long Luck
		long Health
		long SpellPts
		long Fatigue
		long Soul
		long Combat
		long Magic
		long Stealth
		long AttackMin1
		long AttackMax1
		long AttackMin2
		long AttackMax2
		long AttackMin3
		long AttackMax3
		long Gold
	FLAG = Creature Flags (4 bytes, bit field)
        0x0001 = Biped, 0x0002 = Respawn, 0x0004 = Weapon and shield,
        0x0008 = None, 0x0010 = Swims, 0x0020 = Flies, 0x0040 = Walks
        0x0048 = Default flags, 0x0080 = Essential, 0x0400 = Skeleton Blood
        0x0800 = Metal Blood
    NPCO = Item record (36 bytes, 0+ times)
        long    Count     Number of the item
        char    Name[32]  The ID of the item
    NPCS = creature spell (32 bytes, occurs 0+ times)
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
    AI_E = AI Escort (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_F = AI Follow (48 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        short Duration
        char  ID[32]
        short Unknown (0100?)
    AI_T = AI Travel (16 bytes, occurs 0+ times)
        float X
        float Y
        float Z
        long  Unknown (1?)
    AI_W = AI Wander (14 bytes)
        short Distance
        byte  Duration
        byte  TimeOfDay
        byte  Idle[10]
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
        Only present if the scale is not 1.0
  */

  uint32_t BytesRead = 0;

  // read creature ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of CREA!\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading sub record MODL of CREA!\n";
    return false;
  }

  // read next sub record
  uint32_t SubRecName = 0;
  input.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName == cCNAM)
  {
    // read creature's sound gen creature ID (CNAM)
    if (!loadString256(input, SoundGenCreature, Buffer, cCNAM, BytesRead))
    {
      std::cerr << "Error while reading sub record CNAM of CREA!\n";
      return false;
    }

    // read FNAM
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    SoundGenCreature.clear();
  }

  // read FNAM
  // header was already read above
  if (SubRecName != cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  // read creature's name (FNAM)
  if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record FNAM of CREA!\n";
    return false;
  }

  // read next sub record
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cSCRI)
  {
    // read creature's script ID (SCRI)
    if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
    {
      std::cerr << "Error while reading sub record SCRI of CREA!\n";
      return false;
    }

    // read NPDT
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    ScriptID.clear();
  }

  // read NPDT
  // was already read above
  if (SubRecName != cNPDT)
  {
    UnexpectedRecord(cNPDT, SubRecName);
    return false;
  }
  // NPDT's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 96)
  {
    std::cerr << "Error: Sub record NPDT of CREA has invalid length ("
              << SubLength << " bytes), should be 96 bytes.\n";
    return false;
  }
  // read creature data
  input.read(reinterpret_cast<char*>(&CreatureType), 4);
  input.read(reinterpret_cast<char*>(&Level), 4);
  input.read(reinterpret_cast<char*>(&Strength), 4);
  input.read(reinterpret_cast<char*>(&Intelligence), 4);
  input.read(reinterpret_cast<char*>(&Willpower), 4);
  input.read(reinterpret_cast<char*>(&Agility), 4);
  input.read(reinterpret_cast<char*>(&Speed), 4);
  input.read(reinterpret_cast<char*>(&Endurance), 4);
  input.read(reinterpret_cast<char*>(&Personality), 4);
  input.read(reinterpret_cast<char*>(&Luck), 4);
  input.read(reinterpret_cast<char*>(&Health), 4);
  input.read(reinterpret_cast<char*>(&SpellPoints), 4);
  input.read(reinterpret_cast<char*>(&Fatigue), 4);
  input.read(reinterpret_cast<char*>(&Soul), 4);
  input.read(reinterpret_cast<char*>(&Combat), 4);
  input.read(reinterpret_cast<char*>(&Magic), 4);
  input.read(reinterpret_cast<char*>(&Stealth), 4);
  input.read(reinterpret_cast<char*>(&AttackMin1), 4);
  input.read(reinterpret_cast<char*>(&AttackMax1), 4);
  input.read(reinterpret_cast<char*>(&AttackMin2), 4);
  input.read(reinterpret_cast<char*>(&AttackMax2), 4);
  input.read(reinterpret_cast<char*>(&AttackMin3), 4);
  input.read(reinterpret_cast<char*>(&AttackMax3), 4);
  input.read(reinterpret_cast<char*>(&Gold), 4);
  BytesRead += 96;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record NPDT of CREA!\n";
    return false;
  }

  // read FLAG
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  //FLAG's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record FLAG of CREA has invalid length ("
              << SubLength << " bytes), should be four bytes.\n";
    return false;
  }
  // read creature flag
  input.read(reinterpret_cast<char*>(&CreatureFlag), 4);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record FLAG of CREA!\n";
    return false;
  }

  Items.clear();
  ItemRecord temp;
  TravelDestination tempDest;
  AIData.reset();
  removeAIPackages();
  Destinations.clear();
  NPC_AIActivate* activatePointer = nullptr;
  NPC_AIEscortFollow* escortFollowPointer = nullptr;
  NPC_AITravel* travelPointer = nullptr;
  NPC_AIWander* wanderPointer = nullptr;
  Scale = 1.0f;
  bool hasXSCL = false;
  bool hasReadDestination = false;
  uint32_t previousSubRecord = 0;

  while (BytesRead < Size)
  {
    // read next sub record
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cAIDT:
           if (AIData.has_value())
           {
             std::cerr << "Error: Record CREA seems to have two AIDT sub records.\n";
             return false;
           }
           // read AI data (AIDT)
           AIData = NPC_AIData();
           if (!AIData.value().loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AIDT of CREA!\n";
             return false;
           }
           previousSubRecord = cAIDT;
           break;
      case cAI_A:
           // read AI activate data
           activatePointer = new NPC_AIActivate;
           if (!activatePointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_A of CREA!\n";
             delete activatePointer;
             return false;
           }
           AIPackages.push_back(activatePointer);
           previousSubRecord = cAI_A;
           activatePointer = nullptr; // just to be safe
           break;
      case cAI_E:
           // read AI escort data (AI_E)
           escortFollowPointer = new NPC_AIEscort;
           if (!escortFollowPointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_E of CREA!\n";
             delete escortFollowPointer;
             return false;
           }
           AIPackages.push_back(escortFollowPointer);
           previousSubRecord = cAI_E;
           escortFollowPointer = nullptr; // just to be safe
           break;
      case cAI_F:
           // read AI follow data (AI_F)
           escortFollowPointer = new NPC_AIFollow;
           if (!escortFollowPointer->loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_F of CREA!\n";
             delete escortFollowPointer;
             return false;
           }
           AIPackages.push_back(escortFollowPointer);
           previousSubRecord = cAI_F;
           escortFollowPointer = nullptr; // just to be safe
           break;
      case cAI_T:
           // read AI travel data (AI_T)
           travelPointer = new NPC_AITravel;
           if (!travelPointer->loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_T of CREA!\n";
             delete travelPointer;
             return false;
           }
           AIPackages.push_back(travelPointer);
           previousSubRecord = cAI_T;
           travelPointer = nullptr; // just to be safe later
           break;
      case cAI_W:
           // read AI wander data (AI_W)
           wanderPointer = new NPC_AIWander;
           if (!wanderPointer->loadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record AI_W of CREA!\n";
             delete wanderPointer;
             return false;
           }
           AIPackages.push_back(wanderPointer);
           previousSubRecord = cAI_W;
           wanderPointer = nullptr; // just to be safe later
           break;
      case cNPCO:
           if (!temp.loadFromStream(input, Buffer, BytesRead))
           {
             std::cerr << "Error while reading subrecord NPCO of CREA!\n";
             return false;
           }
           Items.emplace_back(temp);
           previousSubRecord = cNPCO;
           break;
      case cNPCS:
           //NPCS's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 32)
           {
             std::cerr << "Error: Sub record NPCS of CREA has invalid length ("
                       << SubLength << " bytes). Should be 32 bytes.\n";
             return false;
           }
           // read spell ID
           memset(Buffer, '\0', 33);
           input.read(Buffer, 32);
           BytesRead += 32;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record NPCS of CREA!\n";
             return false;
           }
           NPC_Spells.push_back(std::string(Buffer));
           previousSubRecord = cNPCS;
           break;
      case cCNDT:
           if ((previousSubRecord != cAI_E) && (previousSubRecord != cAI_F))
           {
             std::cerr << "Error: Sub record before CNDT of CREA was neither "
                       << "AI_E nor AI_F, but \"" << IntTo4Char(previousSubRecord)
                       << "\".\n";
             return false;
           }
           // read escort/follow cell name (CNDT)
           if (!loadString256(input, static_cast<NPC_AIEscortFollow*>(AIPackages.back())->CellName, Buffer, cCNDT, BytesRead))
           {
             std::cerr << "Error while reading sub record CNDT of CREA!\n";
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
           // read destination data (DODT)
           if (!tempDest.partialLoadFromStream(input, BytesRead))
           {
             std::cerr << "Error while reading sub record DODT of CREA!\n";
             return false;
           }
           hasReadDestination = true;
           previousSubRecord = cDODT;
           break;
      case cDNAM:
           if (!hasReadDestination)
           {
             std::cerr << "Error while reading CREA record: DNAM sub record "
                       << "without previous DODT sub record.\n";
             return false;
           }
           // read destination data cell name (DNAM)
           if (!loadString256(input, tempDest.CellName, Buffer, cDNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record DNAM of CREA!\n";
             return false;
           }
           // push record
           Destinations.push_back(tempDest);
           hasReadDestination = false;
           previousSubRecord = cDNAM;
           break;
      case cXSCL:
           if (hasXSCL)
           {
             std::cerr << "Error: Record CREA seems to have two XSCL sub records.\n";
             return false;
           }
           // XSCL's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Sub record XSCL of CREA has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read scale
           input.read(reinterpret_cast<char*>(&Scale), 4);
           BytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record XSCL of CREA!\n";
             return false;
           }
           hasXSCL = true;
           previousSubRecord = cXSCL;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected AIDT, AI_A, AI_E, AI_F, AI_T, AI_W,"
                     << " CNDT, DODT, DNAM, NPCO, NPCS, or XSCL.\n";
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

} // namespace
