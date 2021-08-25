/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2014  Thoronador

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
#include "../../../base/UtilityFunctions.hpp"
#include "../../../base/DirectoryFunctions.hpp"
#include "../../../base/CompressionFunctions.hpp"

namespace SRTP
{

const uint32_t BSA::cIndexNotFound = 0xFFFFFFFF;

BSA::BSA()
: m_Status(bsFresh),
  m_Folders(std::vector<BSAFolderRecord>()),
  m_FolderBlocks(std::vector<BSAFolderBlock>())
{
}

BSA::~BSA()
{
  close();
}

bool BSA::open(const std::string& FileName)
{
  if ((m_Status != bsFresh) && (m_Status != bsClosed))
  {
    std::cerr << "BSA::open: Error: BSA was already opened!\n";
    return false;
  }
  m_Stream.open(FileName, std::ios_base::in | std::ios_base::binary);
  if (!m_Stream)
  {
    std::cerr << "BSA::open: Error while opening file \"" << FileName << "\".\n";
    m_Status = bsClosed;
    return false;
  }
  // file opened, now read the header
  if (!m_Header.loadFromStream(m_Stream))
  {
    std::cerr << "BSA::open: Error while reading header!\n";
    m_Stream.close();
    m_Status = bsClosed;
    return false;
  }

  // header was read successfully
  m_Status = bsOpen;
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
    std::cerr << "BSA::grabAllStructureData: could not read folder data!\n";
    return false;
  }

  if (!grabFolderBlocks())
  {
    std::cerr << "BSA::grabAllStructureData: could not read folder blocks!\n";
    return false;
  }

  if (!grabFileNames())
  {
    std::cerr << "BSA::grabAllStructureData: could not read file names!\n";
    return false;
  }

  return true;
}

