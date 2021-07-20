/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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
#include <string>
#include "ESMReaderCleaner.h"
#include "../base/ReturnCodes.h"
#include "../base/IniFunctions.h"
#include "../../base/DirectoryFunctions.h"
#include "../../base/FileFunctions.h"
#include "DirectoryTraversal.h"

void showHelp()
{
  std::cout << "\ndata_cleaner -d DIRECTORY [-f PLUGINFILE]\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -d DIRECTORY     - path to the Data Files directory of Morrowind\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  --all            - tries to load all master and plugin files from the given\n"
            << "                     Data Files directory (default).\n"
            << "  --all-data-files - same as --all\n"
            << "  --explicit       - sets explicit mode where every data file that has to be\n"
            << "                     scanned has to be given explicitly. This option is\n"
            << "                     mutually exclusive with --all.\n"
            << "  -e               - same as --explicit\n"
            << "  -f PLUGINFILE    - adds the plugin PLUGINFILE to the list of files that will\n"
            << "                     be searched for data files (only in explicit mode)\n"
            << "  --ini            - tries to load list of plugin files from Morrowind.ini\n"
            << "                     (only allowed in explicit mode)\n"
            << "  -i               - same as --ini\n"
            << "  --verbose        - shows some additional information about data files\n"
            << "  --silent         - opposite of --verbose; does not show additional information\n";
}

