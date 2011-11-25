/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include <iostream>
#include <string>
#include <map>
#include <typeinfo>
#include "ESMReaderTranslator.h"
#include "TranslatorXML.h"
#include "TranslateRecords.h"
#include "../base/ESMWriterGeneric.h"
#include "../base/ESMReaderScriptCompiler.h"
#include "../base/records/GenericRecord.h"
#include "../base/records/CreatureRecord.h"
#include "../base/records/NPCRecord.h"
#include "../base/records/PathGridRecord.h"
#include "../base/ReturnCodes.h"

void showHelp()
{
  std::cout << "\ncell_translator -f PLUGINFILE [-o FILENAME1] [-xml FILENAME2]\n"
            << "\n"
            << "(Note: This help is still incomplete.)\n"
            << "options:\n"
            << "  --help             - displays this help message and quits\n"
            << "  -?                 - same as --help\n"
            << "  --version          - displays the version of the programme and quits\n"
            << "  -f PLUGINFILE      - sets the plugin PLUGINFILE as the files that will be\n"
            << "                       translated by this programme. This option is required.\n"
            << "  --output FILENAME1 - sets the name of the created output file to FILENAME1.\n"
            << "                       If omitted, the default output file is \"out.esp\".\n"
            << "  -o FILENAME1       - short for --output\n"
            << "  -xml FILENAME2     - sets the name of the XML file that contains the cell\n"
            << "                       names to FILENAME2. If omitted, the default XML file\n"
            << "                       name is \"cells.xml\".\n"
            << "  -de                - sets translation mode to translate from English to\n"
            << "                       German. Mutually exclusive with -en.\n"
            << "  -en                - sets translation mode to translate from German to\n"
            << "                       English. Mutually exclusive with -de.\n"
            << "  --no-scripts       - Script records will not be processed, if this parameter\n"
            << "                       is specified.\n";
}

