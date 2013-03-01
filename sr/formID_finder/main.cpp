/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013 Thoronador

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
#include <sstream>
#include <string>
#include <set>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif
#include "../../base/UtilityFunctions.h"
#include "../base/Activators.h"
#include "../base/AlchemyPotions.h"
#include "../base/Ammunitions.h"
#include "../base/Apparatuses.h"
#include "../base/Armours.h"
#include "../base/Books.h"
#include "../base/Cells.h"
#include "../base/Containers.h"
#include "../base/DependencySolver.h"
#include "../base/Factions.h"
#include "../base/Floras.h"
#include "../base/FormIDFunctions.h"
#include "../base/Furniture.h"
#include "../base/Ingredients.h"
#include "../base/Keys.h"
#include "../base/MiscObjects.h"
#include "../base/NPCs.h"
#include "../base/PathFunctions.h"
#include "../base/Perks.h"
#include "../base/Quests.h"
#include "../base/Scrolls.h"
#include "../base/Shouts.h"
#include "../base/SoulGems.h"
#include "../base/Spells.h"
#include "../base/TalkingActivators.h"
#include "../base/Trees.h"
#include "../base/Weapons.h"
#include "../base/WordsOfPower.h"
#include "../base/records/TES4HeaderRecord.h"
#include "ESMReaderFinder.h"
#include "ESMReaderFinderReferences.h"

void showGPLNotice()
{
  std::cout << "Form ID finder for Skyrim\n"
            //<< "  This programme is part of the Skyrim Tools Project.\n"
            << "  Copyright (C) 2011, 2012, 2013 Thoronador\n"
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
  std::cout << "Form ID Finder for Skyrim, version 0.22e.rev518~experimental, 2013-02-28\n";
}

int showVersionExitcode()
{
  showVersion();
  return 518;
}

void showHelp()
{
  std::cout << "\nformID_finder [-d DIRECTORY] -p PATTERN\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
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
            << "                     programme will need a significantly longer amount of time\n"
            << "                     to complete a search.\n";
}

/* auxillary function #1:
   Returns true, if the keyword is found in haystack. Character case is taken
   into account, if caseMatters is true.

   parameters:
       haystack    - the string to search in
       keyword     - the string to search for
       caseMatters - If true, case-sensitive search is performed. Otherwise the
                     search is case-insensitive.
*/
bool matchesKeyword(const std::string& haystack, const std::string& keyword, const bool caseMatters)
{
  if (caseMatters)
  {
    return (haystack.find(keyword)!=std::string::npos);
  }
  return (lowerCase(haystack).find(keyword)!=std::string::npos);
}

/*...and another auxillary function (#2):
  Writes all references of a base ID to the given ostream.

  parameters:
      baseID    - baseID of the object
      refMap    - reference map as produced by ESMReaderFinderReferences
      basic_out - the output stream to which the references are written
*/
void showRefIDs(const uint32_t baseID, const std::map<uint32_t, std::vector<SRTP::ESMReaderFinderReferences::CellRefIDPair> >& refMap, std::basic_ostream<char>& basic_out)
{
  basic_out << "        references: ";
  std::map<uint32_t, std::vector<SRTP::ESMReaderFinderReferences::CellRefIDPair> >::const_iterator iter = refMap.find(baseID);
  if (iter!=refMap.end())
  {
    basic_out << iter->second.size()<<"\n";
  }
  else
  {
    basic_out << "none\n";
    return;
  }
  bool hasName = false;
  std::vector<SRTP::ESMReaderFinderReferences::CellRefIDPair>::const_iterator vecIter = iter->second.begin();
  while (vecIter!=iter->second.end())
  {
    hasName = false;
    basic_out << "          ref ID "<<SRTP::getFormIDAsString(vecIter->refID);
    if (SRTP::Cells::getSingleton().hasRecord(vecIter->cellID))
    {
      const SRTP::CellRecord& theCell = SRTP::Cells::getSingleton().getRecord(vecIter->cellID);
      if (theCell.name.isPresent())
      {
        if (!theCell.name.getString().empty())
        {
          basic_out << " in cell \""<<theCell.name.getString()<<"\"\n";
          hasName = true;
        }
      }//if FULL
      else
      {
        //check for coordinates
        if (theCell.gridLocation.presence)
        {
          basic_out << " in exterior cell ["<<theCell.gridLocation.locationX<<";"<<theCell.gridLocation.locationY<<"]\n";
          hasName = true;
        }
      }
    }
    if (!hasName)
    {
      basic_out << " in unnamend cell\n";
    }
    ++vecIter;
  }//while
}