bool BSA::grabFolderData()
{
  if ((m_Status == bsFresh) || (m_Status == bsClosed))
  {
    std::cerr << "BSA::grabFolderData: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != bsOpen)
  {
    std::cerr << "BSA::grabFolderData: Error: BSA has wrong status!\n";
    return false;
  }

  m_Folders.clear();
  uint32_t i;
  BSAFolderRecord tempFolder;
  for (i=0; i<m_Header.folderCount; ++i)
  {
    if (!tempFolder.loadFromStream(m_Stream))
    {
      std::cerr << "BSA::grabFolderData: Error while reading folders!\n";
      m_Status = bsFailed;
      return false;
    }
    m_Folders.push_back(tempFolder);
  }

  m_Status = bsOpenFolderData;
  return m_Stream.good();
}

bool BSA::grabFolderBlocks()
{
  if ((m_Status == bsFresh) || (m_Status == bsClosed))
  {
    std::cerr << "BSA::grabFolderBlocks: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != bsOpenFolderData)
  {
    std::cerr << "BSA::grabFolderBlocks: Error: BSA has wrong status!\n";
    return false;
  }

  m_FolderBlocks.clear();

  uint32_t i;
  BSAFolderBlock tempFolderBlock;
  for (i=0; i<m_Folders.size(); ++i)
  {
    if (!tempFolderBlock.loadFromStream(m_Stream, m_Folders.at(i).count))
    {
      std::cerr << "BSA::grabFolderBlocks: Error while reading!\n";
      m_Status = bsFailed;
      return false;
    }
    m_FolderBlocks.push_back(tempFolderBlock);
  }//for

  std::cout << "Info: Current stream offset is "<<(unsigned int) m_Stream.tellg()<<" bytes.\n";
  m_Status = bsOpenFolderBlocks;
  return m_Stream.good();
}

bool BSA::grabFileNames()
{
  if ((m_Status == bsFresh) || (m_Status == bsClosed))
  {
    std::cerr << "BSA::grabFileNames: Error: BSA was not opened!\n";
    return false;
  }
  if (m_Status != bsOpenFolderBlocks)
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
    std::cerr << "BSA::grabFileNames: Error: internal folder blocks are missing!\n";
    return false;
  }
  //...and now check for files
  unsigned int i;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    if (m_FolderBlocks.at(i).files.empty())
    {
      std::cerr << "BSA::grabFileNames: Error: internal folder block #"<<i<<" contains no files!\n";
      return false;
    }
  }
  // now allocate the space for the file names
  char * namesPointer = new char[m_Header.totalFileNameLength+1];
  memset(namesPointer, 0, m_Header.totalFileNameLength+1);

  // read it all in one go!
  m_Stream.read(namesPointer, m_Header.totalFileNameLength);
  if (!m_Stream.good())
  {
    std::cerr << "BSA::grabFileNames: Error while reading name list!\n";
    delete[] namesPointer;
    namesPointer = NULL;
    m_Status = bsFailed;
    return false;
  }

  // now associate the names with their file records
  std::string nextName = "";
  uint32_t charOffset = 0;
  uint32_t dirIndex = 0;
  uint32_t fileIndex = 0;
  uint32_t namesRead = 0;

  while (charOffset<m_Header.totalFileNameLength)
  {
    //set names until no more data is left
    nextName = &namesPointer[charOffset];
    charOffset = charOffset + nextName.length() +1;
    if (dirIndex>=m_FolderBlocks.size())
    {
      std::cerr << "BSA::grabFileNames: Error: not enough file blocks!\n"
                << "dir idx: " << dirIndex << ", #blocks: "
                << m_FolderBlocks.size() << "\n";
      delete[] namesPointer;
      namesPointer = NULL;
      m_Status = bsFailed;
      return false;
    }
    // enough files?
    if (fileIndex>=m_FolderBlocks.at(dirIndex).files.size())
    {
      // set to start of next folder block
      ++dirIndex;
      fileIndex = 0;
      // another check is necessary here
      if (dirIndex >= m_FolderBlocks.size())
      {
        std::cerr << "BSA::grabFileNames: Error: not enough file blocks (2)!\n"
                  << "dir idx: "<<dirIndex<<", #blocks: "<<m_FolderBlocks.size()<<"\n"
                  << "char offset: "<<charOffset<<", ptr. len.: "<<m_Header.totalFileNameLength<<"\n"
                  << "names done: " << namesRead << "\n";
        delete[] namesPointer;
        namesPointer = NULL;
        m_Status = bsFailed;
        return false;
      }//if
    }//if next folder
    //now finally set it
    m_FolderBlocks.at(dirIndex).files.at(fileIndex).fileName = nextName;
    ++fileIndex;
    ++namesRead;
  }//while

  delete[] namesPointer;
  namesPointer = NULL;

  if (namesRead != m_Header.fileCount)
  {
    std::cerr << "BSA::grabFileNames: Error: number of read file names does not "
              << "match the number given in the header. " << m_Header.fileCount
              << " files should be there, but " << namesRead << " were found!\n";
    m_Status = bsFailed;
    return false;
  }

  m_Status = bsOpenFileNames;
  return true;
}

void BSA::listFileNames(bool withCompressionStatus)
{
  if (m_Status!=bsOpenFileNames)
  {
    std::cerr << "BSA::listFileNames: Error: BSA has wrong status for that operation!\n";
    return;
  }
  uint32_t i, j, compressedFiles, rawFiles;
  compressedFiles = rawFiles = 0;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    std::cout <<m_FolderBlocks.at(i).folderName<<"\n";
    for (j=0; j<m_FolderBlocks[i].files.size(); ++j)
    {
      std::cout <<"    ";
      if (withCompressionStatus)
      {
        if (isFileCompressed(i,j))
        {
          std::cout << "(cmp) ";
          ++compressedFiles;
        }
        else
        {
          std::cout << "(raw) ";
          ++rawFiles;
        }
      }//if compression info requested
      std::cout <<m_FolderBlocks[i].files.at(j).fileName<<"\n";
    }
    std::cout << "\n";
  }//for
  if (withCompressionStatus)
  {
    std::cout << "Compressed files in archive: "<<compressedFiles
              << "\nUncompressed files in archive: "<<rawFiles<<"\n";
  }
}

