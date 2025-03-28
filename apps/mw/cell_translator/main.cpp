/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022, 2023, 2025  Dirk Stolle

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
#include <map>
#include <typeinfo>
#include "ESMReaderTranslator.hpp"
#include "TranslatorXML.hpp"
#include "TranslateRecords.hpp"
#include "../../../lib/mw/ESMWriterGeneric.hpp"
#include "../../../lib/mw/ESMReaderScriptCompiler.hpp"
#include "../../../lib/mw/records/GenericRecord.hpp"
#include "../../../lib/mw/records/CreatureRecord.hpp"
#include "../../../lib/mw/records/NPCRecord.hpp"
#include "../../../lib/mw/records/PathGridRecord.hpp"
#include "../../../lib/mw/RegistryFunctions.hpp"
#include "../../../lib/mw/ReturnCodes.hpp"
#include "../../../lib/mw/script_compiler/ScriptCompiler.hpp"
#include "../../../lib/base/DirectoryFunctions.hpp"
#include "../../../lib/base/FileFunctions.hpp"

void showHelp()
{
  std::cout << "\ncell_translator -f PLUGINFILE [-o FILENAME1] [-xml FILENAME2]\n"
            << "\n"
            << "options:\n"
            << "  --help             - Displays this help message and quits.\n"
            << "  -?                 - same as --help\n"
            << "  --version          - Displays the version of the program and quits.\n"
            << "  -d DIRECTORY       - Set path to the Data Files directory of Morrowind to\n"
            << "                       DIRECTORY. If omitted, the path will be read from the\n"
            << "                       registry or a default value will be used.\n"
            << "  -dir DIRECTORY     - same as -d\n"
            << "  -f PLUGINFILE      - Sets the plugin PLUGINFILE as the files that will be\n"
            << "                       translated by this program. This option is required.\n"
            << "  --output FILENAME1 - Sets the name of the created output file to FILENAME1.\n"
            << "                       If omitted, the default output file is \"out.esp\".\n"
            << "  -o FILENAME1       - short for --output\n"
            << "  --force            - Usually the program would not overwrite existing\n"
            << "                       plugin files, so that giving an existing file as output\n"
            << "                       file would abort the program. If you want to avoid it\n"
            << "                       and overwrite the file anyway, use --force.\n"
            << "  -xml FILENAME2     - Sets the name of the XML file that contains the cell\n"
            << "                       names to FILENAME2. If omitted, the default XML file\n"
            << "                       name is \"cells.xml\".\n"
            << "  -de                - Sets translation mode to translate from English to\n"
            << "                       German. Mutually exclusive with -en.\n"
            << "  -en                - Sets translation mode to translate from German to\n"
            << "                       English. Mutually exclusive with -de.\n"
            << "  --no-scripts       - Script records will not be processed, if this parameter\n"
            << "                       is specified.\n"
            << "  --dare             - Try harder to re-compile changed scripts, even if they\n"
            << "                       might not compile properly. Be warned that this might\n"
            << "                       result in more Morrowind crashes, as long as you use\n"
            << "                       the generated plugin file with Morrowind.\n";
}

void showGPLNotice()
{
  std::cout << "Cell Translator for Morrowind\n"
            << "  This program is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011, 2012, 2013, 2025  Thoronador\n"
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
  std::cout << "Cell Translator for Morrowind, version 0.4.3_rev527, 2025-03-28\n";
}

int showVersionExitcode()
{
  showVersion();
  return 527;
}

