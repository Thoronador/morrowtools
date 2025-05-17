/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2023, 2025  Dirk Stolle

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

#include <filesystem>
#include <iostream>
#include "../../../lib/sr/ESMReaderSingleType.hpp"
#include "../../../lib/sr/ESMWriterContents.hpp"
#include "../../../lib/sr/records/TES4HeaderRecord.hpp"
#include "../../../lib/sr/Races.hpp"
#include "../../../lib/sr/PathFunctions.hpp"
#include "../../../lib/sr/ReturnCodes.hpp"
#include "../../../lib/sr/SR_Constants.hpp"
#include "../../../lib/sr/StringTable.hpp"
#include "../../../lib/sr/StringTableCompound.hpp"

void showHelp()
{
  std::cout << "\nsmall_high_elves [-d DIRECTORY]\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Skyrim to\n"
            << "                     DIRECTORY\n"
            << "  -dir DIRECTORY   - same as -d\n";
}

int main(int argc, char** argv)
{
  std::string SkyrimDir;
  if ((argc > 1) && (argv != nullptr))
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if ((param == "--help") || (param == "-?") || (param == "/?"))
        {
          showHelp();
          return 0;
        }
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // set more than once?
          if (!SkyrimDir.empty())
          {
            std::cerr << "Error: Data directory was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i + 1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              SkyrimDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add it.
              SkyrimDir = slashify(SkyrimDir);
              std::cout << "Data files directory was set to \"" << SkyrimDir << "\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << std::string(argv[i+1]) << "\" is too"
                        << " short to be a proper directory path.\n";
              return SRTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a directory name after \""
                      << param << "\".\n";
            return SRTP::rcInvalidParameter;
          }
        } //data files directory
        else
        {
          // unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return SRTP::rcInvalidParameter;
        }
      } // parameter exists
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return SRTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments present

  SRTP::getDataDir(SkyrimDir, std::nullopt);

  SRTP::Tes4HeaderRecord header_rec, write_header;
  SRTP::ESMReaderSingleType<SRTP::RaceRecord, SRTP::Races, SRTP::cRACE> reader;
  const int readResult = reader.readESM(SkyrimDir + "Skyrim.esm", header_rec, SRTP::Localization::German);
  if (readResult >= 0)
  {
    std::cout << "File was read/skipped successfully! Groups read: "
              << readResult << ".\n" << "Race records read so far: "
              << SRTP::Races::get().getNumberOfRecords() << "\n";
  }
  else
  {
    std::cerr << "Reading file failed!\n";
    return SRTP::rcFileError;
  }

  // String tables
  SRTP::StringTableCompound table_compound;
  const std::string StringPath = std::string("Strings") + MWTP::pathDelimiter + "Skyrim_German";
  if (table_compound.readCompound(SkyrimDir + StringPath))
  {
    std::cout << "Table compound read successfully! Table contains "
              << table_compound.getNumberOfCompoundEntries() << " entries.\n";
  }
  else
  {
    std::cerr << "Reading compound tables " << SkyrimDir << StringPath << ".DLStrings etc. failed!\n";
    return SRTP::rcFileError;
  }

  // --- race stuff
  std::cout << "Removing unwanted stuff...\n";
  SRTP::Races::ListIterator iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    if (iter->second.editorID.find("HighElf") == std::string::npos)
    {
      SRTP::Races::get().removeRecord(iter->second.headerFormID);
      iter = SRTP::Races::get().begin();
    }
    else
    {
      ++iter;
    }
  }
  std::cout << "Race records now: " << SRTP::Races::get().getNumberOfRecords() << "\n";

  iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    std::cout << "RACE \"" << iter->second.editorID << "\":\n";
    // FULL
    if (iter->second.name.isPresent())
    {
      std::cout << "    FULL: " << iter->second.name.getIndex()
                << "    string: \"" << iter->second.name.getString() << "\"\n";
    }

    // DESC
    std::cout << "    DESC: " << iter->second.description.getIndex()
              << "    string: \"" << iter->second.description.getString()
              << "\"\n";

    // DATA
    std::cout << "    DATA: height(m/f): " << iter->second.data.heightMale << "/" << iter->second.data.heightFemale << "\n";
    std::cout << "          weight(m/f): " << iter->second.data.weightMale << "/" << iter->second.data.weightFemale << "\n";
    ++iter;
  }

  /* const float childSize = 0.8;
  const float tinySize = 0.5; */

  // prepare header record for writing
  write_header = header_rec;
  SRTP::Tes4HeaderRecord::MasterFile master;
  master.fileName = "Skyrim.esm";
  master.data = 0;
  write_header.dependencies.push_back(master);
  write_header.authorName = "Thoronador";

  // race record for transfer
  SRTP::RaceRecord* tempRaceRec = nullptr;

  SRTP::ESMFileContents contents;
  SRTP::ESMWriterContents writer(contents);
  // add stuff
  SRTP::Group& currentGroup = contents.addNewGroup();
  currentGroup.headerData.setLabel(SRTP::cRACE);

  iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    // create new record
    tempRaceRec = new SRTP::RaceRecord;
    // get data from the original record
    *tempRaceRec = iter->second;
    // adjust size
    tempRaceRec->data.heightMale = 1.0f;
    tempRaceRec->data.heightFemale = 1.0f;
    // now add it to the group
    currentGroup.addRecord(tempRaceRec);
    // ...and next
    ++iter;
  }

  currentGroup.updateGroupSize();

  namespace fs = std::filesystem;
  const fs::path path = fs::temp_directory_path() / "KleineHochelfen.esm";

  if (writer.writeESM(path.string(), write_header))
  {
    std::cout << "Writing new file (" << path.string() << ") was successful!\n";
  }
  else
  {
    std::cerr << "Writing new file failed!\n";
    return SRTP::rcFileError;
  }

  // prepare tables
  SRTP::StringTableCompound writeCompound;
  SRTP::StringTableCompound::TableType tt;
  iter = SRTP::Races::get().begin();
  while (iter != SRTP::Races::get().end())
  {
    // FULL
    if (iter->second.name.isPresent())
    {
      tt = table_compound.locateString(iter->second.name.getIndex());
      writeCompound.addString(iter->second.name.getIndex(), table_compound.getString(iter->second.name.getIndex()), tt);
    }
    // DESC
    tt = table_compound.locateString(iter->second.description.getIndex());
    writeCompound.addString(iter->second.description.getIndex(), table_compound.getString(iter->second.description.getIndex()), tt);

    ++iter;
  }

  std::cout << "Compound entries in new compound: " << writeCompound.getNumberOfCompoundEntries() << ".\n";
  auto compound_path {path};
  compound_path.replace_filename("KleineHochelfen_German");
  if (writeCompound.writeCompound(compound_path.string()))
  {
    std::cout << "Success at writing string tables!\n";
  }
  else
  {
    std::cerr << "Failed at writing string tables!\n";
    return SRTP::rcFileError;
  }

  // just for checks, try to load it
  const int result = reader.readESM(path.string(), header_rec, SRTP::Localization::German);
  if (result < 0)
  {
    std::cerr << "Error: Could not load saved stuff!\n";
  }
  else
  {
    std::cout << "Loading saved stuff succeeded, read " << result << " group(s)!\n";
  }

  return 0;
}
