/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#include "FileFunctions.h"
#include <sys/stat.h>
#include <utime.h>
#include <unistd.h>
#include <cmath>
#include "UtilityFunctions.h"
#include <iostream>
#include <cstdio>
#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
#elif defined(__linux__) || defined(linux)
  //Linux directory entries
  #include <dirent.h>
#else
  #error "Unknown operating system!"
#endif

int64_t getFileSize64(const std::string& fileName)
{
  struct stat buffer;
  if (stat(fileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    return buffer.st_size;
  }//if
  //An error occured, so we don't have a proper value for file size.
  // Return -1 in this case to indicate an error.
  return -1;
}//function

bool setFileModificationTime(const std::string& FileName, const time_t new_mtime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    if (buffer.st_mtime==new_mtime) return true;
    //change time
    struct utimbuf fileTimes;
    fileTimes.actime = buffer.st_atime;
    fileTimes.modtime = new_mtime;
    return (utime(FileName.c_str(), &fileTimes)==0);
  }//if
  //An error occured, so return false.
  return false;
}

bool getFileSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    FileSize = buffer.st_size;
    FileTime = buffer.st_mtime;
    return true;
  }//if
  //An error occured, so we don't have any proper values for that file.
  // Set values to -1 and return false in this case to indicate an error.
  FileSize = -1;
  FileTime = -1;
  return false;
}//function


float round(const float f)
{
  return floor(f + 0.5);
}

std::string getSizeString(const int64_t fileSize)
{
  if (fileSize<0) return "-"+getSizeString(-fileSize);

  //giga
  if (fileSize>1024*1024*1024)
  {
    return floatToString(round((fileSize*100.0f)/(1024.0f*1024*1024))/100.0f)+" GB";
  }
  if (fileSize>1024*1024)
  {
    return floatToString(round((fileSize*100.0f)/(1024.0f*1024))/100.0f)+" MB";
  }
  if (fileSize>1024)
  {
    return floatToString(round((fileSize*100.0f)/1024.0f)/100.0f)+" KB";
  }
  return floatToString(fileSize)+" byte";
}

bool FileExists(const std::string& FileName)
{
  return (access(FileName.c_str(), F_OK)==0);
}

bool deleteFile(const std::string& fileName)
{
  return (remove(fileName.c_str())==0);
}

FileEntry::FileEntry()
: fileName(""), isDirectory(false)
{ }

std::vector<FileEntry> getDirectoryFileList(const std::string& Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(Directory+"*").c_str(),&sr);
  if (handle == -1)
  {
    std::cout << "getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }
  //search it
  while( _findnext(handle, &sr)==0)
  {
    one.fileName = std::string(sr.name);
    one.isDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
  }//while
  _findclose(handle);
  #elif defined(__linux__) || defined(linux)
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.fileName = std::string(entry->d_name);
    one.isDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
    }
    entry = readdir(direc);
  }//while
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}//function

void splitPathFileExtension(const std::string fileName, const char pathSeperator, std::string& path, std::string& name, std::string& extension)
{
  const std::string::size_type len = fileName.length();
  if (len==0)
  {
    path = "";
    name = "";
    extension = "";
    return;
  }

  //split path from file and ext.
  const std::string::size_type sepPos = fileName.rfind(pathSeperator);
  if (sepPos==std::string::npos)
  {
    path = "";
    name = fileName;
  }
  else
  {
    path = fileName.substr(0, sepPos+1);
    name = fileName.substr(sepPos+1);
  }
  // => now path has the path (including seperator), and name has the file including extension

  //split extension from name
  const std::string::size_type dotPos = name.rfind('.');
  if (dotPos==std::string::npos)
  {
    extension = "";
    return;
  }
  if (dotPos+1<name.length())
  {
    extension = name.substr(dotPos+1);
  }
  else
  {
    extension = "";
  }
  name = name.substr(0, dotPos);
  return;
}
