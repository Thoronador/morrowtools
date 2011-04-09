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

#include "NPCRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

/* *** AIData's functions ****/

void NPC_AIData::clear()
{
  Hello = 0;
  Unknown1 = 0;
  Fight = 0;
  Flee = 0;
  Alarm = 0;
  Unknown2 = 0;
  Unknown3 = 0;
  Unknown4 = 0;
  Flags = 0;

  isPresent = false;
}

bool NPC_AIData::operator==(const NPC_AIData& other) const
{
  return ((isPresent==other.isPresent) and (Hello==other.Hello)
      and (Unknown1==other.Unknown1) and (Fight==other.Fight)
      and (Flee==other.Flee) and (Alarm==other.Alarm)
      and (Unknown2==other.Unknown2) and (Unknown3==other.Unknown3)
      and (Unknown4==other.Unknown4) and (Flags==other.Flags));
}

/* **** AIWander's functions ****/

void NPC_AIWander::clear()
{
  //isPresent = false;
  Distance = Duration = 0;
  Time = 0;
  Idle[0] = Idle[1] = Idle[2] = Idle[3] = Idle[4] = Idle[5] = Idle[6] =
    Idle[7] = 0;
  Reset = 0;
}

bool NPC_AIWander::equals(const NPC_AIWander& other) const
{
  return (/*(isPresent==other.isPresent) and*/ (Distance==other.Distance)
      and (Duration==other.Duration) and (Time==other.Time)
      and (Reset==other.Reset) and (memcmp(Idle, other.Idle, 8)==0));
}

PackageType NPC_AIWander::getPackageType() const
{
  return ptWander;
}

/* **** AIEscortFollow's functions ****/

void NPC_AIEscortFollow::clear()
{
  isPresent = false;
  X = Y = Z = 0.0f;
  Duration = 0;
  TargetID = "";
  Reset = 0;
}

bool NPC_AIEscortFollow::operator==(const NPC_AIEscortFollow& other) const
{
  return ((isPresent==other.isPresent) and (X==other.X) and (Y==other.Y)
      and (Z==other.Z) and (Duration==other.Duration)
      and (TargetID==other.TargetID) and (Reset==other.Reset));
}

/* **** AITravel's functions ****/

void NPC_AITravel::clear()
{
  //isPresent = false;
  X = Y = Z = 0.0f;
  Reset = 0;
}

bool NPC_AITravel::equals(const NPC_AITravel& other) const
{
  return (/*(isPresent==other.isPresent) and */(X==other.X) and (Y==other.Y)
      and (Z==other.Z) and (Reset==other.Reset));
}

PackageType NPC_AITravel::getPackageType() const
{
  return ptTravel;
}

/* **** TravelDestination's functions ****/

bool TravelDestination::operator==(const TravelDestination& other) const
{
  return ((XPos==other.XPos) and (YPos==other.YPos) and (ZPos==other.ZPos)
      and (XRot==other.XRot) and (YRot==other.YRot) and (ZRot==other.ZRot)
      and (CellName==other.CellName));
}

/***** functions for NPCRecord ****/

NPCRecord::NPCRecord()
{
  NPCID = Name = ModelPath = RaceID = FactionID = HeadModel = ClassID =
    HairModel = ScriptID = "";
  //NPC data
  Level = 0;
  Strength = Intelligence = Willpower = Agility = Speed = Endurance =
    Personality = Luck = 0;
  memset(Skills, 0, 27);
  Reputation = 0;
  Health = SpellPoints = Fatigue = 0;
  Disposition = Data_FactionID = 0;
  Rank = 0;
  Unknown1 = 0;
  Gold = 0;
  NPCDataType = ndtNone;
  //end of NPC data
  NPC_Flag = 0;
  Items.clear();
  NPC_Spells.clear();
  AIData.clear();
  AIEscort.clear();
  AIFollow.clear();
  EscortFollowCell = "";
  //AITravel.clear();
  //AIWander.clear();
  AIPackages.clear();
  Destinations.clear();
}

void NPCRecord::removeAIPackages()
{
  while (!AIPackages.empty())
  {
    delete AIPackages.back();
    AIPackages.pop_back();
  }//while
}

