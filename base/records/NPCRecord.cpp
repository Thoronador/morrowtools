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
  if (!isPresent and !other.isPresent) return true;
  return ((isPresent==other.isPresent) and (Hello==other.Hello)
      and (Unknown1==other.Unknown1) and (Fight==other.Fight)
      and (Flee==other.Flee) and (Alarm==other.Alarm)
      and (Unknown2==other.Unknown2) and (Unknown3==other.Unknown3)
      and (Unknown4==other.Unknown4) and (Flags==other.Flags));
}

/* **** BasicAIPackage's functions ****/

NPC_BasicAIPackage::~NPC_BasicAIPackage()
{
  //empty - just here to get rid of compiler warnings
}

/* **** AIWander's functions ****/

void NPC_AIWander::clear()
{
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

/* **** AIActivate's functions ****/

bool NPC_AIActivate::equals(const NPC_AIActivate& other) const
{
  return ((TargetID==other.TargetID) and (Reset==other.Reset));
}

PackageType NPC_AIActivate::getPackageType() const
{
  return ptActivate;
}

/* **** AIEscortFollow's functions ****/

void NPC_AIEscortFollow::clear()
{
  X = Y = Z = 0.0f;
  Duration = 0;
  TargetID = "";
  Reset = 0;
  CellName = "";
}

bool NPC_AIEscortFollow::equals(const NPC_AIEscortFollow& other) const
{
  /* Note: Usually it should be enough to check X, Y and Z for equality with
           their counterparts in 'other', but if one of them is a NaN, we have
           a problem, because one NaN is (by definition of IEEE 754) never equal
           to another NaN, even if their internal bit representation is exactly
           the same. That's why we have expressions like (X!=X) down there, they
           catch the NaNs. */
  return (((X==other.X) or ((X!=X) and (other.X!=other.X)))
      and ((Y==other.Y) or ((Y!=Y) and (other.Y!=other.Y)))
      and ((Z==other.Z) or ((Z!=Z) and (other.Z!=other.Z)))
      and (Duration==other.Duration) and (TargetID==other.TargetID)
      and (Reset==other.Reset) and (CellName==other.CellName));
}

/* **** AIEscort functions ****/

PackageType NPC_AIEscort::getPackageType() const
{
  return ptEscort;
}

/* **** AIFollow functions ****/

PackageType NPC_AIFollow::getPackageType() const
{
  return ptFollow;
}

/* **** AITravel's functions ****/

void NPC_AITravel::clear()
{
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
      and (hasEqualAIPackages(other))
      and (Destinations==other.Destinations));
}

