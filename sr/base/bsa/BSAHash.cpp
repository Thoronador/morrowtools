/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#include "BSAHash.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace SRTP
{

BSAHash calculateHash(const std::filesystem::path& path)
{
  std::string ext = path.extension().string();
  std::string stem = path.stem().string();

  auto lower_case = [](unsigned char c)
  {
    return std::tolower(c);
  };
  std::transform(stem.cbegin(), stem.cend(), stem.begin(), lower_case);
  std::transform(ext.cbegin(), ext.cend(), ext.begin(), lower_case);

  std::vector<std::uint8_t> ordinal_values;
  std::transform(stem.cbegin(), stem.cend(), std::back_inserter(ordinal_values),
                 [](unsigned char c)
                 {
                   return c;
                 });

  const auto ord_size = ordinal_values.size();
  const std::uint8_t c1 = ord_size >= 1 ? ordinal_values[ord_size - 1] : 0;
  const std::uint8_t c2 = ord_size >= 2 ? ordinal_values[ord_size - 2] : 0;
  const std::uint8_t c0 = ord_size > 0 ? ordinal_values[0] : 0;
  std::uint32_t first_hash = static_cast<std::uint32_t>(c1)
                           | (static_cast<std::uint32_t>(c2) << 8)
                           | (static_cast<std::uint32_t>(ord_size) << 16)
                           | (static_cast<std::uint32_t>(c0) << 24);
  if (ext == ".kf")
    first_hash |= static_cast<std::uint32_t>(0x80);
  else if (ext == ".nif")
    first_hash |= static_cast<std::uint32_t>(0x8000);
  else if (ext == ".dds")
    first_hash |= static_cast<std::uint32_t>(0x8080);
  else if (ext == ".wav")
    first_hash |= static_cast<std::uint32_t>(0x80000000UL);

  std::uint32_t second_hash = 0;
  for (unsigned int i = 1; i < ord_size - 2; ++i)
  {
    second_hash = (second_hash * 0x1003f) + ordinal_values[i];
  }

  ordinal_values.clear();
  std::transform(ext.cbegin(), ext.cend(), std::back_inserter(ordinal_values),
                 [](unsigned char c)
                 {
                   return c;
                 });
  std::uint32_t third_hash = 0;
  for (unsigned int i = 0; i < ordinal_values.size(); ++i)
  {
    third_hash = (third_hash * 0x1003f) + ordinal_values[i];
  }
  second_hash += third_hash;
  return (static_cast<BSAHash>(second_hash) << 32) + first_hash;
}

BSAHash calculateDirectoryHash(std::string directoryName)
{
  // Replace slashes by backslashes. That's what BSA archives use and expect.
  for (auto pos = directoryName.find('/'); pos != std::string::npos; pos = directoryName.find('/', pos))
  {
     directoryName.replace(pos, 1, 1, '\\');
  }
  // Use normal hash for the rest.
  return calculateHash(directoryName);
}

}
