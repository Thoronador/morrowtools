/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2014, 2021, 2022  Dirk Stolle

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

#include "BSA.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include "../../../base/SlashFunctions.hpp"
#include "../../../base/UtilityFunctions.hpp"
#include "../../../base/DirectoryFunctions.hpp"
#include "../../../base/CompressionFunctions.hpp"
#if !defined(MWTP_NO_LZ4)
  // Only include lz4 support when it is not disabled.
  #include "../../../base/lz4Compression.hpp"
#endif

namespace SRTP
{

BSA::BSA()
: m_Status(Status::Fresh),
  m_Stream(std::ifstream()),
  m_Header(BSAHeader()),
  m_Folders(std::vector<BSAFolderRecord>()),
  m_FolderBlocks(std::vector<BSAFolderBlock>())
{
}

BSA::~BSA()
{
  close();
}

bool BSA::open(const std::filesystem::path& fileName)
{
  if ((m_Status != Status::Fresh) && (m_Status != Status::Closed))
  {
    std::cerr << "BSA::open: Error: BSA was already opened!\n";
    return false;
  }
  m_Stream.open(fileName, std::ios_base::in | std::ios_base::binary);
  if (!m_Stream)
  {
    std::cerr << "BSA::open: Error while opening file \"" << fileName.string()
              << "\".\n";
    m_Status = Status::Closed;
    return false;
  }
  // file opened, now read the header
  if (!m_Header.loadFromStream(m_Stream))
  {
    std::cerr << "BSA::open: Error while reading header!\n";
    m_Stream.close();
    m_Status = Status::Closed;
    return false;
  }

  // header was read successfully
  m_Status = Status::Open;
  return true;
}

bool BSA::grabAllStructureData()
{
  if (hasAllStructureData())
  {
    std::cerr << "BSA::grabAllStructureData: Structure data was already read completely!\n";
    return true;
  }

  if (!grabFolderData())
  {
    std::cerr << "BSA::grabAllStructureData: Could not read folder data!\n";
    return false;
  }

  if (!grabFolderBlocks())
  {
    std::cerr << "BSA::grabAllStructureData: Could not read folder blocks!\n";
    return false;
  }

  if (!grabFileNames())
  {
    std::cerr << "BSA::grabAllStructureData: Could not read file names!\n";
    return false;
  }

  return true;
}

bool BSA::grabFolderData()
{
  if ((m_Status == Status::Fresh) || (m_Status == Status::Closed))
  {
    std::cerr << "BSA::grabFolderData: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != Status::Open)
  {
    std::cerr << "BSA::grabFolderData: Error: BSA has wrong status!\n";
    return false;
  }
  if ((m_Header.version < 104) || (m_Header.version > 105))
  {
    std::clog << "BSAHeader::grabFolderData: Warning: Unknown BSA version ("
              << m_Header.version << ") detected! This may have unknown side "
              << "effects and could cause errors. But let's give it a try anyway.\n";
  }

  m_Folders.clear();
  BSAFolderRecord tempFolder;
  for (uint32_t i = 0; i < m_Header.folderCount; ++i)
  {
    if (!tempFolder.loadFromStream(m_Stream, m_Header.version))
    {
      std::cerr << "BSA::grabFolderData: Error while reading folders!\n";
      m_Status = Status::Failed;
      return false;
    }
    m_Folders.push_back(tempFolder);
  }

  m_Status = Status::OpenFolderData;
  return m_Stream.good();
}

bool BSA::grabFolderBlocks()
{
  if ((m_Status == Status::Fresh) || (m_Status == Status::Closed))
  {
    std::cerr << "BSA::grabFolderBlocks: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != Status::OpenFolderData)
  {
    std::cerr << "BSA::grabFolderBlocks: Error: BSA has wrong status!\n";
    return false;
  }

  m_FolderBlocks.clear();
  BSAFolderBlock tempFolderBlock;
  for (uint32_t i = 0; i < m_Folders.size(); ++i)
  {
    if (!tempFolderBlock.loadFromStream(m_Stream, m_Folders[i].count))
    {
      std::cerr << "BSA::grabFolderBlocks: Error while reading!\n";
      m_Status = Status::Failed;
      return false;
    }
    m_FolderBlocks.push_back(tempFolderBlock);
  }

  m_Status = Status::OpenFolderBlocks;
  return m_Stream.good();
}

bool BSA::grabFileNames()
{
  if ((m_Status == Status::Fresh) || (m_Status == Status::Closed))
  {
    std::cerr << "BSA::grabFileNames: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != Status::OpenFolderBlocks)
  {
    std::cerr << "BSA::grabFileNames: Error: BSA has wrong status!\n";
    return false;
  }

  if (!m_Stream.good())
  {
    std::cerr << "BSA::grabFileNames: Error: bad stream!\n";
    return false;
  }

  // check for existence of file blocks
  if (m_FolderBlocks.empty())
  {
    std::cerr << "BSA::grabFileNames: Error: Internal folder blocks are missing!\n";
    return false;
  }
  // ...and now check for files
  for (unsigned int i = 0; i < m_FolderBlocks.size(); ++i)
  {
    if (m_FolderBlocks[i].files.empty())
    {
      std::cerr << "BSA::grabFileNames: Error: Internal folder block #" << i
                << " contains no files!\n";
      return false;
    }
  }
  // now allocate the space for the file names
  char * namesPointer = new char[m_Header.totalFileNameLength + 1];
  memset(namesPointer, 0, m_Header.totalFileNameLength + 1);

  // Read it all in one go!
  m_Stream.read(namesPointer, m_Header.totalFileNameLength);
  if (!m_Stream.good())
  {
    std::cerr << "BSA::grabFileNames: Error while reading name list!\n";
    delete[] namesPointer;
    namesPointer = nullptr;
    m_Status = Status::Failed;
    return false;
  }

  // now associate the names with their file records
  std::string nextName = "";
  uint32_t charOffset = 0;
  uint32_t dirIndex = 0;
  uint32_t fileIndex = 0;
  uint32_t namesRead = 0;

  while (charOffset < m_Header.totalFileNameLength)
  {
    // set names until no more data is left
    nextName = &namesPointer[charOffset];
    charOffset = charOffset + nextName.length() + 1;
    if (dirIndex >= m_FolderBlocks.size())
    {
      std::cerr << "BSA::grabFileNames: Error: Not enough file blocks!\n"
                << "dir idx: " << dirIndex << ", #blocks: "
                << m_FolderBlocks.size() << "\n";
      delete[] namesPointer;
      namesPointer = nullptr;
      m_Status = Status::Failed;
      return false;
    }
    // enough files?
    if (fileIndex >= m_FolderBlocks.at(dirIndex).files.size())
    {
      // set to start of next folder block
      ++dirIndex;
      fileIndex = 0;
      // another check is necessary here
      if (dirIndex >= m_FolderBlocks.size())
      {
        std::cerr << "BSA::grabFileNames: Error: Not enough file blocks (2)!\n"
                  << "dir idx: " << dirIndex << ", #blocks: " << m_FolderBlocks.size() << "\n"
                  << "char offset: " << charOffset << ", ptr. len.: " << m_Header.totalFileNameLength << "\n"
                  << "names done: " << namesRead << "\n";
        delete[] namesPointer;
        namesPointer = nullptr;
        m_Status = Status::Failed;
        return false;
      }
    } // if next folder
    // now finally set it
    m_FolderBlocks.at(dirIndex).files.at(fileIndex).fileName = nextName;
    ++fileIndex;
    ++namesRead;
  }

  delete[] namesPointer;
  namesPointer = nullptr;

  if (namesRead != m_Header.fileCount)
  {
    std::cerr << "BSA::grabFileNames: Error: number of read file names does not "
              << "match the number given in the header. " << m_Header.fileCount
              << " files should be there, but " << namesRead << " were found!\n";
    m_Status = Status::Failed;
    return false;
  }

  m_Status = Status::OpenFileNames;
  return true;
}

void BSA::listFileNames(bool withCompressionStatus)
{
  if (m_Status != Status::OpenFileNames)
  {
    std::cerr << "BSA::listFileNames: Error: BSA has wrong status for that operation!\n";
    return;
  }
  uint32_t compressedFiles = 0;
  uint32_t rawFiles = 0;
  for (uint32_t folderIdx = 0; folderIdx < m_FolderBlocks.size(); ++folderIdx)
  {
    const std::string folder = m_FolderBlocks[folderIdx].folderName + "\\";
    for (uint32_t fileIdx = 0; fileIdx < m_FolderBlocks[folderIdx].files.size(); ++fileIdx)
    {
      if (withCompressionStatus)
      {
        if (isFileCompressed(folderIdx, fileIdx))
        {
          std::cout << "(cmp) ";
          ++compressedFiles;
        }
        else
        {
          std::cout << "(raw) ";
          ++rawFiles;
        }
      } // if compression info requested
      std::cout << folder << m_FolderBlocks[folderIdx].files[fileIdx].fileName << "\n";
    }
  }
  if (withCompressionStatus)
  {
    std::cout << "Compressed files in archive: " << compressedFiles
              << "\nUncompressed files in archive: " << rawFiles << "\n";
  }
}

std::vector<std::string> BSA::getDirectories() const
{
  std::vector<std::string> result;
  if ((m_Status != Status::OpenFolderBlocks) && (m_Status != Status::OpenFileNames))
  {
    std::cerr << "BSA::getDirectories: Error: Not all folder data is present "
              << "to properly fulfill the requested operation!\n";
    return result;
  }
  result.reserve(m_FolderBlocks.size());
  for (const auto& elem: m_FolderBlocks)
  {
    result.emplace_back(elem.folderName);
  }
  return result;
}

void BSA::close()
{
  if ((m_Status != Status::Fresh) && (Status::Closed != m_Status))
  {
    m_Stream.close();
    m_Status = Status::Closed;
  }
}

const BSAHeader& BSA::getHeader() const
{
  return m_Header;
}

const std::vector<BSAFolderRecord>& BSA::getFolders() const
{
  return m_Folders;
}

const std::vector<BSAFolderBlock>& BSA::getFolderBlocks() const
{
  return m_FolderBlocks;
}

bool BSA::hasAllStructureData() const
{
  return m_Status == Status::OpenFileNames;
}

bool BSA::hasFolder(const std::string& folderName) const
{
  return getIndexOfFolder(folderName).has_value();
}

std::optional<uint32_t> BSA::getIndexOfFolder(std::string folderName) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexOfFolder: Error: Not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return std::nullopt;
  }

  if (folderName.empty())
    return std::nullopt;
  // transform to lower case
  folderName = lowerCase(folderName);
  for (uint32_t i = 0; i < m_FolderBlocks.size(); ++i)
  {
    if (m_FolderBlocks[i].folderName == folderName)
      return i;
  }
  return std::nullopt;
}

std::optional<uint32_t> BSA::getIndexOfFile(const uint32_t folderIndex, std::string fileName) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexOfFile: Error: Not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return std::nullopt;
  }