bool NPCRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cNPC_, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +NPCID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* RNAM */ +4 /* 4 bytes for length */
        +RaceID.length()+1 /* length of race ID +1 byte for NUL termination */
        +4 /* ANAM */ +4 /* 4 bytes for length */
        +FactionID.length()+1 /* length of faction ID +1 byte for NUL termination */
        +4 /* BNAM */ +4 /* 4 bytes for length */
        +HeadModel.length()+1 /* length of head model +1 byte for NUL termination */
        +4 /* KNAM */ +4 /* 4 bytes for length */
        +HairModel.length()+1 /* length of hair model +1 byte for NUL termination */
        +4 /* NPDT */ +4 /* 4 bytes for length */;
  switch (NPCDataType)
  {
    case ndt12Bytes:
         Size += 12; //fixed length of 12 bytes
         break;
    case ndt52Bytes:
         Size += 52; //fixed length of 52 bytes
         break;
    case ndtNone:
         std::cout << "Error: No data type specified for NPDT subrecord.\n";
         return false;
         break;
  }//swi
  Size = Size + 4 /* FLAG */ +4 /* 4 bytes for length */ +4 /* fixed length of four bytes */
        +Items.size()*(4 /* NPCO */ +4 /* 4 bytes for length */ +36 /* fixed length of 36 bytes */)
        +NPC_Spells.size()*(4 /* NPCS */ +4 /* 4 bytes for length */ +32 /* fixed length of 36 bytes */);


  //add size of the optional stuff
  if (Name!="")
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
          +Name.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (ModelPath!="")
  {
    Size = Size +4 /* MODL */ +4 /* 4 bytes for length */
          +ModelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (ClassID!="")
  {
    Size = Size +4 /* CNAM */ +4 /* 4 bytes for length */
          +ClassID.length()+1 /* length of class ID +1 byte for NUL termination */;
  }
  if (ScriptID!="")
  {
    Size = Size +4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of script+1 byte for NUL termination */;
  }
  if (AIData.isPresent)
  {
    Size = Size +4 /* AIDT */ +4 /* 4 bytes for length */ +12 /* fixed length of 12 bytes */;
  }

  //AI packages
  unsigned int i;
  for (i=0; i<AIPackages.size(); ++i)
  {
    switch(AIPackages[i]->getPackageType())
    {
      case ptActivate:
           Size = Size + 4 /* AI_A */ + 4 /* 4 bytes for length */ +33 /* fixed length of 33 bytes */;
           break;
      case ptEscort:
      case ptFollow:
           Size = Size + 4 /* AI_E / AI_F */ + 4 /* 4 bytes for length */ +48 /* fixed length of 48 bytes */;
           if (static_cast<NPC_AIEscortFollow*>(AIPackages[i])->CellName!="")
           {
             Size = Size +4 /* CNDT */ +4 /* 4 bytes for length */
                    +static_cast<NPC_AIEscortFollow*>(AIPackages[i])->CellName.length()+1 /* length of cell name +1 byte for NUL */;
           }
           break;
      case ptTravel:
           Size = Size + 4 /* AI_T */ + 4 /* 4 bytes for length */ +16 /* fixed length of 16 bytes */;
           break;
      case ptWander:
           Size = Size + 4 /* AI_W */ + 4 /* 4 bytes for length */ +14 /* fixed length of 14 bytes */;
           break;
    }//swi
  }//for (AIPackages)

  //travel service destinations
  for (i=0; i<Destinations.size(); ++i)
  {
    Size = Size + +4 /* DODT */ +4 /* 4 bytes for length */ +24 /*fixed length of 24 bytes */;
    if (Destinations[i].CellName!="")
    {
      Size = Size + +4 /* DNAM */ +4 /* 4 bytes for length */
            +Destinations[i].CellName.length()+1 /*length of cell name +1 byte for NUL termination */;
    }//if
  }//for
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);
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

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = NPCID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(NPCID.c_str(), SubLength);

  if (Name!="")
  {
    //write FNAM
    output.write((char*) &cFNAM, 4);
    SubLength = Name.length()+1;
    //write FNAM's length
    output.write((char*) &SubLength, 4);
    //write sign's name
    output.write(Name.c_str(), SubLength);
  }

  if (ModelPath!="")
  {
    //write MODL
    output.write((char*) &cMODL, 4);
    SubLength = ModelPath.length()+1;
    //write MODL's length
    output.write((char*) &SubLength, 4);
    //write NPC's model path
    output.write(ModelPath.c_str(), SubLength);
  }

  //write RNAM
  output.write((char*) &cRNAM, 4);
  SubLength = RaceID.length()+1;
  //write RNAM's length
  output.write((char*) &SubLength, 4);
  //write race ID
  output.write(RaceID.c_str(), SubLength);

  //write ANAM
  output.write((char*) &cANAM, 4);
  SubLength = FactionID.length()+1;
  //write ANAM's length
  output.write((char*) &SubLength, 4);
  //write faction ID
  output.write(FactionID.c_str(), SubLength);

  //write BNAM
  output.write((char*) &cBNAM, 4);
  SubLength = HeadModel.length()+1;
  //write BNAM's length
  output.write((char*) &SubLength, 4);
  //write head model
  output.write(HeadModel.c_str(), SubLength);

  if (ClassID!="")
  {
    //write CNAM
    output.write((char*) &cCNAM, 4);
    SubLength = ClassID.length()+1;
    //write CNAM's length
    output.write((char*) &SubLength, 4);
    //write class ID
    output.write(ClassID.c_str(), SubLength);
  }

  //write KNAM
  output.write((char*) &cKNAM, 4);
  SubLength = HairModel.length()+1;
  //write KNAM's length
  output.write((char*) &SubLength, 4);
  //write hair model
  output.write(HairModel.c_str(), SubLength);

  if (ScriptID!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  //write NPDT
  output.write((char*) &cNPDT, 4);
  switch ( NPCDataType)
  {
    case ndt12Bytes:
         SubLength = 12; //fixed length of 12 bytes
         break;
    case ndt52Bytes:
         SubLength = 52; //fixed length of 52 bytes
         break;
    case ndtNone:
         std::cout << "Error: No data type specified for NPDT subrecord.\n";
         return false;
         break;
  }//swi
  //write NPDT's length
  output.write((char*) &SubLength, 4);
  //write NPC data
  if (SubLength==52)
  {
    // ---- level
    output.write((char*) &Level, 2);
    // ---- attributes
    output.write((char*) &Strength, 1);
    output.write((char*) &Intelligence, 1);
    output.write((char*) &Willpower, 1);
    output.write((char*) &Agility, 1);
    output.write((char*) &Speed, 1);
    output.write((char*) &Endurance, 1);
    output.write((char*) &Personality, 1);
    output.write((char*) &Luck, 1);
    // ---- skills
    output.write((char*) Skills, 27);
    // ---- reputations
    output.write((char*) &Reputation, 1);
    // ---- secondary attribs
    output.write((char*) &Health, 2);
    output.write((char*) &SpellPoints, 2);
    output.write((char*) &Fatigue, 2);
    // ---- disposition
    output.write((char*) &Disposition, 1);
    // ---- faction stuff
    output.write((char*) &Data_FactionID, 1);
    output.write((char*) &Rank, 1);
    // ---- others
    output.write((char*) &Unknown1, 1);
    output.write((char*) &Gold, 4);
  }
  else
  {
    //12 byte version
    // ---- level
    output.write((char*) &Level, 2);
    // ---- disposition
    output.write((char*) &Disposition, 1);
    // ---- faction stuff
    output.write((char*) &Data_FactionID, 1);
    output.write((char*) &Rank, 1);
    // ---- others
    output.write((char*) &Unknown1, 1);
    output.write(NULof32, 2);//skip the two unknown bytes
    output.write((char*) &Gold, 4);
  }

  //write FLAG
  output.write((char*) &cFLAG, 4);
  SubLength = 4; //fixed length of four bytes
  //write FLAG's length
  output.write((char*) &SubLength, 4);
  //write NPC flag
  output.write((char*) &NPC_Flag, 4);

  //items and spells
  unsigned int len;
  for (i=0; i<Items.size(); ++i)
  {
    //Items
    //write NPCO
    output.write((char*) &cNPCO, 4);
    SubLength = 36; //fixed length of 36 bytes
    //write NPCO's length
    output.write((char*) &SubLength, 4);
    //write item stuff
    // ---- count
    output.write((char*) &(Items[i].Count), 4);
    // ---- item ID
    len = Items[i].Item.length();
    if (len>31)
    {
      len = 31;
    }
    output.write(Items[i].Item.c_str(), len);
    //fill rest with NUL bytes
    output.write(NULof32, 32-len);
  }//for

  for (i=0; i<NPC_Spells.size(); ++i)
  {
    //Spells
    //write NPCS
    output.write((char*) &cNPCS, 4);
    SubLength = 32; //fixed length of 32 bytes
    //write NPCS's length
    output.write((char*) &SubLength, 4);
    //write spell ID
    len = NPC_Spells[i].length();
    if (len>31)
    {
      len = 31;
    }
    output.write(NPC_Spells[i].c_str(), len);
    //fill rest with NUL bytes
    output.write(NULof32, 32-len);
  }//for

  if (AIData.isPresent)
  {
    //write AIDT
    output.write((char*) &cAIDT, 4);
    SubLength = 12; //fixed length of 12 bytes
    //write AIDT's length
    output.write((char*) &SubLength, 4);
    //write AI data
    output.write((char*) &(AIData.Hello), 1);
    output.write((char*) &(AIData.Unknown1), 1);
    output.write((char*) &(AIData.Fight), 1);
    output.write((char*) &(AIData.Flee), 1);
    output.write((char*) &(AIData.Alarm), 1);
    output.write((char*) &(AIData.Unknown2), 1);
    output.write((char*) &(AIData.Unknown3), 1);
    output.write((char*) &(AIData.Unknown4), 1);
    output.write((char*) &(AIData.Flags), 4);
  }//if AIData is present

  //AI packages
  NPC_AIActivate* activatePointer;
  NPC_AIEscortFollow* escort_followPointer;
  NPC_AITravel* travelPointer;
  NPC_AIWander* wanderPointer;
  for (i=0; i<AIPackages.size(); ++i)
  {
    switch (AIPackages[i]->getPackageType())
    {
      case ptActivate:
           //write AI_A
           output.write((char*) &cAI_A, 4);
           SubLength = 33; //fixed length of 33 bytes
           //write AI_A's length
           output.write((char*) &SubLength, 4);
           //write AI activate data
           activatePointer = static_cast<NPC_AIActivate*>(AIPackages[i]);
           // ---- write target ID
           len = activatePointer->TargetID.length();
           if (len>31)
           {
             len = 31;
           }
           output.write(activatePointer->TargetID.c_str(), len);
           // ---- fill rest with NUL
           output.write(NULof32, 32-len);
           // ---- reset flag
           output.write((char*) &(activatePointer->Reset), 1);
           break;
      case ptEscort:
      case ptFollow:
           if (AIPackages[i]->getPackageType()==ptEscort)
           {
             //write AI_E
             output.write((char*) &cAI_E, 4);
           }
           else
           {
             //write AI_F
             output.write((char*) &cAI_F, 4);
           }
           SubLength = 48; //fixed length of 48 bytes
           //write AI_E's/AI_F's length
           output.write((char*) &SubLength, 4);
           //write AI escort/follow data
           escort_followPointer = static_cast<NPC_AIEscortFollow*>(AIPackages[i]);
           output.write((char*) &(escort_followPointer->X), 4);
           output.write((char*) &(escort_followPointer->Y), 4);
           output.write((char*) &(escort_followPointer->Z), 4);
           output.write((char*) &(escort_followPointer->Duration), 2);
           // ---- write target ID
           len = escort_followPointer->TargetID.length();
           if (len>31)
           {
             len = 31;
           }
           output.write(escort_followPointer->TargetID.c_str(), len);
           // ---- fill rest with NUL
           output.write(NULof32, 32-len);
           // ---- reset flag
           output.write((char*) &(escort_followPointer->Reset), 2);

           //check for presence of cell
           if (escort_followPointer->CellName!="")
           {
             //write CNDT
             output.write((char*) &cCNDT, 4);
             SubLength = escort_followPointer->CellName.length()+1; //length of cell name +1 byte for NUL
             //write CNDT's length
             output.write((char*) &SubLength, 4);
             //write AI escort's/follow's cell name
             output.write(escort_followPointer->CellName.c_str(), SubLength);
           }
           break;
      case ptTravel:
           //write AI_T
           output.write((char*) &cAI_T, 4);
           SubLength = 16; //fixed length of 16 bytes
           //write AI_T's length
           output.write((char*) &SubLength, 4);
           //write AI travel data
           travelPointer = static_cast<NPC_AITravel*>(AIPackages[i]);
           output.write((char*) &(travelPointer->X), 4);
           output.write((char*) &(travelPointer->Y), 4);
           output.write((char*) &(travelPointer->Z), 4);
           output.write((char*) &(travelPointer->Reset), 4);
           break;
      case ptWander:
           //write AI_W
           output.write((char*) &cAI_W, 4);
           SubLength = 14; //fixed length of 14 bytes
           //write AI_W's length
           output.write((char*) &SubLength, 4);
           //write AI wander data
           wanderPointer = static_cast<NPC_AIWander*>(AIPackages[i]);
           output.write((char*) &(wanderPointer->Distance), 2);
           output.write((char*) &(wanderPointer->Duration), 2);
           output.write((char*) &(wanderPointer->Time), 1);
           output.write((char*) (wanderPointer->Idle), 8);
           output.write((char*) &(wanderPointer->Reset), 1);
           break;
    }//swi
  }//for AIPackages

  //travel service destinations
  for (i=0; i<Destinations.size(); ++i)
  {
    //write DODT
    output.write((char*) &cDODT, 4);
    SubLength = 24; //fixed length of 24 bytes
    //write DODT's length
    output.write((char*) &SubLength, 4);
    //write destination data
    output.write((char*) &(Destinations[i].XPos), 4);
    output.write((char*) &(Destinations[i].YPos), 4);
    output.write((char*) &(Destinations[i].ZPos), 4);
    output.write((char*) &(Destinations[i].XRot), 4);
    output.write((char*) &(Destinations[i].YRot), 4);
    output.write((char*) &(Destinations[i].ZRot), 4);
    //see if there's a cell name, too
    if (Destinations[i].CellName!="")
    {
      //write DNAM
      output.write((char*) &cDNAM, 4);
      SubLength = Destinations[i].CellName.length()+1; //length of cell name +1 byte for NUL
      //write DNAM's length
      output.write((char*) &SubLength, 4);
      //write destination cell name
      output.write(Destinations[i].CellName.c_str(), SubLength);
    }//if
  }//for

  return output.good();
}

