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

//return codes
const int rcInvalidParameter = 1;
const int rcFileError = 2;
const int rcOutputFailed = 4;

const int rcXMLError = 6;
const int rcXMLEmpty = 7;
const int rcScriptError = 8;
const int rcNoChange = 9;

void showHelp()
{
  std::cout << "\ncell_translator -f PLUGINFILE [-o FILENAME1] [-xml FILENAME2]\n"
            << "\n"
            << "(Note: This help is still incomplete.)\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -f PLUGINFILE    - sets the plugin PLUGINFILE as the files that will be\n"
            << "                     translated by this programme. This option is required.\n"
            << "  -o FILENAME1     - sets the name of the created output file to FILENAME1.\n"
            << "                     If omitted, the default output file is \"out.esp\".\n"
            << "  -xml FILENAME2   - sets the name of the XML file that contains the cell names\n"
            << "                     to FILENAME2. If omitted, the default XML file name is\n"
            << "                     \"cells.xml\".\n";
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
  std::cout << "Cell Translator for Morrowind, version 0.3_rev229, 2011-05-06\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();

  std::string pathToCellsXML = "";
  std::string pluginFile = "";
  std::string outputFileName = "";

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
              return rcInvalidParameter;
            }
            pluginFile = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Plugin file was set to \""<<pluginFile<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else if (param=="-o")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!outputFileName.empty())
            {
              std::cout << "Parameter -o must not occur more than once!\n";
              return rcInvalidParameter;
            }
            outputFileName = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \""<<outputFileName<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else if (param=="-xml")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!pathToCellsXML.empty())
            {
              std::cout << "Parameter -xml must not occur more than once!\n";
              return rcInvalidParameter;
            }
            pathToCellsXML = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "XML file was set to \""<<pathToCellsXML<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  //check for plugin file
  if (pluginFile.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "In particular, the parameter -f followed by a plugin file name is required.\n"
              << "Use the parameter --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
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

  if (pluginFile==outputFileName)
  {
    std::cout << "Warning: plugin file name and output file name are identical!\n"
              << "The plugin file will be overwritten.";
  }

  CellListType cells;
  if (readCellListFromXML(pathToCellsXML, cells, td_en_de))
  {
    if (cells.size()==0)
    {
      std::cout << "The XML file \""<<pathToCellsXML<<"\" did not contain any "
                << "cell name pairs. Thus, nothing can be translated here.\n";
      return rcXMLEmpty;
    }//if (the inner one)
    std::cout << "Parsing XML was successful. There are "<<cells.size()<<" pairs in the list.\n";
  }
  else
  {
    std::cout << "Parsing of file \""<<pathToCellsXML<<"\" failed.\n";
    return rcXMLError;
  }

  //now read the file
  ESMReaderTranslator::VectorType recordVec;
  DepFileList deps;
  deps.clear();
  ESMReaderTranslator reader(&recordVec);
  std::cout << "Reading plugin file and dependencies. This may take a while.\n";
  if (reader.readESM(pluginFile, deps, false)<0)
  {
    std::cout << "Error while reading file \""<<pluginFile<<"\".\nAborting.\n";
    reader.deallocateRecordsInVector();
    return rcFileError;
  }

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
  ESMReaderScriptCompiler sc_reader;
  for (i=0; i<deps.getSize(); ++i)
  {
    DepFileList dummy_deps;
    if (sc_reader.readESM(baseDir+deps.at(i).name, dummy_deps, false)<0)
    {
      std::cout << "Error while reading file \""<<baseDir+deps.at(i).name
                << "\".\nAborting.\n";
      reader.deallocateRecordsInVector();
      return rcFileError;
    }//if
  }//for

  //feeding plugin file to script compiler
  if (sc_reader.readESM(pluginFile, deps, false)<0)
  {
    std::cout << "Error while reading file \""<<pluginFile<<"\".\nAborting.\n";
    reader.deallocateRecordsInVector();
    return rcFileError;
  }//if

  const std::string genericID = typeid(GenericRecord).name();
  const std::string cellID = typeid(CellRecord).name();
  const std::string creatureID = typeid(CreatureRecord).name();
  const std::string infoID = typeid(DialogueInfoRecord).name();
  const std::string npcID = typeid(NPCRecord).name();
  const std::string pathgridID = typeid(PathGridRecord).name();
  const std::string scriptID = typeid(ScriptRecord).name();

  CellListType::const_iterator cell_iter;
  ESMReaderGeneric::VectorType::const_iterator v_iter = recordVec.begin();
  unsigned int changedRecords = 0;
  while (v_iter!=recordVec.end())
  {
    const std::string type_name = typeid(**v_iter).name();
    if (type_name==pathgridID)
    {
      cell_iter = cells.find(dynamic_cast<PathGridRecord*>(*v_iter)->CellName);
      if (cell_iter!=cells.end())
      {
        dynamic_cast<PathGridRecord*>(*v_iter)->CellName = cell_iter->second;
        ++changedRecords;
      }
    }//if path grid
    else if (type_name==cellID)
    {
      if (!translateCellRecord(dynamic_cast<CellRecord*>(*v_iter), cells, changedRecords))
      {
        std::cout << "Error: couldn't translate cell record.\n";
      }
    }
    else if ((type_name==creatureID) or (type_name==npcID))
    {
      translatePreNPCRecord(dynamic_cast<PreNPCRecord*>(*v_iter), cells, changedRecords);
    }//if creature or NPC
    else if (type_name==infoID)
    {
      translateInfoRecord(dynamic_cast<DialogueInfoRecord*>(*v_iter), cells, changedRecords);
    }
    else if (type_name==scriptID)
    {
      if (!translateScriptRecord(dynamic_cast<ScriptRecord*>(*v_iter), cells, changedRecords))
      {
        std::cout << "Error: couldn't translate cells in script \""
                  << dynamic_cast<ScriptRecord*>(*v_iter)->ScriptID<<"\"!\n";
        reader.deallocateRecordsInVector();
        return rcScriptError;
      }//if not translated
    }//if script record
    /// TODO: Maybe translate region names, too.
    ++v_iter;
  }//while

  if (changedRecords==0)
  {
    std::cout << "No records of the file \""<<pluginFile<<"\" were changed.";
    reader.deallocateRecordsInVector();
    return rcNoChange;
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
  ESMWriterGeneric writer(&recordVec);
  if (!writer.writeESM(outputFileName, false /* no master */, deps,
         "(TODO: put description here)"))
  {
    std::cout << "Error: Could not create or write to output file \""<<outputFileName<<"\".\n";
    reader.deallocateRecordsInVector();
    return rcOutputFailed;
  }
  reader.deallocateRecordsInVector();
  std::cout << "Success!\n";
  return 0;
}
