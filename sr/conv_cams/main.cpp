/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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
#include "../base/ESMWriterContents.h"
#include "../base/RegistryFunctions.h"
#include "../base/ReturnCodes.h"
#include "../base/SR_Constants.h"
#include "../base/records/CameraShotRecord.h"
#include "../base/records/TES4HeaderRecord.h"
#include "../../base/FileFunctions.h"
#include "ESMReaderConvCAMS.h"

void showGPLNotice()
{
  std::cout << "CAMS record converter for Skyrim\n"
            << "  Copyright (C) 2012 Thoronador\n"
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
  std::cout << "CAMS record converter for Skyrim, version 0.01.rev457, 2012-03-27\n";
}

int showVersionExitcode()
{
  showVersion();
  return 457;
}

void showHelp()
{
  std::cout << "\nconv_cams -f PLUGINFILE [-d DIRECTORY]\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -v               - same as --version\n"
            << "  -d DIRECTORY     - set path to the Data Files directory of Skyrim to\n"
            << "                     DIRECTORY. If no directory is given, the programme will\n"
            << "                     try to read Skyrim's path from the registry. If that\n"
            << "                     fails, a predefined path will be used.\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -f PLUGINFILE    - sets the plugin PLUGINFILE as the plugin that will be\n"
            << "                     converted to the pre-1.5 patch state. This parameter is\n"
            << "                     mandatory!\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();

  //data files directory - empty at start
  std::string dataDir = "";
  //the plugin that has to be converted - emty at start, too
  std::string pluginFile = "";

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


        else if ((param=="-f") or (param=="-file") or (param=="--plugin-file"))
        {
          //set more than once?
          if (!pluginFile.empty())
          {
            std::cout << "Error: Plugin file was already set!\n";
            return SRTP::rcInvalidParameter;
          }
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            //Is it long enough to be a file? (Minimum should be "a.esp".)
            if (std::string(argv[i+1]).size()>4)
            {
              pluginFile = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as plugin name already
              std::cout << "Plugin file was set to \""<<pluginFile<<"\".\n";
            }
            else
            {
              std::cout << "Parameter \""<<std::string(argv[i+1])<<"\" is too"
                        << " short to be a proper plugin file name.\n";
              return SRTP::rcInvalidParameter;
            }//else
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param<<"\".\n";
            return SRTP::rcInvalidParameter;
          }
        }//plugin file name
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

  //was a plugin file set?
  if (pluginFile.empty())
  {
    std::cout << "You have to specify a plugin file as parameter for this "
              << "programme to run properly.\nUse --help to get a list of "
              << "valid parameters.\n";
    return SRTP::rcInvalidParameter;
  }

  //Has the user specified a data directory?
  if (dataDir.empty())
  {
    //No, so let's search the registry first...
    std::cout << "Warning: Data files directory of Skyrim was not specified, "
              << "will try to read it from the registry.\n";
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

  //Does the plugin file even exist?
  if (!FileExists(dataDir+pluginFile))
  {
    std::cout << "Error: The given plugin file \""<<dataDir+pluginFile
              <<"\" does not exist! Aborting programme.\n";
    return SRTP::rcFileError;
  }


  //now we can start the reading process
  SRTP::Tes4HeaderRecord tes4rec;
  SRTP::ESMReaderConvCAMS reader;
  if (reader.readESM(dataDir+pluginFile, tes4rec)<0)
  {
    std::cout << "Error while reading \""<<dataDir+pluginFile<<"\"!\n";
    return SRTP::rcFileError;
  }

  //now change the records
  unsigned int changedRecords = 0;

  std::vector<SRTP::Group>::iterator groupIter = reader.contents.m_Groups.begin();
  while (groupIter!=reader.contents.m_Groups.end())
  {
    if ((groupIter->headerData.getGroupLabel()==SRTP::cCAMS)
        and (groupIter->headerData.getGroupType()==SRTP::GroupData::cTopLevelGroup))
    {
      //found CAMS top level group
      SRTP::Group::RecIterator recIter = groupIter->getRecBegin();
      while (recIter!=groupIter->getRecEnd())
      {
        if ((*recIter)->getRecordType()==SRTP::cCAMS)
        {
          //CAMS record type found
          SRTP::CameraShotRecord* camPtr = static_cast<SRTP::CameraShotRecord*>(recIter->get());
          if (camPtr!=NULL)
          {
            if ((camPtr->dataLen==SRTP::CameraShotRecord::dlt44Byte) and (!(camPtr->isDeleted())))
            {
              //record has 44 byte variant of DATA subrecord, change it
              camPtr->dataLen = SRTP::CameraShotRecord::dlt40Byte;
              ++changedRecords;
            }//if
          }//if not NULL
        }//if
        ++recIter;
      }//while (inner)
      //update group size for later use/writing
      if (changedRecords>0)
      {
        groupIter->updateGroupSize();
      }
    }//if CAMS found
    ++groupIter;
  }//while

  //did we actually change anything?
  if (changedRecords==0)
  {
    std::cout << "Hint: The file \""<<dataDir+pluginFile<<"\" did not contain "
              << "any CAMS records that need to be changed. Exiting programme "
              << "without creating a new plugin file.\n";
    return SRTP::rcNoChange;
  }

  //find a new name for the modified file to avoid overwriting an existing file
  std::string dummyPath;
  std::string piName, piExt;
  splitPathFileExtension(pluginFile, '\\', dummyPath, piName, piExt);
  std::string newName = "_conv_cams";
  unsigned int i = 0;
  while (FileExists(dataDir+piName+newName+"."+piExt))
  {
    ++i;
    std::ostringstream s_str;
    s_str << i;
    newName = "_conv_cams"+s_str.str();
  }//while
  //get the new name right
  newName = dataDir+piName+newName+"."+piExt;
  std::cout << "Converted plugin file will be saved as \"" <<newName<<"\".\n";

  //save it
  std::cout << "Saving file...\n";
  SRTP::ESMWriterContents writer;
  // ---- copy contents from reader to writer (expensive in terms of memory,
  //      I should implement a faster, more lightweight way to do that)
  writer.contents = reader.contents;
  //now write the actual .esm/.esp file
  if (!writer.writeESM(newName, tes4rec))
  {
    std::cout << "Error: Could not write converted data to \""<<newName<<"\"!\n";
    return SRTP::rcFileError;
  }
  std::cout << "Success!\n";
  return 0;
}