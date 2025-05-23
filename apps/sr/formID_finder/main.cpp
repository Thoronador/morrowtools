/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2015, 2021, 2023  Dirk Stolle

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
#include <sstream>
#include <string>
#include <set>
#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif
#include "../../../lib/base/FileFunctions.hpp"
#include "../../../lib/base/UtilityFunctions.hpp"
#include "../../../lib/sr/Activators.hpp"
#include "../../../lib/sr/AlchemyPotions.hpp"
#include "../../../lib/sr/Ammunitions.hpp"
#include "../../../lib/sr/Apparatuses.hpp"
#include "../../../lib/sr/Armours.hpp"
#include "../../../lib/sr/Books.hpp"
#include "../../../lib/sr/Cells.hpp"
#include "../../../lib/sr/Containers.hpp"
#include "../../../lib/sr/DependencySolver.hpp"
#include "../../../lib/sr/Edition.hpp"
#include "../../../lib/sr/Factions.hpp"
#include "../../../lib/sr/Floras.hpp"
#include "../../../lib/sr/FormIDFunctions.hpp"
#include "../../../lib/sr/Furniture.hpp"
#include "../../../lib/sr/Ingredients.hpp"
#include "../../../lib/sr/Keys.hpp"
#include "../../../lib/sr/Localization.hpp"
#include "../../../lib/sr/MiscObjects.hpp"
#include "../../../lib/sr/NPCs.hpp"
#include "../../../lib/sr/PathFunctions.hpp"
#include "../../../lib/sr/Perks.hpp"
#include "../../../lib/sr/ReturnCodes.hpp"
#include "../../../lib/sr/Quests.hpp"
#include "../../../lib/sr/Scrolls.hpp"
#include "../../../lib/sr/Shouts.hpp"
#include "../../../lib/sr/SoulGems.hpp"
#include "../../../lib/sr/Spells.hpp"
#include "../../../lib/sr/TalkingActivators.hpp"
#include "../../../lib/sr/Trees.hpp"
#include "../../../lib/sr/Weapons.hpp"
#include "../../../lib/sr/WordsOfPower.hpp"
#include "../../../lib/sr/records/TES4HeaderRecord.hpp"
#include "AuxFunctions.hpp"
#include "ESMReaderFinder.hpp"
#include "ESMReaderFinderReferences.hpp"

void showGPLNotice()
{
  std::cout << "Form ID finder for Skyrim\n"
            << "  Copyright (C) 2011, 2012, 2013, 2015, 2021  Thoronador\n"
            << "\n"
            << "  This programme is free software: you can redistribute it and/or\n"
            << "  modify it under the terms of the GNU General Public License as published\n"
            << "  by the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This programme is distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this programme.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Form ID Finder for Skyrim, version 0.25-pre2.rev524, 2021-11-06\n";
}

int showVersionExitcode()
{
  showVersion();
  return 524;
}