NPCRecord::~NPCRecord()
{
  removeAIPackages();
}

NPCRecord& NPCRecord::operator=(const NPCRecord& source)
{
  if (this==&source) return *this;

  NPCID = source.NPCID;
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
  memcpy(Skills, source.Skills, 27);
  Reputation = source.Reputation;
  Health = source.Health;
  SpellPoints = source.SpellPoints;
  Fatigue = source.Fatigue;
  Disposition = source.Disposition;
  Data_FactionID = source.Data_FactionID;
  Rank = source.Rank;
  Unknown1 = source.Unknown1;
  Gold = source.Gold;
  NPCDataType = source.NPCDataType;
  //end of NPC data
  NPC_Flag = source.NPC_Flag;
  Items = source.Items;
  NPC_Spells = source.NPC_Spells;
  AIData = source.AIData;
  AIEscort = source.AIEscort;
  AIFollow = source.AIFollow;
  EscortFollowCell = source.EscortFollowCell;
  //AITravel.clear();
  //AIWander.clear();
  removeAIPackages();
  unsigned int i;
  NPC_BasicAIPackage* pkgPtr;
  for (i=0; i<source.AIPackages.size(); ++i)
  {
    switch (source.AIPackages[i]->getPackageType())
    {
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
  return *this;
}

bool NPCRecord::equals(const NPCRecord& other) const
{
  return ((NPCID==other.NPCID) and (Name==other.Name)
      and (ModelPath==other.ModelPath) and (RaceID==other.RaceID)
      and (FactionID==other.FactionID) and (HeadModel==other.HeadModel)
      and (ClassID==other.ClassID) and (HairModel==other.HairModel)
      and (ScriptID==other.ScriptID)
      and (Level==other.Level) and (Strength==other.Strength)
      and (Intelligence==other.Intelligence) and (Willpower==other.Willpower)
      and (Agility==other.Agility) and (Speed==other.Speed)
      and (Endurance==other.Endurance) and (Personality==other.Personality)
      and (Luck==other.Luck) and (memcmp(Skills, other.Skills, 27)==0)
      and (Reputation==other.Reputation) and (Health==other.Health)
      and (SpellPoints==other.SpellPoints) and (Fatigue==other.Fatigue)
      and (Disposition==other.Disposition) and (Data_FactionID==other.Data_FactionID)
      and (Rank==other.Rank) and (Unknown1==other.Unknown1)
      and (Gold==other.Gold) and (NPCDataType==other.NPCDataType)
      and (NPC_Flag==other.NPC_Flag) and (Items==other.Items)
      and (NPC_Spells==other.NPC_Spells) and (AIData==other.AIData)
      and (AIEscort==other.AIEscort) and (AIFollow==other.AIFollow)
      and (EscortFollowCell==other.EscortFollowCell)
      //and (AITravel==other.AITravel) and (AIWander==other.AIWander)
      and (hasEqualAIPackages(other))
      and (Destinations==other.Destinations));
}

bool NPCRecord::saveToStream(std::ofstream& output) const
{
  #warning Not implemented yet!
  return false;
}

bool NPCRecord::loadFromStream(std::ifstream& in_File)
{
  #warning Not completely implemented yet!\
           The XSCL and AI_A subrecords are still missing.
  int32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);
  /*NPCs:
    NAME = NPC ID string
    FNAM = NPC name (optional, e.g. todwendy in DV)
    MODL = Animation file (optional)
    RNAM = Race Name    }
    ANAM = Faction name } Required, even if empty
    BNAM = Head model   }
    CNAM = Class name
    KNAM = Hair model   }
    SCRI = Script ID
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
		long	Count	  Number of the item
		char	Name[32]  The ID of the item
	NPCS = NPC spell (32 bytes, occurs 0+ times)
		char	Name[32]  The ID of the item
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

  int32_t SubRecName, SubLength, BytesRead;
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
    std::cout << "Error: Subrecord NAME of NPC_ is longer than 255 characters.\n";
    return false;
  }
  //read ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of NPC_!\n";
    return false;
  }
  NPCID = std::string(Buffer);

  bool hasANAM = false;
  bool hasBNAM = false;
  bool hasCNAM = false;
  bool hasFNAM = false;
  bool hasKNAM = false;
  bool hasMODL = false;
  bool hasNPDT = false;
  bool hasRNAM = false;
  bool hasSCRI = false;

  ScriptID = "";
  ClassID = "";
  RaceID = "";
  Name = "";
  ModelPath = "";

  while (!(hasANAM and hasBNAM and hasKNAM and hasNPDT and hasRNAM))
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cANAM:
           if (hasANAM)
           {
             std::cout << "Error: record NPC_ seems to have two ANAM subrecords.\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord ANAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read faction name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ANAM of NPC_!\n";
             return false;
           }
           FactionID = std::string(Buffer);
           hasANAM = true;
           break;
      case cBNAM:
           if (hasBNAM)
           {
             std::cout << "Error: record NPC_ seems to have two BNAM subrecords.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord BNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read head model
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of NPC_!\n";
             return false;
           }
           HeadModel = std::string(Buffer);
           hasBNAM = true;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cout << "Error: record NPC_ seems to have two CNAM subrecords.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord CNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read class ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of NPC_!\n";
             return false;
           }
           ClassID = std::string(Buffer);
           hasCNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cout << "Error: record NPC_ seems to have two FNAM subrecords.\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord FNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read NPC's name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of NPC_!\n";
             return false;
           }
           Name = std::string(Buffer);
           hasFNAM = true;
           break;
      case cKNAM:
           if (hasKNAM)
           {
             std::cout << "Error: record NPC_ seems to have two KNAM subrecords.\n";
             return false;
           }
           //KNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord KNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read hair model
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KNAM of NPC_!\n";
             return false;
           }
           HairModel = std::string(Buffer);
           hasKNAM = true;
           break;
      case cMODL:
           if (hasMODL)
           {
             std::cout << "Error: record NPC_ seems to have two MODL subrecords.\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord MODL of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read model
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of NPC_!\n";
             return false;
           }
           ModelPath = std::string(Buffer);
           hasMODL = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cout << "Error: record NPC_ seems to have two RNAM subrecords.\n";
             return false;
           }
           //RNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord RNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read race name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord RNAM of NPC_!\n";
             return false;
           }
           RaceID = std::string(Buffer);
           hasRNAM = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cout << "Error: record NPC_ seems to have two SCRI subrecords.\n";
             return false;
           }
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord SCRI of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read script ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of NPC_!\n";
             return false;
           }
           ScriptID = std::string(Buffer);
           hasSCRI = true;
           break;
      case cNPDT:
           if (hasNPDT)
           {
             std::cout << "Error: record NPC_ seems to have two NPDT subrecords.\n";
             return false;
           }
           //NPDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if ((SubLength!=52) and (SubLength!=12))
           {
             std::cout << "Error: Subrecord NPDT of NPC_ has invalid length ("<<SubLength
                       << " bytes). Should be 12 or 52 bytes.\n";
             return false;
           }
           //read NPC data
           //now read it
           if (SubLength==52)
           {
             // ---- level
             in_File.read((char*) &Level, 2);
             // ---- attributes
             in_File.read((char*) &Strength, 1);
             in_File.read((char*) &Intelligence, 1);
             in_File.read((char*) &Willpower, 1);
             in_File.read((char*) &Agility, 1);
             in_File.read((char*) &Speed, 1);
             in_File.read((char*) &Endurance, 1);
             in_File.read((char*) &Personality, 1);
             in_File.read((char*) &Luck, 1);
             // ---- skills
             in_File.read((char*) Skills, 27);
             // ---- reputations
             in_File.read((char*) &Reputation, 1);
             // ---- secondary attribs
             in_File.read((char*) &Health, 2);
             in_File.read((char*) &SpellPoints, 2);
             in_File.read((char*) &Fatigue, 2);
             // ---- disposition
             in_File.read((char*) &Disposition, 1);
             // ---- faction stuff
             in_File.read((char*) &Data_FactionID, 1);
             in_File.read((char*) &Rank, 1);
             // ---- others
             in_File.read((char*) &Unknown1, 1);
             in_File.read((char*) &Gold, 4);
             NPCDataType = ndt52Bytes;
           }
           else
           {
             //12 byte version
             // ---- level
             in_File.read((char*) &Level, 2);
             // ---- disposition
             in_File.read((char*) &Disposition, 1);
             // ---- faction stuff
             in_File.read((char*) &Data_FactionID, 1);
             in_File.read((char*) &Rank, 1);
             // ---- others
             in_File.read((char*) &Unknown1, 1);
             in_File.seekg(2, std::ios_base::cur);//skip the two unknown bytes
             in_File.read((char*) &Gold, 4);
             NPCDataType = ndt12Bytes;
           }
           BytesRead += SubLength; //should be 12 or 52 bytes
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NPDT of NPC_!\n";
             return false;
           }
           hasNPDT = true;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     <<"\" found. Expected ANAM, BNAM, CNAM or KNAM.\n";
           return false;
    }//swi
  }//while

  if (!hasCNAM)
  {
    //read CNAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cCNAM)
    {
      UnexpectedRecord(cCNAM, SubRecName);
      return false;
    }
    //CNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: Subrecord CNAM of NPC_ is longer than 255 characters.\n";
      return false;
    }
    //read class ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNAM of NPC_!\n";
      return false;
    }
    ClassID = std::string(Buffer);
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
    std::cout << "Error: Subrecord FLAG of NPC_ has invalid length ("<<SubLength
              << " bytes). Should be four bytes.\n";
    return false;
  }
  //read NPC flag
  in_File.read((char*) &NPC_Flag, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FLAG of NPC_!\n";
    return false;
  }

  //items, spells and AI data
  Items.clear();
  ItemRecord temp;
  NPC_Spells.clear();
  AIData.clear();
  AIEscort.clear();
  AIFollow.clear();
  EscortFollowCell = "";
  //AITravel.clear();
  //AIWander.clear();
  AIPackages.clear();
  Destinations.clear();

  bool hasAIDT = false;
  bool hasAI_E = false;
  bool hasAI_F = false;
  bool hasCNDT = false;
  //bool hasAI_T = false;
  //bool hasAI_W = false;
  bool hasReadDestination = false;
  TravelDestination tempDest;

  NPC_AITravel* travelPointer = NULL;
  NPC_AIWander* wanderPointer = NULL;

  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cNPCO:
           //NPCO's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=36)
           {
             std::cout << "Error: Subrecord NPCO of NPC_ has invalid length ("
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
             std::cout << "Error while reading subrecord NPCO of NPC_!\n";
             return false;
           }
           temp.Item = std::string(Buffer);
           Items.push_back(temp);
           break;
      case cNPCS:
           //NPCS's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=32)
           {
             std::cout << "Error: Subrecord NPCS of NPC_ has invalid length ("
                       << SubLength<<" bytes). Should be 32 bytes.\n";
             return false;
           }
           //read spell ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           BytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NPCS of NPC_!\n";
             return false;
           }
           NPC_Spells.push_back(std::string(Buffer));
           break;
      case cAIDT:
           if (hasAIDT)
           {
             std::cout << "Error: record NPC_ seems to have two AIDT subrecords.\n";
             return false;
           }
           //AIDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=12)
           {
             std::cout << "Error: Subrecord AIDT of NPC_ has invalid length ("<<SubLength
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
             std::cout << "Error while reading subrecord AIDT of NPC_!\n";
             return false;
           }
           AIData.isPresent = true;
           hasAIDT = true;
           break;
      case cAI_E:
           if (hasAI_E)
           {
             std::cout << "Error: record NPC_ seems to have two AI_E subrecords.\n";
             return false;
           }
           //AI_E's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=48)
           {
             std::cout << "Error: Subrecord AI_E of NPC_ has invalid length ("
                       << SubLength<< " bytes). Should be 48 bytes.\n";
             return false;
           }
           //read AI escort data
           in_File.read((char*) &(AIEscort.X), 4);
           in_File.read((char*) &(AIEscort.Y), 4);
           in_File.read((char*) &(AIEscort.Z), 4);
           in_File.read((char*) &(AIEscort.Duration), 2);
           // ---- read target ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           AIEscort.TargetID = std::string(Buffer);
           in_File.read((char*) &(AIEscort.Reset), 2);
           BytesRead += 48;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_E of NPC_!\n";
             return false;
           }
           AIEscort.isPresent = true;
           hasAI_E = true;
           break;
      case cAI_F:
           if (hasAI_F)
           {
             std::cout << "Error: record NPC_ seems to have two AI_F subrecords.\n";
             return false;
           }
           //AI_F's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=48)
           {
             std::cout << "Error: Subrecord AI_F of NPC_ has invalid length ("
                       << SubLength<< " bytes). Should be 48 bytes.\n";
             return false;
           }
           //read AI follow data
           in_File.read((char*) &(AIFollow.X), 4);
           in_File.read((char*) &(AIFollow.Y), 4);
           in_File.read((char*) &(AIFollow.Z), 4);
           in_File.read((char*) &(AIFollow.Duration), 2);
           // ---- read target ID
           memset(Buffer, '\0', 33);
           in_File.read(Buffer, 32);
           AIFollow.TargetID = std::string(Buffer);
           in_File.read((char*) &(AIFollow.Reset), 2);
           BytesRead += 48;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AI_F of NPC_!\n";
             return false;
           }
           AIFollow.isPresent = true;
           hasAI_F = true;
           break;
      case cAI_T:
           /*if (hasAI_T)
           {
             std::cout << "Error: record NPC_ seems to have two AI_T subrecords.\n";
             return false;
           }*/
           //AI_T's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=16)
           {
             std::cout << "Error: Subrecord AI_T of NPC_ has invalid length ("
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
             std::cout << "Error while reading subrecord AI_T of NPC_!\n";
             delete travelPointer;
             return false;
           }
           AIPackages.push_back(travelPointer);
           travelPointer = NULL; //just to be safe later
           //AITravel.isPresent = true;
           //hasAI_T = true;
           break;
      case cAI_W:
           /*if (hasAI_W)
           {
             std::cout << "Error: record NPC_ seems to have two AI_W subrecords.\n";
             return false;
           }*/
           //AI_W's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=14)
           {
             std::cout << "Error: Subrecord AI_W of NPC_ has invalid length ("
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
             std::cout << "Error while reading subrecord AI_W of NPC_!\n";
             delete wanderPointer;
             return false;
           }
           AIPackages.push_back(wanderPointer);
           wanderPointer = NULL; //just to be safe later
           //AITravel.isPresent = true;
           //AIWander.isPresent = true;
           //hasAI_W = true;
           break;
      case cCNDT:
           if (hasCNDT)
           {
             std::cout << "Error: record NPC_ seems to have two CNDT subrecords.\n";
             return false;
           }
           //CNDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord CNDT of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read escort/follow cell name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNDT of NPC_!\n";
             return false;
           }
           EscortFollowCell = std::string(Buffer);
           hasCNDT = true;
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
             std::cout << "Error: Subrecord DODT of NPC_ has invalid length ("
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
             std::cout << "Error while reading subrecord DODT of NPC_!\n";
             return false;
           }
           tempDest.CellName = "";
           hasReadDestination = true;
           break;
      case cDNAM:
           if (!hasReadDestination)
           {
             std::cout << "Error while reading NPC record: DNAM subrecord "
                       << "without previous DODT subrecord.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: Subrecord DNAM of NPC_ is longer than 255 characters.\n";
             return false;
           }
           //read destination data
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of NPC_!\n";
             return false;
           }
           tempDest.CellName = std::string(Buffer);
           //push record
           Destinations.push_back(tempDest);
           hasReadDestination = false;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     <<"\" found. Expected NPCO, NPCS, AIDT or AI_W.\n";
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

bool NPCRecord::hasEqualAIPackages(const NPCRecord& other) const
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
      if (!(AIPackages.at(i)->getPackageType()!=other.AIPackages.at(i)->getPackageType()))
      {
        return false;
      }
      switch (AIPackages.at(i)->getPackageType())
      {
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