  if (folderIndex >= m_FolderBlocks.size())
  {
    std::cerr << "BSA::getIndexOfFile: Error: Folder index exceeds allowed maximum!\n";
    return std::nullopt;
  }

  fileName = lowerCase(fileName);
  for (uint32_t i = 0; i < m_FolderBlocks[folderIndex].files.size(); ++i)
  {
    if (m_FolderBlocks[folderIndex].files[i].fileName == fileName)
      return i;
  }

  return std::nullopt;
}

bool BSA::getIndexPairForFile(const std::string& fileName, std::optional<uint32_t>& folderIndex, std::optional<uint32_t>& fileIndex) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexPairForFile: Error: Not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    folderIndex.reset();
    fileIndex.reset();
    return false;
  }

  const std::string::size_type delimPos = fileName.rfind('\\');
  if (delimPos == std::string::npos)
  {
    std::cerr << "BSA::getIndexPairForFile: Error: File name contains no folder!\n";
    folderIndex.reset();
    fileIndex.reset();
    return false;
  }
  if (delimPos == fileName.length() - 1)
  {
    std::cerr << "BSA::getIndexPairForFile: Error: Given file name only contains directory!\n";
    folderIndex.reset();
    fileIndex.reset();
    return false;
  }

  folderIndex = getIndexOfFolder(fileName.substr(0, delimPos));
  if (!folderIndex.has_value())
  {
    // no such folder, thus no file in that folder
    fileIndex.reset();
    return true;
  }

  fileIndex = getIndexOfFile(folderIndex.value(), fileName.substr(delimPos + 1));
  return true;
}

