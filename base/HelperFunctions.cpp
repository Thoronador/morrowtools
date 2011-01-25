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

#include "HelperFunctions.h"
#include <sys/stat.h>
#include <unistd.h>

int64_t getFileSize64(const std::string& FileName)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer)==0)
  {
    //stat() was successful
    return buffer.st_size;
  }//if
  //An error occured, so we don't have a proper value for file size.
  // Return -1 in this case to indicate an error.
  return -1;
}//function

bool FileExists(const std::string& FileName)
{
  return (access(FileName.c_str(), F_OK)==0);
}
