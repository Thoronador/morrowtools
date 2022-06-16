/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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
#include "../../../../../sr/base/bsa/BSAHeader.hpp"

TEST_CASE("BSAHeader")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    BSAHeader header;

    REQUIRE( header.fileID == 0 );
    REQUIRE( header.version == 0 );
    REQUIRE( header.offset == 0 );
    REQUIRE( header.archiveFlags == 0 );
    REQUIRE( header.folderCount == 0 );
    REQUIRE( header.fileCount == 0 );
    REQUIRE( header.totalFolderNameLength == 0 );
    REQUIRE( header.totalFileNameLength == 0 );
    REQUIRE( header.fileFlags == 0 );
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load header")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should succeed.
      BSAHeader header;
      REQUIRE( header.loadFromStream(stream) );
      // Check data.
      REQUIRE( header.fileID == 0x00415342 );
      REQUIRE( header.version == 104 );
      REQUIRE( header.offset == 36 );
      REQUIRE( header.archiveFlags == 0x00000003 );
      REQUIRE( header.folderCount == 7 );
      REQUIRE( header.fileCount == 197 );
      REQUIRE( header.totalFolderNameLength == 136 );
      REQUIRE( header.totalFileNameLength == 0x00000B81 );
      REQUIRE( header.fileFlags == 0 );
    }

    SECTION("corrupt data: bad stream on entry")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      char buffer[100];
      stream.read(buffer, 100);
      REQUIRE_FALSE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before file ID can be read")
    {
      std::istringstream stream;
      stream.str(std::string("BS"));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: invalid header")
    {
      const auto data = "ABC\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before version can be read")
    {
      const auto data = "BSA\0\x68\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("default: version is not 104, but loads anyway")
    {
      {
        const auto data = "BSA\0\x67\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stream.good() );

        // Reading should succeed.
        BSAHeader header;
        REQUIRE( header.loadFromStream(stream) );
        // Check data.
        REQUIRE( header.version == 103 );
      }

      {
        const auto data = "BSA\0\x69\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stream.good() );

        // Reading should succeed.
        BSAHeader header;
        REQUIRE( header.loadFromStream(stream) );
        // Check data.
        REQUIRE( header.version == 105 );
      }
    }

    SECTION("corrupt data: stream ends before offset can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: offset is not 36")
    {
      {
        const auto data = "BSA\0\x68\0\0\0\x23\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stream.good() );

        // Reading should fail.
        BSAHeader header;
        REQUIRE_FALSE( header.loadFromStream(stream) );
      }

      {
        const auto data = "BSA\0\x68\0\0\0\x25\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( stream.good() );

        // Reading should fail.
        BSAHeader header;
        REQUIRE_FALSE( header.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before archive flags can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before folder count can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before file count can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before folder name length can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before file name length can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }

    SECTION("corrupt data: stream ends before file flags can be read")
    {
      const auto data = "BSA\0\x68\0\0\0\x24\0\0\0\x03\0\0\0\x07\0\0\0\xC5\0\0\0\x88\0\0\0\x81\x0B\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( stream.good() );

      // Reading should fail.
      BSAHeader header;
      REQUIRE_FALSE( header.loadFromStream(stream) );
    }
  }

  SECTION("hasNamesForFolders")
  {
    BSAHeader header;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.hasNamesForFolders() );

    header.archiveFlags = 1;
    REQUIRE( header.hasNamesForFolders() );

    header.archiveFlags = 2;
    REQUIRE_FALSE( header.hasNamesForFolders() );

    header.archiveFlags = 255;
    REQUIRE( header.hasNamesForFolders() );
  }

  SECTION("hasNamesForFiles")
  {
    BSAHeader header;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.hasNamesForFiles() );

    header.archiveFlags = 2;
    REQUIRE( header.hasNamesForFiles() );

    header.archiveFlags = 4;
    REQUIRE_FALSE( header.hasNamesForFiles() );

    header.archiveFlags = 255;
    REQUIRE( header.hasNamesForFiles() );
  }

  SECTION("filesCompressedByDefault")
  {
    BSAHeader header;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.filesCompressedByDefault() );

    header.archiveFlags = 4;
    REQUIRE( header.filesCompressedByDefault() );

    header.archiveFlags = 2;
    REQUIRE_FALSE( header.filesCompressedByDefault() );

    header.archiveFlags = 255;
    REQUIRE( header.filesCompressedByDefault() );
  }

  SECTION("isXboxArchive")
  {
    BSAHeader header;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.isXboxArchive() );

    header.archiveFlags = 64;
    REQUIRE( header.isXboxArchive() );

    header.archiveFlags = 2;
    REQUIRE_FALSE( header.isXboxArchive() );

    header.archiveFlags = 255;
    REQUIRE( header.isXboxArchive() );
  }

  SECTION("usesXMemCodec, BSA version 103")
  {
    BSAHeader header;
    header.version = 103;

    // In version 103 the bit has a different (unknown) meaning, so it shall
    // never indicate XMem there.
    header.archiveFlags = 512;
    REQUIRE_FALSE( header.usesXMemCodec() );

    header.archiveFlags = 65535;
    REQUIRE_FALSE( header.usesXMemCodec() );
  }

  SECTION("usesXMemCodec, BSA version 104")
  {
    BSAHeader header;
    header.version = 104;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.usesXMemCodec() );

    header.archiveFlags = 512;
    REQUIRE( header.usesXMemCodec() );

    header.archiveFlags = 32;
    REQUIRE_FALSE( header.usesXMemCodec() );

    header.archiveFlags = 65535;
    REQUIRE( header.usesXMemCodec() );
  }

  SECTION("usesXMemCodec, BSA version 105")
  {
    BSAHeader header;
    header.version = 105;

    header.archiveFlags = 0;
    REQUIRE_FALSE( header.usesXMemCodec() );

    header.archiveFlags = 512;
    REQUIRE( header.usesXMemCodec() );

    header.archiveFlags = 32;
    REQUIRE_FALSE( header.usesXMemCodec() );

    header.archiveFlags = 65535;
    REQUIRE( header.usesXMemCodec() );
  }

  SECTION("contains")
  {
    BSAHeader header;
    header.version = 104;

    SECTION("meshes")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Meshes) );

      header.fileFlags = 1;
      REQUIRE( header.contains(BSAHeader::ContentType::Meshes) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Meshes) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Meshes) );
    }

    SECTION("textures")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Textures) );

      header.fileFlags = 2;
      REQUIRE( header.contains(BSAHeader::ContentType::Textures) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Textures) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Textures) );
    }

    SECTION("menus")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Menus) );

      header.fileFlags = 4;
      REQUIRE( header.contains(BSAHeader::ContentType::Menus) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Menus) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Menus) );
    }

    SECTION("sounds")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Sounds) );

      header.fileFlags = 8;
      REQUIRE( header.contains(BSAHeader::ContentType::Sounds) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Sounds) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Sounds) );
    }

    SECTION("voices")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Voices) );

      header.fileFlags = 16;
      REQUIRE( header.contains(BSAHeader::ContentType::Voices) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Voices) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Voices) );
    }

    SECTION("shaders")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Shaders) );

      header.fileFlags = 32;
      REQUIRE( header.contains(BSAHeader::ContentType::Shaders) );

      header.fileFlags = 2;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Shaders) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Shaders) );
    }

    SECTION("trees")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Trees) );

      header.fileFlags = 64;
      REQUIRE( header.contains(BSAHeader::ContentType::Trees) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Trees) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Trees) );
    }

    SECTION("fonts")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Fonts) );

      header.fileFlags = 128;
      REQUIRE( header.contains(BSAHeader::ContentType::Fonts) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Fonts) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Fonts) );
    }

    SECTION("misc")
    {
      header.fileFlags = 0;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Misc) );

      header.fileFlags = 256;
      REQUIRE( header.contains(BSAHeader::ContentType::Misc) );

      header.fileFlags = 32;
      REQUIRE_FALSE( header.contains(BSAHeader::ContentType::Misc) );

      header.fileFlags = 65535;
      REQUIRE( header.contains(BSAHeader::ContentType::Misc) );
    }
  }
}