std::vector<BSA::DirectoryStruct> BSA::getDirectories() const
{
  std::vector<DirectoryStruct> result;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getDirectories: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return result;
  }
  DirectoryStruct tempStruct;
  unsigned int i;
  const unsigned int count = m_FolderBlocks.size();
  for (i=0; i<count; ++i)
  {
    tempStruct.index = i;
    tempStruct.name = m_FolderBlocks[i].folderName;
    result.push_back(tempStruct);
  }//for
  return result;
}

std::vector<BSA::FileStruct> BSA::getFilesOfDirectory(const uint32_t folderIndex, const bool fullName) const
{
  std::vector<FileStruct> result;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getFilesOfDirectory: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return result;
  }
  if (folderIndex>=m_FolderBlocks.size())
  {
    std::cerr << "BSA::getFilesOfDirectory: Error: invalid folder index!\n";
    return result;
  }
  FileStruct tempStruct;
  const unsigned int count = m_FolderBlocks[folderIndex].files.size();
  unsigned int i;
  for (i=0; i<count; ++i)
  {
    tempStruct.folderIndex = folderIndex;
    tempStruct.fileIndex = i;
    tempStruct.name = m_FolderBlocks[folderIndex].files[i].fileName;
    if (fullName)
    {
      tempStruct.name = m_FolderBlocks[folderIndex].folderName+'\\'+tempStruct.name;
    }
    tempStruct.compressed = isFileCompressed(folderIndex, i);
    result.push_back(tempStruct);
  }//for
  return result;
}

void BSA::close()
{
  if ((m_Status!=bsFresh) and (bsClosed!=m_Status))
  {
    m_Stream.close();
    m_Status = bsClosed;
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
  return (m_Status==bsOpenFileNames);
}

bool BSA::hasFolder(const std::string& folderName) const
{
  return (getIndexOfFolder(folderName)!=cIndexNotFound);
}

uint32_t BSA::getIndexOfFolder(std::string folderName) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexOfFolder: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return cIndexNotFound;
  }

  if (folderName.empty()) return cIndexNotFound;
  //transform to lower case
  folderName = lowerCase(folderName);
  uint32_t i;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    if (m_FolderBlocks.at(i).folderName==folderName) return i;
  }//for
  return cIndexNotFound;
}

uint32_t BSA::getIndexOfFile(const uint32_t folderIndex, std::string fileName) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexOfFile: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    return cIndexNotFound;
  }

  if (folderIndex>=m_FolderBlocks.size())
  {
    std::cerr << "BSA::getIndexOfFile: Error: folder index exceeds allowed maximum!\n";
    return cIndexNotFound;
  }

  fileName = lowerCase(fileName);
  uint32_t i;
  for (i=0; i<m_FolderBlocks[folderIndex].files.size(); ++i)
  {
    if (m_FolderBlocks[folderIndex].files.at(i).fileName==fileName) return i;
  }

  return cIndexNotFound;
}

bool BSA::getIndexPairForFile(const std::string& fileName, uint32_t& folderIndex, uint32_t& fileIndex) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::getIndexPairForFile: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    folderIndex = cIndexNotFound;
    fileIndex = cIndexNotFound;
    return false;
  }

  const std::string::size_type delimPos = fileName.rfind('\\');
  if (delimPos==std::string::npos)
  {
    std::cerr << "BSA::getIndexPairForFile: Error: file name contains no folder!\n";
    folderIndex = cIndexNotFound;
    fileIndex = cIndexNotFound;
    return false;
  }
  if (delimPos==fileName.length()-1)
  {
    std::cerr << "BSA::getIndexPairForFile: Error: given file name only contains directory!\n";
    folderIndex = cIndexNotFound;
    fileIndex = cIndexNotFound;
    return false;
  }

  folderIndex = getIndexOfFolder(fileName.substr(0, delimPos));
  if (folderIndex==cIndexNotFound)
  {
    //no such folder, thus no file in that folder
    fileIndex = cIndexNotFound;
    return true;
  }

  fileIndex = getIndexOfFile(folderIndex, fileName.substr(delimPos+1));
  return true;
}

