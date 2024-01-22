/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2022, 2024  Dirk Stolle

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

#ifndef MWTP_BASE_FILEGUARD_HPP
#define MWTP_BASE_FILEGUARD_HPP

#include <filesystem>

namespace MWTP
{

/// guard to ensure file deletion when it goes out of scope
class FileGuard
{
  private:
    std::filesystem::path path;
  public:
      /** \brief Creates a new file guard.
       *
       * \param filePath   path of the file to delete when the guard goes out of scope
       */
    FileGuard(const std::filesystem::path& filePath)
    : path(filePath)
    { }

    FileGuard(const FileGuard& op) = delete;
    FileGuard(FileGuard&& op) = delete;

    ~FileGuard()
    {
      std::error_code error;
      (void) std::filesystem::remove(path, error);
    }
};

} // namespace

#endif // MWTP_BASE_FILEGUARD_HPP
