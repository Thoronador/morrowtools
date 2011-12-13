#include <iostream>
#include <string>
#include <set>
#include "../../base/FileFunctions.h"
#include "../../base/UtilityFunctions.h"
#include "../../mw/base/ReturnCodes.h"
#include "../base/Ammunitions.h"
#include "../base/Books.h"
#include "../base/FormIDFunctions.h"
#include "../base/MiscObjects.h"
#include "../base/RegistryFunctions.h"
#include "../base/Shouts.h"
#include "../base/Spells.h"
#include "../base/StringTable.h"
#include "../base/Weapons.h"
#include "../base/WordsOfPower.h"
#include "../base/records/TES4HeaderRecord.h"
#include "ESMReaderFinder.h"

void showGPLNotice()
{
  std::cout << "Form ID finder for Skyrim\n"
            //<< "  This programme is part of the Skyrim Tools Project.\n"
            << "  Copyright (C) 2011 Thoronador\n"
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
  std::cout << "Form ID Finder for Skyrim, version 0.05.rev326, 2011-12-13\n";
}

int showVersionExitcode()
{
  showVersion();
  return 326;
}

void showHelp()
{
  std::cout << "\nformID_finder -d DIRECTORY -p PATTERN\n"
            << "\n"
            << "(Note: This help is still incomplete.)\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Skyrim to\n"
            << "                     DIRECTORY\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -p TEXT          - set the search pattern to be TEXT.\n"
            << "  --keyword TEXT   - same as -p\n"
            << "  --case-sensitive - use case-sensitive search, i.e. upper and lower case\n"
            << "                     version of the same letter won't match. Deactivated by\n"
            << "                     default.\n";
}

//auxillary function
bool matchesKeyword(const std::string& haystack, const std::string& keyword, const bool caseMatters)
{
  if (caseMatters)
  {
    return (haystack.find(keyword)!=std::string::npos);
  }
  return (lowerCase(haystack).find(keyword)!=std::string::npos);
}

