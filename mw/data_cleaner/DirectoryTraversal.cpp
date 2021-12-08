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

#include "DirectoryTraversal.hpp"
#include <iostream>
#include <vector>
// includes for directory listing
#if defined(_WIN32)
  #include <io.h>
#elif defined(__linux__) || defined(linux)
  #include <dirent.h>
#else
  #error "Unknown operating system!"
#endif
#include "../../base/UtilityFunctions.hpp"

namespace MWTP
{

/* structure for file list entries */
struct FileEntry {
         std::string FileName;
         bool IsDirectory;
}; // struct

std::vector<FileEntry> getFilesInDirectory(const std::string& directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  // Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(directory + "*").c_str(), &sr);
  if (handle == -1)
  {
    std::cout << "getFilesInDirectory: ERROR: unable to open directory \""
              << directory << "\". Returning empty list.\n";
    return result;
  }
  // search it
  while( _findnext(handle, &sr)==0)
  {
    one.FileName = std::string(sr.name);
    one.IsDirectory = ((sr.attrib & _A_SUBDIR) == _A_SUBDIR);
    result.push_back(one);
  }
  _findclose(handle);
  #elif defined(__linux__) || defined(linux)
  // Linux part
  DIR * direc = opendir(directory.c_str());
  if (direc == NULL)
  {
    std::cout << "getFilesInDirectory: ERROR: unable to open directory \""
              << directory << "\". Returning empty list.\n";
    return result;
  }

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.FileName = std::string(entry->d_name);
    one.IsDirectory = entry->d_type==DT_DIR;
    // check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
    }
    entry = readdir(direc);
  }
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}

void getAllDataFiles(const std::string& dir, DepFileList& files)
{
  const std::vector<FileEntry> dirFiles = getFilesInDirectory(dir);
  for (const auto& entry: dirFiles)
  {
    if (!entry.IsDirectory)
    {
      const auto nameLen = entry.FileName.length();
      if (nameLen > 4)
      {
        // check for .esm/.esp file
        const auto lowerCaseExtension = lowerCase(entry.FileName.substr(nameLen - 4, 4));
        if ((lowerCaseExtension == ".esp") || (lowerCaseExtension == ".esm"))
        {
          files.push_back(DepFile(entry.FileName));
        }
      }
    }
  }
}

void getDeletableMeshes(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables)
{
  const std::vector<FileEntry> files = getFilesInDirectory(dir);
  for (const auto& entry: files)
  {
    if (entry.IsDirectory)
    {
      // It's a directory, so check that one, too.
      if ((entry.FileName != "..") && (entry.FileName != "."))
      {
        // go down in directory hierarchy
        getDeletableMeshes(dir + entry.FileName + "\\", positives, deletables);
      }
    }
    else
    {
      // It's a file. Is it in the list of required files?
      if (positives.find(dir + entry.FileName) == positives.end())
      {
        // It's not in the list, so add it to the list of deletable files.
        // ...if it's a .nif file!
        const auto nameLen = entry.FileName.length();
        if (nameLen > 4)
        {
          if (lowerCase(entry.FileName.substr(nameLen -4, 4)) == ".nif")
          {
            deletables.insert(dir + entry.FileName);
          }
        }
      }
    }
  }
}

void getDeletableIcons(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables)
{
  const std::vector<FileEntry> files = getFilesInDirectory(dir);
  for (const auto& entry: files)
  {
    if (entry.IsDirectory)
    {
      // It's a directory, so check that one, too.
      if ((entry.FileName != "..") && (entry.FileName != "."))
      {
        //go down in directory hierarchy
        getDeletableIcons(dir + entry.FileName + "\\", positives, deletables);
      }
    }
    else
    {
      // It's a file. Is it in the list of required files?
      if (positives.find(dir + entry.FileName) == positives.end())
      {
        // It's not in the list, so add it to the list of deletable files.
        const unsigned int nameLen = entry.FileName.length();
        if (nameLen > 4)
        {
          const auto ext = lowerCase(entry.FileName.substr(nameLen - 4, 4));
          if (ext == ".dds")
          {
            // Maybe there is one as .tga instead?
            if (positives.find(dir + entry.FileName.substr(0, nameLen - 4) + ".tga") == positives.end())
            {
              deletables.insert(dir + entry.FileName);
            }
          }
          else if (ext == ".tga")
          {
            // Maybe it's a .dds instead?
            if (positives.find(dir + entry.FileName.substr(0, nameLen - 4) + ".dds") == positives.end())
            {
              deletables.insert(dir + entry.FileName);
            }
          }
        } // if name is longer than four characters
      }
    }
  }
}

} // namespace
