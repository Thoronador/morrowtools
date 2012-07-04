/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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
    - [wishlist] more patterns

  --------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include "ESMReaderSpells.h"
#include "ESMWriterSpells.h"
#include "../../base/FileFunctions.h"
#include "../base/GameSettings.h"
#include "../base/IniFunctions.h"
#include "../base/MagicEffects.h"
#include "../base/ReturnCodes.h"
#include "../base/Spells.h"


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
            << "                     trailing backslash\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  -f PLUGINFILE    - adds the plugin PLUGINFILE to the list of files that will\n"
            << "                     be searched for spells\n"
            << "  -i               - tries to load list of plugin files from Morrowind.ini\n"
            << "  --ini            - same as -i\n"
            << "  -id              - tries to load list of plugin files from Morrowind.ini and\n"
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
  std::cout << "Spell Renamer for Morrowind\n"
            << "  This programme is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011, 2012  Thoronador\n"
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
  std::cout << "Spell Renamer for Morrowind, version 0.1.1_rev487, 2012-07-04\n";
}

int showVersionExitcode()
{
  showVersion();
  return 487;
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
        else if (param=="--version-with-exitcode")
        {
          return showVersionExitcode();
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
            return MWTP::rcInvalidParameter;
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
    const int iniReturnCode = getFilesFromMorrowindIni(baseDir, files);
    if (iniReturnCode!=0)
    {
      return iniReturnCode;
    }
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
  MWTP::ESMReaderSpells reader;
  std::cout << "Reading files, this may take a while.\n";
  MWTP::DepFileList removedFiles;
  i = 0;
  while (i<files.getSize())
  {
    MWTP::TES3Record DummyHead;//It's not actually used after the read function, but
                          // readESM() needs one as parameter.
    const int read_result = reader.readESM(baseDir+files.at(i).name, DummyHead, verbose);
    if (read_result<=-1)
    {
      std::cout << "Error while reading file \""<<baseDir+files.at(i).name
                <<"\".\nAborting.\n";
      return MWTP::rcFileError;
    }//if
    else if (read_result==0)
    {
      //file contains no relevant data, so push it onto the list of unneccessary
      //files and remove it
      removedFiles.push_back(files.at(i));
      files.removeEntry(i);
      //We do not increase counter (i) here, because the index will now contain
      //the next file, which will be read in the next iteration.
    }
    else
    {
      //something was read and file was not removed from list, so increase counter
      ++i;
    }
  }//while

  //check for removed files
  if (removedFiles.getSize()>0)
  {
    if (removedFiles.getSize()==1)
    {
      std::cout << "Info: The following file was removed from the list, "
                << "because it didn't contain any spell data:\n";
    }
    else
    {
      //two or more files
      std::cout << "Info: The following "<<removedFiles.getSize()<<" files were"
                << " removed from the list, because they didn't contain any "
                << "spell data:\n";
    }
    removedFiles.writeDeps();
    std::cout << std::endl;
  }//if

  std::cout << "Info: "<<files.getSize()<<" Master/ Plugin file(s) containing "
            << MWTP::Spells::getSingleton().getNumberOfRecords()<<" spell(s) were read.\n";

  std::vector<std::string> prefixes;
  prefixes.clear();
  //spell school settings present?
  MWTP::GameSettings& GMSTs = MWTP::GameSettings::getSingleton();
  MWTP::GMSTRecord record;

  //alteration -> 0
  if (GMSTs.hasSetting("sSchoolAlteration"))
  {
    record = GMSTs.getSetting("sSchoolAlteration");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolAlteration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for alteration found.\n";
    return MWTP::rcDataError;
  }

  //conjuration -> 1
  if (GMSTs.hasSetting("sSchoolConjuration"))
  {
    record = GMSTs.getSetting("sSchoolConjuration");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolConjuration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for conjuration found.\n";
    return MWTP::rcDataError;
  }

  //destruction -> 2
  if (GMSTs.hasSetting("sSchoolDestruction"))
  {
    record = GMSTs.getSetting("sSchoolDestruction");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolDestruction\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for destruction found.\n";
    return MWTP::rcDataError;
  }

  //illusion -> 3
  if (GMSTs.hasSetting("sSchoolIllusion"))
  {
    record = GMSTs.getSetting("sSchoolIllusion");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolIllusion\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for illusion found.\n";
    return MWTP::rcDataError;
  }

  //mysticism -> 4
  if (GMSTs.hasSetting("sSchoolMysticism"))
  {
    record = GMSTs.getSetting("sSchoolMysticism");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolMysticism\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for mysticism found.\n";
    return MWTP::rcDataError;
  }

  //restoration -> 5
  if (GMSTs.hasSetting("sSchoolRestoration"))
  {
    record = GMSTs.getSetting("sSchoolRestoration");
    if (record.Type!=MWTP::gtString)
    {
      std::cout << "Error: GMST \"sSchoolRestoration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cout << "Error: No GMST for restoration found.\n";
    return MWTP::rcDataError;
  }

  //delete all non-spells (abilities, powers,...) and update spells with naming scheme
  MWTP::Spells::ListIterator spell_end = MWTP::Spells::getSingleton().getEnd();
  MWTP::Spells::ListIterator spell_cur = MWTP::Spells::getSingleton().getBegin();
  while (spell_cur!=spell_end)
  {
    //Is it a spell?
    if (spell_cur->second.Type!=MWTP::stSpell)
    {
      const std::string temp = spell_cur->first;
      ++spell_cur;
      //delete spell so we don't save it into the final plugin
      MWTP::Spells::getSingleton().removeRecord(temp);
    }//if not spell
    else
    {
      bool unchanged = false;
      MWTP::SpellRecord rec = spell_cur->second;
      if (!rec.Enchs.empty())
      {
        if (MWTP::MagicEffects::getSingleton().hasEffect(rec.Enchs.at(0).EffectID))
        {
          const int school = MWTP::MagicEffects::getSingleton().getEffect(rec.Enchs.at(0).EffectID).SpellSchool;
          if ((school<0) or (school>=prefixes.size()))
          {
            std::cout << "Error: effect "<<rec.Enchs.at(0).EffectID<<" has an "
                      << "invalid spell school index of "<<school<<".\n";
            return MWTP::rcDataError;
          }//if school invalid
          const std::string newName = prefixes[school]+" "+rec.Name;
          if (allowTruncatedName or (newName.length()<=MWTP::SpellRecord::cMaximumSpellNameLength))
          {
            rec.Name = newName;
            MWTP::Spells::getSingleton().addRecord(rec);
          }//if
          else
          {
            /*If it is not changed, then we don't need to include it in the
              plugin file and can delete it right here. */
            //implement deletion
            const std::string delete_ID = spell_cur->first;
            ++spell_cur;
            //delete spell so we don't save it into the final plugin
            MWTP::Spells::getSingleton().removeRecord(delete_ID);
            //set unchanged flag to avoid iterator increase at the end
            unchanged = true;
          }//else
        }//if
        else
        {
          std::cout << "Error: effect with ID "<<rec.Enchs.at(0).EffectID
                    <<" does not exist.\n";
          return MWTP::rcDataError;
        }
      }
      else
      {
        std::cout << "Error: spell \""<<spell_cur->first<<"\" does not have any"
                  << " enchantment data.\n";
        return MWTP::rcDataError;
      }
      if (!unchanged)
      {
        ++spell_cur;
      }
    }
  }//while

  std::cout << "Info: "<<MWTP::Spells::getSingleton().getNumberOfRecords()<<" spell(s) processed.\n";

  //write spells to file
  if (MWTP::Spells::getSingleton().getNumberOfRecords()==0)
  {
    std::cout << "No spells available. No new file will be created.\n";
    return MWTP::rcNoSpells;
  }

  MWTP::ESMWriterSpells writer;
  MWTP::TES3Record headerData;
  MWTP::ESMWriter::setOldStyleHeaderData(headerData, false, files,
      "Umbenannte/ umsortierte Zauber fuer Morrowind (generiert durch spell_rename.exe)");
  if (writer.writeESM(baseDir+outputFileName, false, headerData))
  {
    std::cout << "Output file \""<<baseDir+outputFileName<<"\" was created successfully.\n";
    //now check file time of created file
    MWTP::DepFile outDep = outputFileName;
    if (getFileSizeAndModificationTime(baseDir+outputFileName, outDep.size, outDep.modified))
    {
      //only change, if last file isn't master, because plugins will be loaded
      // after master anyway. And output is plugin.
      if (!files.at(files.getSize()-1).isMasterFile())
      {
        //do we need to change plugin's modification time
        if (files.at(files.getSize()-1).modified>=outDep.modified)
        {
          if (setFileModificationTime(baseDir+outputFileName, files.at(files.getSize()-1).modified+60))
          {
            std::cout << "Hint: Could not change modification time of \""
                      << outputFileName<<"\" to ensure loading order.\n";
            return 0; //return zero anyway, file was created
          }
          else
          {
            std::cout << "Hint: Modification time of \""<<outputFileName
                      <<"\" was changed to ensure loading order.\n";
            return 0; //return zero, success
          }
        }//if
      }//if not master
    }//if
    return 0;
  }//if
  std::cout << "Error: Could not create or write to output file \""<<baseDir+outputFileName<<"\".\n";
  return MWTP::rcOutputFailed;
}