int main(int argc, char **argv)
{
  showGPLNotice();

  //data files directory - empty at start
  std::string dataDir = "";
  //word to be searched for - empty at start, user has to(!) set it
  std::string searchKeyword = "";
  bool caseSensitive = false;

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
        else if (param=="--version")
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
            return MWTP::rcInvalidParameter;
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
              return MWTP::rcInvalidParameter;
            }//else
          }
          else
          {
            std::cout << "Error: You have to specify a directory name after \""
                      << param<<"\".\n";
            return MWTP::rcInvalidParameter;
          }
        }//data files directory
        else if ((param=="-p") or (param=="--keyword"))
        {
          //set more than once?
          if (!searchKeyword.empty())
          {
            std::cout << "Error: search keyword was already set!\n";
            return MWTP::rcInvalidParameter;
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
            return MWTP::rcInvalidParameter;
          }
        }//keyword
        else if (param=="--case-sensitive")
        {
          //set more than once?
          if (caseSensitive)
          {
            std::cout << "Error: parameter \""<<param<<"\" was specified twice!\n";
            return MWTP::rcInvalidParameter;
          }
          caseSensitive = true;
          std::cout << "Case-sensitive search modus enabled.\n";
        }//case sensitive
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
          return MWTP::rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }

  //Has the user specified a data directory?
  if (dataDir.empty())
  {
    //No, so let's search the registry first...
    std::cout << "Warning: Data files directory of Skyrim was not specified, "
              << "will will try to read it from the registry.\n";
    if (!SRTP::getSkryrimPathFromRegistry(dataDir))
    {
      std::cout << "Error: Could not find Skyrim's installation path in registry!\n";
      dataDir.clear();
    }
    else
    {
      if (!dataDir.empty())
      {
        //Does it have a trailing (back)slash?
        if (dataDir.at(dataDir.length()-1)!='\\')
        {
          dataDir = dataDir + "\\";
        }//if not backslash
        /*add data dir to path, because installed path points only to Skyrim's
          main direkctory */
        dataDir = dataDir +"Data\\";
        std::cout << "Data files directory was set to \""<<dataDir<<"\" via registry.\n";
      }
      else
      {
        std::cout << "Error: Installation path in registry is empty!\n";
      }
    }

    //check again, in case registry failed
    if (dataDir.empty())
    {
      //empty, so let's try a default value.
      dataDir = "C:\\Program Files\\Steam\\SteamApps\\common\\skyrim\\Data\\";
      std::cout << "Warning: Data files directory of Skyrim was not specified, "
                << "will use default path \""<<dataDir<<"\". This might not work"
                << " properly on your machine, use the parameter -d to specify "
                << "the proper path.\n";
    }
  }//if no data dir is given

  //keyword given?
  if (searchKeyword.empty())
  {
    std::cout << "Error: No search keyword was specified. Use the parameter --keyword"
              << " to specify the stuff you want that programme to search for.\n";
    return MWTP::rcInvalidParameter;
  }//if no keyword given

  std::cout << "\n\nSearching for \""<<searchKeyword<<"\" using case-";
  if (!caseSensitive) std::cout <<"in";
  std::cout << "sensitive search. This may take a while...\n";

  //adjust keyword to selected case-sensitivity
  if (!caseSensitive) searchKeyword = lowerCase(searchKeyword);

  //try to find the language component of the string table's file name
  std::string languageComponent = "";
  std::string part_path, part_name, part_ext;

  std::vector<FileEntry> files = getDirectoryFileList(dataDir+"Strings\\");
  if (files.size()<3)
  {
    std::cout << "Error: could not find string table files for Skyrim.esm!\n";
    return MWTP::rcFileError;
  }

  std::set<std::string> presentStuff;

  unsigned int i;
  for (i=0; i<files.size(); ++i)
  {
    if ((!files[i].isDirectory) and (lowerCase(files[i].fileName.substr(0, 7))=="skyrim_"))
    {
      splitPathFileExtension(files[i].fileName, '\\', part_path, part_name, part_ext);
      if ((lowerCaseCompare(part_ext, "dlstrings")==0) or (lowerCaseCompare(part_ext, "strings")==0)
        or (lowerCaseCompare(part_ext, "ilstrings")==0))
      {
        //Do we have a language component yet?
        if (languageComponent.empty())
        {
          languageComponent = part_name.substr(7);
          presentStuff.insert(lowerCase(part_ext));
        }
        else
        {
          if (part_name.substr(7)==languageComponent)
          {
            presentStuff.insert(lowerCase(part_ext));
          }
        }//else
      }//if string file extension
    }//if name starts with skyrim
  }//for

  if (presentStuff.size()<3)
  {
    std::cout << "Error: Could not find string table files!\n";
    return MWTP::rcFileError;
  }

  if ((!FileExists(dataDir+"Strings\\Skyrim_"+languageComponent+".dlstrings"))
     or (!FileExists(dataDir+"Strings\\Skyrim_"+languageComponent+".ilstrings"))
     or (!FileExists(dataDir+"Strings\\Skyrim_"+languageComponent+".strings")))
  {
    std::cout << "Error: At least one string table file ist missing!\n";
    return MWTP::rcFileError;
  }

  //read string tables
  SRTP::StringTable table;
  if (!table.readTable(dataDir+"Strings\\Skyrim_"+languageComponent+".dlstrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Error while reading string tables!\n";
    return MWTP::rcDataError;
  }
  if (!table.readTable(dataDir+"Strings\\Skyrim_"+languageComponent+".ilstrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Error while reading string tables!\n";
    return MWTP::rcDataError;
  }
  if (!table.readTable(dataDir+"Strings\\Skyrim_"+languageComponent+".strings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Error while reading string tables!\n";
    return MWTP::rcDataError;
  }

  SRTP::ESMReaderFinder reader;
  SRTP::Tes4HeaderRecord tes4rec;

  if (!reader.readESM(dataDir+"Skyrim.esm", tes4rec))
  {
    std::cout << "Error while reading "<<dataDir+"Skyrim.esm!\n";
    return MWTP::rcFileError;
  }

  unsigned int totalMatches = 0;

  //check ammunitions for matches
  unsigned int ammoMatches = 0;
  SRTP::Ammunitions::ListIterator ammo_iter = SRTP::Ammunitions::getSingleton().getBegin();
  while (ammo_iter!=SRTP::Ammunitions::getSingleton().getEnd())
  {
    if (ammo_iter->second.hasFULL)
    {
      if (table.hasString(ammo_iter->second.nameStringID))
      {
        if (matchesKeyword(table.getString(ammo_iter->second.nameStringID), searchKeyword, caseSensitive))
        {
          //found matching ammo record
          if (ammoMatches==0)
          {
            std::cout << "\n\nMatching ammunition:\n";
          }
          std::cout << "    \""<<table.getString(ammo_iter->second.nameStringID)<<"\", form ID "<<SRTP::getFormIDAsString(ammo_iter->second.headerFormID)<<"\n";
          ++ammoMatches;
          ++totalMatches;
        }//if match found
      }//if table has string
    }//if hasFULL
    ++ammo_iter;
  }//while
  if (ammoMatches>0)
  {
    std::cout << "Total matching ammunition: "<<ammoMatches<<"\n";
  }

  //check books for matches
  {
    unsigned int bookMatches = 0;
    SRTP::Books::ListIterator book_iter = SRTP::Books::getSingleton().getBegin();
    while (book_iter!=SRTP::Books::getSingleton().getEnd())
    {
      if (book_iter->second.hasFULL or (book_iter->second.titleStringID!=0))
      {
        if (table.hasString(book_iter->second.titleStringID))
        {
          if (matchesKeyword(table.getString(book_iter->second.titleStringID), searchKeyword, caseSensitive))
          {
            //found matching book record
            if (bookMatches==0)
            {
              std::cout << "\n\nMatching books:\n";
            }
            std::cout << "    \""<<table.getString(book_iter->second.titleStringID)<<"\", form ID "<<SRTP::getFormIDAsString(book_iter->second.headerFormID)<<"\n";
            ++bookMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++book_iter;
    }//while
    if (bookMatches>0)
    {
      std::cout << "Total matching books: "<<bookMatches<<"\n";
    }
  }//scope for book stuff

  //check misc. objects for matches
  {
    unsigned int miscMatches = 0;
    SRTP::MiscObjects::ListIterator misc_iter = SRTP::MiscObjects::getSingleton().getBegin();
    while (misc_iter!=SRTP::MiscObjects::getSingleton().getEnd())
    {
      if (misc_iter->second.hasFULL)
      {
        if (table.hasString(misc_iter->second.fullNameStringID))
        {
          if (matchesKeyword(table.getString(misc_iter->second.fullNameStringID), searchKeyword, caseSensitive))
          {
            //found matching misc object record
            if (miscMatches==0)
            {
              std::cout << "\n\nMatching misc. objects:\n";
            }
            std::cout << "    \""<<table.getString(misc_iter->second.fullNameStringID)<<"\", form ID "<<SRTP::getFormIDAsString(misc_iter->second.headerFormID)<<"\n";
            ++miscMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++misc_iter;
    }//while
    if (miscMatches>0)
    {
      std::cout << "Total matching misc. objects: "<<miscMatches<<"\n";
    }
  }//scope for misc. object stuff

  //check spells for matches
  {
    unsigned int spellMatches = 0;
    SRTP::Spells::ListIterator spell_iter = SRTP::Spells::getSingleton().getBegin();
    while (spell_iter!=SRTP::Spells::getSingleton().getEnd())
    {
      if (spell_iter->second.hasFULL)
      {
        if (table.hasString(spell_iter->second.nameStringID))
        {
          if (matchesKeyword(table.getString(spell_iter->second.nameStringID), searchKeyword, caseSensitive))
          {
            //found matching spell record
            if (spellMatches==0)
            {
              std::cout << "\n\nMatching spells:\n";
            }
            std::cout << "    \""<<table.getString(spell_iter->second.nameStringID)
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(spell_iter->second.headerFormID)
                      <<"\n        editor ID \""<<spell_iter->second.editorID<<"\"\n";
            ++spellMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++spell_iter;
    }//while
    if (spellMatches>0)
    {
      std::cout << "Total matching spells: "<<spellMatches<<"\n";
    }
  }//scope for spell stuff

  //check shouts for matches
  {
    unsigned int shoutMatches = 0;
    SRTP::Shouts::ListIterator shout_iter = SRTP::Shouts::getSingleton().getBegin();
    while (shout_iter!=SRTP::Shouts::getSingleton().getEnd())
    {
      if (shout_iter->second.hasFULL or (shout_iter->second.fullNameStringID!=0))
      {
        if (table.hasString(shout_iter->second.fullNameStringID))
        {
          if (matchesKeyword(table.getString(shout_iter->second.fullNameStringID), searchKeyword, caseSensitive))
          {
            //found matching shout record
            if (shoutMatches==0)
            {
              std::cout << "\n\nMatching dragon shouts:\n";
            }
            std::cout << "    \""<<table.getString(shout_iter->second.fullNameStringID)
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
      std::cout << "Total matching dragon shouts: "<<shoutMatches<<"\n";
    }
  }//scope for shout stuff

  //check words of power for matches
  {
    unsigned int wordMatches = 0;
    SRTP::WordsOfPower::ListIterator word_iter = SRTP::WordsOfPower::getSingleton().getBegin();
    while (word_iter!=SRTP::WordsOfPower::getSingleton().getEnd())
    {
      if (word_iter->second.hasFULL)
      {
        if (table.hasString(word_iter->second.nameStringID))
        {
          if (matchesKeyword(table.getString(word_iter->second.nameStringID), searchKeyword, caseSensitive)
            or matchesKeyword(word_iter->second.editorID, searchKeyword, caseSensitive))
          {
            //found matching word of power record
            if (wordMatches==0)
            {
              std::cout << "\n\nMatching words of power:\n";
            }
            std::cout << "    \""<<table.getString(word_iter->second.nameStringID)
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
      std::cout << "Total matching words of power: "<<wordMatches<<"\n";
    }
  }//scope for word of power stuff

  //check weapons for matches
  {
    unsigned int weaponMatches = 0;
    SRTP::Weapons::ListIterator weapon_iter = SRTP::Weapons::getSingleton().getBegin();
    while (weapon_iter!=SRTP::Weapons::getSingleton().getEnd())
    {
      if (weapon_iter->second.hasFULL or (weapon_iter->second.nameStringID!=0))
      {
        if (table.hasString(weapon_iter->second.nameStringID))
        {
          if (matchesKeyword(table.getString(weapon_iter->second.nameStringID), searchKeyword, caseSensitive))
          {
            //found matching weapon record
            if (weaponMatches==0)
            {
              std::cout << "\n\nMatching weapons:\n";
            }
            std::cout << "    \""<<table.getString(weapon_iter->second.nameStringID)
                      <<"\"\n        form ID "<<SRTP::getFormIDAsString(weapon_iter->second.headerFormID)
                      <<"\n        editor ID \""<<weapon_iter->second.editorID<<"\"\n";
            ++weaponMatches;
            ++totalMatches;
          }//if match found
        }//if table has string
      }//if hasFULL
      ++weapon_iter;
    }//while
    if (weaponMatches>0)
    {
      std::cout << "Total matching weapons: "<<weaponMatches<<"\n";
    }
  }//scope for weapon stuff

  //to do...

  std::cout << "\nTotal matching objects found: "<<totalMatches<<"\n";
  return 0;
}