bool NPCRecord::loadFromStream(std::ifstream& in_File)
{
  #warning Not completely implemented yet!\
           The XSCL subrecord is still missing.
  uint32_t Size;
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
  AIPackages.clear();
  Destinations.clear();

  bool hasAIDT = false;
  bool hasReadDestination = false;
  TravelDestination tempDest;

  NPC_AIActivate* activatePointer = NULL;
  NPC_AIEscortFollow* escortFollowPointer = NULL;
  NPC_AITravel* travelPointer = NULL;
  NPC_AIWander* wanderPointer = NULL;
  int32_t previousSubRecord = 0;

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
           previousSubRecord = cNPCO;
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
           previousSubRecord = cNPCS;
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
           previousSubRecord = cAIDT;
           break;
      case cAI_A:
           //AI_A's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=33)
           {
             std::cout << "Error: Subrecord AI_A of NPC_ has invalid length ("
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
             std::cout << "Error while reading subrecord AI_A of NPC_!\n";
             delete activatePointer;
             return false;
           }
           activatePointer->TargetID = std::string(Buffer);
           AIPackages.push_back(activatePointer);
           activatePointer = NULL; //just to be safe
           previousSubRecord = cAI_A;
           break;
      case cAI_E:
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
             std::cout << "Error while reading subrecord AI_E of NPC_!\n";
             delete escortFollowPointer;
             return false;
           }
           escortFollowPointer->CellName = "";
           AIPackages.push_back(escortFollowPointer);
           escortFollowPointer = NULL; //just to be safe
           previousSubRecord = cAI_E;
           break;
      case cAI_F:
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
             std::cout << "Error while reading subrecord AI_F of NPC_!\n";
             delete escortFollowPointer;
             return false;
           }
           escortFollowPointer->CellName = "";
           AIPackages.push_back(escortFollowPointer);
           escortFollowPointer = NULL; //just to be safe
           previousSubRecord = cAI_F;
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
           previousSubRecord = cAI_T;
           break;
      case cAI_W:
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
           previousSubRecord = cAI_W;
           break;
      case cCNDT:
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
           if ((previousSubRecord==cAI_E) or (previousSubRecord==cAI_F))
           {
             //last record was AI escort or follow, so set it's cell name
             (static_cast<NPC_AIEscortFollow*>(AIPackages.back()))->CellName = std::string(Buffer);
           }
           else
           {
             std::cout << "Error: Subrecord before CNDT of NPC_ was neither "
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
           previousSubRecord = cDODT;
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
           previousSubRecord = cDNAM;
           break;
      default:
           std::cout << "Unexpected record name \""<<IntTo4Char(SubRecName)
                     << "\" found. Expected NPCO, NPCS, AIDT, AI_E, AI_F, AI_T,"
                     << " AI_W, DODT or DNAM.\n";
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