bool BSA::hasFile(const std::string& fileName) const
{
  std::optional<uint32_t> folderIdx;
  std::optional<uint32_t> fileIdx;
  if (!getIndexPairForFile(fileName, folderIdx, fileIdx))
    return false;

  return folderIdx.has_value() && fileIdx.has_value();
}

bool BSA::isValidIndexPair(const uint32_t folderIndex, const uint32_t fileIndex) const
{
  if (folderIndex >= m_FolderBlocks.size())
  {
    return false;
  }

  if (m_FolderBlocks[folderIndex].files.size() <= fileIndex)
  {
    return false;
  }

  return true;
}

bool BSA::isFileCompressed(const uint32_t folderIndex, const uint32_t fileIndex) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::isFileCompressed: Error: Not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    throw std::runtime_error(std::string("BSA::isFileCompressed: Error: Not ")
              + "all structure data is present to properly fulfill the requested operation!");
  }

  if (!isValidIndexPair(folderIndex, fileIndex))
  {
    std::cerr << "BSA::isFileCompressed: Error: Invalid indices given!\n";
    throw std::invalid_argument("BSA::isFileCompressed: Error: Invalid indices given!");
  }

  return m_Header.filesCompressedByDefault() ^ m_FolderBlocks.at(folderIndex).files.at(fileIndex).isCompressionToggled();
}

