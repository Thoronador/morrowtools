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

#include "FactionRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

bool FactionRankData::operator==(const FactionRankData& other) const
{
  return ((Attr1==other.Attr1) and (Attr2==other.Attr2)
      and (FirstSkill==other.FirstSkill) and (SecondSkill==other.SecondSkill)
      and (Faction==other.Faction));
}

bool FactionReactionData::operator==(const FactionReactionData& other) const
{
  return ((OtherFaction==other.OtherFaction) and (Disposition==other.Disposition));
}

FactionRecord::FactionRecord()
{
  FactionID = Name = "";
  RankNames.clear();
  //faction data
  AttrID1 = -1;
  AttrID2 = -1;
  RankData.clear();
  SkillIDs[0]=SkillIDs[1]=SkillIDs[2]=SkillIDs[3]=SkillIDs[4]=SkillIDs[5]=-1;
  Unknown = 0;
  FactionFlags = 0;
  //end of faction data
  Reactions.clear();
}

bool FactionRecord::equals(const FactionRecord& other) const
{
  return ((FactionID==other.FactionID) and (Name==other.Name)
      and (RankNames==other.RankNames) and (AttrID1==other.AttrID1)
      and (AttrID2==other.AttrID2) and (RankData==other.RankData)
      and (SkillIDs[0]==other.SkillIDs[0]) and (SkillIDs[1]==other.SkillIDs[1])
      and (SkillIDs[2]==other.SkillIDs[2]) and (SkillIDs[3]==other.SkillIDs[3])
      and (SkillIDs[4]==other.SkillIDs[4]) and (SkillIDs[5]==other.SkillIDs[5])
      and (Unknown==other.Unknown) and (FactionFlags==other.FactionFlags)
      and (Reactions==other.Reactions));
}

bool FactionRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cFACT, 4);
  int32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +FactionID.length()+1 /*length of string +1 byte for NUL-termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /*length of string +1 byte for NUL-termination */
        +RankNames.size() * (4 /* RNAM */ +4 /* 4 bytes for length */ +32 /*fixed length */)
        +4 /* FADT */ +4 /* 4 bytes for length */ +240 /* fixed length is 240 bytes */;
  unsigned int i;
  for (i=0; i<Reactions.size(); ++i)
  {
    Size = Size + 4 /* ANAM */ +4 /* 4 bytes for length */
           + Reactions[i].OtherFaction.length() /* length of string (no NUL termination) */
           + 4 /* INTV */ +4 /* 4 bytes for length */ +4 /* size of INTV (fixed) */;
  }//for
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Faction:
    NAME = Faction ID string
    FNAM = Faction name string
    RNAM = Rank Name (32 bytes)
        Occurs 10 times for each rank in order
    FADT = Faction data (240 bytes)
        long AttributeID1
        long AttributeID2
        RankData[10]
            long Attribute1
            long Attribute2
            long FirstSkill
            long SecondSkill
            long Faction
        long SkillID[6]
        long Unknown1 (-1)?
        long Flags (1 = Hidden from Player)
    ANAM = Faction name string
    INTV = Faction reaction value (4 bytes, long)
        The ANAM/INTV occur in pairs for each faction with
        a reaction adjustment (usually -4 to +4) */

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  int32_t SubLength;
  SubLength = FactionID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write faction ID
  output.write(FactionID.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  SubLength = Name.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write faction name
  output.write(Name.c_str(), SubLength);

  for (i=0; i<RankNames.size(); ++i)
  {
    //write RNAM
    output.write((char*) &cRNAM, 4);
    //RNAM's length
    SubLength = 32;//length fixed at 32 bytes
    output.write((char*) &SubLength, 4);
    //write rank name
    unsigned int len = RankNames[i].length();
    if (len>32) len = 32;
    output.write(RankNames[i].c_str(), len);
    //fill rest with zeroes
    output.write(NULof32, 32-len);
  }//for

  //write FADT
  output.write((char*) &cFADT, 4);
  //FADT's length
  SubLength = 240;//length is fixed
  output.write((char*) &SubLength, 4);
  //write faction data
  // ---- attributes
  output.write((char*) &AttrID1, 4);
  output.write((char*) &AttrID2, 4);
  unsigned int from_rank_data = RankData.size();
  if (from_rank_data>10) from_rank_data = 10;
  // ---- write present ranks
  for (i=0; i<from_rank_data; ++i)
  {
    output.write((char*) &(RankData[i].Attr1), 4);
    output.write((char*) &(RankData[i].Attr2), 4);
    output.write((char*) &(RankData[i].FirstSkill), 4);
    output.write((char*) &(RankData[i].SecondSkill), 4);
    output.write((char*) &(RankData[i].Faction), 4);
  }//for
  //write more "ranks" (fill with zero) to get to a total of ten
  for (i=from_rank_data; i<10; ++i)
  {
    output.write(NULof32, 20);
  }//for
  // ---- skills
  for (i=0; i<6; ++i)
  {
    output.write((char*) &(SkillIDs[i]), 4);
  }
  // ---- unknown
  output.write((char*) &Unknown, 4);
  // ---- flags
  output.write((char*) &FactionFlags, 4);

  //write faction reactions
  for (i=0; i<Reactions.size(); ++i)
  {
    //write ANAM
    output.write((char*) &cANAM, 4);
    //ANAM's length
    SubLength = Reactions[i].OtherFaction.length();//length of string (no NUL-termination)
    output.write((char*) &SubLength, 4);
    //write other faction name
    output.write(Reactions[i].OtherFaction.c_str(), SubLength);

    //write INTV
    output.write((char*) &cINTV, 4);
    //INTV's length
    SubLength = 4;//always four bytes
    output.write((char*) &SubLength, 4);
    //write disp.
    output.write((char*) &(Reactions[i].Disposition), 4);
  }//for

  return output.good();
}