void showHelp()
{
  std::cout << "\nformID_finder [-d DIRECTORY] -p PATTERN\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the program and quits\n"
            << "  -v               - same as --version\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Skyrim to\n"
            << "                     DIRECTORY\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -p TEXT          - set the search pattern to be TEXT.\n"
            << "  --keyword TEXT   - same as -p\n"
            << "  --case-sensitive - use case-sensitive search, i.e. upper and lower case\n"
            << "                     version of the same letter won't match. Deactivated by\n"
            << "                     default.\n"
            << "  --all-quest-info - shows complete quest info, including texts for journal\n"
            << "                     entries and objectives.\n"
            << "  --faction-ranks  - shows the ranks of matching factions, too.\n"
            << "  --ranks          - same as --faction-ranks\n"
            << "  --ref-id         - try to find reference IDs, too. With this parameter the\n"
            << "                     program will need a significantly longer amount of time\n"
            << "                     to complete a search.\n"
            << "  --skyrim-se      - assume that Skyrim Special Edition is installed and use\n"
            << "                     that installation.\n"
            << "  --oldrim         - assume that the old Skyrim of 2011 is installed and use\n"
            << "                     that installation.\n"
            << "  --english | --en - set the language to load to English. This only triggers,\n"
            << "                     if there are no lose string table files and loading from\n"
            << "                     the BSA file takes place. Same holds for other language\n"
            << "                     options.  Default is German, if no language is set.\n"
            << "  --french | --fr  - set the language to load to French.\n"
            << "  --german | --de  - set the language to load to German.\n"
            << "  --italian | --it - set the language to load to Italian.\n"
            << "  --polish | --pl  - set the language to load to Polish.\n"
            << "  --russian | --ru - set the language to load to Russian.\n"
            << "  --spanish | --es - set the language to load to Spanish.\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();

  // data files directory - empty at start
  std::string dataDir = "";
  // word to be searched for - empty at start, user has to(!) set it
  std::string searchKeyword = "";
  bool caseSensitive = false;
  bool allQuestInfo = false;
  bool listFactionRanks = false;
  bool sendData = false;
  std::string sendParam1st = "";
  std::string sendParam2nd = "";
  bool withReferences = false;
  bool showFiles = false;
  std::optional<SRTP::Edition> edition = std::nullopt;
  std::optional<SRTP::Localization> localization = std::nullopt;

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
        // version information requested?
        else if ((param == "--version") || (param == "-v"))
        {
          showVersion();
          return 0;
        }
        else if (param == "--version-with-exitcode")
        {
          return showVersionExitcode();
        }
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // set more than once?
          if (!dataDir.empty())
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
              dataDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add it.
              dataDir = slashify(dataDir);
              std::cout << "Data files directory was set to \"" << dataDir << "\".\n";
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
        else if ((param == "-p") || (param == "--keyword"))
        {
          // set more than once?
          if (!searchKeyword.empty())
          {
            std::cerr << "Error: Search keyword was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            searchKeyword = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as search keyword already
            std::cout << "Search keyword was set to \"" << searchKeyword << "\".\n";
          }
          else
          {
            std::cerr << "Error: You have to enter some text after \""
                      << param << "\".\n";
            return SRTP::rcInvalidParameter;
          }
        } // keyword
        else if (param == "--case-sensitive")
        {
          // set more than once?
          if (caseSensitive)
          {
            std::cerr << "Error: Parameter \"" << param << "\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          caseSensitive = true;
          std::cout << "Case-sensitive search mode enabled.\n";
        } // case sensitive
        else if (param == "--all-quest-info")
        {
          // set more than once?
          if (allQuestInfo)
          {
            std::cerr << "Error: Parameter \"" << param << "\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          allQuestInfo = true;
          std::cout << "Complete quest texts enabled.\n";
        } // all quest info
        else if ((param == "--faction-ranks") || (param == "--ranks"))
        {
          // set more than once?
          if (listFactionRanks)
          {
            std::cerr << "Error: Parameter \"" << param << "\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          listFactionRanks = true;
          std::cout << "Listing faction ranks was enabled.\n";
        } // faction rank info
        else if (param == "--send-data")
        {
          // set more than once?
          if (sendData)
          {
            std::cerr << "Error: Parameter " << param << " was already specified!\n";
            return SRTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i + 2 < argc) && (argv[i+1] != nullptr) && (argv[i+2] != nullptr))
          {
            sendData = true;
            sendParam1st = std::string(argv[i+1]);
            sendParam2nd = std::string(argv[i+2]);
            i = i + 2; // skip next two parameters, because they are used as send params already
            std::cout << "Data sending mode was requested (\"" << sendParam1st
                      << "\", \"" << sendParam2nd << "\").\n";
          }
          else
          {
            std::cerr << "Error: You have to enter two string parameters after \""
                      << param << "\".\n";
            return SRTP::rcInvalidParameter;
          }
        } // send data
        else if ((param == "--special-edition") || (param == "--skyrim-se"))
        {
          // set more than once?
          if (edition.has_value())
          {
            std::cerr << "Error: Skyrim edition was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          edition = SRTP::Edition::SpecialEdition;
          std::cout << "Info: Handling Skyrim as Skyrim Special Edition.\n";
        } // edition: Skyrim SE
        else if ((param == "--original-edition") || (param == "--oldrim"))
        {
          // set more than once?
          if (edition.has_value())
          {
            std::cerr << "Error: Skyrim edition was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          edition = SRTP::Edition::Skyrim2011;
          std::cout << "Info: Handling Skyrim as Skyrim of 2011.\n";
        } // edition: Skyrim (original)
        else if ((param == "--english") || (param == "--en"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::English;
        } // string table language: English
        else if ((param == "--french") || (param == "--fr"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::French;
        } // string table language: French
        else if ((param == "--german") || (param == "--de"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::German;
        } // string table language: German
        else if ((param == "--italian") || (param == "--it"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::Italian;
        } // string table language: Italian
        else if ((param == "--polish") || (param == "--pl"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::Polish;
        } // string table language: Polish
        else if ((param == "--russian") || (param == "--ru"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::Russian;
        } // string table language: Russian
        else if ((param == "--spanish") || (param == "--es"))
        {
          // set more than once?
          if (localization.has_value())
          {
            std::cerr << "Error: Localization was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          localization = SRTP::Localization::Spanish;
        } // string table language: Spanish
        else if ((param == "--ref-id") || (param == "--ref-ids") || (param == "--ref")
                  || (param == "--refs") || (param == "--references"))
        {
          // set more than once?
          if (withReferences)
          {
            std::cerr << "Error: Parameter \"" << param << "\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          withReferences = true;
          std::cout << "Search for reference IDs activated.\n";
        } // references
        else if (param == "--show-files")
        {
          // set more than once?
          if (showFiles)
          {
            std::cerr << "Error: Parameter " << param << " was already specified!\n";
            return SRTP::rcInvalidParameter;
          }
          showFiles = true;
        } // show files
        else if (searchKeyword.empty())
        {
          // assume search keyword was given without prior --keyword option
          searchKeyword = std::string(argv[i]);
          std::cout << "Assuming sloppy command line parameters, search keyword was set to \""
                    << searchKeyword << "\".\n";
        }
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
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  // Has the user specified a data directory?
  SRTP::getDataDir(dataDir, edition);

  // keyword given?
  if (searchKeyword.empty())
  {
    std::cout << "Error: No search keyword was specified. Use the parameter --keyword"
              << " to specify the stuff you want that programme to search for.\n";
    return SRTP::rcInvalidParameter;
  }

  // adjust keyword to selected case-sensitivity
  if (!caseSensitive)
    searchKeyword = lowerCase(searchKeyword);

  std::cout << "\n\nSearching for \"" << searchKeyword << "\" using case-";
  if (!caseSensitive)
    std::cout << "in";
  std::cout << "sensitive search. This may take a while...\n";
  if (withReferences)
  {
    std::cout << "Since you want to search for reference IDs, too, this may take even longer...\n";
  }

  std::vector<std::string> loadOrder;
  {
    std::vector<std::string> esmNames;
    esmNames.push_back("Skyrim.esm");
    if (FileExists(dataDir + "Update.esm"))
      esmNames.push_back("Update.esm");
    if (FileExists(dataDir + "Dawnguard.esm"))
      esmNames.push_back("Dawnguard.esm");
    if (FileExists(dataDir + "HearthFires.esm"))
      esmNames.push_back("HearthFires.esm");
    if (FileExists(dataDir + "Dragonborn.esm"))
      esmNames.push_back("Dragonborn.esm");

    if (FileExists(dataDir + "ccBGSSSE001-Fish.esm"))
      esmNames.push_back("ccBGSSSE001-Fish.esm");
    if (FileExists(dataDir + "ccBGSSSE025-AdvDSGS.esm"))
      esmNames.push_back("ccBGSSSE025-AdvDSGS.esm");
    if (FileExists(dataDir + "ccBGSSSE037-Curios.esl"))
      esmNames.push_back("ccBGSSSE037-Curios.esl");
    if (FileExists(dataDir + "ccQDRSSE001-SurvivalMode.esl"))
      esmNames.push_back("ccQDRSSE001-SurvivalMode.esl");

    if (!SRTP::getLoadOrder(esmNames, dataDir, loadOrder))
    {
      std::cerr << "Error: could not determine load order of ESM files!\n";
      return SRTP::rcFileError;
    }
  }

  SRTP::ESMReaderFinder reader(loadOrder);
  SRTP::Tes4HeaderRecord tes4rec;

  // read the usual stuff (for base IDs)
  for (const auto& element: loadOrder)
  {
    if (element != "Update.esm")
    {
      reader.requestIndexMapUpdate(element);
      if (reader.readESM(dataDir + element, tes4rec, localization) < 0)
      {
        std::cerr << "Error while reading " << dataDir + element << "!\n";
        return SRTP::rcFileError;
      }
    }
  }

  SRTP::ESMReaderFinderReferences readerReferences(loadOrder);
  if (withReferences)
  {
    for (const auto& fileName: loadOrder)
    {
      if (fileName != "Update.esm")
      {
        readerReferences.requestIndexMapUpdate(fileName);
        if (readerReferences.readESM(dataDir + fileName, tes4rec, localization) < 0)
        {
          std::cerr << "Error while reading references from "
                    << dataDir + fileName << "!\n";
          return SRTP::rcFileError;
        }
      }
    }
  } // if references requested

  std::ostringstream string_out;
  std::basic_ostream<char>& basic_out = sendData ? string_out : std::cout;

  /*Adjust show files value.

    Files are shown by default. However, if sendData is true, then they are not
    shown for compatibility reasons, unless the --show-files parameter was set.
  */
  showFiles = (showFiles || !sendData);

  unsigned int totalMatches = 0;

  auto listMatches = [&] (const auto& mgr, const std::string_view typePlural)
  {
    unsigned int typedMatches = 0;
    auto mgr_iter = mgr.begin();
    const auto end = mgr.end();
    while (mgr_iter != end)
    {
      if (mgr_iter->second.name.isPresent())
      {
        if (!mgr_iter->second.name.getString().empty())
        {
          if (matchesKeyword(mgr_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching record
            if (typedMatches == 0)
            {
              basic_out << "\n\nMatching " << typePlural << ":\n";
            }
            basic_out << "    \"" << mgr_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(mgr_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << mgr_iter->second.editorID << "\"\n";
            if (withReferences)
            {
              showRefIDs(mgr_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++typedMatches;
            ++totalMatches;
          }
        }
      }
      ++mgr_iter;
    }
    if (typedMatches > 0)
    {
      basic_out << "Total matching " << typePlural << ": " << typedMatches << "\n";
    }
  };

  listMatches(SRTP::Activators::get(), "activators");
  listMatches(SRTP::AlchemyPotions::get(), "alchemy potions");
  listMatches(SRTP::Ammunitions::get(), "ammunition");
  listMatches(SRTP::Apparatuses::get(), "apparatuses");
  listMatches(SRTP::Armours::get(), "armours");

  // check books for matches
  {
    unsigned int bookMatches = 0;
    auto book_iter = SRTP::Books::get().begin();
    while (book_iter != SRTP::Books::get().end())
    {
      if (book_iter->second.title.isPresent())
      {
        if (!book_iter->second.title.getString().empty())
        {
          if (matchesKeyword(book_iter->second.title.getString(), searchKeyword, caseSensitive))
          {
            // found matching book record
            if (bookMatches == 0)
            {
              basic_out << "\n\nMatching books:\n";
            }
            basic_out << "    \"" << book_iter->second.title.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(book_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << book_iter->second.editorID << "\"\n";
            if (withReferences)
            {
              showRefIDs(book_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++bookMatches;
            ++totalMatches;
          }
        }
      }
      ++book_iter;
    }
    if (bookMatches > 0)
    {
      basic_out << "Total matching books: " << bookMatches << "\n";
    }
  }

  listMatches(SRTP::Containers::get(), "containers");

  // check factions for matches
  {
    unsigned int factionMatches = 0;
    auto faction_iter = SRTP::Factions::get().begin();
    while (faction_iter != SRTP::Factions::get().end())
    {
      if (faction_iter->second.name.isPresent())
      {
        if (!faction_iter->second.name.getString().empty())
        {
          if (matchesKeyword(faction_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching faction record
            if (factionMatches == 0)
            {
              basic_out << "\n\nMatching factions:\n";
            }
            basic_out << "    \"" << faction_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(faction_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << faction_iter->second.editorID << "\"\n";
            if (listFactionRanks)
            {
              basic_out << "        ranks: ";
              const auto rankCount = faction_iter->second.ranks.size();
              if (rankCount == 0)
              {
                // no ranks found
                basic_out << "none\n";
              }
              else
              {
                basic_out << rankCount << "\n";
                for (const auto& rank: faction_iter->second.ranks)
                {
                  basic_out << "          (" << rank.index
                            << ") male: ";
                  if (!rank.maleName.isPresent())
                  {
                    basic_out << "(none)";
                  }
                  else
                  {
                    basic_out << "\"" << rank.maleName.getString() << "\"";
                  }
                  basic_out << ", female: ";
                  if (!rank.femaleName.isPresent())
                  {
                    basic_out << "(none)\n";
                  }
                  else
                  {
                    basic_out << "\"" << rank.femaleName.getString() << "\"\n";
                  }
                }
              } // else (at least one rank)
            } // if faction ranks requested
            ++factionMatches;
            ++totalMatches;
          }
        }
      }
      ++faction_iter;
    }
    if (factionMatches > 0)
    {
      basic_out << "Total matching factions: " << factionMatches << "\n";
    }
  }

  listMatches(SRTP::Floras::get(), "floras");
  listMatches(SRTP::Furniture::get(), "furniture");
  listMatches(SRTP::Ingredients::get(), "ingredients");
  listMatches(SRTP::Keys::get(), "keys");
  listMatches(SRTP::MiscObjects::get(), "misc. objects");
  listMatches(SRTP::NPCs::get(), "NPCs");

  // check perks for matches - no reference checks
  {
    unsigned int perkMatches = 0;
    auto perk_iter = SRTP::Perks::get().begin();
    while (perk_iter != SRTP::Perks::get().end())
    {
      if (perk_iter->second.name.isPresent())
      {
        if (!perk_iter->second.name.getString().empty())
        {
          if (matchesKeyword(perk_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching perk record
            if (perkMatches == 0)
            {
              basic_out << "\n\nMatching perks:\n";
            }
            basic_out << "    \"" << perk_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(perk_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << perk_iter->second.editorID << "\"\n";
            ++perkMatches;
            ++totalMatches;
          }
        }
      }
      ++perk_iter;
    }
    if (perkMatches > 0)
    {
      basic_out << "Total matching perks: " << perkMatches << "\n";
    }
  }

  // check quests for matches
  {
    unsigned int questMatches = 0;
    bool prefix;
    auto quest_iter = SRTP::Quests::get().begin();
    while (quest_iter != SRTP::Quests::get().end())
    {
      if (quest_iter->second.name.isPresent())
      {
        if (!quest_iter->second.name.getString().empty())
        {
          if (matchesKeyword(quest_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching quest record
            if (questMatches == 0)
            {
              basic_out << "\n\nMatching quests:\n";
            }
            basic_out << "    \"" << quest_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(quest_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << quest_iter->second.editorID << "\"\n";
            // indices
            const auto idx_count = quest_iter->second.indices.size();
            if (!allQuestInfo)
            {
              basic_out << "        indices: ";
              for (std::vector<SRTP::IndexEntry>::size_type i = 0; i < idx_count; ++i)
              {
                if (i != 0)
                  basic_out << ", ";
                basic_out << quest_iter->second.indices[i].index;
                if (quest_iter->second.indices[i].hasFinishingQSDT())
                  basic_out << " (finishes)";
              }
              if (idx_count == 0)
                basic_out << "none";
              basic_out << "\n";
            }
            else
            {
              // full quest info requested
              basic_out << "        indices:\n";
              for (const auto& index: quest_iter->second.indices)
              {
                basic_out << "          index " << index.index << "\n";
                // run through QSDTs
                for (const auto& qsdt: index.theQSDTs)
                {
                  prefix = false;
                  if (qsdt.isFinisher)
                  {
                    basic_out << "            (finishes quest) ";
                    prefix = true;
                  }
                  if (qsdt.logEntry.isPresent())
                  {
                    if (!prefix)
                    {
                      prefix = true;
                      basic_out << "            ";
                    }
                    basic_out << "\"" << qsdt.logEntry.getString() << "\"";
                  }
                  if (prefix)
                    basic_out << "\n";
                }
                // check for objective
                if (quest_iter->second.hasQOBJForIndex(index.index))
                {
                  const SRTP::QOBJEntry& ziel = quest_iter->second.getQOBJForIndex(index.index);
                  if (ziel.displayText.isPresent())
                  {
                    basic_out << "            [new objective] \"" << ziel.displayText.getString() << "\"\n";
                  }
                }
              } // for i
              if (idx_count == 0)
                basic_out << "          none\n";
            } // else
            ++questMatches;
            ++totalMatches;
          }
        }
      }
      ++quest_iter;
    }
    if (questMatches > 0)
    {
      basic_out << "Total matching quests: " << questMatches << "\n";
    }
  }

  listMatches(SRTP::Scrolls::get(), "scrolls");
  listMatches(SRTP::SoulGems::get(), "soul gems");
  listMatches(SRTP::Spells::get(), "spells");

  // check shouts for matches - no reference checks
  {
    unsigned int shoutMatches = 0;
    auto shout_iter = SRTP::Shouts::get().begin();
    while (shout_iter != SRTP::Shouts::get().end())
    {
      if (shout_iter->second.name.isPresent())
      {
        if (!shout_iter->second.name.getString().empty())
        {
          if (matchesKeyword(shout_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching shout record
            if (shoutMatches == 0)
            {
              basic_out << "\n\nMatching dragon shouts:\n";
            }
            basic_out << "    \"" << shout_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(shout_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << shout_iter->second.editorID << "\"\n";
            ++shoutMatches;
            ++totalMatches;
          }
        }
      }
      ++shout_iter;
    }
    if (shoutMatches > 0)
    {
      basic_out << "Total matching dragon shouts: " << shoutMatches << "\n";
    }
  }

  // check words of power for matches (also checks editor ID for matches)
  {
    unsigned int wordMatches = 0;
    auto word_iter = SRTP::WordsOfPower::get().begin();
    while (word_iter != SRTP::WordsOfPower::get().end())
    {
      if (word_iter->second.name.isPresent())
      {
        if (!word_iter->second.name.getString().empty())
        {
          if (matchesKeyword(word_iter->second.name.getString(), searchKeyword, caseSensitive)
            || matchesKeyword(word_iter->second.editorID, searchKeyword, caseSensitive))
          {
            // found matching word of power record
            if (wordMatches == 0)
            {
              basic_out << "\n\nMatching words of power:\n";
            }
            basic_out << "    \"" << word_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(word_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << word_iter->second.editorID << "\"\n";
            ++wordMatches;
            ++totalMatches;
          }
        }
      }
      ++word_iter;
    }
    if (wordMatches > 0)
    {
      basic_out << "Total matching words of power: " << wordMatches << "\n";
    }
  }

  listMatches(SRTP::TalkingActivators::get(), "talking activators");

  // check trees for matches - no reference checks
  {
    unsigned int treeMatches = 0;
    auto tree_iter = SRTP::Trees::get().begin();
    while (tree_iter != SRTP::Trees::get().end())
    {
      if (tree_iter->second.name.isPresent())
      {
        if (!tree_iter->second.name.getString().empty())
        {
          if (matchesKeyword(tree_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            // found matching tree record
            if (treeMatches == 0)
            {
              basic_out << "\n\nMatching trees:\n";
            }
            basic_out << "    \"" << tree_iter->second.name.getString()
                      << "\"\n        form ID " << SRTP::getFormIDAsStringWithFile(tree_iter->second.headerFormID, loadOrder, showFiles)
                      << "\n        editor ID \"" << tree_iter->second.editorID << "\"\n";
            ++treeMatches;
            ++totalMatches;
          }
        }
      }
      ++tree_iter;
    }
    if (treeMatches > 0)
    {
      basic_out << "Total matching trees: " << treeMatches << "\n";
    }
  }

  listMatches(SRTP::Weapons::get(), "weapons");

  basic_out << "\nTotal matching objects found: " << totalMatches << "\n";

  if (sendData)
  {
    #if defined(_WIN32)
    COPYDATASTRUCT cds;
    cds.dwData = 0;
    cds.cbData = string_out.str().length() + 1;
    cds.lpData = (void*) string_out.str().c_str();
    const char * ptrSend1 = (sendParam1st == "NULL") ? nullptr : sendParam1st.c_str();
    const char * ptrSend2 = (sendParam2nd == "NULL") ? nullptr : sendParam2nd.c_str();
    HWND receiver = FindWindow(ptrSend1, ptrSend2);
    if (receiver == nullptr)
    {
      std::cerr << "Error in send data mode: Cannot find receiving window!\n";
      return SRTP::rcWindowNotFound;
    }
    int msgResult = SendMessage(receiver, WM_COPYDATA, 0, (LPARAM) &cds);
    #else
    // check for socket
    if (sendParam1st != "socket")
    {
      std::cerr << "Error: Parameter --send-data expects socket as first part!\n";
      return SRTP::rcInvalidParameter;
    }
    // create Unix domain socket
    const int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
      std::cerr << "Error: Could not open Unix domain socket!\n";
      return SRTP::rcSocketError;
    }
    // prepare socket address structure
    struct sockaddr_un serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_UNIX;
    snprintf(serv_addr.sun_path, sendParam2nd.length() + 1, sendParam2nd.c_str());
    // now connect
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_un)) != 0)
    {
      std::cerr << "Error: Could not connect via Unix domain socket!\n";
      return SRTP::rcSocketError;
    }
    // write to socket
    const std::string socketBuffer = string_out.str();
    write(socket_fd, socketBuffer.c_str(), socketBuffer.length() + 1);
    // ...and close socket
    close(socket_fd);
    #endif
  } // if sendData
  return 0;
}
