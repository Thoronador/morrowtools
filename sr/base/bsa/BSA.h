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

#ifndef SR_BSA_H
#define SR_BSA_H

#include "BSAHeader.h"
#include "BSAFolderRecord.h"
#include "BSAFolderBlock.h"
#include <vector>

namespace SRTP
{

struct BSA
{
  public:
    /* constructor */
    BSA();

    /* destructor */
    ~BSA();

    /* tries to open the given BSA file and reads its header. Returns true in
       case of success.

       parameters:
           FileName - name of the BSA file that shall be opened
    */
    bool open(const std::string& FileName);

    void close();

    /* function to access the header record - read-only */
    const BSAHeader& getHeader() const;

    /* function to access the folder records - read-only */
    const std::vector<BSAFolderRecord>& getFolders() const;

    /* function to access the folder blocks - read-only */
    const std::vector<BSAFolderBlock>& getFolderBlocks() const;

    /* returns true, if all structural data was read and the BSA is ready for
       file extractions
    */
    bool hasAllStructureData() const;

    /* tries to read all structural data and returns true in case of success

       remarks:
           That function basically calls grabFolderData(), grabFolderBlocks()
           and grabFileNames() in sequence and returns once they are all done
           or when an error occurs - whichever comes first.
    */
    bool grabAllStructureData();

    /* returns true, if the given folder is in the archive

       parameters:
           folderName - name of the folder (should be all lower case)
    */
    bool hasFolder(const std::string& folderName) const;

    // constant representing the index for "not found"
    static const uint32_t cIndexNotFound;

    /* returns the index of the folder, if the given folder is in the archive.
       Returns cIndexNotFound otherwise.

       parameters:
           folderName - name of the folder (should be all lower case)
    */
    uint32_t getIndexOfFolder(std::string folderName) const;

    /* returns the index of the file within the given folder block, if the
       given folder and file are in the archive. Returns cIndexNotFound
       otherwise.

       parameters:
           folderIndex - the index of the folder block
           fileName    - name of the file without directory path (should be all
                         lower case)
    */
    uint32_t getIndexOfFile(const uint32_t folderIndex, std::string fileName) const;

    /* will set folderIndex and fileIndex to the given folder and file indices
       for the specified file and return true in case of success. Returns false
       in case of error, e.g. if the archive has not been opened yet.

       parameters:
           fileName    - name of the file in the archive, including full path
           folderIndex - uint32 that will hold the folder's index
           fileIndex   - uint32 that will hold the file's index within that folder

       remarks:
           The boolean return value does NOT indicate, whether the specified
           file is in the archive or not; it just indicates, whether or not the
           function could be executed successfully.
           If the given file exists in the archive, both folderIndex and
           fileIndex will not be cIndexNotFound.
    */
    bool getIndexPairForFile(const std::string& fileName, uint32_t& folderIndex, uint32_t& fileIndex) const;

    /* returns true, if the given file is in the archive

       parameters:
           fileName - name of the file, including directory path (should be all lower case)
    */
    bool hasFile(const std::string& fileName) const;

    /* returns true, if the file at the given indices is compressed, returns
       false otherwise

       parameters:
           folderIndex - index of the folder
           fileIndex   - index of the file within the folder
    */
    bool isFileCompressed(const uint32_t folderIndex, const uint32_t fileIndex) const;

    /* tries to read the folder records and returns true in case of success */
    bool grabFolderData();

    /* tries to read the folder blocks and returns true in case of success */
    bool grabFolderBlocks();

    /* tries to read the file names and returns true in case of success */
    bool grabFileNames();

    /* lists all files within the archive

       parameters:
           withCompression status - if set to true, it will also be listed, if
                                    a file is compressed or not
    */
    void listFileNames(bool withCompressionStatus);

    /* tries to extract the file with the given file name and writes it to the
       specified destination. Returns true in case of success, false on failure.

       parameters:
           inArchiveFileName - name of the file in the archive, including full path
           outputFileName    - name of the destination file on HDD
    */
    bool extractFile(const std::string& inArchiveFileName, const std::string& outputFileName);
  protected:
    /* returns true, if the given indices identify a valid index pair

       parameters:
           folderIndex - index of the folder
           fileIndex   - index of the file within the folder
    */
    bool isValidIndexPair(const uint32_t folderIndex, const uint32_t fileIndex) const;

    //enum for internal status
    enum Status {bsFresh, bsOpen, bsOpenFolderData, bsOpenFolderBlocks,
                 bsOpenFileNames, bsClosed, bsFailed};

    //the internal status
    Status m_Status;

    //the file stream associated with the file after open() was called
    std::ifstream m_Stream;

    //data read from the stream...
    BSAHeader m_Header;
    std::vector<BSAFolderRecord> m_Folders;
    std::vector<BSAFolderBlock> m_FolderBlocks;
}; //struct

} //namespace

#endif // SR_BSA_H
