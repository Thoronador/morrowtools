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

/*---------------------------------------------------------------------------

  To do:
  ======
    - allow to read a customized list of master and plugin files and not just
      the standard files (Morrowind.esm, Tribunal.esm, Bloodmoon.esm)

  --------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include "..\base\ESMReader.h"
#include "..\base\ESMWriter.h"
#include "..\base\GameSettings.h"
#include "..\base\MagicEffects.h"
#include "..\base\Spells.h"
#include "..\base\HelperFunctions.h"

//return codes
const int rcInvalidParameter = 1;
const int rcFileError = 2;
const int rcDataError = 3;
const int rcOutputFailed = 4;

void showHelp()
{
  std::cout << "\nspell_rename [-o FILENAME1] [--allow-truncate] -d DIRECTORY [-f PLUGINFILE]\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -o FILENAME1     - sets the name of the created output file to FILENAME1.\n"
            << "                     If omitted, the default output file is \"out.esp\".\n"
            << "  -d DIRECTORY     - path to the Data Files directory of Morrowind, including\n"
            << "                     trailing backslash (If omitted, the programme might end or\n"
            << "                     crash before doing its job.)\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -f PLUGINFILE    - adds the plugin PLUGINFILE to the list of files that will\n"
            << "                     be searched for spells\n"
            << " -i                - tries to load list of plugin files from Morrowind.ini\n"
            << " --ini             - same as -i\n"
            << " -id               - tries to load list of plugin files from Morrowind.ini and\n"
            << "                     discards all files that have been added via -f parameters\n"
            << "  --verbose        - shows some additional information about data files\n"
            << "  --silent         - opposite of --verbose; does not show additonal information\n"
            << "  --allow-truncate - In order to avoid errors during loading of the created\n"
            << "                     plugin file, the programme usually does not change the\n"
            << "                     name of spells that get too long for Morrowind. (Names\n"
            << "                     will get longer due to the added prefix for the sorting\n"
            << "                     order.)\n"
            << "                     However, this can result in some spells not being sorted\n"
            << "                     in the manner you expect them to be sorted. If you want to\n"
            << "                     sort those spells, too, then specify this option. But be\n"
            << "                     aware that this can result in error messages during the\n"
            << "                     loading process of the plugin and/or names that are just\n"
            << "                     cut off at the end (i.e. they might be missing the last\n"
            << "                     few letters). You have been warned.\n";
}

void showGPLNotice()
{
  std::cout << "Spell renamer for Morrowind\n"
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
  std::cout << "Spell renamer for Morrowind, version 0.1_rev009, 2011-01-26\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::string outputFileName = "out.esp";
  std::string baseDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
  bool allowTruncatedName = false;
  bool verbose = false;
  bool doIni = false;
  bool discardBeforeIni = false;
  //list of .esp files to scan for spells
  DepFileList files;
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
        //allow truncated names parameter
        else if (param=="--allow-truncate")
        {
          allowTruncatedName = true;
          std::cout << "Truncated names are allowed due to \"--allow-truncate\".\n";
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
        //parameter for setting output file
        else if ((param=="-o") or (param=="--output"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            outputFileName = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \""<<outputFileName<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param<<"\".\n";
            return rcInvalidParameter;
          }
        }//output file
        else if ((param=="-d") or (param=="-dir") or (param=="--data-files"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            //Is it long enough to be a directoy? (Minimum should be "./".)
            if (std::string(argv[i+1]).size()>1)
            {
              baseDir = std::string(argv[i+1]);
              ++i; //skip next parameter, because it's used as directory name already
              std::cout << "Data files directory was set to \""<<baseDir<<"\".\n";
              //Does it have a trailing (back)slash?
              if (baseDir.at(baseDir.length()-1)!='\\')
              {
                baseDir = baseDir + "\\";
                std::cout << "Missing trailing backslash was added.\n";
              }//if slash
            }
            else
            {
              std::cout << "Parameter \""<<std::string(argv[i+1])<<"\" is too"
                        << " short to be a proper directory path.\n";
              return rcInvalidParameter;
            }//else
          }
          else
          {
            std::cout << "Error: You have to specify a directory name after \""
                      << param<<"\".\n";
            return rcInvalidParameter;
          }
        }//data files directory
        //add plugin file to list
        else if ((param=="-f") or (param=="--add-file"))
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
            return rcInvalidParameter;
          }
        }//plugin file
        //try to read from Morrowind.ini?
        else if ((param=="-i") or (param=="--ini"))
        {
          doIni = true;
          discardBeforeIni = false;
        }
        else if ((param=="-id") or (param=="--ini-with-discard"))
        {
          doIni = true;
          discardBeforeIni = true;
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
  }//if argc

  //now do the Morrowind.ini stuff
  if (doIni)
  {
    if (discardBeforeIni)
    {
      if (!files.isEmpty())
      {
        std::cout << "Discarding all listed file names as specified by "
                  << "--ini-with-discard parameter.\n";
        files.clear();
      }
    }

    //check for Morrowind.ini
    if (!FileExists(baseDir+"..\\Morrowind.ini"))
    {
      std::cout << "Couldn't find Morrowind.ini!\n";
      return rcFileError;
    }
    //now read the files from the .ini
    std::ifstream iniFile;
    iniFile.open((baseDir+"..\\Morrowind.ini").c_str(), std::ios::in | std::ios::binary);
    if (!iniFile)
    {
      std::cout << "Could not open Morrowind.ini!\n";
      return rcFileError;
    }
    const size_t bufferSize = 512;
    char Buffer[bufferSize];
    memset(Buffer, 0, bufferSize);
    std::string ini_line = "";
    bool in_gf_section = false;
    while (iniFile.getline(Buffer, bufferSize-1))
    {
      ini_line = std::string(Buffer);
      //skip empty lines
      if (ini_line.length()>0)
      {
        //clear carriage return at end of line, if present
        if (ini_line.at(ini_line.length()-1)=='\r')
        {
          ini_line.erase(ini_line.length()-1);
        }

        if (!in_gf_section)
        {
          in_gf_section = (ini_line == "[Game Files]");
        }
        else
        {
          if (ini_line.length()>10)
          {
            if (ini_line.substr(0,8) == "GameFile")
            {
              //we've got one line
              const size_t pos = ini_line.find('=');
              if ((pos>8) &&  (pos!=std::string::npos))
              {
                files.push_back(ini_line.substr(pos+1, std::string::npos));
              }
            }//if is GameFile line
          }//required minimum length?
        }//else
      }//if line not empty
      memset(Buffer, 0, bufferSize);
    }//while
    iniFile.close();
  }//if ini


  //check file list
  if (files.isEmpty())
  {
    files.push_back("Morrowind.esm");
    if (FileExists(baseDir+"Tribunal.esm"))
    {
      files.push_back("Tribunal.esm");
    }
    if (FileExists(baseDir+"Bloodmoon.esm"))
    {
      files.push_back("Bloodmoon.esm");
    }
  }
  else
  { //There are already some files in the list, but are the master files in there, too?
    //Let's add master files, if neccessary.
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
  }//else

  std::cout << "List of active files:\n";
  files.writeDeps();

  //read all files
  std::cout << "Reading files, this may take a while.\n";
  unsigned int i;
  for (i=0; i<files.getSize(); ++i)
  {
    if (!ReadESM(baseDir+files.at(i).name, verbose))
    {
      std::cout << "Error while reading file \""<<baseDir+files.at(i).name
                <<"\".\nAborting.\n";
      return rcFileError;
    }
    //try to get file size (used later in writing plugin file header)
    files.at(i).size = getFileSize64(baseDir+files.at(i).name);
  }//for
  std::cout << "Info: "<<files.getSize()<<" Master/ Plugin file(s) containing "
            << Spells::getSingleton().getNumberOfSpells()<<" spell(s) were read.\n";

  std::vector<std::string> prefixes;
  prefixes.clear();
  //spell school settings present?
  GameSettings& GMSTs = GameSettings::getSingleton();
  GMSTRecord record;

  //alteration -> 0
  if (GMSTs.hasSetting("sSchoolAlteration"))
  {
    record = GMSTs.getSetting("sSchoolAlteration");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolAlteration\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for alteration found.\n";
    return rcDataError;
  }

  //conjuration -> 1
  if (GMSTs.hasSetting("sSchoolConjuration"))
  {
    record = GMSTs.getSetting("sSchoolConjuration");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolConjuration\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for conjuration found.\n";
    return rcDataError;
  }

  //destruction -> 2
  if (GMSTs.hasSetting("sSchoolDestruction"))
  {
    record = GMSTs.getSetting("sSchoolDestruction");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolDestruction\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for destruction found.\n";
    return rcDataError;
  }

  //illusion -> 3
  if (GMSTs.hasSetting("sSchoolIllusion"))
  {
    record = GMSTs.getSetting("sSchoolIllusion");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolIllusion\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for illusion found.\n";
    return rcDataError;
  }

  //mysticism -> 4
  if (GMSTs.hasSetting("sSchoolMysticism"))
  {
    record = GMSTs.getSetting("sSchoolMysticism");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolMysticism\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for mysticism found.\n";
    return rcDataError;
  }

  //restoration -> 5
  if (GMSTs.hasSetting("sSchoolRestoration"))
  {
    record = GMSTs.getSetting("sSchoolRestoration");
    if (record.Type!=gtString)
    {
      std::cout << "Error: GMST \"sSchoolRestoration\" does not seem to be a string.\n";
      return rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for restoration found.\n";
    return rcDataError;
  }

  //delete all non-spells (abilities, powers,...) and update spells with naming scheme
  SpellListIterator spell_end = Spells::getSingleton().getEnd();
  SpellListIterator spell_cur = Spells::getSingleton().getBegin();
  while (spell_cur!=spell_end)
  {
    //Is it a spell?
    if (spell_cur->second.Type!=stSpell)
    {
      const std::string temp = spell_cur->first;
      ++spell_cur;
      //delete spell so we don't save it into the final plugin
      Spells::getSingleton().removeSpell(temp);
    }//if not spell
    else
    {
      bool unchanged = false;
      SpellRecord rec = spell_cur->second;
      if (!rec.Enchs.empty())
      {
        if (MagicEffects::getSingleton().hasEffect(rec.Enchs.at(0).EffectID))
        {
          const int school = MagicEffects::getSingleton().getEffect(rec.Enchs.at(0).EffectID).SpellSchool;
          if (school<0 or school>=prefixes.size())
          {
            std::cout << "Error: effect "<<rec.Enchs.at(0).EffectID<<" has an "
                      << "invalid spell school index of "<<school<<".\n";
            return rcDataError;
          }//if school invalid
          const std::string newName = prefixes[school]+" "+rec.Name;
          if (allowTruncatedName or (newName.length()<=Spells::cMaximumSpellNameLength))
          {
            rec.Name = newName;
            Spells::getSingleton().addSpell(spell_cur->first, rec);
          }//if
          else
          {
            /*If it is not changed, then we don't need to include it in the
              plugin file and can delete it right here. */
            //implement deletion
            const std::string delete_ID = spell_cur->first;
            ++spell_cur;
            //delete spell so we don't save it into the final plugin
            Spells::getSingleton().removeSpell(delete_ID);
            //set unchanged flag to avoid iterator increase at the end
            unchanged = true;
          }//else
        }//if
        else
        {
          std::cout << "Error: effect with ID "<<rec.Enchs.at(0).EffectID
                    <<" does not exist.\n";
          return rcDataError;
        }
      }
      else
      {
        std::cout << "Error: spell \""<<spell_cur->first<<"\" does not have any"
                  << " enchantment data.\n";
        return rcDataError;
      }
      if (!unchanged)
      {
        ++spell_cur;
      }
    }
  }//while

  std::cout << "Info: "<<Spells::getSingleton().getNumberOfSpells()<<" spell(s) processed.\n";

  //write spells to file
  if (Spells::getSingleton().getNumberOfSpells()==0)
  {
    std::cout << "No spells available. No new file will be created.\n";
    return 0;
  }
  if (WriteESMofSpells(baseDir+outputFileName, false, files))
  {
    std::cout << "Output file \""<<baseDir+outputFileName<<"\" was created successfully.\n";
    return 0;
  }//if
  std::cout << "Error: Could not create or write to output file \""<<baseDir+outputFileName<<"\".\n";
  return rcOutputFailed;
}
