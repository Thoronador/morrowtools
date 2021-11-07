/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021  Thoronador

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

#include "FileFunctions.hpp"
#include <filesystem>
#include <sys/stat.h> // stat()
#include <cmath>   // std::round()
#include "UtilityFunctions.hpp" // floatToString()

int64_t getFileSize64(const std::string& fileName)
{
  std::error_code error;
  const auto size = std::filesystem::file_size(fileName, error);
  if (error)
  {
    // An error occurred, so we don't have a proper value for file size.
    // Return -1 in this case to indicate an error.
    return -1;
  }
  // file_size() was successful.
  return static_cast<int64_t>(size);
}

template <typename FromTimeT, typename ToTimeT>
ToTimeT clock_cast_simple(const FromTimeT tp)
{
  const auto from_now = FromTimeT::clock::now();
  const auto to_now = ToTimeT::clock::now();
  // This is not an exact conversion, i. e. it might be off by a few 100 nanoseconds.
  // It depends on how much time is spent between the two calls to now().
  // However, on some file systems the resolution for file time is one second,
  // and even NTFS has just 10 ms resolution, so the error is at least one order
  // of magnitude above the resolution the file system can handle.
  return to_now + (tp - from_now);
}

std::filesystem::file_time_type time_t_to_file_time(const time_t _time)
{
  using namespace std::chrono;
  const system_clock::time_point sys = system_clock::from_time_t(_time);
  return clock_cast_simple<system_clock::time_point, std::filesystem::file_time_type>(sys);
}

bool setFileModificationTime(const std::string& FileName, const time_t new_mtime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer) == 0)
  {
    // stat() was successful.
    if (buffer.st_mtime == new_mtime)
      return true;
    // change time
    namespace fs = std::filesystem;
    const fs::path path(FileName);
    // The codebase is still C++17 and not C++20, so some of the more advanced
    // features to convert file system time are not available yet.
    std::error_code error;
    const auto new_file_time = time_t_to_file_time(new_mtime);
    fs::last_write_time(path, new_file_time, error);
    return !error;
  }
  // An error occurred, so return false.
  return false;
}

bool getFileSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime)
{
  struct stat buffer;
  if (stat(FileName.c_str(), &buffer) == 0)
  {
    // stat() was successful
    FileSize = buffer.st_size;
    FileTime = buffer.st_mtime;
    return true;
  }
  // An error occurred, so we don't have any proper values for that file.
  // Set values to -1 and return false in this case to indicate an error.
  FileSize = -1;
  FileTime = -1;
  return false;
}


std::string getSizeString(const int64_t fileSize)
{
  if (fileSize < 0)
    return "-" + getSizeString(-fileSize);

  // giga
  if (fileSize > 1024 * 1024 * 1024)
  {
    return floatToString(std::round((fileSize * 100.0f) / (1024.0f * 1024 * 1024)) / 100.0f) + " GB";
  }
  // mega
  if (fileSize > 1024 * 1024)
  {
    return floatToString(std::round((fileSize * 100.0f) / (1024.0f * 1024)) / 100.0f) + " MB";
  }
  // kilo
  if (fileSize > 1024)
  {
    return floatToString(std::round((fileSize * 100.0f) / 1024.0f) / 100.0f) + " KB";
  }
  return floatToString(fileSize) + " byte";
}

bool FileExists(const std::string& FileName)
{
  std::error_code error;
  const auto status = std::filesystem::status(FileName, error);
  return !error && std::filesystem::exists(status);
}

bool deleteFile(const std::string& fileName)
{
  std::error_code error;
  return std::filesystem::remove(fileName, error) && !error;
}

FileEntry::FileEntry()
: fileName(""), isDirectory(false)
{ }

std::optional<std::vector<FileEntry> > getDirectoryFileList(const std::string& Directory)
{
  namespace fs = std::filesystem;

  std::error_code error;
  fs::directory_iterator iter(Directory, error);
  if (error)
  {
    // Returning empty optional to indicate error.
    return std::nullopt;
  }

  std::vector<FileEntry> result;
  FileEntry one;
  for (const auto& entry: iter)
  {
    one.fileName = entry.path().filename().string();
    const auto status = fs::status(entry.path());
    one.isDirectory = fs::is_directory(status);
    // Check for socket, pipe, block device and char device, which we don't want.
    const auto type = status.type();
    if (type != fs::file_type::socket && type != fs::file_type::fifo && type != fs::file_type::block
        && type != fs::file_type::character)
    {
      result.push_back(one);
    }
  }
  return result;
}

void splitPathFileExtension(const std::string fileName, const char pathSeperator, std::string& path, std::string& name, std::string& extension)
{
  const std::string::size_type len = fileName.length();
  if (len == 0)
  {
    path = "";
    name = "";
    extension = "";
    return;
  }

  // split path from file and ext.
  const std::string::size_type sepPos = fileName.rfind(pathSeperator);
  if (sepPos == std::string::npos)
  {
    path = "";
    name = fileName;
  }
  else
  {
    path = fileName.substr(0, sepPos + 1);
    name = fileName.substr(sepPos + 1);
  }
  // => now path has the path (including separator), and name has the file including extension

  // split extension from name
  const std::string::size_type dotPos = name.rfind('.');
  if (dotPos == std::string::npos)
  {
    extension = "";
    return;
  }
  if (dotPos + 1 < name.length())
  {
    extension = name.substr(dotPos + 1);
  }
  else
  {
    extension = "";
  }
  name = name.substr(0, dotPos);
  return;
}
