/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

#include "../../../locate_catch.hpp"
#include <filesystem>
#include "../../../../../sr/base/bsa/BSA.hpp"

void writeBsaHeaderForTest(const std::string& fileName)
{
  using namespace std::string_view_literals;
  const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;

  // write BSA header to file
  std::ofstream file(fileName, std::ios::out | std::ios::binary);
  file.write(data.data(), data.size());
  file.close();
}

bool writeBsa(const std::string_view content, const std::filesystem::path& path)
{
  std::ofstream stream(path, std::ios::out | std::ios::binary);
  if (!stream.good())
    return false;
  if (!stream.write(content.data(), content.size()).good())
    return false;
  stream.close();
  return stream.good();
}

// guard to ensure file deletion when it goes out of scope
class FileGuard
{
  private:
    std::filesystem::path path;
  public:
    FileGuard(const std::filesystem::path& filePath)
    : path(filePath)
    { }

    FileGuard(const FileGuard& op) = delete;
    FileGuard(FileGuard&& op) = delete;

    ~FileGuard()
    {
      std::filesystem::remove(path);
    }
};


TEST_CASE("BSA")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSA bsa;

    REQUIRE_FALSE( bsa.hasAllStructureData() );
    REQUIRE( bsa.getDirectories().empty() );
    REQUIRE( bsa.getDirectoryBlocks().empty() );
  }

  SECTION("open")
  {
    using namespace std::string_view_literals;

    SECTION("file does not exist")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.open("/does-not/exist.bsa") );
    }

    SECTION("load header-only BSA")
    {
      const auto fileName = "test_sr_bsa_open_header-only.bsa";
      writeBsaHeaderForTest(fileName);

      BSA bsa;
      REQUIRE( bsa.open(fileName) );
      bsa.close();

      REQUIRE( std::filesystem::remove(fileName) );
    }

    SECTION("try to re-open header-only BSA")
    {
      const auto fileName = "test_sr_bsa_open_-reopen-header-only.bsa";
      writeBsaHeaderForTest(fileName);

      BSA bsa;
      REQUIRE( bsa.open(fileName) );
      // Re-open attempt shall fail.
      REQUIRE_FALSE( bsa.open(fileName) );
      bsa.close();

      REQUIRE( std::filesystem::remove(fileName) );
    }
  }

  SECTION("grabAllStructureData")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabAllStructureData() );
    }

    SECTION("failure: directory data is truncated")
    {
      const std::filesystem::path path{"test_sr_bsa_grabAllStructureData_directory_data_truncated.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabAllStructureData() );
    }

    SECTION("failure: directory blocks are truncated")
    {
      const std::filesystem::path path{"test_sr_bsa_grabAllStructureData_directory_blocks_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabAllStructureData() );
    }

    SECTION("failure: file names are truncated")
    {
      const std::filesystem::path path{"test_sr_bsa_grabAllStructureData_file_names_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.tx"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabAllStructureData() );
    }

    SECTION("successful read of version 104 BSA")
    {
      const std::filesystem::path path{"test_sr_bsa_grabAllStructureData_v104_fine.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      REQUIRE( bsa.hasAllStructureData() );
    }

    SECTION("grabbing data twice does not do any harm")
    {
      const std::filesystem::path path{"test_sr_bsa_grabAllStructureData_twice.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );
      // Second attempt also succeeds, but it's a no-op.
      REQUIRE( bsa.grabAllStructureData() );
      // Data is still present.
      REQUIRE( bsa.hasAllStructureData() );
    }
  }

  SECTION("grabDirectoryData")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabDirectoryData() );
    }

    SECTION("failure: wrong internal status - data has already been read")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_wrong_status.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      // Grabbing data again fails.
      REQUIRE_FALSE( bsa.grabDirectoryData() );
    }

    SECTION("failure: directory data is truncated")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_truncated.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabDirectoryData() );
      REQUIRE( bsa.getDirectories().empty() );
    }

    SECTION("BSA version before 104 can be read, if rest matches v104 layout")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_v103_fine.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x67\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.getDirectories().size() == 2 );
      // Check first directory.
      REQUIRE( bsa.getDirectories().at(0).nameHash == 0x0bbe7240730a6e67ULL );
      REQUIRE( bsa.getDirectories().at(0).count == 1 );
      REQUIRE( bsa.getDirectories().at(0).offset == 93 );
      // Check second directory.
      REQUIRE( bsa.getDirectories().at(1).nameHash == 0x5a8492dc730e7365ULL );
      REQUIRE( bsa.getDirectories().at(1).count == 2 );
      REQUIRE( bsa.getDirectories().at(1).offset == 121 );

      REQUIRE_FALSE( bsa.hasAllStructureData() );
    }

    SECTION("successful read of version 104 BSA")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_v104_fine.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.getDirectories().size() == 2 );
      // Check first directory.
      REQUIRE( bsa.getDirectories().at(0).nameHash == 0x0bbe7240730a6e67ULL );
      REQUIRE( bsa.getDirectories().at(0).count == 1 );
      REQUIRE( bsa.getDirectories().at(0).offset == 93 );
      // Check second directory.
      REQUIRE( bsa.getDirectories().at(1).nameHash == 0x5a8492dc730e7365ULL );
      REQUIRE( bsa.getDirectories().at(1).count == 2 );
      REQUIRE( bsa.getDirectories().at(1).offset == 121 );

      REQUIRE_FALSE( bsa.hasAllStructureData() );
    }

    SECTION("successful read of version 105 BSA")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_v105_fine.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x69\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0\0\0\0\0]\0\0\0\0\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0\0\0\0\0y\0\0\0\0\0\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.getDirectories().size() == 2 );
      // Check first directory.
      REQUIRE( bsa.getDirectories().at(0).nameHash == 0x0bbe7240730a6e67ULL );
      REQUIRE( bsa.getDirectories().at(0).count == 1 );
      REQUIRE( bsa.getDirectories().at(0).offset == 93 );
      // Check second directory.
      REQUIRE( bsa.getDirectories().at(1).nameHash == 0x5a8492dc730e7365ULL );
      REQUIRE( bsa.getDirectories().at(1).count == 2 );
      REQUIRE( bsa.getDirectories().at(1).offset == 121 );

      REQUIRE_FALSE( bsa.hasAllStructureData() );
    }

    SECTION("BSA version newer than 105 can be read, if remaining layout matches v105")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryData_v106_fine.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x6A\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0\0\0\0\0]\0\0\0\0\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0\0\0\0\0y\0\0\0\0\0\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.getDirectories().size() == 2 );
      // Check first directory.
      REQUIRE( bsa.getDirectories().at(0).nameHash == 0x0bbe7240730a6e67ULL );
      REQUIRE( bsa.getDirectories().at(0).count == 1 );
      REQUIRE( bsa.getDirectories().at(0).offset == 93 );
      // Check second directory.
      REQUIRE( bsa.getDirectories().at(1).nameHash == 0x5a8492dc730e7365ULL );
      REQUIRE( bsa.getDirectories().at(1).count == 2 );
      REQUIRE( bsa.getDirectories().at(1).offset == 121 );

      REQUIRE_FALSE( bsa.hasAllStructureData() );
    }
  }

  SECTION("grabDirectoryBlocks")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabDirectoryBlocks() );
    }

    SECTION("failure: BSA is open, but directory data has not been read yet")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryBlocks_open.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabDirectoryBlocks() );
      REQUIRE( bsa.getDirectoryBlocks().empty() );
    }

    SECTION("successful read of version 104 BSA")
    {
      const std::filesystem::path path{"test_sr_bsa_grabDirectoryBlocks_v104_fine.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );
      REQUIRE( bsa.getDirectoryBlocks().size() == 2 );
      // Check first block.
      REQUIRE( bsa.getDirectoryBlocks().at(0).name == "some\\thing" );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.size() == 1 );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.at(0).nameHash == 0x95d0a72774047374ULL );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.at(0).fileSize == 16 );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.at(0).offset == 169 );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.at(0).fileName.empty() );
      // Check second block.
      REQUIRE( bsa.getDirectoryBlocks().at(1).name == "something\\else" );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.size() == 2 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(0).nameHash == 0x95d0a6c262036172ULL );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(0).fileSize == 7 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(0).offset == 185 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(0).fileName.empty() );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(1).nameHash == 0x95d0a6c266036f6fULL );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(1).fileSize == 14 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(1).offset == 192 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(1).fileName.empty() );

      REQUIRE_FALSE( bsa.hasAllStructureData() );
    }
  }

  SECTION("grabFileNames")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.grabFileNames() );
    }

    SECTION("failure: BSA is open, but previous data has not been read yet")
    {
      const std::filesystem::path path{"test_sr_bsa_grabFileNames_open.bsa"};
      FileGuard guard{path};
      const auto content = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0"sv;
      REQUIRE( writeBsa(content, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE_FALSE( bsa.grabFileNames() );
    }

    SECTION("failure: file block count mismatch")
    {
      const std::filesystem::path path{"test_sr_bsa_grabFileNames_file_block_mismatch.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar\0txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );
      REQUIRE_FALSE( bsa.grabFileNames() );
    }

    SECTION("failure: mismatch with file count in header")
    {
      const std::filesystem::path path{"test_sr_bsa_grabFileNames_file_count_mismatch.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x33\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );
      REQUIRE_FALSE( bsa.grabFileNames() );
    }

    SECTION("successful read of version 104 BSA")
    {
      const std::filesystem::path path{"test_sr_bsa_grabFileNames_v104_fine.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );
      REQUIRE( bsa.grabFileNames() );
      REQUIRE( bsa.getDirectoryBlocks().size() == 2 );
      // Check first block.
      REQUIRE( bsa.getDirectoryBlocks().at(0).name == "some\\thing" );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.size() == 1 );
      REQUIRE( bsa.getDirectoryBlocks().at(0).files.at(0).fileName == "test.txt" );
      // Check second block.
      REQUIRE( bsa.getDirectoryBlocks().at(1).name == "something\\else" );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.size() == 2 );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(0).fileName == "bar.txt" );
      REQUIRE( bsa.getDirectoryBlocks().at(1).files.at(1).fileName == "foo.txt" );

      REQUIRE( bsa.hasAllStructureData() );
    }
  }

  SECTION("getDirectoryNames")
  {
    using namespace std::string_view_literals;

    SECTION("BSA has not been opened / no structure data")
    {
      BSA bsa;
      REQUIRE( bsa.getDirectoryNames().empty() );
    }

    SECTION("successful even if file names have not been read yet")
    {
      const std::filesystem::path path{"test_sr_bsa_getDirectoryNames_no_file_names.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );

      const auto dirs = bsa.getDirectoryNames();
      REQUIRE( dirs.size() == 2 );
      REQUIRE( dirs[0] == "some\\thing" );
      REQUIRE( dirs[1] == "something\\else" );
    }

    SECTION("successful also if file names have been read")
    {
      const std::filesystem::path path{"test_sr_bsa_getDirectoryNames_with_file_names.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabDirectoryData() );
      REQUIRE( bsa.grabDirectoryBlocks() );
      REQUIRE( bsa.grabFileNames() );

      const auto dirs = bsa.getDirectoryNames();
      REQUIRE( dirs.size() == 2 );
      REQUIRE( dirs[0] == "some\\thing" );
      REQUIRE( dirs[1] == "something\\else" );
    }
  }

  SECTION("getIndexOfDirectory")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.getIndexOfDirectory("foo\\bar").has_value() );
    }

    SECTION("empty directory name")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexOfDirectory_empty_name.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );
      REQUIRE_FALSE( bsa.getIndexOfDirectory("").has_value() );
    }

    SECTION("directory not found")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexOfDirectory_not found.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );
      REQUIRE_FALSE( bsa.getIndexOfDirectory("does\\not\\exist").has_value() );
    }

    SECTION("directory exists")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexOfDirectory_match.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );
      auto index = bsa.getIndexOfDirectory("some\\thing");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 0 );

      index = bsa.getIndexOfDirectory("something\\else");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 1 );

      // Check case-insensitivity for input.
      index = bsa.getIndexOfDirectory("sOmEtHiNG\\eLSe");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 1 );
    }
  }

  SECTION("getIndexOfFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.getIndexOfFile(1, "foo.bar").has_value() );
    }

    SECTION("directory index is out of range")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexOfFile_range_error.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const uint32_t over9000 = 9001;
      REQUIRE_FALSE( bsa.getIndexOfFile(over9000, "foo.bar").has_value() );
    }

    SECTION("matches")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexOfFile_match.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      // find 1st file
      auto index = bsa.getIndexOfFile(0, "test.txt");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 0 );

      // wrong directory index
      index = bsa.getIndexOfFile(1, "test.txt");
      REQUIRE_FALSE( index.has_value() );

      // find 2nd file
      index = bsa.getIndexOfFile(1, "bar.txt");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 0 );

      // find 3rd file
      index = bsa.getIndexOfFile(1, "foo.txt");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 1 );

      // case-insensitivity
      index = bsa.getIndexOfFile(1, "FoO.TxT");
      REQUIRE( index.has_value() );
      REQUIRE( index.value() == 1 );
    }
  }

  SECTION("getIndexPairForFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      std::optional<uint32_t> directoryIndex = 0xDEADBEEF;
      std::optional<uint32_t> fileIndex = 0xDEADBEEF;
      REQUIRE_FALSE( bsa.getIndexPairForFile("baz\\foo.bar", directoryIndex, fileIndex) );
      REQUIRE_FALSE( directoryIndex.has_value() );
      REQUIRE_FALSE( fileIndex.has_value() );
    }

    SECTION("invalid path format")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexPairForFile_path_format.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      std::optional<uint32_t> directoryIndex = 0xDEADBEEF;
      std::optional<uint32_t> fileIndex = 0xDEADBEEF;
      REQUIRE_FALSE( bsa.getIndexPairForFile("foo.bar", directoryIndex, fileIndex) );
      REQUIRE_FALSE( directoryIndex.has_value() );
      REQUIRE_FALSE( fileIndex.has_value() );

      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE_FALSE( bsa.getIndexPairForFile("bar\\", directoryIndex, fileIndex) );
      REQUIRE_FALSE( directoryIndex.has_value() );
      REQUIRE_FALSE( fileIndex.has_value() );
    }

    SECTION("matches")
    {
      const std::filesystem::path path{"test_sr_bsa_getIndexPairForFile_match.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      std::optional<uint32_t> directoryIndex = 0xDEADBEEF;
      std::optional<uint32_t> fileIndex = 0xDEADBEEF;
      // directory does not exist
      REQUIRE( bsa.getIndexPairForFile("blob\\test.txt", directoryIndex, fileIndex) );
      REQUIRE_FALSE( directoryIndex.has_value() );
      REQUIRE_FALSE( fileIndex.has_value() );

      // directory exists, but file does not
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("some\\thing\\blob.dat", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 0 );
      REQUIRE_FALSE( fileIndex.has_value() );

      // directory exists, but file does not
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("something\\else\\blob.dat", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 1 );
      REQUIRE_FALSE( fileIndex.has_value() );

      // directory and file exist
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("some\\thing\\test.txt", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 0 );
      REQUIRE( fileIndex.has_value() );
      REQUIRE( fileIndex.value() == 0 );

      // directory and file exist
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("something\\else\\foo.txt", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 1 );
      REQUIRE( fileIndex.has_value() );
      REQUIRE( fileIndex.value() == 1 );

      // directory and file exist
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("something\\else\\bar.txt", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 1 );
      REQUIRE( fileIndex.has_value() );
      REQUIRE( fileIndex.value() == 0 );

      // Check for case-insensitivity.
      directoryIndex = 0xDEADBEEF;
      fileIndex = 0xDEADBEEF;
      REQUIRE( bsa.getIndexPairForFile("SOMEthIng\\ELse\\BAR.txT", directoryIndex, fileIndex) );
      REQUIRE( directoryIndex.has_value() );
      REQUIRE( directoryIndex.value() == 1 );
      REQUIRE( fileIndex.has_value() );
      REQUIRE( fileIndex.value() == 0 );
    }
  }

  SECTION("hasFile")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.hasFile("baz\\foo.bar") );
    }

    SECTION("matches")
    {
      const std::filesystem::path path{"test_sr_bsa_hasFile_match.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      // Check existing files.
      REQUIRE( bsa.hasFile("some\\thing\\test.txt") );
      REQUIRE( bsa.hasFile("something\\else\\foo.txt") );
      REQUIRE( bsa.hasFile("something\\else\\bar.txt") );

      // Check existing files, but use different upper / lower case.
      REQUIRE( bsa.hasFile("SOME\\thiNG\\tEst.txt") );
      REQUIRE( bsa.hasFile("something\\ELSE\\fOO.txt") );
      REQUIRE( bsa.hasFile("SoMeThInG\\eLsE\\bar.TXT") );

      // Test some files that do not exist in the archive.
      REQUIRE_FALSE( bsa.hasFile("something\\else\\what.dds") );
      REQUIRE_FALSE( bsa.hasFile("some\\thing\\not-here.dds") );
      REQUIRE_FALSE( bsa.hasFile("what\\ever\\is-missing.dds") );
    }
  }

  SECTION("isFileCompressed")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_THROWS( bsa.isFileCompressed(1, 1) );
    }

    SECTION("index is out of range")
    {
      const std::filesystem::path path{"test_sr_bsa_isFileCompressed_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      constexpr uint32_t over9000 = 9001;
      REQUIRE_THROWS( bsa.isFileCompressed(over9000, 1) );
      REQUIRE_THROWS( bsa.isFileCompressed(0, over9000) );
    }

    SECTION("archive is not compressed by default and files do not toggle")
    {
      const std::filesystem::path path{"test_sr_bsa_isFileCompressed_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      REQUIRE_FALSE( bsa.isFileCompressed(0, 0) );
      REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
      REQUIRE_FALSE( bsa.isFileCompressed(1, 1) );
    }

    SECTION("archive is not compressed by default, but some files do toggle their compression")
    {
      const std::filesystem::path path{"test_sr_bsa_isFileCompressed_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\x40\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      REQUIRE( bsa.isFileCompressed(0, 0) ); // toggled compression flag for this file
      REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
      REQUIRE_FALSE( bsa.isFileCompressed(1, 1) );
    }

    SECTION("archive is compressed by default and files do not toggle")
    {
      const std::filesystem::path path{"test_sr_bsa_isFileCompressed_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      REQUIRE( bsa.isFileCompressed(0, 0) );
      REQUIRE( bsa.isFileCompressed(1, 0) );
      REQUIRE( bsa.isFileCompressed(1, 1) );
    }

    SECTION("archive is compressed by default, but some files do toggle their compression")
    {
      const std::filesystem::path path{"test_sr_bsa_isFileCompressed_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\x40\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      REQUIRE_FALSE( bsa.isFileCompressed(0, 0) ); // toggled compression flag for this file
      REQUIRE( bsa.isFileCompressed(1, 0) );
      REQUIRE( bsa.isFileCompressed(1, 1) );
    }
  }

  SECTION("extractFile (with index parameters)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.extractFile(1, 1, "/tmp/foo.out") );
    }

    SECTION("index is out of range")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      constexpr uint32_t over9000 = 9001;
      REQUIRE_FALSE( bsa.extractFile(over9000, 1, "/tmp/cannot.extract") );
      REQUIRE_FALSE( bsa.extractFile(0, over9000, "/tmp/cannot.extract") );
    }

    SECTION("data corruption: offset is out of range")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_offset_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\xFF\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("data corruption: file data is too short")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_file_data_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This fails."sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("data corruption: data size of compressed data is too small")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_compressed_data_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x03\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0x\x9C\x0B\xC9\xC8,V\0\xA2\x44\x85\x92\xD4\xE2\x12=.\0.\xC5\x05.foobar\x0A\x0E\0\0\0x\x9CK\xCB\xCFW(O,V\xC8H-J\xD5\xE3\x02\0&&\x04\xAC"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("data corruption: fail to read decompressed size value")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_decompressed_size_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x1A\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("data corruption: compressed data is too short")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_compressed_data_truncated.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x1A\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0x\x9C\x0B\xC9"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("data corruption: decompression fails")
    {
      const std::filesystem::path path{"test_sr_bsa_extractFile_idx_decompression_failure.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x1A\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0x\x9C\xFF\xFF\xFF\xAA\xAA\x11\x11\x44\x85\x92\xD4\xE2\x12=.\0.\xC5\x05.foobar\x0A\x0E\0\0\0x\x9CK\xCB\xCFW(O,V\xC8H-J\xD5\xE3\x02\0&&\x04\xAC"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto destination { std::filesystem::temp_directory_path() / "extract_test_fail.txt" };
      FileGuard guard_destination{destination};
      REQUIRE_FALSE( bsa.extractFile(0, 0, destination.string()) );
    }

    SECTION("extract uncompressed files from v104 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_idx_no_compression.bsa"};
      FileGuard bsa_guard{bsa_path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_test.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.isFileCompressed(0, 0) );
        REQUIRE( bsa.extractFile(0, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_bar.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
        REQUIRE( bsa.extractFile(1, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_foo.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.isFileCompressed(1, 1) );
        REQUIRE( bsa.extractFile(1, 1, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }
    }

    SECTION("extract compressed files from v104 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_idx_with_compression_v104.bsa"};
      FileGuard bsa_guard{bsa_path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x1A\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0x\x9C\x0B\xC9\xC8,V\0\xA2\x44\x85\x92\xD4\xE2\x12=.\0.\xC5\x05.foobar\x0A\x0E\0\0\0x\x9CK\xCB\xCFW(O,V\xC8H-J\xD5\xE3\x02\0&&\x04\xAC"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_test.txt" };
        FileGuard guard{path};
        // File is compressed.
        REQUIRE( bsa.isFileCompressed(0, 0) );
        REQUIRE( bsa.extractFile(0, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_bar.txt" };
        FileGuard guard{path};
        // File is not compressed, because compression is toggled.
        REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
        REQUIRE( bsa.extractFile(1, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_foo.txt" };
        FileGuard guard{path};
        // File is compressed.
        REQUIRE( bsa.isFileCompressed(1, 1) );
        REQUIRE( bsa.extractFile(1, 1, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }
    }

    SECTION("extract compressed files from v105 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_idx_with_compression_v105.bsa"};
      FileGuard bsa_guard{bsa_path};
      const auto data = "BSA\0\x69\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0\0\0\0\0m\0\0\0\0\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0\0\0\0\0\x89\0\0\0\0\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95#\0\0\0\xB9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xDC\0\0\0oo\x03\x66\xC2\xA6\xD0\x95!\0\0\0\xE3\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0\x04\"M\x18`@\x82\x10\0\0\x80This is a test.\x0A\0\0\0\0foobar\x0A\x0E\0\0\0\x04\"M\x18`@\x82\x0E\0\0\x80\x66oo was here.\x0A\0\0\0\0"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_test_v105.txt" };
        FileGuard guard{path};
        // File is compressed.
        REQUIRE( bsa.isFileCompressed(0, 0) );
        REQUIRE( bsa.extractFile(0, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_bar_v105.txt" };
        FileGuard guard{path};
        // File is not compressed, because compression is toggled.
        REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
        REQUIRE( bsa.extractFile(1, 0, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_foo_v105.txt" };
        FileGuard guard{path};
        // File is compressed.
        REQUIRE( bsa.isFileCompressed(1, 1) );
        REQUIRE( bsa.extractFile(1, 1, path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }
    }
  }

  SECTION("extractFile (with file name parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      REQUIRE_FALSE( bsa.extractFile("foo\\bar.baz", "/tmp/foo.out") );
    }

    SECTION("extract uncompressed files from v104 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_by_name_no_compression.bsa"};
      FileGuard bsa_guard{bsa_path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_test.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.extractFile("some\\thing\\test.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_foo.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.extractFile("something\\else\\foo.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_bar.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.extractFile("something\\else\\bar.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // attempt to extract non-existent file
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_none.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.extractFile("something\\else\\not-found.txt", path.string()) );
      }
    }

    SECTION("extract compressed files from v104 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_by_name_with_compression_v104.bsa"};
      FileGuard guard_bsa{bsa_path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x1A\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xC3\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x1A\0\0\0\xCA\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0x\x9C\x0B\xC9\xC8,V\0\xA2\x44\x85\x92\xD4\xE2\x12=.\0.\xC5\x05.foobar\x0A\x0E\0\0\0x\x9CK\xCB\xCFW(O,V\xC8H-J\xD5\xE3\x02\0&&\x04\xAC"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_test.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.isFileCompressed(0, 0) );
        REQUIRE( bsa.extractFile("some\\thing\\test.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_foo.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.isFileCompressed(1, 1) );
        REQUIRE( bsa.extractFile("something\\else\\foo.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_bar.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
        REQUIRE( bsa.extractFile("something\\else\\bar.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // attempt to extract non-existent file
      {
        const auto path { std::filesystem::temp_directory_path() / "extract_none.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.extractFile("something\\else\\not-found.txt", path.string()) );
      }
    }

    SECTION("extract compressed files from v105 archive")
    {
      const std::filesystem::path bsa_path{"test_sr_bsa_extractFile_by_name_with_compression_v105.bsa"};
      FileGuard guard_bsa{bsa_path};
      const auto data = "BSA\0\x69\0\0\0\x24\0\0\0\x07\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\0\0\0gn\x0As@r\xBE\x0B\x01\0\0\0\0\0\0\0m\0\0\0\0\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0\0\0\0\0\x89\0\0\0\0\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95#\0\0\0\xB9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0@\xDC\0\0\0oo\x03\x66\xC2\xA6\xD0\x95!\0\0\0\xE3\0\0\0test.txt\0bar.txt\0foo.txt\0\x10\0\0\0\x04\"M\x18`@\x82\x10\0\0\x80This is a test.\x0A\0\0\0\0foobar\x0A\x0E\0\0\0\x04\"M\x18`@\x82\x0E\0\0\x80\x66oo was here.\x0A\0\0\0\0"sv;
      REQUIRE( writeBsa(data, bsa_path) );

      BSA bsa;
      REQUIRE( bsa.open(bsa_path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // extract test.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "v105_extract_test.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.isFileCompressed(0, 0) );
        REQUIRE( bsa.extractFile("some\\thing\\test.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // extract foo.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "v105_extract_foo.txt" };
        FileGuard guard{path};
        REQUIRE( bsa.isFileCompressed(1, 1) );
        REQUIRE( bsa.extractFile("something\\else\\foo.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // extract bar.txt
      {
        const auto path { std::filesystem::temp_directory_path() / "v105_extract_bar.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.isFileCompressed(1, 0) );
        REQUIRE( bsa.extractFile("something\\else\\bar.txt", path.string()) );

        std::ifstream stream(path, std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // attempt to extract non-existent file
      {
        const auto path { std::filesystem::temp_directory_path() / "v105_extract_none.txt" };
        FileGuard guard{path};
        REQUIRE_FALSE( bsa.extractFile("something\\else\\not-found.txt", path.string()) );
      }
    }
  }

  SECTION("extractDirectory (with index parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractDirectory(1, "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }

    SECTION("index is out of range")
    {
      const std::filesystem::path path{"test_sr_bsa_extractDirectory_by_index_out_of_range.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const int over9000 = 9001;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractDirectory(over9000, "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }

    SECTION("successfully extract a directory")
    {
      const std::filesystem::path path{"test_sr_bsa_extractDirectory_v104_uncompressed.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const int directory_idx = 1; // 2nd directory
      const auto output_path { std::filesystem::temp_directory_path() / "bsa_extract_directory_by_idx" };
      uint32_t counter = 42;
      REQUIRE( bsa.extractDirectory(directory_idx, output_path.string(), counter) );
      REQUIRE( counter == 2 );
      // Both files must exist.
      REQUIRE( std::filesystem::exists(output_path / "foo.txt") );
      REQUIRE( std::filesystem::exists(output_path / "bar.txt") );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // Check content of foo.txt.
      {
        std::ifstream stream(output_path / "foo.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // Check content of bar.txt.
      {
        std::ifstream stream(output_path / "bar.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // Remove the output directory and all its content.
      REQUIRE( std::filesystem::remove_all(output_path) > 2 );
    }
  }

  SECTION("extractDirectory (with folder name parameter)")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractDirectory("foo\\bar", "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }

    SECTION("directory does not exist")
    {
      const std::filesystem::path path{"test_sr_bsa_extractDirectory_by_name_unknown_directory.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractDirectory("does\\not\\exist", "/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }

    SECTION("successfully extract a directory")
    {
      const std::filesystem::path path{"test_sr_bsa_extractDirectory_v104_uncompressed.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto output_path { std::filesystem::temp_directory_path() / "bsa_extract_directory_by_name" };
      uint32_t counter = 42;
      REQUIRE( bsa.extractDirectory("something\\else", output_path.string(), counter) );
      REQUIRE( counter == 2 );
      // Both files must exist.
      REQUIRE( std::filesystem::exists(output_path / "foo.txt") );
      REQUIRE( std::filesystem::exists(output_path / "bar.txt") );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // Check content of foo.txt.
      {
        std::ifstream stream(output_path / "foo.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // Check content of bar.txt.
      {
        std::ifstream stream(output_path / "bar.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // Remove the output directory and all its content.
      REQUIRE( std::filesystem::remove_all(output_path) > 2 );
    }
  }

  SECTION("extractAll")
  {
    using namespace std::string_view_literals;

    SECTION("no structure data")
    {
      BSA bsa;
      uint32_t counter = 42;
      REQUIRE_FALSE( bsa.extractAll("/tmp/directory-out", counter) );
      REQUIRE( counter == 0 );
    }

    SECTION("successfully extract a complete archive")
    {
      const std::filesystem::path path{"test_sr_bsa_extractAll_v104_uncompressed.bsa"};
      FileGuard guard{path};
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x02\0\0\0\x03\0\0\0\x1A\0\0\0\x19\0\0\0\0\x01\0\0gn\x0As@r\xBE\x0B\x01\0\0\0]\0\0\0es\x0Es\xDC\x92\x84Z\x02\0\0\0y\0\0\0\x0Bsome\\thing\0ts\x04t'\xA7\xD0\x95\x10\0\0\0\xA9\0\0\0\x0Fsomething\\\x65lse\0ra\x03\x62\xC2\xA6\xD0\x95\x07\0\0\0\xB9\0\0\0oo\x03\x66\xC2\xA6\xD0\x95\x0E\0\0\0\xC0\0\0\0test.txt\0bar.txt\0foo.txt\0This is a test.\x0A\x66oobar\x0A\x66oo was here.\x0A"sv;
      REQUIRE( writeBsa(data, path) );

      BSA bsa;
      REQUIRE( bsa.open(path.string()) );
      REQUIRE( bsa.grabAllStructureData() );

      const auto output_path { std::filesystem::temp_directory_path() / "bsa_extract_all" };
      uint32_t counter = 42;
      REQUIRE( bsa.extractAll(output_path.string(), counter) );
      REQUIRE( counter == 3 );
      // All three files must exist.
      REQUIRE( std::filesystem::exists(output_path / "some" / "thing" / "test.txt") );
      REQUIRE( std::filesystem::exists(output_path / "something" / "else" / "foo.txt") );
      REQUIRE( std::filesystem::exists(output_path / "something" / "else" / "bar.txt") );

      const char eof = static_cast<char>(std::char_traits<char>::eof());

      // Check content of test.txt.
      {
        std::ifstream stream(output_path / "some" / "thing" / "test.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "This is a test.\n" );
      }

      // Check content of foo.txt.
      {
        std::ifstream stream(output_path / "something" / "else" / "foo.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foo was here.\n" );
      }

      // Check content of bar.txt.
      {
        std::ifstream stream(output_path / "something" / "else" / "bar.txt", std::ios::in | std::ios::binary);
        std::string content;
        std::getline(stream, content, eof);
        REQUIRE( content == "foobar\n" );
      }

      // Remove the output directory and all its content.
      REQUIRE( std::filesystem::remove_all(output_path) > 3 );
    }
  }
}
