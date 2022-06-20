/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2022  Dirk Stolle

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

#ifndef SR_BSA_HPP
#define SR_BSA_HPP

#include "BSAHeader.hpp"
#include "BSAFolderRecord.hpp"
#include "BSAFolderBlock.hpp"
#include <filesystem>
#include <optional>
#include <vector>

namespace SRTP
{

struct BSA
{
  public:
    BSA();

    ~BSA();

    /** \brief Opens the given BSA file and reads its header.
     *
     * \param fileName name of the BSA file that shall be opened
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool open(const std::filesystem::path& fileName);

    void close();

    /** \brief Gets the header data. Note that this is only
     *
     * \return Returns the header data.
     * \remarks Note that this information is only accurate, after open() has
     *          been called successfully.
     */
    const BSAHeader& getHeader() const;

    /** \brief Gets the folder record information.
     *
     * \return Returns the folder record information.
     * \remarks This information is only accurate, if either grabFolderData() or
     *          grabAllStructureData() has been called successfully.
     */
    const std::vector<BSAFolderRecord>& getFolders() const;

    /** \brief Gets the folder block information.
     *
     * \return Returns the folder block information.
     * \remarks This information is only accurate, if either grabFolderBlocks() or
     *          grabAllStructureData() has been called successfully.
     */
    const std::vector<BSAFolderBlock>& getFolderBlocks() const;

    /** \brief Checks whether all structural data was read and the BSA is ready
     *         for file extractions.
     *
     * \return Returns true, if the BSA has all structural data.
     *         Returns false otherwise.
     */
    bool hasAllStructureData() const;

    /** \brief Reads all structural data. and returns true in case of success
     *
     * \return Returns true in case of success. Returns false, if an error
     *         occurred.
     * \remarks The function basically calls grabFolderData(), grabFolderBlocks()
     *          and grabFileNames() in sequence and returns once they are all
     *          done or when an error occurs - whichever comes first.
     */
    bool grabAllStructureData();

    /** \brief Checks whether the given folder is in the archive.
     *
     * \param folderName  name of the folder (should be all lower case)
     * \return Returns true, if the folder exists in the archive.
     *         Returns false otherwise.
     */
    bool hasFolder(const std::string& folderName) const;

    /** \brief Gets the index of the give folder in the archive.
     *
     * \param folderName  name of the folder (should be all lower case)
     * \return Returns the index of the folder, if the given folder is in the
     *         archive. Returns an empty optional otherwise.
     */
    std::optional<uint32_t> getIndexOfFolder(std::string folderName) const;

    /** \brief Gets the index of the file within the given folder block.
     *
     * \param folderIndex  the index of the folder block
     * \param fileName     name of the file without directory path (should be
     *                     all lower case)
     * \return Returns the index of the file within the given folder block, if
     *         the given folder and file are in the archive.
     *         Returns an empty optional otherwise.
     */
    std::optional<uint32_t> getIndexOfFile(const uint32_t folderIndex, std::string fileName) const;

    /** \brief Set @folderIndex and @fileIndex to the folder and file indexes
     *         for the specified file.
     *
     * \param fileName     name of the file in the archive, including full path
     * \param folderIndex  optional of uint32_t that will hold the folder's index
     * \param fileIndex    optional of uint32_t that will hold the file's index within that folder
     * \return Returns true in case of success. Returns false in case of error,
     *         e.g. if the archive has not been opened yet.
     * \remark The boolean return value does NOT indicate, whether the specified
     *         file is in the archive or not; it just indicates, whether or not
     *         the function could be executed successfully.
     *         If the given file exists in the archive, both @folderIndex and
     *         @fileIndex will not be empty optionals.
     */
    bool getIndexPairForFile(const std::string& fileName, std::optional<uint32_t>& folderIndex, std::optional<uint32_t>& fileIndex) const;

    /** \brief Checks whether the given file is in the archive.
     *
     * \param fileName  name of the file, including directory path (should be
     *        all lower case)
     * \return Returns true, if the given file is in the archive.
     *         Returns false otherwise.
     */
    bool hasFile(const std::string& fileName) const;