bool BSA::hasFile(const std::string& fileName) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::hasFile: Error: not all structure data is present to "
              << "properly fulfill the requested operation!\n";
    return cIndexNotFound;
  }

  uint32_t folderIdx, fileIdx;
  if (!getIndexPairForFile(fileName, folderIdx, fileIdx)) return false;

  return ((folderIdx!=cIndexNotFound) and (fileIdx!=cIndexNotFound));
}

bool BSA::isValidIndexPair(const uint32_t folderIndex, const uint32_t fileIndex) const
{
  if ((folderIndex==cIndexNotFound) or (fileIndex==cIndexNotFound)
      or (folderIndex>=m_FolderBlocks.size()))
  {
    return false;
  }

  if (m_FolderBlocks.at(folderIndex).files.size()<=fileIndex)
  {
    return false;
  }

  return true;
}

bool BSA::isFileCompressed(const uint32_t folderIndex, const uint32_t fileIndex) const
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::isFileCompressed: Error: not all structure data is "
              << "present to properly fulfill the requested operation!\n";
    throw std::runtime_error(std::string("BSA::isFileCompressed: Error: not ")
              +"all structure data is present to properly fulfill the requested operation!");
    return false;
  }

  if (!isValidIndexPair(folderIndex, fileIndex))
  {
    std::cerr << "BSA::isFileCompressed: Error: invalid indices given!\n";
    throw std::invalid_argument("BSA::isFileCompressed: Error: invalid indices given!");
    return false;
  }

  return (m_Header.filesCompressedByDefault() xor m_FolderBlocks.at(folderIndex).files.at(fileIndex).isCompressionToggled());
}

bool BSA::extractFile(const uint32_t folderIndex, const uint32_t fileIndex, const std::string& outputFileName)
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFile: Error: not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  if (!isValidIndexPair(folderIndex, fileIndex))
  {
    std::cerr << "BSA::extractFile: Hint: file is not in the archive!\n";
    return false;
  }

  const uint32_t extractedFileSize = m_FolderBlocks[folderIndex].files[fileIndex].getRealFileSize();
  m_Stream.seekg(m_FolderBlocks[folderIndex].files[fileIndex].offset, std::ios_base::beg);
  if (!m_Stream.good())
  {
    std::cerr << "BSA::extractFile: Error: bad internal stream, could not jump to file's offset!\n";
    return false;
  }

  uint8_t * buffer = NULL; //buffer for output data
  if (isFileCompressed(folderIndex, fileIndex))
  {
    if (extractedFileSize<4)
    {
      std::cerr << "BSA::extractFile: Error: size is too small to contain any compressed data!\n";
      return false;
    }
    uint32_t decompSize = 0;
    //read size of decompressed file
    m_Stream.read((char*) &decompSize, 4);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: could not read file's uncompressed size!\n";
      return false;
    }
    //read compressed stuff
    uint8_t * compressedBuffer = new uint8_t[extractedFileSize-4];
    m_Stream.read((char*) compressedBuffer, extractedFileSize-4);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: could not read compressed file data from archive!\n";
      delete [] compressedBuffer;
      return false;
    }
    //allocate buffer for decompressed data
    buffer = new uint8_t[decompSize];
    if (!MWTP::zlibDecompress(compressedBuffer, extractedFileSize-4, buffer, decompSize))
    {
      std::cerr << "BSA::extractFile: Error: decompression failed!\n";
      delete [] compressedBuffer;
      delete [] buffer;
      return false;
    }
    //success, delete compressed data buffer
    delete [] compressedBuffer;
    compressedBuffer = NULL;
  }
  else
  {
    //handle uncompressed data
    buffer = new uint8_t[extractedFileSize];
    m_Stream.read((char*) buffer, extractedFileSize);
    if (!m_Stream.good())
    {
      std::cerr << "BSA::extractFile: Error: bad internal stream, could read file data from archive!\n";
      delete[] buffer;
      buffer = NULL;
      return false;
    }
  }

  std::ofstream outputStream;
  outputStream.open(outputFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!outputStream)
  {
    std::cerr << "BSA::extractFile: Error: could not open/create file \""
              <<outputFileName<<"\" for writing!\n";
    delete[] buffer;
    buffer = NULL;
    return false;
  }

  outputStream.write((const char*) buffer, extractedFileSize);
  if (!outputStream.good())
  {
    std::cerr << "BSA::extractFile: Error: could not write data to file \""
              <<outputFileName<<"\"!\n";
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  outputStream.close();
  //free buffer
  delete[] buffer;
  buffer = NULL;
  return true;
}