bool FactionRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Faction:
    NAME = Faction ID string
    FNAM = Faction name string
    RNAM = Rank Name (32 bytes)
        Occurs 10 times for each rank in order
    FADT = Faction data (240 bytes)
        long AttributeID1
        long AttributeID2
        RankData[10]
            long Attribute1
            long Attribute2
            long FirstSkill
            long SecondSkill
            long Faction
        long SkillID[6]
        long Unknown1 (-1)?
        long Flags (1 = Hidden from Player)
    ANAM = Faction name string
    INTV = Faction reaction value (4 bytes, long)
        The ANAM/INTV occur in pairs for each faction with
        a reaction adjustment (usually -4 to +4) */

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
    std::cout << "Error: subrecord NAME of FACT is longer than 255 characters.\n";
    return false;
  }
  //read faction ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of FACT!\n";
    return false;
  }
  FactionID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
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
    std::cout << "Error: subrecord FNAM of FACT is longer than 255 characters.\n";
    return false;
  }
  //read faction name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of FACT!\n";
    return false;
  }
  Name = std::string(Buffer);

  RankNames.clear();
  //read RNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  while (SubRecName==cRNAM)
  {
    //RNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=32)
    {
      std::cout << "Error: subrecord RNAM of FACT has invalid length ("
                << SubLength<<" bytes). Should be 32 bytes.\n";
      return false;
    }
    //read rank name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord RNAM of FACT!\n";
      return false;
    }
    RankNames.push_back(std::string(Buffer));

    //read next RNAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }//while

  //read FADT
  //subrecord's name was already read above
  if (SubRecName!=cFADT)
  {
    UnexpectedRecord(cFADT, SubRecName);
    return false;
  }
  //FADT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=240)
  {
    std::cout << "Error: subrecord FADT of FACT has invalid length ("<<SubLength
              << " bytes). Should be 240 bytes.\n";
    return false;
  }
  //read faction data
  in_File.read((char*) &AttrID1, 4);
  in_File.read((char*) &AttrID2, 4);
  BytesRead += 8;
  RankData.clear();
  FactionRankData temp;
  unsigned int i;
  for (i=0; i<10; ++i)
  {
    in_File.read((char*) &(temp.Attr1), 4);
    in_File.read((char*) &(temp.Attr2), 4);
    in_File.read((char*) &(temp.FirstSkill), 4);
    in_File.read((char*) &(temp.SecondSkill), 4);
    in_File.read((char*) &(temp.Faction), 4);
    BytesRead += 20;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FADT of FACT!\n";
      return false;
    }
    RankData.push_back(temp);
  }//for
  //skills
  in_File.read((char*) &(SkillIDs[0]), 4);
  in_File.read((char*) &(SkillIDs[1]), 4);
  in_File.read((char*) &(SkillIDs[2]), 4);
  in_File.read((char*) &(SkillIDs[3]), 4);
  in_File.read((char*) &(SkillIDs[4]), 4);
  in_File.read((char*) &(SkillIDs[5]), 4);
  in_File.read((char*) &Unknown, 4);
  in_File.read((char*) &FactionFlags, 4);
  BytesRead += 32;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FADT of FACT!\n";
    return false;
  }

  //read faction reactions
  Reactions.clear();
  FactionReactionData temp_reac;
  while (BytesRead<Size)
  {
    //read ANAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cANAM)
    {
      UnexpectedRecord(cANAM, SubRecName);
      std::cout << "Debug: Size is "<<Size<<" bytes, BytesRead is "<<BytesRead
                <<" bytes. File pos. is "<<in_File.tellg()<<" bytes.\n";
      return false;
    }
    //ANAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord ANAM of FACT is longer than 255 characters.\n";
      return false;
    }
    //read other faction's name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord ANAM of FACT!\n";
      return false;
    }
    temp_reac.OtherFaction = std::string(Buffer);

    //read INTV
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cINTV)
    {
      UnexpectedRecord(cINTV, SubRecName);
      return false;
    }
    //INTV's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=4)
    {
      std::cout << "Error: subrecord INTV of FACT has invalid length ("
                << SubLength<<" bytes). Should be four bytes.\n";
      return false;
    }
    //read reaction adjustment
    in_File.read((char*) &(temp_reac.Disposition), 4);
    BytesRead += 4;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord INTV of FACT!\n";
      return false;
    }
    Reactions.push_back(temp_reac);
  }//while

  return in_File.good();
}