void showGPLNotice()
{
  std::cout << "Cell Translator for Morrowind\n"
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
  std::cout << "Cell Translator for Morrowind, version 0.3_rev250, 2011-05-24\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();

  std::string pathToCellsXML = "";
  std::string pluginFile = "";
  std::string outputFileName = "";
  bool process_scripts = true;
  bool translationDirectionSpecified = false;
  MWTP::TransDir translationDirection = MWTP::td_en_de;

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
        else if (param=="-f")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!pluginFile.empty())
            {
              std::cout << "Parameter -f must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            pluginFile = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Plugin file was set to \""<<pluginFile<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else if ((param=="-o") or (param=="--output"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!outputFileName.empty())
            {
              std::cout << "Parameter -o must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            outputFileName = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \""<<outputFileName<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else if (param=="-xml")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!pathToCellsXML.empty())
            {
              std::cout << "Parameter -xml must not occur more than once!\n";
              return MWTP::rcInvalidParameter;
            }
            pathToCellsXML = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "XML file was set to \""<<pathToCellsXML<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else if (param=="--no-scripts")
        {
          if (!process_scripts)
          {
            std::cout << "Error: parameter --no-script was given more than "
                      << "once.\n";
            return MWTP::rcInvalidParameter;
          }
          process_scripts = false;
          std::cout << "Scripts will not be processed, as requested via --no-scripts.\n";
        }
        else if ((param=="-de") or (param=="--deutsch"))
        {
          if (translationDirectionSpecified)
          {
            std::cout << "Error: more than one translation mode was specified.\n";
            return MWTP::rcInvalidParameter;
          }
          translationDirection = MWTP::td_en_de;
          translationDirectionSpecified = true;
          std::cout << "Translation mode was set: English to German.\n";
        }//if de
        else if ((param=="-en") or (param=="--english"))
        {
          if (translationDirectionSpecified)
          {
            std::cout << "Error: more than one translation mode was specified.\n";
            return MWTP::rcInvalidParameter;
          }
          translationDirection = MWTP::td_de_en;
          translationDirectionSpecified = true;
          std::cout << "Translation mode was set: German to English.\n";
        }//if de
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

  //check for plugin file
  if (pluginFile.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "In particular, the parameter -f followed by a plugin file name is required.\n"
              << "Use the parameter --help to get a list of valid parameters.\n";
    return MWTP::rcInvalidParameter;
  }
  //check XML file
  if (pathToCellsXML.empty())
  {
    pathToCellsXML = "cells.xml";
    std::cout << "Setting XML file name to cells.xml, because no XML file name "
              << "was specified.\n";
  }
  //check output
  if (outputFileName.empty())
  {
    outputFileName = "out.esp";
    std::cout << "Setting output file name to out.esp, because no output file "
              << "name was specified.\n";
  }
  //check translation direction
  if (!translationDirectionSpecified)
  {
    translationDirection = MWTP::td_en_de;
    std::cout << "No translation mode was specified, setting default: English "
              << "to German.\n";
  }

  if (pluginFile==outputFileName)
  {
    std::cout << "Warning: plugin file name and output file name are identical!\n"
              << "The plugin file will be overwritten.";
  }

  MWTP::CellListType cells;
  if (readCellListFromXML(pathToCellsXML, cells, translationDirection))
  {
    if (cells.size()==0)
    {
      std::cout << "The XML file \""<<pathToCellsXML<<"\" did not contain any "
                << "cell name pairs. Thus, nothing can be translated here.\n";
      return MWTP::rcXMLEmpty;
    }//if (the inner one)
    std::cout << "Parsing XML was successful. There are "<<cells.size()<<" pairs in the list.\n";
  }
  else
  {
    std::cout << "Parsing of file \""<<pathToCellsXML<<"\" failed.\n";
    return MWTP::rcXMLError;
  }

  //now read the file
  MWTP::ESMReaderTranslator::VectorType recordVec;
  MWTP::DepFileList deps;
  deps.clear();
  MWTP::ESMReaderTranslator reader(&recordVec);
  std::cout << "Reading plugin file and dependencies. This may take a while.\n";
  if (reader.readESM(pluginFile, deps, false)<0)
  {
    std::cout << "Error while reading file \""<<pluginFile<<"\".\nAborting.\n";
    reader.deallocateRecordsInVector();
    return MWTP::rcFileError;
  }

  if (process_scripts)
  {
    const std::string::size_type pos = pluginFile.rfind('\\');
    std::string baseDir;
    if (pos!=std::string::npos)
    {
      baseDir = pluginFile.substr(0,pos+1);
    }
    else
    {
      baseDir = ".\\";
    }

    std::cout <<"Debug: deps before sort:\n";
    deps.writeDeps();
    deps.sort();
    deps.removeDuplicates();
    std::cout <<"Debug: deps after sort:\n";
    deps.writeDeps();
    std::cout << "---End of info---\n";

    //read the dependency files to get the neccessary data for the script compiler
    unsigned int i;
    MWTP::ESMReaderScriptCompiler sc_reader;
    for (i=0; i<deps.getSize(); ++i)
    {
      MWTP::DepFileList dummy_deps;
      if (sc_reader.readESM(baseDir+deps.at(i).name, dummy_deps, false)<0)
      {
        std::cout << "Error while reading file \""<<baseDir+deps.at(i).name
                  << "\".\nAborting.\n";
        reader.deallocateRecordsInVector();
        return MWTP::rcFileError;
      }//if
    }//for

    //feeding plugin file to script compiler
    if (sc_reader.readESM(pluginFile, deps, false)<0)
    {
      std::cout << "Error while reading file \""<<pluginFile<<"\".\nAborting.\n";
      reader.deallocateRecordsInVector();
      return MWTP::rcFileError;
    }//if
  }//if process scripts

  const std::string genericID = typeid(MWTP::GenericRecord).name();
  const std::string cellID = typeid(MWTP::CellRecord).name();
  const std::string creatureID = typeid(MWTP::CreatureRecord).name();
  const std::string infoID = typeid(MWTP::DialogueInfoRecord).name();
  const std::string npcID = typeid(MWTP::NPCRecord).name();
  const std::string pathgridID = typeid(MWTP::PathGridRecord).name();
  const std::string scriptID = typeid(MWTP::ScriptRecord).name();

  MWTP::CellListType::const_iterator cell_iter;
  MWTP::ESMReaderGeneric::VectorType::const_iterator v_iter = recordVec.begin();
  unsigned int changedRecords = 0;
  while (v_iter!=recordVec.end())
  {
    const std::string type_name = typeid(**v_iter).name();
    if (type_name==pathgridID)
    {
      cell_iter = cells.find(dynamic_cast<MWTP::PathGridRecord*>(*v_iter)->CellName);
      if (cell_iter!=cells.end())
      {
        dynamic_cast<MWTP::PathGridRecord*>(*v_iter)->CellName = cell_iter->second;
        ++changedRecords;
      }
    }//if path grid
    else if (type_name==cellID)
    {
      if (!translateCellRecord(dynamic_cast<MWTP::CellRecord*>(*v_iter), cells, changedRecords))
      {
        std::cout << "Error: couldn't translate cell record.\n";
      }
    }
    else if ((type_name==creatureID) or (type_name==npcID))
    {
      translatePreNPCRecord(dynamic_cast<MWTP::PreNPCRecord*>(*v_iter), cells, changedRecords);
    }//if creature or NPC
    else if (type_name==infoID)
    {
      translateInfoRecord(dynamic_cast<MWTP::DialogueInfoRecord*>(*v_iter), cells, changedRecords);
    }
    else if ((type_name==scriptID) and (process_scripts))
    {
      if (!translateScriptRecord(dynamic_cast<MWTP::ScriptRecord*>(*v_iter), cells, changedRecords))
      {
        std::cout << "Error: couldn't translate cells in script \""
                  << dynamic_cast<MWTP::ScriptRecord*>(*v_iter)->ScriptID<<"\"!\n";
        reader.deallocateRecordsInVector();
        return MWTP::rcScriptError;
      }//if not translated
    }//if script record
    /// TODO: Maybe translate region names, too.
    ++v_iter;
  }//while

  if (changedRecords==0)
  {
    std::cout << "No records of the file \""<<pluginFile<<"\" were changed.";
    reader.deallocateRecordsInVector();
    return MWTP::rcNoChange;
  }
  else if (changedRecords==1)
  {
    std::cout << "One single record was changed during the translation "
              << "process.\n";
  }
  else
  {
    std::cout << changedRecords<<" records were changed during the translation "
              << "process.\n";
  }

  //try to write stuff to the output file
  MWTP::ESMWriterGeneric writer(&recordVec);
  if (!writer.writeESM(outputFileName, false /* no master */, deps,
         "(TODO: put description here)"))
  {
    std::cout << "Error: Could not create or write to output file \""<<outputFileName<<"\".\n";
    reader.deallocateRecordsInVector();
    return MWTP::rcOutputFailed;
  }
  reader.deallocateRecordsInVector();
  std::cout << "Success!\n";
  return 0;
}