int main(int argc, char **argv)
{
  showGPLNotice();

  std::string pathToCellsXML = "";
  std::string dataDir = ""; //Data Files directory - empty at start
  std::string pluginFile = "";
  std::string outputFileName = "";
  bool process_scripts = true;
  bool tryHarder = false;
  bool forceOutput = false;
  bool translationDirectionSpecified = false;
  MWTP::TransDir translationDirection = MWTP::td_en_de;

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
        else if (param == "--version")
        {
          showVersion();
          return 0;
        }
        // version + exitcode
        else if (param == "--version-with-exitcode")
        {
          return showVersionExitcode();
        }
        // data files directory
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          // Was directory set more than once?
          if (!dataDir.empty())
          {
            std::cerr << "Error: Data Files directory was already set!\n";
            return MWTP::rcInvalidParameter;
          }
          // enough parameters?
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              dataDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add one.
              dataDir = slashify(dataDir);
              std::cerr << "Data Files directory was set to \"" << dataDir
                        << "\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << argv[i+1] << "\" is too short to "
                        << "be a proper directory path.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a directory name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // plugin file name
        else if (param == "-f")
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            if (!pluginFile.empty())
            {
              std::cerr << "Parameter -f must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            pluginFile = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "Plugin file was set to \"" << pluginFile << "\".\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // output file name
        else if ((param == "-o") || (param == "--output"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            if (!outputFileName.empty())
            {
              std::cerr << "Parameter -o must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            outputFileName = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "Output file was set to " << outputFileName << ".\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // force
        else if (param == "--force")
        {
          if (forceOutput)
          {
            std::cerr << "Error: Parameter --force was given more than once.\n";
            return MWTP::rcInvalidParameter;
          }
          forceOutput = true;
        }
        // XML file
        else if (param == "-xml")
        {
          if ((i + 1 < argc) && (argv[i+1] != nullptr))
          {
            if (!pathToCellsXML.empty())
            {
              std::cerr << "Parameter -xml must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            pathToCellsXML = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "XML file was set to \"" << pathToCellsXML << "\".\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // no scripts
        else if (param == "--no-scripts")
        {
          if (!process_scripts)
          {
            std::cerr << "Error: parameter --no-script was given more than "
                      << "once.\n";
            return MWTP::rcInvalidParameter;
          }
          process_scripts = false;
          std::cout << "Scripts will not be processed, as requested via --no-scripts.\n";
        }
        // daring script compiler
        else if ((param == "--dare") || (param == "--try-harder"))
        {
          if (tryHarder)
          {
            std::cerr << "Error: Parameter " << param << " was given more than once.\n";
            return MWTP::rcInvalidParameter;
          }
          tryHarder = true;
          std::cout << "Will try harder to compile scripts, as requested via "
                    << param << ". However, be aware that this might increase "
                    << "instability of Morrowind, as long as you use the generated file!\n";
        }
        // translate from English to German
        else if ((param == "-de") || (param == "--deutsch"))
        {
          if (translationDirectionSpecified)
          {
            std::cerr << "Error: More than one translation mode was specified.\n";
            return MWTP::rcInvalidParameter;
          }
          translationDirection = MWTP::td_en_de;
          translationDirectionSpecified = true;
          std::cout << "Translation mode was set: English to German.\n";
        }
        // translate from German to English
        else if ((param == "-en") || (param == "--english"))
        {
          if (translationDirectionSpecified)
          {
            std::cerr << "Error: More than one translation mode was specified.\n";
            return MWTP::rcInvalidParameter;
          }
          translationDirection = MWTP::td_de_en;
          translationDirectionSpecified = true;
          std::cout << "Translation mode was set: German to English.\n";
        }
        else
        {
          // unknown or wrong parameter
          std::cerr << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return MWTP::rcInvalidParameter;
        }
      } // parameter exists
      else
      {
        std::cout << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments present
  else
  {
    std::cerr << "You have to specify certain parameters for this program to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }

  // check for plugin file
  if (pluginFile.empty())
  {
    std::cout << "You have to specify certain parameters for this program to run properly.\n"
              << "In particular, the parameter -f followed by a plugin file name is required.\n"
              << "Use the parameter --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }
  // check XML file
  if (pathToCellsXML.empty())
  {
    pathToCellsXML = "cells.xml";
    std::clog << "Setting XML file name to cells.xml, because no XML file name "
              << "was specified.\n";
  }
  // check output
  if (outputFileName.empty())
  {
    outputFileName = "out.esp";
    std::clog << "Setting output file name to out.esp, because no output file "
              << "name was specified.\n";
  }
  // check translation direction
  if (!translationDirectionSpecified)
  {
    translationDirection = MWTP::td_en_de;
    std::clog << "No translation mode was specified, setting default: English "
              << "to German.\n";
  }

  // check for possible file conflicts
  if (pluginFile == outputFileName)
  {
    std::cout << "Warning: Plugin file name and output file name are identical!\n"
              << "The plugin file would be overwritten, so we abort the program.\n";
    return MWTP::rcFileNameConflict;
  }
  // avoid overwriting main ESM files (better be safe than sorry)
  const std::string lcOutput = lowerCase(outputFileName);
  if ((lcOutput == "morrowind.esm") || (lcOutput == "tribunal.esm") || (lcOutput == "bloodmoon.esm"))
  {
    std::cerr << "Error: You have given Morrowind.esm, Tribunal.esm or "
              << "Bloodmoon.esm as desired output file name!\nAre you nuts? "
              << "The plugin file would overwrite the original master file, so"
              << " we abort the program for your own safety. Think (again)!\n";
    return MWTP::rcFileNameConflict;
  }

  // Has the user specified a data directory?
  if (dataDir.empty())
  {
    // No, so let's search the registry first...
    std::cout << "Warning: Data files directory of Morrowind was not specified, "
              << "will try to read it from the registry.\n";
    if (!MWTP::getMorrowindPathFromRegistry(dataDir))
    {
      std::cout << "Error: Could not find Morrowind's installation path in registry!\n";
      dataDir.clear();
    }
    else
    {
      if (!dataDir.empty())
      {
        // Does it have a trailing (back)slash? If not, add one.
        dataDir = slashify(dataDir);
        /*add Data Files dir to path, because installed path points only to
          Morrowinds's main directory */
        dataDir = dataDir + "Data Files" + MWTP::pathDelimiter;
        std::cout << "Data Files directory was set to \"" << dataDir << "\" via registry.\n";
      }
      else
      {
        std::cout << "Error: Installation path in registry is empty!\n";
      }
    }

    // check again, in case registry failed
    if (dataDir.empty())
    {
      // empty, so let's try a default value.
      dataDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
      std::cout << "Warning: Data files directory of Morrowind was not specified, "
                << "will use default path \"" << dataDir << "\". This might not work"
                << " properly on your machine, use the parameter -d to specify "
                << "the proper path.\n";
    }
  } // if no data dir is given

  // avoid overwriting any existing plugin files
  if (FileExists(dataDir + outputFileName) && !forceOutput)
  {
    std::cerr << "Error: You have given the name of a file that already exists"
              << " as desired output file name! Aborting to avoid potential "
              << "data loss by overwriting the existing plugin file.\n"
              << "If you are sure you want to overwrite it, use the --force parameter.\n";
    return MWTP::rcFileNameConflict;
  }

  MWTP::CellListType cells;
  if (readCellListFromXML(pathToCellsXML, cells, translationDirection))
  {
    if (cells.empty())
    {
      std::cerr << "The XML file \"" << pathToCellsXML << "\" did not contain any "
                << "cell name pairs. Thus, nothing can be translated here.\n";
      return MWTP::rcXMLEmpty;
    }
    std::cout << "Parsing XML was successful. There are " << cells.size()
              << " pairs in the list.\n";
  }
  else
  {
    std::cout << "Parsing of file \"" << pathToCellsXML << "\" failed.\n";
    return MWTP::rcXMLError;
  }

  // now read the file
  MWTP::ESMReaderTranslator::VectorType recordVec;
  MWTP::ESMReaderTranslator reader(&recordVec);
  MWTP::TES3Record tes3Header;
  std::cout << "Reading plugin file and dependencies. This may take a while.\n";
  if (reader.readESM(dataDir + pluginFile, tes3Header) < 0)
  {
    std::cerr << "Error while reading file " << dataDir + pluginFile
              << ".\nAborting.\n";
    reader.deallocateRecordsInVector();
    return MWTP::rcFileError;
  }

  if (process_scripts)
  {
    MWTP::DepFileList deps;
    deps.clear();
    deps = tes3Header.dependencies;

    #ifdef DEBUG
    std::cout <<"Debug: deps before sort:\n";
    deps.writeDeps();
    deps.sort();
    deps.removeDuplicates();
    std::cout <<"Debug: deps after sort:\n";
    deps.writeDeps();
    std::cout << "---End of info---\n";
    #endif

    // read the dependency files to get the necessary data for the script compiler
    MWTP::ESMReaderScriptCompiler sc_reader;
    for (unsigned int i = 0; i < deps.getSize(); ++i)
    {
      MWTP::TES3Record dummy;
      if (sc_reader.readESM(dataDir + deps.at(i).name, dummy) < 0)
      {
        std::cerr << "Error while reading file " << dataDir + deps.at(i).name
                  << ".\nAborting.\n";
        reader.deallocateRecordsInVector();
        return MWTP::rcFileError;
      }
    }

    // feeding plugin file to script compiler
    if (sc_reader.readESM(dataDir + pluginFile, tes3Header) < 0)
    {
      std::cerr << "Error while reading file " << dataDir + pluginFile
                << ".\nAborting.\n";
      reader.deallocateRecordsInVector();
      return MWTP::rcFileError;
    }
  } // if process scripts

  const std::string cellID = typeid(MWTP::CellRecord).name();
  const std::string creatureID = typeid(MWTP::CreatureRecord).name();
  const std::string infoID = typeid(MWTP::DialogueInfoRecord).name();
  const std::string npcID = typeid(MWTP::NPCRecord).name();
  const std::string pathgridID = typeid(MWTP::PathGridRecord).name();
  const std::string scriptID = typeid(MWTP::ScriptRecord).name();

  MWTP::ESMReaderGeneric::VectorType::const_iterator v_iter = recordVec.begin();
  unsigned int changedRecords = 0;
  while (v_iter!=recordVec.end())
  {
    const std::string type_name = typeid(**v_iter).name();
    if (type_name == pathgridID)
    {
      const auto cell_iter = cells.find(dynamic_cast<MWTP::PathGridRecord*>(*v_iter)->CellName);
      if (cell_iter != cells.end())
      {
        dynamic_cast<MWTP::PathGridRecord*>(*v_iter)->CellName = cell_iter->second;
        ++changedRecords;
      }
    }//if path grid
    else if (type_name == cellID)
    {
      if (!translateCellRecord(dynamic_cast<MWTP::CellRecord*>(*v_iter), cells, changedRecords))
      {
        std::cout << "Error: Could not translate cell record.\n";
      }
    }
    else if ((type_name == creatureID) || (type_name == npcID))
    {
      translatePreNPCRecord(dynamic_cast<MWTP::PreNPCRecord*>(*v_iter), cells, changedRecords);
    }
    else if (type_name == infoID)
    {
      translateInfoRecord(dynamic_cast<MWTP::DialogueInfoRecord*>(*v_iter), cells, changedRecords);
    }
    else if ((type_name == scriptID) && (process_scripts))
    {
      // get original script text
      std::string changedText = dynamic_cast<MWTP::ScriptRecord*>(*v_iter)->ScriptText;
      // Do we need to change anything?
      if (replaceCellsInScriptText(changedText, cells))
      {
        // yes, change needed
        if (!canCompileScriptProperly(*dynamic_cast<MWTP::ScriptRecord*>(*v_iter)) && !tryHarder)
        {
          std::cerr << "Error: Cannot properly compile script \""
                    << dynamic_cast<MWTP::ScriptRecord*>(*v_iter)->recordID
                    << "\". Aborting!\n";
          reader.deallocateRecordsInVector();
          return MWTP::rcCouldNotDo;
        }
        dynamic_cast<MWTP::ScriptRecord*>(*v_iter)->ScriptText = changedText;
        if (!MWTP::ScriptCompiler::CompileScript(changedText, *dynamic_cast<MWTP::ScriptRecord*>(*v_iter)))
        {
          std::cerr << "Error: Script \"" << dynamic_cast<MWTP::ScriptRecord*>(*v_iter)->recordID
                    << "\" did not recompile without errors!\n";
          reader.deallocateRecordsInVector();
          return MWTP::rcScriptError;
        }
        ++changedRecords;
      } // if change needed
    } // if script record
    /// TODO: Maybe translate region names, too.
    ++v_iter;
  } // while

  if (changedRecords == 0)
  {
    std::cout << "No records of the file " << pluginFile << " were changed.\n";
    reader.deallocateRecordsInVector();
    return MWTP::rcNoChange;
  }
  else if (changedRecords == 1)
  {
    std::cout << "One single record was changed during the translation "
              << "process.\n";
  }
  else
  {
    std::cout << changedRecords << " records were changed during the translation "
              << "process.\n";
  }

  // try to write stuff to the output file
  MWTP::ESMWriterGeneric writer(&recordVec);
  //TODO: adjust description before writing
  tes3Header.description = "(TODO: put description here)";
  if (!writer.writeESM(dataDir+outputFileName, false /* no master */, tes3Header))
  {
    std::cout << "Error: Could not create or write to output file \""
              << dataDir + outputFileName << "\".\n";
    reader.deallocateRecordsInVector();
    return MWTP::rcOutputFailed;
  }
  reader.deallocateRecordsInVector();
  std::cout << "Success!\nFile was saved as \"" << dataDir + outputFileName << "\".\n";
  return 0;
}