bool BSA::extractFile(const uint32_t folderIndex, const uint32_t fileIndex, const std::string& outputFileName)
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFile: Error: Not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  if (!isValidIndexPair(folderIndex, fileIndex))
  {
    std::cerr << "BSA::extractFile: Hint: File is not in the archive!\n";
    return false;
  }

  uint32_t extractedFileSize = m_FolderBlocks[folderIndex].files[fileIndex].getRealFileSize();
  m_Stream.seekg(m_FolderBlocks[folderIndex].files[fileIndex].offset, std::ios_base::beg);
  if (!m_Stream.good())
  {
    std::cerr << "BSA::extractFile: Error: Bad internal stream, could not jump to file's offset!\n";
    return false;
  }

  uint8_t * buffer = nullptr; // buffer for output data
  if (isFileCompressed(folderIndex, fileIndex))
  {
    const bool compressionUsesLZ4 = m_Header.version >= 105;
    #if defined(MWTP_NO_LZ4)
    if (compressionUsesLZ4)
    {
      std::cerr << "BSA::extractFile: Error: This archive uses LZ4 for "
                << "compressed data, but LZ4 decompression is not enabled "
                << "in the current build of this program!\n";
      return false;
    }
    #endif
    if (extractedFileSize < 4)
    {
      std::cerr << "BSA::extractFile: Error: Size is too small to contain any compressed data!\n";
      return false;
    }
    uint32_t decompSize = 0;
    // read size of decompressed file
    m_Stream.read(reinterpret_cast<char*>(&decompSize), 4);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: Could not read file's uncompressed size!\n";
      return false;
    }
    // read compressed stuff
    uint8_t * compressedBuffer = new uint8_t[extractedFileSize - 4];
    m_Stream.read(reinterpret_cast<char*>(compressedBuffer), extractedFileSize - 4);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: Could not read compressed file data from archive!\n";
      delete [] compressedBuffer;
      return false;
    }
    // allocate buffer for decompressed data
    buffer = new uint8_t[decompSize];
    #if defined(MWTP_NO_LZ4)
    const bool success = MWTP::zlibDecompress(compressedBuffer, extractedFileSize - 4, buffer, decompSize);
    #else
    const bool success = compressionUsesLZ4 ?
        MWTP::lz4Decompress(compressedBuffer, extractedFileSize - 4, buffer, decompSize)
        : MWTP::zlibDecompress(compressedBuffer, extractedFileSize - 4, buffer, decompSize);
    #endif
    if (!success)
    {
      std::cerr << "BSA::extractFile: Error: Decompression failed!\n";
      delete [] compressedBuffer;
      delete [] buffer;
      return false;
    }
    // success, delete compressed data buffer
    delete [] compressedBuffer;
    compressedBuffer = nullptr;
    // Adjust size for file output, because decompressed data is usually bigger.
    extractedFileSize = decompSize;
  }
  else
  {
    // handle uncompressed data
    buffer = new uint8_t[extractedFileSize];
    m_Stream.read(reinterpret_cast<char*>(buffer), extractedFileSize);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: Bad internal stream, could read file data from archive!\n";
      delete[] buffer;
      buffer = nullptr;
      return false;
    }
  }

  std::ofstream outputStream;
  outputStream.open(outputFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!outputStream)
  {
    std::cerr << "BSA::extractFile: Error: Could not open/create file \""
              << outputFileName << "\" for writing!\n";
    delete[] buffer;
    buffer = nullptr;
    return false;
  }

  outputStream.write(reinterpret_cast<const char*>(buffer), extractedFileSize);
  if (!outputStream.good())
  {
    std::cerr << "BSA::extractFile: Error: Could not write data to file \""
              << outputFileName << "\"!\n";
    delete[] buffer;
    buffer = nullptr;
    return false;
  }
  outputStream.close();
  // free buffer
  delete[] buffer;
  buffer = nullptr;
  return true;
}

