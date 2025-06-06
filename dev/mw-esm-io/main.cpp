/*
 -------------------------------------------------------------------------------
    This file is part of the development tools for the Morrowind Tools Project.
    Copyright (C) 2025  Dirk Stolle

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

#include <filesystem>
#include <iostream>
#include "../../lib/mw/ReturnCodes.hpp"
#include "../../lib/mw/ESMReaderContents.hpp"
#include "../../lib/mw/ESMWriterContents.hpp"

void showUsage()
{
  std::cout << "Usage:\n"
            << "  mw-esm-io path/to/file.esm\n";
}

std::filesystem::path getDestinationPath(const std::string& sourceFileName)
{
  namespace fs = std::filesystem;

  const fs::path path{sourceFileName};
  const auto ext = path.extension();
  const auto stem = path.stem();

  fs::path out_path{fs::temp_directory_path() / path.filename()};
  std::error_code error;
  uint_least32_t counter = 0;
  while (fs::exists(out_path, error) && !error)
  {
    ++counter;
    auto replacement = stem;
    replacement += std::string("_") + std::to_string(counter);
    replacement += ext;
    out_path.replace_filename(replacement);
  }

  return out_path;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    showUsage();
    return MWTP::rcInvalidParameter;
  }
  const std::string esmFileName(argv[1]);
  if (esmFileName == "--help" || esmFileName == "-?")
  {
    showUsage();
    return 0;
  }

  MWTP::ESMReaderContents::VectorType vector;
  MWTP::ESMReaderContents reader(vector);

  MWTP::TES3Record header;
  const int records_read = reader.readESM(esmFileName, header);
  if (records_read < 0)
  {
    std::cerr << "Could not read ESM file " << esmFileName << ".\n";
    return MWTP::rcFileError;
  }

  std::cout << "Info: Read " << records_read << " records from " << esmFileName
            << ".\n";

  MWTP::ESMWriterContents writer(vector);
  std::string outputFileName = getDestinationPath(esmFileName).string();
  const bool isESM = (esmFileName.size() >= 4)
                  && (esmFileName.rfind(".esm") == esmFileName.size() - 4);
  if (!writer.writeESM(outputFileName, isESM, header))
  {
    std::cerr << "Could not write ESM file to " << outputFileName << "!\n";
    return MWTP::rcFileError;
  }

  std::cout << "Data was successfully written to " << outputFileName << ".\n";
  return 0;
}
