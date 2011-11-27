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

#include "FileFunctions.h"
#include <sys/stat.h>
#include <utime.h>
#include <unistd.h>
#include <cmath>
#include "UtilityFunctions.h"

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

/*
time_t getFileModificationTime(const std::string& FileName)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    return buffer.st_mtime;
  }//if
  //An error occured, so we don't have a proper value for time.
  // Return -1 in this case to indicate an error.
  return -1;
}//function
*/

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
