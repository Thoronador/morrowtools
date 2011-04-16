/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "CreatureRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

CreatureRecord::CreatureRecord()
{
  CreatureID = Model = Name = SoundGenCreature = "";
  //creature data
  CreatureType = 0;
  Level = 0;
  Strength = Intelligence = Willpower = Agility = 0;
  Speed = Endurance = Personality = Luck = 0;
  Health = SpellPoints = Fatigue = 0;
  Soul = 0;
  Combat = Magic = Stealth = 0;
  AttackMin1 = AttackMax1 = AttackMin2 = AttackMax2 = AttackMin3 = AttackMax3 = 0;
  Gold = 0;
  //end of creature data
  CreatureFlag = 0;
  Items.clear();
  CreatureSpells.clear();
  AIData.clear();
  AIPackages.clear();
  Destinations.clear();
  ScriptID = "";
  Scale = 1.0f;
}

CreatureRecord::~CreatureRecord()
{
  removeAIPackages();
}

CreatureRecord& CreatureRecord::operator=(const CreatureRecord& source)
{
  if (this==&source) return *this;
  CreatureID = source.CreatureID;
  Model = source.Model;
  Name = source.Name;
  SoundGenCreature = source.SoundGenCreature;
  //creature data
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
  //end of creature data
  CreatureFlag = source.CreatureFlag;
  Items = source.Items;
  CreatureSpells = source.CreatureSpells;
  AIData = source.AIData;
  //AI packages
  removeAIPackages();
  unsigned int i;
  NPC_BasicAIPackage* pkgPtr;
  for (i=0; i<source.AIPackages.size(); ++i)
  {
    switch (source.AIPackages[i]->getPackageType())
    {
      case ptActivate:
           pkgPtr = new NPC_AIActivate;
           *pkgPtr = *(static_cast<NPC_AIActivate*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptEscort:
           pkgPtr = new NPC_AIEscort;
           *pkgPtr = *(static_cast<NPC_AIEscort*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptFollow:
           pkgPtr = new NPC_AIFollow;
           *pkgPtr = *(static_cast<NPC_AIFollow*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptTravel:
           pkgPtr = new NPC_AITravel;
           *pkgPtr = *(static_cast<NPC_AITravel*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptWander:
           pkgPtr = new NPC_AIWander;
           *pkgPtr = *(static_cast<NPC_AIWander*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
    }//swi
  }//for
  Destinations = source.Destinations;
  ScriptID = source.ScriptID;
  Scale = source.Scale;
  return *this;
}

bool CreatureRecord::equals(const CreatureRecord& other) const
{
  return ((CreatureID==other.CreatureID) and (Model==other.Model)
      and (Name==other.Name) and (SoundGenCreature==other.SoundGenCreature)
      and (CreatureType==other.CreatureType) and (Level==other.Level)
      and (Strength==other.Strength) and (Intelligence==other.Intelligence)
      and (Willpower==other.Willpower) and (Agility==other.Agility)
      and (Speed==other.Speed) and (Endurance==other.Endurance)
      and (Personality==other.Personality) and (Luck==other.Luck)
      and (Health==other.Health) and (SpellPoints==other.SpellPoints)
      and (Fatigue==other.Fatigue) and (Soul==other.Soul)
      and (Combat==other.Combat) and (Magic==other.Magic)
      and (Stealth==other.Stealth) and (AttackMin1==other.AttackMin1)
      and (AttackMax1==other.AttackMax1) and (AttackMin2==other.AttackMin2)
      and (AttackMax2==other.AttackMax2) and (AttackMin3==other.AttackMin3)
      and (AttackMax3==other.AttackMax3) and (Gold==other.Gold)
      and (CreatureFlag==other.CreatureFlag) and (Items==other.Items)
      and (CreatureSpells==other.CreatureSpells) and (AIData==other.AIData)
      and (hasEqualAIPackages(other)) and (Destinations==other.Destinations)
      and (ScriptID==other.ScriptID) and (Scale==other.Scale));
}

bool CreatureRecord::saveToStream(std::ofstream& output) const
{
  #warning Not implemented yet!
  return false;
}

bool CreatureRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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

  int32_t SubRecName;
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
    std::cout << "Error: subrecord NAME of CREA is longer than 255 characters.\n";
    return false;
  }
  //read creature ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of CREA!\n";
    return false;
  }
  CreatureID = std::string(Buffer);

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
    std::cout << "Error: subrecord MODL of CREA is longer than 255 characters.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of CREA!\n";
    return false;
  }
  Model = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cCNAM)
  {
    //CNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord CNAM of CREA is longer than 255 characters.\n";
      return false;
    }
    //read creature's sound gen creature ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNAM of CREA!\n";
      return false;
    }
    SoundGenCreature = std::string(Buffer);

    //read FNAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }//if SoundGenCreature subrecord
  else
  {
    SoundGenCreature = "";
  }

  //read FNAM
  //was already read above
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord FNAM of CREA is longer than 255 characters.\n";
    return false;
  }
  //read creature's name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of CREA!\n";
    return false;
  }
  Name = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cSCRI)
  {
    //SCRI's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord SCRI of CREA is longer than 255 characters.\n";
      return false;
    }
    //read creature's script ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SCRI of CREA!\n";
      return false;
    }
    ScriptID = std::string(Buffer);

    //read NPDT
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }//if Script subrecord
  else
  {
    ScriptID = "";
  }

  //read NPDT
  //was already read above
  if (SubRecName!=cNPDT)
  {
    UnexpectedRecord(cNPDT, SubRecName);
    return false;
  }
  //NPDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=96)
  {
    std::cout << "Error: Subrecord NPDT of CREA has invalid length ("<<SubLength
              << "bytes), should be 96 bytes.\n";
    return false;
  }
  //read creature data
  in_File.read((char*) &CreatureType, 4);
  in_File.read((char*) &Level, 4);
  in_File.read((char*) &Strength, 4);
  in_File.read((char*) &Intelligence, 4);
  in_File.read((char*) &Willpower, 4);
  in_File.read((char*) &Agility, 4);
  in_File.read((char*) &Speed, 4);
  in_File.read((char*) &Endurance, 4);
  in_File.read((char*) &Personality, 4);
  in_File.read((char*) &Luck, 4);
  in_File.read((char*) &Health, 4);
  in_File.read((char*) &SpellPoints, 4);
  in_File.read((char*) &Fatigue, 4);
  in_File.read((char*) &Soul, 4);
  in_File.read((char*) &Combat, 4);
  in_File.read((char*) &Magic, 4);
  in_File.read((char*) &Stealth, 4);
  in_File.read((char*) &AttackMin1, 4);
  in_File.read((char*) &AttackMax1, 4);
  in_File.read((char*) &AttackMin2, 4);
  in_File.read((char*) &AttackMax2, 4);
  in_File.read((char*) &AttackMin3, 4);
  in_File.read((char*) &AttackMax3, 4);
  in_File.read((char*) &Gold, 4);
  BytesRead += 96;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NPDT of CREA!\n";
    return false;
  }

  //read FLAG
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFLAG)
  {
    UnexpectedRecord(cFLAG, SubRecName);
    return false;
  }
  //FLAG's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: Subrecord FLAG of CREA has invalid length ("<<SubLength
              << "bytes), should be four bytes.\n";
    return false;
  }
  //read creature flag
  in_File.read((char*) &CreatureFlag, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FLAG of CREA!\n";
    return false;
  }

  Items.clear();
  ItemRecord temp;
  TravelDestination tempDest;
  AIData.clear();
  removeAIPackages();
  Destinations.clear();
  NPC_AIActivate* activatePointer = NULL;
  NPC_AIEscortFollow* escortFollowPointer = NULL;
  NPC_AITravel* travelPointer = NULL;
  NPC_AIWander* wanderPointer = NULL;
  Scale = 1.0f;
  bool hasAIDT = false;
  bool hasXSCL = false;
  bool hasReadDestination = false;
  int32_t previousSubRecord = 0;

  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cAIDT:
           if (hasAIDT)
           {
             std::cout << "Error: record CREA seems to have two AIDT subrecords.\n";
             return false;
           }
           //AIDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=12)
           {
             std::cout << "Error: Subrecord AIDT of CREA has invalid length ("<<SubLength
                       << " bytes). Should be 12 bytes.\n";
             return false;
           }
           //read AI data
           in_File.read((char*) &(AIData.Hello), 1);
           in_File.read((char*) &(AIData.Unknown1), 1);
           in_File.read((char*) &(AIData.Fight), 1);
           in_File.read((char*) &(AIData.Flee), 1);
           in_File.read((char*) &(AIData.Alarm), 1);
           in_File.read((char*) &(AIData.Unknown2), 1);
           in_File.read((char*) &(AIData.Unknown3), 1);
           in_File.read((char*) &(AIData.Unknown4), 1);
           in_File.read((char*) &(AIData.Flags), 4);
           BytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AIDT of CREA!\n";
             return false;
           }
           AIData.isPresent = true;
           hasAIDT = true;
           previousSubRecord = cAIDT;
           break;
      case cAI_A:
           //AI_A's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=33)
           {
             std::cout << "Error: Subrecord AI_A of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 33 bytes.\n";
             return false;
           }
           //read AI activate data
           activatePointer = new NPC_AIActivate;
           // ---- read target ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           // ---- reset flag
           in_File.read((char*) &(activatePointer->Reset), 1);
           BytesRead += 33;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_A of CREA!\n";
             delete activatePointer;
             return false;
           }
           activatePointer->TargetID = std::string(Buffer);
           AIPackages.push_back(activatePointer);
           previousSubRecord = cAI_A;
           activatePointer = NULL; //just to be safe
           break;
      case cAI_E:
           //AI_E's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=48)
           {
             std::cout << "Error: Subrecord AI_E of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 48 bytes.\n";
             return false;
           }
           //read AI escort data
           escortFollowPointer = new NPC_AIEscort;
           in_File.read((char*) &(escortFollowPointer->X), 4);
           in_File.read((char*) &(escortFollowPointer->Y), 4);
           in_File.read((char*) &(escortFollowPointer->Z), 4);
           in_File.read((char*) &(escortFollowPointer->Duration), 2);
           // ---- read target ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           escortFollowPointer->TargetID = std::string(Buffer);
           in_File.read((char*) &(escortFollowPointer->Reset), 2);
           BytesRead += 48;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_E of CREA!\n";
             delete escortFollowPointer;
             return false;
           }
           escortFollowPointer->CellName = "";
           AIPackages.push_back(escortFollowPointer);
           previousSubRecord = cAI_E;
           escortFollowPointer = NULL; //just to be safe
           break;
      case cAI_F:
           //AI_F's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=48)
           {
             std::cout << "Error: Subrecord AI_F of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 48 bytes.\n";
             return false;
           }
           //read AI follow data
           escortFollowPointer = new NPC_AIFollow;
           in_File.read((char*) &(escortFollowPointer->X), 4);
           in_File.read((char*) &(escortFollowPointer->Y), 4);
           in_File.read((char*) &(escortFollowPointer->Z), 4);
           in_File.read((char*) &(escortFollowPointer->Duration), 2);
           // ---- read target ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           escortFollowPointer->TargetID = std::string(Buffer);
           in_File.read((char*) &(escortFollowPointer->Reset), 2);
           BytesRead += 48;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_F of CREA!\n";
             delete escortFollowPointer;
             return false;
           }
           escortFollowPointer->CellName = "";
           AIPackages.push_back(escortFollowPointer);
           previousSubRecord = cAI_F;
           escortFollowPointer = NULL; //just to be safe
           break;
      case cAI_T:
           //AI_T's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=16)
           {
             std::cout << "Error: Subrecord AI_T of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 16 bytes.\n";
             return false;
           }
           //read AI travel data
           travelPointer = new NPC_AITravel;
           in_File.read((char*) &(travelPointer->X), 4);
           in_File.read((char*) &(travelPointer->Y), 4);
           in_File.read((char*) &(travelPointer->Z), 4);
           in_File.read((char*) &(travelPointer->Reset), 4);
           BytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_T of CREA!\n";
             delete travelPointer;
             return false;
           }
           AIPackages.push_back(travelPointer);
           previousSubRecord = cAI_T;
           travelPointer = NULL; //just to be safe later
           break;
      case cAI_W:
           //AI_W's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=14)
           {
             std::cout << "Error: Subrecord AI_W of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 14 bytes.\n";
             return false;
           }
           //read AI wander data
           wanderPointer = new NPC_AIWander;
           in_File.read((char*) &(wanderPointer->Distance), 2);
           in_File.read((char*) &(wanderPointer->Duration), 2);
           in_File.read((char*) &(wanderPointer->Time), 1);
           in_File.read((char*) (wanderPointer->Idle), 8);
           in_File.read((char*) &(wanderPointer->Reset), 1);
           BytesRead += 14;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_W of CREA!\n";
             delete wanderPointer;
             return false;
           }
           AIPackages.push_back(wanderPointer);
           previousSubRecord = cAI_W;
           wanderPointer = NULL; //just to be safe later
           break;
      case cNPCO:
           //NPCO's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=36)
           {
             std::cout << "Error: Subrecord NPCO of CREA has invalid length ("
                       << SubLength<<" bytes). Should be 36 bytes.\n";
             return false;
           }
           //read item data
           // ---- read count
           in_File.read((char*) &(temp.Count), 4);
           // ---- read item ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           BytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NPCO of CREA!\n";
             return false;
           }
           temp.Item = std::string(Buffer);
           Items.push_back(temp);
           previousSubRecord = cNPCO;
           break;
      case cNPCS:
           //NPCS's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=32)
           {
             std::cout << "Error: Subrecord NPCS of CREA has invalid length ("
                       << SubLength<<" bytes). Should be 32 bytes.\n";
             return false;
           }
           //read spell ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           BytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NPCS of CREA!\n";
             return false;
           }
           CreatureSpells.push_back(std::string(Buffer));
           previousSubRecord = cNPCS;
           break;
      case cCNDT:
           //CNDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord CNDT of CREA is longer than 255 characters.\n";
             return false;
           }
           //read escort/follow cell name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNDT of CREA!\n";
             return false;
           }
           if ((previousSubRecord==cAI_E) or (previousSubRecord==cAI_F))
           {
             //last record was AI escort or follow, so set it's cell name
             (static_cast<NPC_AIEscortFollow*>(AIPackages.back()))->CellName = std::string(Buffer);
           }
           else
           {
             std::cout << "Error: Subrecord before CNDT of CREA was neither "
                       << "AI_E nor AI_F, but \""<<IntTo4Char(previousSubRecord)
                       << "\".\n";
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
           //DODT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=24)
           {
             std::cout << "Error: Subrecord DODT of CREA has invalid length ("
                       << SubLength<< " bytes). Should be 24 bytes.\n";
             return false;
           }
           //read destination data
           in_File.read((char*) &(tempDest.XPos), 4);
           in_File.read((char*) &(tempDest.YPos), 4);
           in_File.read((char*) &(tempDest.ZPos), 4);
           in_File.read((char*) &(tempDest.XRot), 4);
           in_File.read((char*) &(tempDest.YRot), 4);
           in_File.read((char*) &(tempDest.ZRot), 4);
           BytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DODT of CREA!\n";
             return false;
           }
           tempDest.CellName = "";
           hasReadDestination = true;
           previousSubRecord = cDODT;
           break;
      case cDNAM:
           if (!hasReadDestination)
           {
             std::cout << "Error while reading CREA record: DNAM subrecord "
                       << "without previous DODT subrecord.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord DNAM of CREA is longer than 255 characters.\n";
             return false;
           }
           //read destination data
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of CREA!\n";
             return false;
           }
           tempDest.CellName = std::string(Buffer);
           //push record
           Destinations.push_back(tempDest);
           hasReadDestination = false;
           previousSubRecord = cDNAM;
           break;
      case cXSCL:
           if (hasXSCL)
           {
             std::cout << "Error: record CREA seems to have two XSCL subrecords.\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: Subrecord XSCL of CREA has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read scale
           in_File.read((char*) &Scale, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSCL of CREA!\n";
             return false;
           }
           hasXSCL = true;
           previousSubRecord = cXSCL;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     << "\" found. Expected AIDT, AI_A, AI_E, AI_F, AI_T, AI_W,"
                     << " CNDT, DODT, DNAM, NPCO, NPCS, or XSCL.\n";
           return false;
    }//swi
  }//while
  //check for destination record
  if (hasReadDestination)
  {
    Destinations.push_back(tempDest);
    hasReadDestination = false;
  }

  return in_File.good();
}

bool CreatureRecord::hasEqualAIPackages(const CreatureRecord& other) const
{
  const size_t len = AIPackages.size();
  if (other.AIPackages.size()!=len) return false;
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    if ((AIPackages.at(i)==NULL) xor (other.AIPackages.at(i)==NULL))
      return false;
    if (AIPackages.at(i)!=NULL)
    {
      //Do they have the same type?
      if (AIPackages.at(i)->getPackageType()!=other.AIPackages.at(i)->getPackageType())
      {
        return false;
      }
      switch (AIPackages.at(i)->getPackageType())
      {
        case ptActivate:
             if (!(dynamic_cast<NPC_AIActivate*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIActivate*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptEscort:
        case ptFollow:
             if (!(dynamic_cast<NPC_AIEscortFollow*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIEscortFollow*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptTravel:
             if (!(dynamic_cast<NPC_AITravel*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AITravel*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptWander:
             if (!(dynamic_cast<NPC_AIWander*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIWander*>(other.AIPackages.at(i))))
               return false;
             break;
      }//swi
    }//not NULL
  }//for
  return true;
}

void CreatureRecord::removeAIPackages()
{
  while (!AIPackages.empty())
  {
    delete AIPackages.back();
    AIPackages.pop_back();
  }//while
}
