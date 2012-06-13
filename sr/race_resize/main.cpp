/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include <iostream>
#include "../base/ESMReaderSingleType.h"
#include "../base/ESMWriterContents.h"
#include "../base/records/TES4HeaderRecord.h"
#include "../base/Races.h"
#include "../base/RegistryFunctions.h"
#include "../base/SR_Constants.h"
#include "../base/StringTable.h"
#include "../base/StringTableCompound.h"

int main()
{
  SRTP::Tes4HeaderRecord header_rec, write_header;
  SRTP::ESMReaderSingleType<SRTP::RaceRecord, SRTP::Races, SRTP::cRACE> reader;
  //SRTP::ESMReaderAll reader;

  std::string SkyrimDir;
  if (!SRTP::getSkryrimPathFromRegistry(SkyrimDir))
  {
    std::cout << "Error: Could not find Skyrim's installation path in registry!\n";
    return 1;
  }
  if (!SkyrimDir.empty())
  {
    //Does it have a trailing (back)slash?
    if (SkyrimDir.at(SkyrimDir.length()-1)!='\\')
    {
      SkyrimDir = SkyrimDir + "\\";
    }//if not backslash
    /*add data dir to path, because installed path points only to Skyrim's
      main direkctory */
    SkyrimDir = SkyrimDir +"Data\\";
  }
  else
  {
    std::cout << "Error: Installation path in registry is empty!\n";
    return 1;
  }

  const int readResult = reader.readESM(SkyrimDir+"Skyrim.esm", header_rec);
  if (readResult>=0)
  {
    std::cout << "File was read/skipped successfully! Groups read: "<<readResult<<".\n";
    std::cout << "Race records read so far: "<<SRTP::Races::getSingleton().getNumberOfRecords()<<"\n";
  }
  else
  {
    std::cout << "Reading file failed!\n";
  }

  //String tables
  SRTP::StringTableCompound table_compound;
  if (table_compound.readCompound(SkyrimDir+"Strings\\Skyrim_German"))
  {
    std::cout << "Table compound read successfully! Table contains "<<table_compound.getNumberOfCompoundEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading compound tables "<<SkyrimDir<<"Strings\\Skyrim_German.DLStrings etc. failed!\n";
    return 1;
  }

  // --- race stuff
  std::cout << "Removing unwanted stuff...\n";
  SRTP::Races::ListIterator iter = SRTP::Races::getSingleton().getBegin();
  while (iter!=SRTP::Races::getSingleton().getEnd())
  {
    if (iter->second.editorID.find("HighElf")==std::string::npos)
    {
      SRTP::Races::getSingleton().removeRecord(iter->second.headerFormID);
      iter = SRTP::Races::getSingleton().getBegin();
    }
    else
    {
      ++iter;
    }
  }//while
  std::cout << "Race records now: "<<SRTP::Races::getSingleton().getNumberOfRecords()<<"\n";


  iter = SRTP::Races::getSingleton().getBegin();
  while (iter!=SRTP::Races::getSingleton().getEnd())
  {
    std::cout << "RACE \""<<iter->second.editorID<<"\":\n";
    //FULL
    if (iter->second.hasFULL)
    {
      std::cout << "    FULL: "<<iter->second.nameStringID;
      if (table_compound.hasString(iter->second.nameStringID))
      {
        std::cout << "    string: \""<< table_compound.getString(iter->second.nameStringID)<<"\"";
      }
      std::cout << std::endl;
    }//if
    //DESC

      std::cout << "    DESC: "<<iter->second.descriptionStringID;
      if (table_compound.hasString(iter->second.descriptionStringID))
      {
        std::cout << "    string: \""<< table_compound.getString(iter->second.descriptionStringID)<<"\"";
      }
      std::cout << std::endl;

    //DATA
    std::cout << "    DATA: height(m/f): "<<iter->second.data.heightMale<<"/"<<iter->second.data.heightFemale<<"\n";
    std::cout << "          weight(m/f): "<<iter->second.data.weightMale<<"/"<<iter->second.data.weightFemale<<"\n";
    ++iter;
  }//while

  const float childSize = 0.8;
  const float tinySize = 0.5;

  //prepare header record for writing
  write_header = header_rec;
  SRTP::Tes4HeaderRecord::MasterFile master;
  master.fileName = "Skyrim.esm";
  master.data = 0;
  write_header.dependencies.push_back(master);
  write_header.authorName = "Thoronador";

  //race record for transfer
  SRTP::RaceRecord* tempRaceRec = NULL;

  //the writer class itself
  SRTP::ESMWriterContents writer;
  //add stuff
  SRTP::Group& currentGroup = writer.contents.addNewGroup();
  currentGroup.headerData.setGroupLabel(SRTP::cRACE);

  iter = SRTP::Races::getSingleton().getBegin();
  while (iter!=SRTP::Races::getSingleton().getEnd())
  {
    //create new record
    tempRaceRec = new SRTP::RaceRecord;
    //get data from the original record
    *tempRaceRec = iter->second;
    //adjust size
    tempRaceRec->data.heightMale = 1.0f;
    tempRaceRec->data.heightFemale = 1.0f;
    //now add it to the group
    currentGroup.addRecord(tempRaceRec);
    //...and next
    ++iter;
  }//while

  currentGroup.updateGroupSize();

  if (writer.writeESM("C:\\Temp\\KleineHochelfen_normalEins.esm", write_header))
  {
    std::cout << "Writing new file was successful!\n";
    //just for checks, try to load it
    int result = reader.readESM("C:\\Temp\\KleineHochelfen.esm", header_rec);
    if (result<0)
    {
      std::cout << "Error: could not load saved stuff!\n";
    }
    else
    {
      std::cout << "Loading saved stuff succeeded, read "<<result<<" group(s)!\n";
    }
  }
  else
  {
    std::cout << "Writing new file failed!\n";
    return 1;
  }

  //prepare tables
  SRTP::StringTableCompound writeCompound;
  SRTP::StringTableCompound::TableType tt;
  iter = SRTP::Races::getSingleton().getBegin();
  while (iter!=SRTP::Races::getSingleton().getEnd())
  {
    //FULL
    if (iter->second.hasFULL)
    {
      tt = table_compound.locateString(iter->second.nameStringID);
      writeCompound.addString(iter->second.nameStringID, table_compound.getString(iter->second.nameStringID), tt);
    }//if
    //DESC

      tt = table_compound.locateString(iter->second.descriptionStringID);
      writeCompound.addString(iter->second.descriptionStringID, table_compound.getString(iter->second.descriptionStringID), tt);

    ++iter;
  }//while

  std::cout << "Compound entries in new compound: "<<writeCompound.getNumberOfCompoundEntries()<<".\n";
  if (writeCompound.writeCompound("C:\\Temp\\KleineHochelfen_German"))
  {
    std::cout << "Success at writing string tables!\n";
  }
  else
  {
    std::cout << "Failed at writing string tables!\n";
  }

  return 0;
}