void showGPLNotice()
{
  std::cout << "Data Files Cleaner for Morrowind\n"
            << "  This programme is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011 Thoronador\n"
            << "\n"
            << "  The Morrowind Tools are free software: you can redistribute them and/or\n"
            << "  modify them under the terms of the GNU General Public License as published\n"
            << "  by the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  The Morrowind Tools are distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Data Files Cleaner for Morrowind, version 0.1_rev255, 2011-05-30\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::string baseDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
  bool verbose = false;
  bool doIni = false;
  bool doAllPlugins = true;
  bool specifiedMode = false;

  //list of .esp files to consider for scan
  MWTP::DepFileList files;
  files.clear();

  if (argc>1 and argv!=NULL)
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if (param=="--help" or param=="-?")
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
        //verbose mode
        else if (param=="--verbose")
        {
          verbose = true;
          std::cout << "Additional information allowed due to \"--verbose\".\n";
        }
        //silent mode
        else if (param=="--silent")
        {
          verbose = false;
          std::cout << "Don't show additional information due to \"--silent\".\n";
        }
        else if ((param=="-d") or (param=="-dir") or (param=="--data-files"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            //Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size()>1)
            {
              baseDir = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as directory name already
              //Does it have a trailing (back)slash? If not, add one.
              baseDir = slashify(baseDir);
              std::cout << "Data files directory was set to \""<<baseDir<<"\".\n";
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
        //add plugin file to list
        else if ((param=="-f") or (param=="--add-file"))
        {
          if (!doAllPlugins)
          {
            if ((i+1<argc) and (argv[i+1]!=NULL))
            {
              const std::string pluginFileName = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as file name already
              files.push_back(pluginFileName);
              std::cout << "Plugin file \""<<pluginFileName<<"\" was added.\n";
            }
            else
            {
              std::cout << "Error: You have to specify a file name after \""
                        << param<<"\".\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cout << "Error: parameter \""<<param<<"\" can only be specified"
                      << " in explicit mode.\n";
            return MWTP::rcInvalidParameter;
          }
        }//plugin file
        //try to read from Morrowind.ini?
        else if ((param=="-i") or (param=="--ini"))
        {
          if (!doAllPlugins)
          {
            if (!doIni)
            {
              doIni = true;
            }
            else
            {
              std::cout << "Error: parameter \""<<param<<"\" has been specified"
                        << " more than once.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cout << "Error: parameter \""<<param<<"\" can only be specified"
                      << " in explicit mode.\n";
            return MWTP::rcInvalidParameter;
          }
        }
        //try to get all plugin and master files from Data Files directory
        else if ((param=="--all-data-files") or (param=="--all"))
        {
          if (!specifiedMode)
          {
            doAllPlugins = true;
            specifiedMode = true;
          }
          else
          {
            std::cout << "Error: parameter \""<<param<<"\" is not allowed here,"
                      << " because mode of operation has already been specified.\n";
            return MWTP::rcInvalidParameter;
          }
        }
        //sets explicit mode
        else if ((param=="--explicit") or (param=="-e"))
        {
          if (!specifiedMode)
          {
            doAllPlugins = false;
            specifiedMode = true;
          }
          else
          {
            std::cout << "Error: parameter \""<<param<<"\" is not allowed here,"
                      << " because mode of operation has already been specified.\n";
            return MWTP::rcInvalidParameter;
          }
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
  }//if argc

  if (doIni)
  {
    const int iniReturnCode = MWTP::getFilesFromMorrowindIni(baseDir, files);
    if (iniReturnCode!=0)
    {
      return iniReturnCode;
    }
  }//if
  if (doAllPlugins)
  {
    MWTP::getAllDataFiles(baseDir, files);
  }//if

  //Let's add master files, if necessary.
  if (!files.hasDepFile("Bloodmoon.esm") and FileExists(baseDir+"Bloodmoon.esm"))
  {
    files.push_front("Bloodmoon.esm");
  }//if
  if (!files.hasDepFile("Tribunal.esm") and FileExists(baseDir+"Tribunal.esm"))
  {
    files.push_front("Tribunal.esm");
  }//if
  if (!files.hasDepFile("Morrowind.esm"))
  {
    files.push_front("Morrowind.esm");
  }//if

  //try to get file information
  unsigned int i;
  for (i=0; i<files.getSize(); ++i)
  {
    getFileSizeAndModificationTime(baseDir+files.at(i).name, files.at(i).size,
                                   files.at(i).modified);
  }//for

  //sort files according to Morrowind's load order
  files.sort();
  //remove duplicate entries in list
  const unsigned int duplicates = files.removeDuplicates();
  if (duplicates!=0)
  {
    if (duplicates==1)
    {
      std::cout << "One duplicate file has been removed from the list.\n";
    }
    else
    {
      std::cout << duplicates <<" duplicates files have been removed from the list.\n";
    }
  }

  std::cout << "List of active files:\n";
  files.writeDeps();

  //read all files
  MWTP::ESMReaderCleaner reader(baseDir);
  std::cout << "Reading files, this may take a while.\n";
  i = 0;
  while (i<files.getSize())
  {
    MWTP::TES3Record DummyHead;//It's not actually used after the read function,
                                // but readESM() needs one as parameter.
    const int read_result = reader.readESM(baseDir+files.at(i).name, DummyHead, verbose);
    if (read_result<0)
    {
      std::cout << "Error while reading file \""<<baseDir+files.at(i).name
                <<"\".\nAborting.\n";
      return MWTP::rcFileError;
    }//if
    //something was read and file was not removed from list, so increase counter
    ++i;
  }//while

  std::cout << "Done reading. Checking for unused files...\n";

  std::set<std::string> DeletableMeshes;
  std::set<std::string> DeletableIcons;

  //get mesh files
  MWTP::getDeletableMeshes(baseDir+"Meshes\\", reader.MeshSet, DeletableMeshes);
  MWTP::getDeletableIcons(baseDir+"Icons\\", reader.IconSet, DeletableIcons);

  if (!DeletableIcons.empty() or !DeletableMeshes.empty())
  {
    std::cout << "There are "<<DeletableMeshes.size()<<" meshes and "
              <<DeletableIcons.size()<<" icons that could be deleted.\n";
    i=0;
    std::set<std::string>::const_iterator iter;

    //Try to get the cumulative size of all files in question.
    int64_t sizeSum = 0;
    // ---- the meshes first
    iter = DeletableMeshes.begin();
    while (iter!=DeletableMeshes.end())
    {
      const int64_t fs = getFileSize64(*iter);
      if (fs==-1)
      {
        iter = DeletableMeshes.end();
        sizeSum = -1;
      }
      else
      {
        sizeSum += fs;
        ++iter;
      }
    }//while
    // ---- and the icons follow
    if (sizeSum!=-1)
    {
      iter = DeletableIcons.begin();
      while (iter!=DeletableIcons.end())
      {
        const int64_t fs = getFileSize64(*iter);
        if (fs==-1)
        {
          iter = DeletableIcons.end();
          sizeSum = -1;
        }
        else
        {
          sizeSum += fs;
          ++iter;
        }
      }//while
      if (sizeSum!=-1)
      {
        //print the size
        std::cout << "This would free "<<getSizeString(sizeSum);
        if (sizeSum>1024)
        {
          std::cout << " ("<<sizeSum<<" bytes)";
        }
        std::cout <<".\n";
      }//if
    }//if no error occurred

    std::string input;
    do
    {
      std::cout << "Do you want to delete those files (y/n)?\n";
      std::cin >> input;
      trim(input);
      input = lowerCase(input);
    } while ((input!="y") and (input!="yes") and (input!="j") and (input!="ja")
         and (input!="n") and (input!="no") and (input!="nein"));
    //Does the user want to delete the stuff?
    if ((input=="y") or (input=="yes") or (input=="j") or (input=="ja"))
    {
      std::cout << "Deleting files...\n";
      //delete meshes
      iter = DeletableMeshes.begin();
      while (iter!=DeletableMeshes.end())
      {
        if (!deleteFile(*iter))
        {
          std::cout << "Error: Could not delete file \""<<*iter<<"\".\n";
        }
        ++iter;
      }//while
      //delete icons
      iter = DeletableIcons.begin();
      while (iter!=DeletableIcons.end())
      {
        if (!deleteFile(*iter))
        {
          std::cout << "Error: Could not delete file \""<<*iter<<"\".\n";
        }
        ++iter;
      }//while
      std::cout << "Done.\n";
    }
    else
    {
      std::cout << "You chose not to delete the unused files.\n";
    }
  }//if there are any files that could be deleted
  else
  {
    std::cout << "There are no unused icons or meshes that could be deleted.\n";
  }
  return 0;
}
