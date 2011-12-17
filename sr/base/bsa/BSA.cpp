/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "BSA.h"
#include <iostream>

namespace SRTP
{

BSA::BSA()
{
  m_Status = bsFresh;
  m_Folders.clear();
}

BSA::~BSA()
{
  close();
}

bool BSA::open(const std::string& FileName)
{
  if (m_Status==bsOpen)
  {
    std::cout << "BSA::open: Error: BSA was already opened!\n";
    return false;
  }
  m_Stream.open(FileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!m_Stream)
  {
    std::cout << "BSA::open: Error while opening file \""<<FileName<<"\".\n";
    m_Status = bsClosed;
    return false;
  }
  //file opened, now read the header
  if (!m_Header.loadFromStream(m_Stream))
  {
    std::cout << "BSA::open: Error while reading header!\n";
    m_Stream.close();
    m_Status = bsClosed;
    return false;
  }

  //header was read successfully
  m_Status = bsOpen;
  return true;
}

bool BSA::grabFolderData()
{
  if (m_Status!=bsOpen)
  {
    std::cout << "BSA::grabFolderData: Error: BSA was not opened!\n";
    return false;
  }

  m_Folders.clear();
  uint32_t i;
  BSAFolderRecord tempFolder;
  for (i=0; i<m_Header.folderCount; ++i)
  {
    if (!tempFolder.loadFromStream(m_Stream))
    {
      std::cout << "BSA::grabFolderData: Error while reading folders!\n";
      m_Status = bsFailed;
      return false;
    }
    m_Folders.push_back(tempFolder);
  }//for

  return m_Stream.good();
}

bool BSA::grabFolderBlocks()
{
  if (m_Status!=bsOpen)
  {
    std::cout << "BSA::grabFolderBlocks: Error: BSA was not opened!\n";
    return false;
  }

  m_FolderBlocks.clear();

  uint32_t i;
  BSAFolderBlock tempFolderBlock;
  for (i=0; i<m_Folders.size(); ++i)
  {
    if (!tempFolderBlock.loadFromStream(m_Stream, m_Folders.at(i).count))
    {
      std::cout << "BSA::grabFolderBlocks: Error while reading!\n";
      m_Status = bsFailed;
      return false;
    }
    m_FolderBlocks.push_back(tempFolderBlock);
  }//for

  std::cout << "Info: Current stream offset is "<<(unsigned int) m_Stream.tellg()<<" bytes.\n";
  return m_Stream.good();
}

bool BSA::grabFileNames()
{
  if (m_Status!=bsOpen)
  {
    std::cout << "BSA::grabFileNames: Error: BSA was not opened!\n";
    return false;
  }

  if (!m_Stream.good())
  {
    std::cout << "BSA::grabFileNames: Error: bad stream!\n";
    return false;
  }

  //check for existence of file blocks
  if (m_FolderBlocks.empty())
  {
    std::cout << "BSA::grabFileNames: Error: internal folder blocks are missing!\n";
    return false;
  }
  //...and now check for files
  unsigned int i;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    if (m_FolderBlocks.at(i).files.empty())
    {
      std::cout << "BSA::grabFileNames: Error: internal folder block #"<<i<<" contains no files!\n";
      return false;
    }
  }//for
  //now allocate the space for the file names
  char * namesPointer = new char[m_Header.totalFileNameLength+1];
  memset(namesPointer, 0, m_Header.totalFileNameLength+1);

  //read it all in one go!
  m_Stream.read(namesPointer, m_Header.totalFileNameLength);
  if (!m_Stream.good())
  {
    std::cout << "BSA::grabFileNames: Error while reading name list!\n";
    delete[] namesPointer;
    namesPointer = NULL;
    return false;
  }

  //now associate the names with their file records
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
      std::cout << "BSA::grabFileNames: Error: not enough file blocks!\n"
                << "dir idx: "<<dirIndex<<", #blocks: "<<m_FolderBlocks.size()<<"\n";
      delete[] namesPointer;
      namesPointer = NULL;
      return false;
    }
    //enough files?
    if (fileIndex>=m_FolderBlocks.at(dirIndex).files.size())
    {
      //set to start of next folder block
      ++dirIndex;
      fileIndex = 0;
      //another check is neccessary here
      if (dirIndex>=m_FolderBlocks.size())
      {
        std::cout << "BSA::grabFileNames: Error: not enough file blocks (2)!\n"
                  << "dir idx: "<<dirIndex<<", #blocks: "<<m_FolderBlocks.size()<<"\n"
                  << "char offset: "<<charOffset<<", ptr. len.: "<<m_Header.totalFileNameLength<<"\n"
                  << "names done: "<<namesRead<<"\n";
        delete[] namesPointer;
        namesPointer = NULL;
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

  if (namesRead!=m_Header.fileCount)
  {
    std::cout << "BSA::grabFileNames: Error: number of read file names does not "
              << "match the number given in the header. "<<m_Header.fileCount
              << " files shoud be there, but "<<namesRead<<" were found!\n";
    return false;
  }

  return true;
}

void BSA::listFileNames()
{
  uint32_t i, j;
  for (i=0; i<m_FolderBlocks.size(); ++i)
  {
    std::cout <<m_FolderBlocks.at(i).folderName<<"\n";
    for (j=0; j<m_FolderBlocks[i].files.size(); ++j)
    {
      std::cout <<"    "<<m_FolderBlocks[i].files.at(j).fileName<<"\n";
    }
    std::cout << "\n";
  }//for

}

void BSA::close()
{
  if ((m_Status==bsOpen) or (bsFailed==m_Status))
  {
    m_Stream.close();
  }
  m_Status = bsClosed;
}

const BSAHeader& BSA::getHeader() const
{
  return m_Header;
}

const std::vector<BSAFolderRecord>& BSA::getFolders() const
{
  return m_Folders;
}

} //namespace
