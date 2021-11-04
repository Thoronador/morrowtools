/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include <iostream>
#include "../base/ESMReaderSingleType.hpp"
#include "../base/ESMWriterContents.hpp"
#include "../base/records/TES4HeaderRecord.hpp"
#include "../base/Races.hpp"
#include "../base/PathFunctions.hpp"
#include "../base/ReturnCodes.hpp"
#include "../base/SR_Constants.hpp"
#include "../base/StringTable.hpp"
#include "../base/StringTableCompound.hpp"

int main()
{
  SRTP::Tes4HeaderRecord header_rec, write_header;
  SRTP::ESMReaderSingleType<SRTP::RaceRecord, SRTP::Races, SRTP::cRACE> reader;
  //SRTP::ESMReaderAll reader;

  std::string SkyrimDir;
  SRTP::getDataDir(SkyrimDir, std::nullopt);

  const int readResult = reader.readESM(SkyrimDir+"Skyrim.esm", header_rec);
  if (readResult>=0)
  {
    std::cout << "File was read/skipped successfully! Groups read: "<<readResult<<".\n";
    std::cout << "Race records read so far: " << SRTP::Races::get().getNumberOfRecords() << "\n";
  }
  else
  {
    std::cout << "Reading file failed!\n";
    return SRTP::rcFileError;
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
    return SRTP::rcFileError;
  }

  // --- race stuff
  std::cout << "Removing unwanted stuff...\n";
  SRTP::Races::ListIterator iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    if (iter->second.editorID.find("HighElf")==std::string::npos)
    {
      SRTP::Races::get().removeRecord(iter->second.headerFormID);
      iter = SRTP::Races::get().begin();
    }
    else
    {
      ++iter;
    }
  }//while
  std::cout << "Race records now: "<<SRTP::Races::get().getNumberOfRecords()<<"\n";

  iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    std::cout << "RACE \""<<iter->second.editorID<<"\":\n";
    //FULL
    if (iter->second.name.isPresent())
    {
      std::cout << "    FULL: "<<iter->second.name.getIndex();
      std::cout << "    string: \""<<iter->second.name.getString()<<"\"";
      std::cout << std::endl;
    }//if
    //DESC

    std::cout << "    DESC: "<<iter->second.description.getIndex();
    std::cout << "    string: \""<< iter->second.description.getString()<<"\"";
    std::cout << std::endl;

    //DATA
    std::cout << "    DATA: height(m/f): "<<iter->second.data.heightMale<<"/"<<iter->second.data.heightFemale<<"\n";
    std::cout << "          weight(m/f): "<<iter->second.data.weightMale<<"/"<<iter->second.data.weightFemale<<"\n";
    ++iter;
  }//while

  /* const float childSize = 0.8;
  const float tinySize = 0.5; */

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
  currentGroup.headerData.setLabel(SRTP::cRACE);

  iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
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
  iter = SRTP::Races::get().begin();
  while (iter!=SRTP::Races::get().end())
  {
    //FULL
    if (iter->second.name.isPresent())
    {
      tt = table_compound.locateString(iter->second.name.getIndex());
      writeCompound.addString(iter->second.name.getIndex(), table_compound.getString(iter->second.name.getIndex()), tt);
    }//if
    //DESC
    tt = table_compound.locateString(iter->second.description.getIndex());
    writeCompound.addString(iter->second.description.getIndex(), table_compound.getString(iter->second.description.getIndex()), tt);

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
