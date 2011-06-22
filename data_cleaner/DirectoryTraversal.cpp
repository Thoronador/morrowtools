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

#include "DirectoryTraversal.h"
#include <iostream>
#include <vector>
//includes for directory listing
#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
#elif defined(__linux__) || defined(linux)
  //Linux directory entries
  #include <dirent.h>
#else
  #error "Unknown operating system!"
#endif
#include "..\base\UtilityFunctions.h"

namespace MWTP
{

/* structure for file list entries */
struct FileEntry {
         std::string FileName;
         bool IsDirectory;
};//struct

std::vector<FileEntry> getFilesInDirectory(const std::string& directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(directory+"*").c_str(),&sr);
  if (handle == -1)
  {
    std::cout << "getFilesInDirectory: ERROR: unable to open directory \""
              <<directory<<"\". Returning empty list.\n";
    return result;
  }
  //search it
  while( _findnext(handle, &sr)==0)
  {
    one.FileName = std::string(sr.name);
    one.IsDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
  }//while
  _findclose(handle);
  #elif defined(__linux__) || defined(linux)
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "getFilesInDirectory: ERROR: unable to open directory \""
              <<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.FileName = std::string(entry->d_name);
    one.IsDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
      entry = readdir(direc);
    }
  }//while
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}//function getFilesInDirectory

void getAllDataFiles(const std::string& dir, DepFileList& files)
{
  std::vector<FileEntry> dirFiles = getFilesInDirectory(dir);
  unsigned int i;
  const unsigned int len = dirFiles.size();
  for (i=0; i<len; ++i)
  {
    if (!dirFiles[i].IsDirectory)
    {
      const unsigned int nameLen = dirFiles[i].FileName.length();
      if (nameLen>4)
      {
        //check for .esm/.esp file
        if ((lowerCase(dirFiles[i].FileName.substr(nameLen-4, 4))==".esp")
            or (lowerCase(dirFiles[i].FileName.substr(nameLen-4, 4))==".esm"))
        {
          files.push_back(dirFiles[i].FileName);
        }
      }
    }
  }//for
}

void getDeletableMeshes(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables)
{
  std::vector<FileEntry> files = getFilesInDirectory(dir);
  unsigned int i;
  const unsigned int len = files.size();
  for (i=0; i<len; ++i)
  {
    if (files[i].IsDirectory)
    {
      //It's a directory, so check that one, too.
      if ((files[i].FileName!="..") and (files[i].FileName!="."))
      {
        //go down in directory hierarchy
        getDeletableMeshes(dir+files[i].FileName+"\\", positives, deletables);
      }
    }
    else
    {
      //It's a file. Is it in the list of required files?
      if (positives.find(dir+files[i].FileName)==positives.end())
      {
        //It's not in the list, so add it to the list of deletable files.
        // ...if it's a .nif file!
        const unsigned int nameLen = files[i].FileName.length();
        if (nameLen>4)
        {
          if (lowerCase(files[i].FileName.substr(nameLen-4, 4))==".nif")
          {
            deletables.insert(dir+files[i].FileName);
          }
        }//name longer than four characters
      }
    }
  }//for
}//function getDeletableMeshes

void getDeletableIcons(const std::string& dir, const std::set<std::string, ci_less>& positives, std::set<std::string>& deletables)
{
  std::vector<FileEntry> files = getFilesInDirectory(dir);
  unsigned int i;
  const unsigned int len = files.size();
  for (i=0; i<len; ++i)
  {
    if (files[i].IsDirectory)
    {
      //It's a directory, so check that one, too.
      if ((files[i].FileName!="..") and (files[i].FileName!="."))
      {
        //go down in directory hierarchy
        getDeletableIcons(dir+files[i].FileName+"\\", positives, deletables);
      }
    }
    else
    {
      //It's a file. Is it in the list of required files?
      if (positives.find(dir+files[i].FileName)==positives.end())
      {
        //It's not in the list, so add it to the list of deletable files.
        const unsigned int nameLen = files[i].FileName.length();
        if (nameLen>4)
        {
          const std::string ext = lowerCase(files[i].FileName.substr(nameLen-4, 4));
          if (ext==".dds")
          {
            //Maybe there is one as .tga instead?
            if (positives.find(dir+files[i].FileName.substr(0,nameLen-4)+".tga")==positives.end())
            {
              deletables.insert(dir+files[i].FileName);
            }
          }
          else if (ext==".tga")
          {
            //Maybe it's a .dds instead?
            if (positives.find(dir+files[i].FileName.substr(0,nameLen-4)+".dds")==positives.end())
            {
              deletables.insert(dir+files[i].FileName);
            }
          }
        }//if name is longer than four characters
      }
    }
  }//for
}//function getDeletableIcons

} //namespace