bool BSA::extractFile(const std::string& inArchiveFileName, const std::string& outputFileName)
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFile: Error: Not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  std::optional<uint32_t> folderIndex, fileIndex;
  getIndexPairForFile(inArchiveFileName, folderIndex, fileIndex);
  if (!folderIndex.has_value() || !fileIndex.has_value())
  {
    std::cerr << "BSA::extractFile: Hint: File " << inArchiveFileName
              << " is not in the archive!\n";
    return false;
  }

  // use the above function for the rest
  return extractFile(folderIndex.value(), fileIndex.value(), outputFileName);
}

bool BSA::extractFolder(const uint32_t folderIndex, const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFolder: Error: Not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  if (folderIndex >= m_FolderBlocks.size())
  {
    std::cerr << "BSA::extractFolder: Error: Invalid folder index!\n";
    return false;
  }

  // check for output directory
  if (!directoryExists(outputDirName))
  {
    if (!createDirectoryRecursive(outputDirName))
    {
      std::cerr << "BSA::extractFolder: Error: Could not create destination directory \""
                << outputDirName << "\".\n";
      return false;
    }
  }

  // now extract each file in that directory
  for (uint32_t file_index = 0; file_index < m_FolderBlocks[folderIndex].files.size(); ++file_index)
  {
    if (!extractFile(folderIndex, file_index, outputDirName + MWTP::pathDelimiter
         + m_FolderBlocks[folderIndex].files[file_index].fileName))
    {
      std::cerr << "BSA::extractFolder: Error: Could not extract file \""
                << m_FolderBlocks[folderIndex].folderName + MWTP::pathDelimiter
                  + m_FolderBlocks[folderIndex].files[file_index].fileName << "\".\n";
      return false;
    }
    ++extractedFileCount;
  }

  return true;
}

bool BSA::extractFolder(const std::string& folderName, const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFolder: Error: Not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  const std::optional<uint32_t> fIdx = getIndexOfFolder(folderName);
  if (!fIdx.has_value())
  {
    std::cerr << "BSA::extractFolder: Error: Archive has no folder named \""
              << folderName <<"\", thus it cannot be extracted!\n";
    return false;
  }

  return extractFolder(fIdx.value(), outputDirName, extractedFileCount);
}

bool BSA::extractAll(const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractAll: Error: Not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  // check for output directory
  if (!directoryExists(outputDirName))
  {
    if (!createDirectoryRecursive(outputDirName))
    {
      std::cerr << "BSA::extractAll: Error: Could not create destination directory \""
                << outputDirName << "\".\n";
      return false;
    }
  }

  for (uint32_t i = 0; i < m_FolderBlocks.size(); ++i)
  {
    #if defined(_WIN32)
    // Windows file systems take backslashes in folderName directly, because
    // that is their usual directory separator.
    const auto directoryPath = outputDirName + MWTP::pathDelimiter + m_FolderBlocks[i].folderName;
    #else
    // Unix-like file systems use slash instead of backslash, so it has to be
    // replaced in folderName.
    const auto directoryPath = outputDirName + MWTP::pathDelimiter + MWTP::flipBackslashes(m_FolderBlocks[i].folderName);
    #endif
    // create output directory, if necessary
    if (!directoryExists(directoryPath))
    {
      if (!createDirectoryRecursive(directoryPath))
      {
        std::cerr << "BSA::extractAll: Error: Could not create destination subdirectory \""
                  << directoryPath << "\".\n";
        return false;
      }
    }
    // now extract each file in that directory
    for (uint32_t j = 0; j < m_FolderBlocks[i].files.size(); ++j)
    {
      if (!extractFile(i, j, directoryPath + MWTP::pathDelimiter
                             + m_FolderBlocks[i].files[j].fileName))
      {
        std::cerr << "BSA::extractAll: Error: Could not extract file \""
                  << m_FolderBlocks[i].folderName + MWTP::pathDelimiter
                    + m_FolderBlocks[i].files[j].fileName << "\".\n";
        return false;
      }
      ++extractedFileCount;
    }
  }

  return true;
}

} // namespace