bool BSA::extractFile(const std::string& inArchiveFileName, const std::string& outputFileName)
{
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFile: Error: not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  uint32_t folderIndex, fileIndex;
  getIndexPairForFile(inArchiveFileName, folderIndex, fileIndex);

  //use the above function for the rest
  return extractFile(folderIndex, fileIndex, outputFileName);
}

bool BSA::extractFolder(const uint32_t folderIndex, const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFolder: Error: not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  if ((folderIndex==cIndexNotFound) or (folderIndex>=m_FolderBlocks.size()))
  {
    std::cerr << "BSA::extractFolder: Error: invalid folder index!\n";
    return false;
  }

  //check for output directory
  if (!directoryExists(outputDirName))
  {
    if (!createDirectoryRecursive(outputDirName))
    {
      std::cerr << "BSA::extractFolder: Error: Could not create destination directory \""
                << outputDirName << "\".\n";
      return false;
    }
  }

  uint32_t file_index;
  //now extract each file in that directory
  for (file_index=0; file_index<m_FolderBlocks[folderIndex].files.size(); ++file_index)
  {
    if (!extractFile(folderIndex, file_index, outputDirName+MWTP::pathDelimiter
         +m_FolderBlocks[folderIndex].files[file_index].fileName))
    {
      std::cerr << "BSA::extractFolder: Error: Could not extract file \""
                << m_FolderBlocks[folderIndex].folderName+MWTP::pathDelimiter
                  +m_FolderBlocks[folderIndex].files[file_index].fileName<< "\".\n";
      return false;
    }//if
    ++extractedFileCount;
  }//for file_index

  return true;
}

bool BSA::extractFolder(const std::string& folderName, const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractFolder: Error: not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  const uint32_t fIdx = getIndexOfFolder(folderName);
  if (fIdx==cIndexNotFound)
  {
    std::cerr << "BSA::extractFolder: Error: archive has no folder named \""
              << folderName <<"\", thus it cannot be extracted!\n";
    return false;
  }

  return extractFolder(fIdx, outputDirName, extractedFileCount);
}

bool BSA::extractAll(const std::string& outputDirName, uint32_t& extractedFileCount)
{
  extractedFileCount = 0;
  if (!hasAllStructureData())
  {
    std::cerr << "BSA::extractAll: Error: not all structure data is present "
              << "to properly fulfill the requested operation!\n";
    return false;
  }

  //check for output directory
  if (!directoryExists(outputDirName))
  {
    if (!createDirectoryRecursive(outputDirName))
    {
      std::cerr << "BSA::extractAll: Error: Could not create destination directory \""
                << outputDirName << "\".\n";
      return false;
    }
  }

  uint32_t i, j;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    //create output directory, if necessary
    if (!directoryExists(outputDirName+MWTP::pathDelimiter+m_FolderBlocks[i].folderName))
    {
      if (!createDirectoryRecursive(outputDirName+MWTP::pathDelimiter+m_FolderBlocks[i].folderName))
      {
        std::cerr << "BSA::extractAll: Error: Could not create destination subdirectory \""
                  << outputDirName+MWTP::pathDelimiter+m_FolderBlocks[i].folderName << "\".\n";
        return false;
      }
    }
    //now extract each file in that directory
    for (j=0; j<m_FolderBlocks[i].files.size(); ++j)
    {
      if (!extractFile(i, j, outputDirName+MWTP::pathDelimiter+m_FolderBlocks[i].folderName
           +MWTP::pathDelimiter+m_FolderBlocks[i].files[j].fileName))
      {
        std::cerr << "BSA::extractAll: Error: Could not extract file \""
                  <<m_FolderBlocks[i].folderName+MWTP::pathDelimiter
                    +m_FolderBlocks[i].files[j].fileName<< "\".\n";
        return false;
      }//if
      ++extractedFileCount;
    }//for j
  }//for i

  return true;
}

} //namespace
