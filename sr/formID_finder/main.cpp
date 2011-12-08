#include <iostream>
#include <string>
#include <set>
#include "../../base/FileFunctions.h"
#include "../../base/UtilityFunctions.h"
#include "../../mw/base/ReturnCodes.h"
#include "../base/records/Tes4HeaderRecord.h"
#include "../base/StringTable.h"
#include "ESMReaderFinder.h"

void showGPLNotice()
{
  std::cout << "Form ID finder for Skyrim\n"
            << "  This programme is part of the Skyrim Tools Project.\n"
            << "  Copyright (C) 2011 Thoronador\n"
            << "\n"
            << "  The Skyrim Tools are free software: you can redistribute them and/or\n"
            << "  modify them under the terms of the GNU General Public License as published\n"
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
  std::cout << "Form ID Finder for Skyrim, version 0.01.rev314, 2011-12-08\n";
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
            << "  -d DIRECTORY     - path to the Data Files directory of Skyrim, including\n"
            << "                     trailing backslash\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -p TEXT          - set the search pattern to be TEXT.\n"
            << "  --keyword TEXT   - same as -p\n"
            << "  --case-sensitive - use case-sensitive search, i.e. upper and lower case\n"
            << "                     version of the same letter won't match. Deactivated by\n"
            << "                     default.\n";
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
        if ((param=="--help") or (param=="-?"))
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
    //No, so let's try a default value.
    dataDir = "C:\\Program Files\\Steam\\SteamApps\\common\\skyrim\\Data\\";
    std::cout << "Warning: Data files directory of Skyrim was not specified, "
              << "will use default path \""<<dataDir<<"\". This might not work"
              << " properly on your machine, use the parameter -d to specify "
              << "the proper path.\n";
  }
  //keyword given?
  if (searchKeyword.empty())
  {
    std::cout << "Error: No search keyword was specified. Use the parameter -p"
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


  //to do...

  return 0;
}