/* the main functions, obviously */
int main(int argc, char **argv)
{
  showGPLNotice();

  //data files directory - empty at start
  std::string dataDir = "";
  //word to be searched for - empty at start, user has to(!) set it
  std::string searchKeyword = "";
  bool caseSensitive = false;
  bool allQuestInfo = false;
  bool listFactionRanks = false;
  bool sendData = false;
  std::string sendParam1st ="", sendParam2nd ="";
  bool withReferences = false;

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-?") or (param=="/?"))
        {
          showHelp();
          return 0;
        }//if help wanted
        //version information requested?
        else if ((param=="--version") or (param=="-v"))
        {
          showVersion();
          return 0;
        }
        else if (param=="--version-with-exitcode")
        {
          return showVersionExitcode();
        }
        else if ((param=="-d") or (param=="-dir") or (param=="--data-files"))
        {
          //set more than once?
          if (!dataDir.empty())
          {
            std::cout << "Error: Data directory was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            //Is it long enough to be a directoy? (Minimum should be "./".)
            if (std::string(argv[i+1]).size()>1)
            {
              dataDir = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as directory name already
              std::cout << "Data files directory was set to \""<<dataDir<<"\".\n";
              //Does it have a trailing (back)slash?
              if (dataDir.at(dataDir.length()-1)!='\\')
              {
                dataDir = dataDir + "\\";
                std::cout << "Missing trailing backslash was added.\n";
              }//if slash
            }
            else
            {
              std::cout << "Parameter \""<<std::string(argv[i+1])<<"\" is too"
                        << " short to be a proper directory path.\n";
              return SRTP::rcInvalidParameter;
            }//else
          }
          else
          {
            std::cout << "Error: You have to specify a directory name after \""
                      << param<<"\".\n";
            return SRTP::rcInvalidParameter;
          }
        }//data files directory
        else if ((param=="-p") or (param=="--keyword"))
        {
          //set more than once?
          if (!searchKeyword.empty())
          {
            std::cout << "Error: search keyword was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            searchKeyword = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as search keyword already
            std::cout << "Search keyword was set to \""<<searchKeyword<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to enter some text after \""
                      << param<<"\".\n";
            return SRTP::rcInvalidParameter;
          }
        }//keyword
        else if (param=="--case-sensitive")
        {
          //set more than once?
          if (caseSensitive)
          {
            std::cout << "Error: parameter \""<<param<<"\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          caseSensitive = true;
          std::cout << "Case-sensitive search modus enabled.\n";
        }//case sensitive
        else if (param=="--all-quest-info")
        {
          //set more than once?
          if (allQuestInfo)
          {
            std::cout << "Error: parameter \""<<param<<"\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          allQuestInfo = true;
          std::cout << "Complete quest texts enabled.\n";
        }//all quest info
        else if ((param=="--faction-ranks") or (param=="--ranks"))
        {
          //set more than once?
          if (listFactionRanks)
          {
            std::cout << "Error: parameter \""<<param<<"\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          listFactionRanks = true;
          std::cout << "Listing faction ranks was enabled.\n";
        }//faction rank info
        else if (param=="--send-data")
        {
          //set more than once?
          if (sendData)
          {
            std::cout << "Error: parameter "<<param<<" was already specified!\n";
            return SRTP::rcInvalidParameter;
          }
          //enough parameters?
          if ((i+2<argc) and (argv[i+1]!=NULL) and (argv[i+2]!=NULL))
          {
            sendData = true;
            sendParam1st = std::string(argv[i+1]);
            sendParam2nd = std::string(argv[i+2]);
            i=i+2; //skip next two parameters, because they are used as send params already
            std::cout << "Data sending mode was requested (\""<<sendParam1st<<"\", \""<<sendParam2nd<<"\").\n";
          }
          else
          {
            std::cout << "Error: You have to enter two string parameters after \""
                      << param<<"\".\n";
            return SRTP::rcInvalidParameter;
          }
        }//send data
        else if ((param=="--ref-id") or (param=="--ref-ids") or (param=="--ref")
                  or (param=="--refs")or (param=="--references"))
        {
          //set more than once?
          if (withReferences)
          {
            std::cout << "Error: parameter \""<<param<<"\" was specified twice!\n";
            return SRTP::rcInvalidParameter;
          }
          withReferences = true;
          std::cout << "Search for reference IDs activated.\n";
        }//references
        else if (searchKeyword.empty())
        {
          //assume search keyword was given without prior --keyword option
          searchKeyword = std::string(argv[i]);
          std::cout << "Assuming sloppy command line parameters, search keyword was set to \""<<searchKeyword<<"\".\n";
        }
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return SRTP::rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return SRTP::rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  //Has the user specified a data directory?
  SRTP::getDataDir(dataDir);

  //keyword given?
  if (searchKeyword.empty())
  {
    std::cout << "Error: No search keyword was specified. Use the parameter --keyword"
              << " to specify the stuff you want that programme to search for.\n";
    return SRTP::rcInvalidParameter;
  }//if no keyword given

  //adjust keyword to selected case-sensitivity
  if (!caseSensitive) searchKeyword = lowerCase(searchKeyword);

  std::cout << "\n\nSearching for \""<<searchKeyword<<"\" using case-";
  if (!caseSensitive) std::cout <<"in";
  std::cout << "sensitive search. This may take a while...\n";
  if (withReferences)
  {
    std::cout << "Since you want to search for reference IDs, too, this may take even longer...\n";
  }

  std::vector<std::string> loadOrder;
  {
    std::vector<std::string> esmNames;
    esmNames.push_back("Skyrim.esm");
    if (FileExists(dataDir+"Update.esm"))
      esmNames.push_back("Update.esm");
    if (FileExists(dataDir+"Dawnguard.esm"))
      esmNames.push_back("Dawnguard.esm");
    if (FileExists(dataDir+"HearthFires.esm"))
      esmNames.push_back("HearthFires.esm");
    if (FileExists(dataDir+"Dragonborn.esm"))
      esmNames.push_back("Dragonborn.esm");

    if (!SRTP::getLoadOrder(esmNames, dataDir, loadOrder))
    {
      std::cout << "Error: could not determine load order of ESM files!\n";
      return SRTP::rcFileError;
    }
  }

  SRTP::ESMReaderFinder reader(loadOrder);
  SRTP::Tes4HeaderRecord tes4rec;

  //read the usual stuff (for base IDs)
  unsigned int lo_idx;
  for (lo_idx=0; lo_idx<loadOrder.size(); ++lo_idx)
  {
    if (loadOrder[lo_idx]!="Update.esm")
    {
      reader.requestIndexMapUpdate(loadOrder[lo_idx]);
      if (reader.readESM(dataDir+loadOrder[lo_idx], tes4rec)<0)
      {
        std::cout << "Error while reading "<<dataDir+loadOrder[lo_idx]<<"!\n";
        return SRTP::rcFileError;
      }
    }
  }//for i

  SRTP::ESMReaderFinderReferences readerReferences(loadOrder);
  if (withReferences)
  {
    for (lo_idx=0; lo_idx<loadOrder.size(); ++lo_idx)
    {
      if (loadOrder[lo_idx]!="Update.esm")
      {
        readerReferences.requestIndexMapUpdate(loadOrder[lo_idx]);
        if (readerReferences.readESM(dataDir+loadOrder[lo_idx], tes4rec)<0)
        {
          std::cout << "Error while reading references from "<<dataDir+loadOrder[lo_idx]<<"!\n";
          return SRTP::rcFileError;
        }
      }
    }
  }//if references requested

  std::ostringstream string_out;
  std::basic_ostream<char>& basic_out = sendData ? string_out : std::cout;

  unsigned int totalMatches = 0;

  //check activator for matches
  {
    unsigned int activatorMatches = 0;
    SRTP::Activators::ListIterator activator_iter = SRTP::Activators::getSingleton().getBegin();
    while (activator_iter!=SRTP::Activators::getSingleton().getEnd())
    {
      if (activator_iter->second.name.isPresent())
      {
        if (!activator_iter->second.name.getString().empty())
        {
          if (matchesKeyword(activator_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching activator record
            if (activatorMatches==0)
            {
              basic_out << "\n\nMatching activators:\n";
            }
            basic_out << "    \""<<activator_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(activator_iter->second.headerFormID)
                      <<"\n        editor ID \""<<activator_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(activator_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++activatorMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++activator_iter;
    }//while
    if (activatorMatches>0)
    {
      basic_out << "Total matching activators: "<<activatorMatches<<"\n";
    }
  }//scope for activator stuff

  //check alchemy for matches
  {
    unsigned int alchemyMatches = 0;
    SRTP::AlchemyPotions::ListIterator alchemy_iter = SRTP::AlchemyPotions::getSingleton().getBegin();
    while (alchemy_iter!=SRTP::AlchemyPotions::getSingleton().getEnd())
    {
      if (alchemy_iter->second.name.isPresent())
      {
        if (!alchemy_iter->second.name.getString().empty())
        {
          if (matchesKeyword(alchemy_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching alchemy record
            if (alchemyMatches==0)
            {
              basic_out << "\n\nMatching alchemy potions:\n";
            }
            basic_out << "    \""<<alchemy_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(alchemy_iter->second.headerFormID)
                      <<"\n        editor ID \""<<alchemy_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(alchemy_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++alchemyMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++alchemy_iter;
    }//while
    if (alchemyMatches>0)
    {
      basic_out << "Total matching alchemy potions: "<<alchemyMatches<<"\n";
    }
  }//scope for alchemy stuff

  //check ammunitions for matches
  {
    unsigned int ammoMatches = 0;
    SRTP::Ammunitions::ListIterator ammo_iter = SRTP::Ammunitions::getSingleton().getBegin();
    while (ammo_iter!=SRTP::Ammunitions::getSingleton().getEnd())
    {
      if (ammo_iter->second.name.isPresent())
      {
        if (!ammo_iter->second.name.getString().empty())
        {
          if (matchesKeyword(ammo_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching ammo record
            if (ammoMatches==0)
            {
              basic_out << "\n\nMatching ammunition:\n";
            }
            basic_out << "    \""<<ammo_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(ammo_iter->second.headerFormID)
                      <<"\n        editor ID \""<<ammo_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(ammo_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++ammoMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++ammo_iter;
    }//while
    if (ammoMatches>0)
    {
      basic_out << "Total matching ammunition: "<<ammoMatches<<"\n";
    }
  }//scope for ammo stuff

  //check apparatuses for matches
  {
    unsigned int appaMatches = 0;
    SRTP::Apparatuses::ListIterator appa_iter = SRTP::Apparatuses::getSingleton().getBegin();
    while (appa_iter!=SRTP::Apparatuses::getSingleton().getEnd())
    {
      if (appa_iter->second.name.isPresent())
      {
        if (!appa_iter->second.name.getString().empty())
        {
          if (matchesKeyword(appa_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching apparatus record
            if (appaMatches==0)
            {
              basic_out << "\n\nMatching apparatuses:\n";
            }
            basic_out << "    \""<<appa_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(appa_iter->second.headerFormID)
                      <<"\n        editor ID \""<<appa_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(appa_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++appaMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if name is present
      ++appa_iter;
    }//while
    if (appaMatches>0)
    {
      basic_out << "Total matching apparatuses: "<<appaMatches<<"\n";
    }
  }//scope for apparatus stuff

  //check armour for matches
  {
    unsigned int armourMatches = 0;
    SRTP::Armours::ListIterator armour_iter = SRTP::Armours::getSingleton().getBegin();
    while (armour_iter!=SRTP::Armours::getSingleton().getEnd())
    {
      if (armour_iter->second.name.isPresent())
      {
        if (!armour_iter->second.name.getString().empty())
        {
          if (matchesKeyword(armour_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching armour record
            if (armourMatches==0)
            {
              basic_out << "\n\nMatching armours:\n";
            }
            basic_out << "    \""<<armour_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(armour_iter->second.headerFormID)
                      <<"\n        editor ID \""<<armour_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(armour_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++armourMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++armour_iter;
    }//while
    if (armourMatches>0)
    {
      basic_out << "Total matching armours: "<<armourMatches<<"\n";
    }
  }//scope for armour stuff

  //check books for matches
  {
    unsigned int bookMatches = 0;
    SRTP::Books::ListIterator book_iter = SRTP::Books::getSingleton().getBegin();
    while (book_iter!=SRTP::Books::getSingleton().getEnd())
    {
      if (book_iter->second.title.isPresent())
      {
        if (!book_iter->second.title.getString().empty())
        {
          if (matchesKeyword(book_iter->second.title.getString(), searchKeyword, caseSensitive))
          {
            //found matching book record
            if (bookMatches==0)
            {
              basic_out << "\n\nMatching books:\n";
            }
            basic_out << "    \""<<book_iter->second.title.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(book_iter->second.headerFormID)
                      <<"\n        editor ID \""<<book_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(book_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++bookMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++book_iter;
    }//while
    if (bookMatches>0)
    {
      basic_out << "Total matching books: "<<bookMatches<<"\n";
    }
  }//scope for book stuff

  //check containers for matches
  {
    unsigned int containerMatches = 0;
    SRTP::Containers::ListIterator container_iter = SRTP::Containers::getSingleton().getBegin();
    while (container_iter!=SRTP::Containers::getSingleton().getEnd())
    {
      if (container_iter->second.name.isPresent())
      {
        if (!container_iter->second.name.getString().empty())
        {
          if (matchesKeyword(container_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching container record
            if (containerMatches==0)
            {
              basic_out << "\n\nMatching containers:\n";
            }
            basic_out << "    \""<<container_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(container_iter->second.headerFormID)
                      <<"\n        editor ID \""<<container_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(container_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++containerMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++container_iter;
    }//while
    if (containerMatches>0)
    {
      basic_out << "Total matching containers: "<<containerMatches<<"\n";
    }
  }//scope for container stuff

  //check factions for matches
  {
    unsigned int factionMatches = 0;
    SRTP::Factions::ListIterator faction_iter = SRTP::Factions::getSingleton().getBegin();
    while (faction_iter!=SRTP::Factions::getSingleton().getEnd())
    {
      if (faction_iter->second.name.isPresent())
      {
        if (!faction_iter->second.name.getString().empty())
        {
          if (matchesKeyword(faction_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching faction record
            if (factionMatches==0)
            {
              basic_out << "\n\nMatching factions:\n";
            }
            basic_out << "    \""<<faction_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(faction_iter->second.headerFormID)
                      <<"\n        editor ID \""<<faction_iter->second.editorID<<"\"\n";
            if (listFactionRanks)
            {
              basic_out << "        ranks: ";
              const unsigned int rankCount = faction_iter->second.ranks.size();
              if (rankCount==0)
              {
                basic_out << "none\n";
              }//no ranks found
              else
              {
                basic_out << rankCount <<"\n";
                unsigned int i;
                for (i=0; i<rankCount; ++i)
                {
                  basic_out << "          ("<<faction_iter->second.ranks[i].index
                            << ") male: ";
                  if (!faction_iter->second.ranks[i].maleName.isPresent())
                  {
                    basic_out << "(none)";
                  }
                  else
                  {
                    basic_out << "\""<<faction_iter->second.ranks[i].maleName.getString()<<"\"";
                  }
                  basic_out << ", female: ";
                  if (!faction_iter->second.ranks[i].femaleName.isPresent())
                  {
                    basic_out << "(none)\n";
                  }
                  else
                  {
                    basic_out << "\""<<faction_iter->second.ranks[i].femaleName.getString()<<"\"\n";
                  }
                }//for
              }//else (at least one rank)
            }//if faction ranks requested
            ++factionMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++faction_iter;
    }//while
    if (factionMatches>0)
    {
      basic_out << "Total matching factions: "<<factionMatches<<"\n";
    }
  }//scope for faction stuff

  //check flora for matches
  {
    unsigned int floraMatches = 0;
    SRTP::Floras::ListIterator flora_iter = SRTP::Floras::getSingleton().getBegin();
    while (flora_iter!=SRTP::Floras::getSingleton().getEnd())
    {
        if (flora_iter->second.name.isPresent())
        {
          if (matchesKeyword(flora_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching flora record
            if (floraMatches==0)
            {
              basic_out << "\n\nMatching flora:\n";
            }
            basic_out << "    \""<<flora_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(flora_iter->second.headerFormID)
                      <<"\n        editor ID \""<<flora_iter->second.editorID<<"\"\n";
            ++floraMatches;
            ++totalMatches;
          }//if match found
        }//if name present
      ++flora_iter;
    }//while
    if (floraMatches>0)
    {
      basic_out << "Total matching florae: "<<floraMatches<<"\n";
    }
  }//scope for flora stuff

  //check furniture for matches
  {
    unsigned int furnitureMatches = 0;
    SRTP::Furniture::ListIterator furniture_iter = SRTP::Furniture::getSingleton().getBegin();
    while (furniture_iter!=SRTP::Furniture::getSingleton().getEnd())
    {
      if (furniture_iter->second.name.isPresent())
      {
        if (!furniture_iter->second.name.getString().empty())
        {
          if (matchesKeyword(furniture_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching furniture record
            if (furnitureMatches==0)
            {
              basic_out << "\n\nMatching furniture:\n";
            }
            basic_out << "    \""<<furniture_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(furniture_iter->second.headerFormID)
                      <<"\n        editor ID \""<<furniture_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(furniture_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++furnitureMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++furniture_iter;
    }//while
    if (furnitureMatches>0)
    {
      basic_out << "Total matching furniture: "<<furnitureMatches<<"\n";
    }
  }//scope for furniture stuff

  //check ingredients for matches
  {
    unsigned int ingredMatches = 0;
    SRTP::Ingredients::ListIterator ingred_iter = SRTP::Ingredients::getSingleton().getBegin();
    while (ingred_iter!=SRTP::Ingredients::getSingleton().getEnd())
    {
      //if (ingred_iter->second.hasFULL)
      //{
        if (ingred_iter->second.name.isPresent())
        {
          if (matchesKeyword(ingred_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching ingredient record
            if (ingredMatches==0)
            {
              basic_out << "\n\nMatching ingredients:\n";
            }
            basic_out << "    \""<<ingred_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(ingred_iter->second.headerFormID)
                      <<"\n        editor ID \""<<ingred_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(ingred_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++ingredMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      //}//if hasFULL
      ++ingred_iter;
    }//while
    if (ingredMatches>0)
    {
      basic_out << "Total matching ingredients: "<<ingredMatches<<"\n";
    }
  }//scope for ingredient stuff

  //check keys for matches
  {
    unsigned int keyMatches = 0;
    SRTP::Keys::ListIterator key_iter = SRTP::Keys::getSingleton().getBegin();
    while (key_iter!=SRTP::Keys::getSingleton().getEnd())
    {
        if (key_iter->second.name.isPresent())
        {
          if (matchesKeyword(key_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching key record
            if (keyMatches==0)
            {
              basic_out << "\n\nMatching keys:\n";
            }
            basic_out << "    \""<<key_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(key_iter->second.headerFormID)
                      <<"\n        editor ID \""<<key_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(key_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++keyMatches;
            ++totalMatches;
          }//if match found
        }//if name is present
      ++key_iter;
    }//while
    if (keyMatches>0)
    {
      basic_out << "Total matching keys: "<<keyMatches<<"\n";
    }
  }//scope for key stuff

  //check misc. objects for matches
  {
    unsigned int miscMatches = 0;
    SRTP::MiscObjects::ListIterator misc_iter = SRTP::MiscObjects::getSingleton().getBegin();
    while (misc_iter!=SRTP::MiscObjects::getSingleton().getEnd())
    {
      if (misc_iter->second.fullName.isPresent())
      {
        if (!misc_iter->second.fullName.getString().empty())
        {
          if (matchesKeyword(misc_iter->second.fullName.getString(), searchKeyword, caseSensitive))
          {
            //found matching misc object record
            if (miscMatches==0)
            {
              basic_out << "\n\nMatching misc. objects:\n";
            }
            basic_out << "    \""<<misc_iter->second.fullName.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(misc_iter->second.headerFormID)
                      <<"\n        editor ID \""<<misc_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(misc_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++miscMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++misc_iter;
    }//while
    if (miscMatches>0)
    {
      basic_out << "Total matching misc. objects: "<<miscMatches<<"\n";
    }
  }//scope for misc. object stuff

  //check NPCs for matches
  {
    unsigned int NPCMatches = 0;
    SRTP::NPCs::ListIterator npc_iter = SRTP::NPCs::getSingleton().getBegin();
    while (npc_iter!=SRTP::NPCs::getSingleton().getEnd())
    {
      if (npc_iter->second.name.isPresent())
      {
        if (!npc_iter->second.name.getString().empty())
        {
          if (matchesKeyword(npc_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching NPC record
            if (NPCMatches==0)
            {
              basic_out << "\n\nMatching NPCs:\n";
            }
            basic_out << "    \""<<npc_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(npc_iter->second.headerFormID)
                      <<"\n        editor ID \""<<npc_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(npc_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++NPCMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++npc_iter;
    }//while
    if (NPCMatches>0)
    {
      basic_out << "Total matching NPCs: "<<NPCMatches<<"\n";
    }
  }//scope for NPC stuff

  //check perks for matches
  {
    unsigned int perkMatches = 0;
    SRTP::Perks::ListIterator perk_iter = SRTP::Perks::getSingleton().getBegin();
    while (perk_iter!=SRTP::Perks::getSingleton().getEnd())
    {
      if (perk_iter->second.name.isPresent())
      {
        if (!perk_iter->second.name.getString().empty())
        {
          if (matchesKeyword(perk_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching perk record
            if (perkMatches==0)
            {
              basic_out << "\n\nMatching perks:\n";
            }
            basic_out << "    \""<<perk_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(perk_iter->second.headerFormID)
                      <<"\n        editor ID \""<<perk_iter->second.editorID<<"\"\n";
            ++perkMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++perk_iter;
    }//while
    if (perkMatches>0)
    {
      basic_out << "Total matching perks: "<<perkMatches<<"\n";
    }
  }//scope for perk stuff

  //check quests for matches
  {
    unsigned int questMatches = 0;
    unsigned int i, j;
    bool prefix;
    SRTP::Quests::ListIterator quest_iter = SRTP::Quests::getSingleton().getBegin();
    while (quest_iter!=SRTP::Quests::getSingleton().getEnd())
    {
      if (quest_iter->second.name.isPresent())
      {
        if (!quest_iter->second.name.getString().empty())
        {
          if (matchesKeyword(quest_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching quest record
            if (questMatches==0)
            {
              basic_out << "\n\nMatching quests:\n";
            }
            basic_out << "    \""<<quest_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(quest_iter->second.headerFormID)
                      <<"\n        editor ID \""<<quest_iter->second.editorID<<"\"\n";
            //indices
            const unsigned int idx_count = quest_iter->second.indices.size();
            if (!allQuestInfo)
            {
              basic_out << "        indices: ";
              for (i=0; i<idx_count; ++i)
              {
                if (i!=0) basic_out<<", ";
                basic_out << quest_iter->second.indices[i].index;
                if (quest_iter->second.indices[i].hasFinishingQSDT()) basic_out<< " (finishes)";
              }
              if (idx_count==0) basic_out <<"none";
              basic_out << "\n";
            }
            else
            {
              //full quest info requested
              basic_out << "        indices:\n";
              for (i=0; i<idx_count; ++i)
              {
                basic_out << "          index "<<quest_iter->second.indices[i].index<<"\n";
                //run through QSDTs
                const unsigned int qsdt_count = quest_iter->second.indices[i].theQSDTs.size();
                for (j=0; j<qsdt_count; ++j)
                {
                  prefix = false;
                  if (quest_iter->second.indices[i].theQSDTs[j].isFinisher)
                  {
                    basic_out << "            (finishes quest)";
                    prefix = true;
                  }
                  if (quest_iter->second.indices[i].theQSDTs[j].logEntry.isPresent())
                  {
                    if (!prefix)
                    {
                      prefix = true;
                      basic_out << "            ";
                    }
                    basic_out << "\""<<quest_iter->second.indices[i].theQSDTs[j].logEntry.getString()<<"\"";
                  }
                  if (prefix) basic_out << "\n";
                }//for j
                //check for objective
                if (quest_iter->second.hasQOBJForIndex(quest_iter->second.indices[i].index))
                {
                  const SRTP::QuestRecord::QOBJEntry& ziel = quest_iter->second.getQOBJForIndex(quest_iter->second.indices[i].index);
                  if (ziel.displayText.isPresent())
                  {
                    basic_out <<"            [new objective] \""<<ziel.displayText.getString()<<"\"\n";
                  }
                }
              }//for i
              if (idx_count==0) basic_out <<"          none\n";
            }//else
            ++questMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++quest_iter;
    }//while
    if (questMatches>0)
    {
      basic_out << "Total matching quests: "<<questMatches<<"\n";
    }
  }//scope for quest stuff

  //check scrolls for matches
  {
    unsigned int scrollMatches = 0;
    SRTP::Scrolls::ListIterator scroll_iter = SRTP::Scrolls::getSingleton().getBegin();
    while (scroll_iter!=SRTP::Scrolls::getSingleton().getEnd())
    {
      //if (scroll_iter->second.hasFULL)
      //{
        if (scroll_iter->second.name.isPresent())
        {
          if (matchesKeyword(scroll_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching spell record
            if (scrollMatches==0)
            {
              basic_out << "\n\nMatching scrolls:\n";
            }
            basic_out << "    \""<<scroll_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(scroll_iter->second.headerFormID)
                      <<"\n        editor ID \""<<scroll_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(scroll_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++scrollMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      //}//if hasFULL
      ++scroll_iter;
    }//while
    if (scrollMatches>0)
    {
      basic_out << "Total matching scrolls: "<<scrollMatches<<"\n";
    }
  }//scope for scrolls

  //check soul gems for matches
  {
    unsigned int soulgemMatches = 0;
    SRTP::SoulGems::ListIterator soulgem_iter = SRTP::SoulGems::getSingleton().getBegin();
    while (soulgem_iter!=SRTP::SoulGems::getSingleton().getEnd())
    {
      if (soulgem_iter->second.name.isPresent())
      {
        if (!soulgem_iter->second.name.getString().empty())
        {
          if (matchesKeyword(soulgem_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching SoulGem record
            if (soulgemMatches==0)
            {
              basic_out << "\n\nMatching soul gems:\n";
            }
            basic_out << "    \""<<soulgem_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(soulgem_iter->second.headerFormID)
                      <<"\n        editor ID \""<<soulgem_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(soulgem_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++soulgemMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++soulgem_iter;
    }//while
    if (soulgemMatches>0)
    {
      basic_out << "Total matching soul gems: "<<soulgemMatches<<"\n";
    }
  }//scope for soul gem stuff

  //check spells for matches
  {
    unsigned int spellMatches = 0;
    SRTP::Spells::ListIterator spell_iter = SRTP::Spells::getSingleton().getBegin();
    while (spell_iter!=SRTP::Spells::getSingleton().getEnd())
    {
      if (spell_iter->second.name.isPresent())
      {
        if (!spell_iter->second.name.getString().empty())
        {
          if (matchesKeyword(spell_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching spell record
            if (spellMatches==0)
            {
              basic_out << "\n\nMatching spells:\n";
            }
            basic_out << "    \""<<spell_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(spell_iter->second.headerFormID)
                      <<"\n        editor ID \""<<spell_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(spell_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++spellMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++spell_iter;
    }//while
    if (spellMatches>0)
    {
      basic_out << "Total matching spells: "<<spellMatches<<"\n";
    }
  }//scope for spell stuff

  //check shouts for matches
  {
    unsigned int shoutMatches = 0;
    SRTP::Shouts::ListIterator shout_iter = SRTP::Shouts::getSingleton().getBegin();
    while (shout_iter!=SRTP::Shouts::getSingleton().getEnd())
    {
      if (shout_iter->second.fullName.isPresent())
      {
        if (!shout_iter->second.fullName.getString().empty())
        {
          if (matchesKeyword(shout_iter->second.fullName.getString(), searchKeyword, caseSensitive))
          {
            //found matching shout record
            if (shoutMatches==0)
            {
              basic_out << "\n\nMatching dragon shouts:\n";
            }
            basic_out << "    \""<<shout_iter->second.fullName.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(shout_iter->second.headerFormID)
                      <<"\n        editor ID \""<<shout_iter->second.editorID<<"\"\n";
            ++shoutMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++shout_iter;
    }//while
    if (shoutMatches>0)
    {
      basic_out << "Total matching dragon shouts: "<<shoutMatches<<"\n";
    }
  }//scope for shout stuff

  //check words of power for matches
  {
    unsigned int wordMatches = 0;
    SRTP::WordsOfPower::ListIterator word_iter = SRTP::WordsOfPower::getSingleton().getBegin();
    while (word_iter!=SRTP::WordsOfPower::getSingleton().getEnd())
    {
      if (word_iter->second.name.isPresent())
      {
        if (!word_iter->second.name.getString().empty())
        {
          if (matchesKeyword(word_iter->second.name.getString(), searchKeyword, caseSensitive)
            or matchesKeyword(word_iter->second.editorID, searchKeyword, caseSensitive))
          {
            //found matching word of power record
            if (wordMatches==0)
            {
              basic_out << "\n\nMatching words of power:\n";
            }
            basic_out << "    \""<<word_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(word_iter->second.headerFormID)
                      <<"\n        editor ID \""<<word_iter->second.editorID<<"\"\n";
            ++wordMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++word_iter;
    }//while
    if (wordMatches>0)
    {
      basic_out << "Total matching words of power: "<<wordMatches<<"\n";
    }
  }//scope for word of power stuff

  //check talking activators for matches
  {
    unsigned int talkingActivatorMatches = 0;
    SRTP::TalkingActivators::ListIterator talkingActivator_iter = SRTP::TalkingActivators::getSingleton().getBegin();
    while (talkingActivator_iter!=SRTP::TalkingActivators::getSingleton().getEnd())
    {
      if (talkingActivator_iter->second.name.isPresent())
      {
        if (!talkingActivator_iter->second.name.getString().empty())
        {
          if (matchesKeyword(talkingActivator_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching talking activator record
            if (talkingActivatorMatches==0)
            {
              basic_out << "\n\nMatching talking activators:\n";
            }
            basic_out << "    \""<<talkingActivator_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(talkingActivator_iter->second.headerFormID)
                      <<"\n        editor ID \""<<talkingActivator_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(talkingActivator_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++talkingActivatorMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++talkingActivator_iter;
    }//while
    if (talkingActivatorMatches>0)
    {
      basic_out << "Total matching talking activators: "<<talkingActivatorMatches<<"\n";
    }
  }//scope for talking activator stuff

  //check trees for matches
  {
    unsigned int treeMatches = 0;
    SRTP::Trees::ListIterator tree_iter = SRTP::Trees::getSingleton().getBegin();
    while (tree_iter!=SRTP::Trees::getSingleton().getEnd())
    {
      if (tree_iter->second.name.isPresent())
      {
        if (!tree_iter->second.name.getString().empty())
        {
          if (matchesKeyword(tree_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching alchemy record
            if (treeMatches==0)
            {
              basic_out << "\n\nMatching trees:\n";
            }
            basic_out << "    \""<<tree_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(tree_iter->second.headerFormID)
                      <<"\n        editor ID \""<<tree_iter->second.editorID<<"\"\n";
            ++treeMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++tree_iter;
    }//while
    if (treeMatches>0)
    {
      basic_out << "Total matching trees: "<<treeMatches<<"\n";
    }
  }//scope for tree stuff

  //check weapons for matches
  {
    unsigned int weaponMatches = 0;
    SRTP::Weapons::ListIterator weapon_iter = SRTP::Weapons::getSingleton().getBegin();
    while (weapon_iter!=SRTP::Weapons::getSingleton().getEnd())
    {
      if (weapon_iter->second.name.isPresent())
      {
        if (!weapon_iter->second.name.getString().empty())
        {
          if (matchesKeyword(weapon_iter->second.name.getString(), searchKeyword, caseSensitive))
          {
            //found matching weapon record
            if (weaponMatches==0)
            {
              basic_out << "\n\nMatching weapons:\n";
            }
            basic_out << "    \""<<weapon_iter->second.name.getString()
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(weapon_iter->second.headerFormID)
                      <<"\n        editor ID \""<<weapon_iter->second.editorID<<"\"\n";
            if (withReferences)
            {
              showRefIDs(weapon_iter->second.headerFormID, readerReferences.refMap, basic_out);
            }
            ++weaponMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++weapon_iter;
    }//while
    if (weaponMatches>0)
    {
      basic_out << "Total matching weapons: "<<weaponMatches<<"\n";
    }
  }//scope for weapon stuff

  basic_out << "\nTotal matching objects found: "<<totalMatches<<"\n";

  if (sendData)
  {
    #if defined(_WIN32)
    COPYDATASTRUCT cds;
    cds.dwData = 0;
    cds.cbData = string_out.str().length()+1;
    cds.lpData = (void*) string_out.str().c_str();
    const char * ptrSend1 = (sendParam1st=="NULL") ? NULL : sendParam1st.c_str();
    const char * ptrSend2 = (sendParam2nd=="NULL") ? NULL : sendParam2nd.c_str();
    HWND receiver = FindWindow(ptrSend1, ptrSend2);
    if (receiver==NULL)
    {
      std::cout << "Error in send data mode: cannot find receiving window!\n";
      return SRTP::rcWindowNotFound;
    }
    int msgResult = SendMessage(receiver, WM_COPYDATA, 0, (long int) &cds);
    #else
    //check for socket
    if (sendParam1st!="socket")
    {
      std::cout << "Error: parameter --send-data expects socket as first part!\n";
      return SRTP::rcInvalidParameter;
    }
    //create unix domain socket
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
      std::cout << "Error: could not open Unix domain socket!\n";
      return SRTP::rcSocketError;
    }
    //prepare socket address structure
    struct sockaddr_un serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_UNIX;
    snprintf(serv_addr.sun_path, sendParam2nd.length()+1, sendParam2nd.c_str());
    //now connect
    if (connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_un))!=0)
    {
      std::cout << "Error: could not connect via Unix domain socket!\n";
      return SRTP::rcSocketError;
    }
    //write to socket
    const std::string socketBuffer = string_out.str();
    write(socket_fd, socketBuffer.c_str(), socketBuffer.length()+1);
    //...and close socket
    close(socket_fd);
    #endif
  }//if sendData
  return 0;
}