    /** \brief Checks whether a given file is compressed.
     *
     * \param folderIndex  index of the folder
     * \param fileIndex    index of the file within the folder
     * \return Returns true, if the file at the given indexes is compressed.
     *         Returns false otherwise.
     * \throws std::runtime_error if indices are invalid or the structural data
     *         required to determine the compression status is missing
     */
    bool isFileCompressed(const uint32_t folderIndex, const uint32_t fileIndex) const;

    /** \brief Reads the folder records.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabFolderData();

    /** \brief Reads the folder blocks.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabFolderBlocks();

    /** \brief Reads the file names.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabFileNames();

    /** \brief Lists all files within the archive.
     *
     * \param withCompressionStatus  if set to true, it will also be listed,
     *                               whether or not a file is compressed
     */
    void listFileNames(bool withCompressionStatus);

    /** Gets all directories within the archive.
     *
     * \returns Returns a vector containing all directory names within the archive.
     * \remarks Only works properly, if either grabAllStructureData() or both of
     *          grabFolderData() and grabFolderBlocks() have been called with
     *          success.
     */
    std::vector<std::string> getDirectories() const;

    /** \brief Extracts the file with the given indexes and writes it to the
     *         specified destination.
     *
     * \param folderIndex     folder index of the wanted file
     * \param fileIndex       file index of the wanted file
     * \param outputFileName  name of the destination file on HDD
     * \return Returns true in case of success, false on failure.
     */
    bool extractFile(const uint32_t folderIndex, const uint32_t fileIndex, const std::string& outputFileName);

    /** \brief Extracts the file with the given file name and writes it to the
     * specified destination.
     *
     * \param inArchiveFileName  name of the file in the archive, including full path
     * \param outputFileName     name of the destination file on HDD
     * \return Returns true in case of success. Returns false on failure.
     */
    bool extractFile(const std::string& inArchiveFileName, const std::string& outputFileName);

    /** \brief Extracts all files from the given archive folder and places them
     * in the given output directory.
     *
     * \param folderIndex         index of the folder
     * \param outputDirName       path of the output directory, without
     *                            (back)slash at the end
     * \param extractedFileCount  variable that will contain the number of
     *                            extracted files
     * \return Returns true in case of success.
     *         Returns false if an error occurred.
     */
    bool extractFolder(const uint32_t folderIndex, const std::string& outputDirName, uint32_t& extractedFileCount);

    /** \brief Extracts all files from the given archive folder and places them
     * in the given output directory.
     *
     * \param folderName          name of the folder in the archive
     * \param outputDirName       path of the output directory, without
     *                            (back)slash at the end
     * \param extractedFileCount  variable that will contain the number of
     *                            extracted files
     * \return Returns true in case of success.
     *         Returns false if an error occurred.
     */
    bool extractFolder(const std::string& folderName, const std::string& outputDirName, uint32_t& extractedFileCount);

    /** \brief Extracts all files from the archive and places them in the given
     * output directory.
     *
     * \param outputDirName       path of the output directory, without
     *                            (back)slash at the end
     * \param extractedFileCount  variable that will contain the number of
     *                            extracted files
     * \return Returns true in case of success.
     *         Returns false if an error occurred.
     */
    bool extractAll(const std::string& outputDirName, uint32_t& extractedFileCount);
  protected:
    /** \brief Check whether the given indexes identify a valid index pair.
     *
     * \param folderIndex  index of the folder
     * \param fileIndex    index of the file within the folder
     * \return Returns true, if the given indexes identify a valid index pair.
     *         Returns false otherwise.
     */
    bool isValidIndexPair(const uint32_t folderIndex, const uint32_t fileIndex) const;

    /// enumeration type for internal status
    enum class Status { Fresh, Open, OpenFolderData, OpenFolderBlocks,
                        OpenFileNames, Closed, Failed };

    Status m_Status; /**< internal status */

    std::ifstream m_Stream; /**< file stream associated with the archive after open() was called */

    // data read from the stream...
    BSAHeader m_Header;
    std::vector<BSAFolderRecord> m_Folders;
    std::vector<BSAFolderBlock> m_FolderBlocks;
}; // struct

} // namespace

#endif // SR_BSA_HPP
