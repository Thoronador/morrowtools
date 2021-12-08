/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

/*---------------------------------------------------------------------------

  To do:
  ======
    - [wishlist] more patterns

  --------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <vector>
#include "ESMReaderSpells.hpp"
#include "ESMWriterSpells.hpp"
#include "../../base/DirectoryFunctions.hpp"
#include "../../base/FileFunctions.hpp"
#include "../base/GameSettings.hpp"
#include "../base/IniFunctions.hpp"
#include "../base/MagicEffects.hpp"
#include "../base/ReturnCodes.hpp"
#include "../base/Spells.hpp"


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
            << "  This program is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011, 2012, 2021  Thoronador\n"
            << "\n"
            << "  This program is free software: you can redistribute it and/or modify\n"
            << "  it under the terms of the GNU General Public License as published by\n"
            << "  the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This program is distributed in the hope that it will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Spell Renamer for Morrowind, version 0.1.3_rev525, 2021-12-08\n";
}

int showVersionExitcode()
{
  showVersion();
  return 525;
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::string outputFileName = "out.esp";
  std::string baseDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
  bool allowTruncatedName = false;
  bool doIni = false;
  bool discardBeforeIni = false;
  //list of .esp files to scan for spells
  MWTP::DepFileList files;
  files.clear();

  if (argc > 1 && argv != nullptr)
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if (param == "--help" || param == "-?" || param == "/?")
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
        else if (param == "--version-with-exitcode")
        {
          return showVersionExitcode();
        }
        // allow truncated names parameter
        else if (param == "--allow-truncate")
        {
          allowTruncatedName = true;
          std::cout << "Truncated names are allowed due to \"--allow-truncate\".\n";
        }
        // parameter for setting output file
        else if ((param == "-o") || (param == "--output"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            outputFileName = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \"" << outputFileName << "\".\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // data files directory
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              baseDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add it.
              baseDir = slashify(baseDir);
              std::cout << "Data files directory was set to \"" << baseDir << "\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << std::string(argv[i+1]) << "\" is too"
                        << " short to be a proper directory path.\n";
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
        // add plugin file to list
        else if ((param == "-f") || (param == "--add-file"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            const std::string pluginFileName = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            files.push_back(MWTP::DepFile(pluginFileName));
            std::cout << "Plugin file \"" << pluginFileName << "\" was added.\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // try to read from Morrowind.ini?
        else if ((param == "-i") || (param == "--ini"))
        {
          doIni = true;
          discardBeforeIni = false;
        }
        else if ((param == "-id") || (param == "--ini-with-discard"))
        {
          doIni = true;
          discardBeforeIni = true;
        }
        else
        {
          // unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return MWTP::rcInvalidParameter;
        }
      }
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i;
    }
  }

  // now do the Morrowind.ini stuff
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
    if (iniReturnCode != 0)
    {
      return iniReturnCode;
    }
  }

  // check file list
  if (files.isEmpty())
  {
    files.push_back(MWTP::DepFile("Morrowind.esm"));
    if (FileExists(baseDir + "Tribunal.esm"))
    {
      files.push_back(MWTP::DepFile("Tribunal.esm"));
    }
    if (FileExists(baseDir + "Bloodmoon.esm"))
    {
      files.push_back(MWTP::DepFile("Bloodmoon.esm"));
    }
  }
  else
  { //There are already some files in the list, but are the master files in there, too?
    //Let's add master files, if necessary.
    if (!files.hasDepFile("Bloodmoon.esm") && FileExists(baseDir + "Bloodmoon.esm"))
    {
      files.push_front(MWTP::DepFile("Bloodmoon.esm"));
    }
    if (!files.hasDepFile("Tribunal.esm") && FileExists(baseDir + "Tribunal.esm"))
    {
      files.push_front(MWTP::DepFile("Tribunal.esm"));
    }
    if (!files.hasDepFile("Morrowind.esm"))
    {
      files.push_front(MWTP::DepFile("Morrowind.esm"));
    }
  }

  // try to get file information
  for (unsigned int i = 0; i < files.getSize(); ++i)
  {
    getFileSizeAndModificationTime(baseDir + files.at(i).name, files.at(i).size,
                                   files.at(i).modified);
  }

  // sort files according to Morrowind's load order
  files.sort();
  // remove duplicate entries in list
  const unsigned int duplicates = files.removeDuplicates();
  if (duplicates != 0)
  {
    if (duplicates == 1)
    {
      std::cout << "One duplicate file has been removed from the list.\n";
    }
    else
    {
      std::cout << duplicates << " duplicate files have been removed from the list.\n";
    }
  }

  std::cout << "List of active files:\n";
  files.writeDeps();

  // read all files
  MWTP::ESMReaderSpells reader;
  std::cout << "Reading files, this may take a while.\n";
  MWTP::DepFileList removedFiles;
  size_t i = 0;
  while (i < files.getSize())
  {
    MWTP::TES3Record DummyHead; // It's not actually used after the read
                                // function, but readESM() needs one as parameter.
    const int read_result = reader.readESM(baseDir + files.at(i).name, DummyHead);
    if (read_result <= -1)
    {
      std::cerr << "Error while reading file \"" << baseDir + files.at(i).name
                << "\".\nAborting.\n";
      return MWTP::rcFileError;
    }
    else if (read_result == 0)
    {
      // file contains no relevant data, so push it onto the list of unnecessary
      // files and remove it
      removedFiles.push_back(files.at(i));
      files.removeEntry(i);
      // We do not increase counter (i) here, because the index will now contain
      // the next file, which will be read in the next iteration.
    }
    else
    {
      // something was read and file was not removed from list, so increase counter
      ++i;
    }
  }

  // check for removed files
  if (removedFiles.getSize() > 0)
  {
    if (removedFiles.getSize() == 1)
    {
      std::cout << "Info: The following file was removed from the list, "
                << "because it didn't contain any spell data:\n";
    }
    else
    {
      // two or more files
      std::cout << "Info: The following " << removedFiles.getSize() << " files were"
                << " removed from the list, because they didn't contain any "
                << "spell data:\n";
    }
    removedFiles.writeDeps();
    std::cout << std::endl;
  }

  std::cout << "Info: " << files.getSize() << " Master/ Plugin file(s) containing "
            << MWTP::Spells::get().getNumberOfRecords() << " spell(s) were read.\n";

  std::vector<std::string> prefixes;
  prefixes.clear();
  // spell school settings present?
  MWTP::GameSettings& GMSTs = MWTP::GameSettings::get();
  MWTP::GMSTRecord record;

  // alteration -> 0
  if (GMSTs.hasSetting("sSchoolAlteration"))
  {
    record = GMSTs.getSetting("sSchoolAlteration");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolAlteration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for alteration found.\n";
    return MWTP::rcDataError;
  }

  // conjuration -> 1
  if (GMSTs.hasSetting("sSchoolConjuration"))
  {
    record = GMSTs.getSetting("sSchoolConjuration");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolConjuration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for conjuration found.\n";
    return MWTP::rcDataError;
  }

  // destruction -> 2
  if (GMSTs.hasSetting("sSchoolDestruction"))
  {
    record = GMSTs.getSetting("sSchoolDestruction");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolDestruction\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for destruction found.\n";
    return MWTP::rcDataError;
  }

  // illusion -> 3
  if (GMSTs.hasSetting("sSchoolIllusion"))
  {
    record = GMSTs.getSetting("sSchoolIllusion");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolIllusion\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for illusion found.\n";
    return MWTP::rcDataError;
  }

  // mysticism -> 4
  if (GMSTs.hasSetting("sSchoolMysticism"))
  {
    record = GMSTs.getSetting("sSchoolMysticism");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolMysticism\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for mysticism found.\n";
    return MWTP::rcDataError;
  }

  //restoration -> 5
  if (GMSTs.hasSetting("sSchoolRestoration"))
  {
    record = GMSTs.getSetting("sSchoolRestoration");
    if (record.Type != MWTP::GMSTType::String)
    {
      std::cerr << "Error: GMST \"sSchoolRestoration\" does not seem to be a string.\n";
      return MWTP::rcDataError;
    }
    prefixes.push_back(record.sVal.substr(0,1));
  }
  else
  {
    std::cerr << "Error: No GMST for restoration found.\n";
    return MWTP::rcDataError;
  }

  // delete all non-spells (abilities, powers,...) and update spells with naming scheme
  MWTP::Spells::ListIterator spell_end = MWTP::Spells::get().end();
  MWTP::Spells::ListIterator spell_cur = MWTP::Spells::get().begin();
  while (spell_cur != spell_end)
  {
    // Is it a spell?
    if (spell_cur->second.Type != MWTP::SpellType::Spell)
    {
      const std::string temp = spell_cur->first;
      ++spell_cur;
      // delete spell so we don't save it into the final plugin
      MWTP::Spells::get().removeRecord(temp);
    }
    else
    {
      bool unchanged = false;
      MWTP::SpellRecord rec = spell_cur->second;
      if (!rec.Enchs.empty())
      {
        if (MWTP::MagicEffects::get().hasEffect(rec.Enchs.at(0).EffectID))
        {
          const int school = MWTP::MagicEffects::get().getEffect(rec.Enchs.at(0).EffectID).SpellSchool;
          if ((school < 0) || (school >= prefixes.size()))
          {
            std::cerr << "Error: Effect " << rec.Enchs.at(0).EffectID
                      << " has an invalid spell school index of " << school
                      << ".\n";
            return MWTP::rcDataError;
          }
          const std::string newName = prefixes[school] + " " + rec.Name;
          if (allowTruncatedName || (newName.length() <= MWTP::SpellRecord::cMaximumSpellNameLength))
          {
            rec.Name = newName;
            MWTP::Spells::get().addRecord(rec);
          }
          else
          {
            /*If it is not changed, then we don't need to include it in the
              plugin file and can delete it right here. */
            // implement deletion
            const std::string delete_ID = spell_cur->first;
            ++spell_cur;
            // delete spell so we don't save it into the final plugin
            MWTP::Spells::get().removeRecord(delete_ID);
            // set unchanged flag to avoid iterator increase at the end
            unchanged = true;
          }
        }
        else
        {
          std::cerr << "Error: effect with ID " << rec.Enchs.at(0).EffectID
                    << " does not exist.\n";
          return MWTP::rcDataError;
        }
      }
      else
      {
        std::cerr << "Error: spell \"" << spell_cur->first
                  << "\" does not have any enchantment data.\n";
        return MWTP::rcDataError;
      }
      if (!unchanged)
      {
        ++spell_cur;
      }
    }
  }

  std::cout << "Info: " << MWTP::Spells::get().getNumberOfRecords() << " spell(s) processed.\n";

  // write spells to file
  if (MWTP::Spells::get().getNumberOfRecords() == 0)
  {
    std::cout << "No spells available. No new file will be created.\n";
    return MWTP::rcNoSpells;
  }

  MWTP::ESMWriterSpells writer;
  MWTP::TES3Record headerData;
  MWTP::ESMWriter::setOldStyleHeaderData(headerData, false, files,
      "Umbenannte/ umsortierte Zauber fuer Morrowind (generiert durch spell_rename.exe)");
  if (writer.writeESM(baseDir + outputFileName, false, headerData))
  {
    std::cout << "Output file \"" << baseDir + outputFileName
              << "\" was created successfully.\n";
    // now check file time of created file
    MWTP::DepFile outDep(outputFileName);
    if (getFileSizeAndModificationTime(baseDir + outputFileName, outDep.size, outDep.modified))
    {
      // only change, if last file isn't master, because plugins will be loaded
      // after master anyway. And output is plugin.
      if (!files.at(files.getSize() - 1).isMasterFile())
      {
        // Do we need to change plugin's modification time?
        if (files.at(files.getSize() - 1).modified >= outDep.modified)
        {
          if (setFileModificationTime(baseDir + outputFileName, files.at(files.getSize() - 1).modified + 60))
          {
            std::cout << "Hint: Could not change modification time of \""
                      << outputFileName << "\" to ensure loading order.\n";
            return 0; // return zero anyway, file was created
          }
          else
          {
            std::cout << "Hint: Modification time of \"" << outputFileName
                      << "\" was changed to ensure loading order.\n";
            return 0; // success
          }
        }
      } // if not master
    }
    return 0;
  }
  std::cerr << "Error: Could not create or write to output file \""
            << baseDir + outputFileName << "\".\n";
  return MWTP::rcOutputFailed;
}
