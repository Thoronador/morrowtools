/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2022, 2023, 2023, 2024  Dirk Stolle

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
#include "BSADirectoryBlock.hpp"
#include "BSADirectoryRecord.hpp"
#include <filesystem>
#include <optional>
#include <ostream>
#include <unordered_set>
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

    /** \brief Gets the directory record information.
     *
     * \return Returns the directory record information.
     * \remarks This information is only accurate, if either grabDirectoryData() or
     *          grabAllStructureData() has been called successfully.
     */
    const std::vector<BSADirectoryRecord>& getDirectories() const;

    /** \brief Gets the directory block information.
     *
     * \return Returns the directory block information.
     * \remarks This information is only accurate, if either grabDirectoryBlocks() or
     *          grabAllStructureData() has been called successfully.
     */
    const std::vector<BSADirectoryBlock>& getDirectoryBlocks() const;

    /** \brief Checks whether all structural data was read and the BSA is ready
     *         for file extractions.
     *
     * \return Returns true, if the BSA has all structural data.
     *         Returns false otherwise.
     */
    bool hasAllStructureData() const;

    /** \brief Reads all structural data.
     *
     * \return Returns true in case of success. Returns false, if an error
     *         occurred.
     * \remarks The function basically calls grabDirectoryData(), grabDirectoryBlocks()
     *          and grabFileNames() in sequence and returns once they are all
     *          done or when an error occurs - whichever comes first.
     */
    bool grabAllStructureData();

    /** \brief Checks whether the given directory is in the archive.
     *
     * \param directoryName  name of the directory (should be all lower case)
     * \return Returns true, if the directory exists in the archive.
     *         Returns false otherwise.
     */
    bool hasDirectory(const std::string& directoryName) const;


    /** \brief Checks whether the given intermediate directory is in the archive.
     *
     * \remark An intermediate directory is a directory which does not directly
     *         exist in the archive but is part of the directory hierarchy. For
     *         example, an archive may have the directory "foo\bar\baz", but
     *         that does not mean that either "foo" or "foo\bar" have to exist
     *         as separate directories in the BSA. That can be a hurdle when
     *         one attempts to mirror the directory structure of the BSA to a
     *         real file system which, unlike the BSA format, does not allow to
     *         leave out those intermediate directories. This method tries to
     *         bridge that gap.
     * \param directoryName  name of the directory (should be all lower case)
     * \return Returns true, if the intermediate directory exists within the
     *         directory hierarchy of the archive.
     *         Returns false otherwise.
     */
    bool hasIntermediateDirectory(const std::string& directoryName) const;


    /** \brief Gets virtual sub-directories of a given directory.
     *
     * \remark A "virtual" sub-directory is a directory which may or may not
     *         exist in the archive, but is part of the directory hierarchy
     *         directly below the given directory. For example, an archive may
     *         have the directory "foo\bar\baz", but that does not mean that
     *         either "foo" or "foo\bar" have to exist as separate directories
     *         in the BSA. However, this method will return "bar" as a virtual
     *         sub-directory of "foo" and "baz" as a virtual sub-directory of
     *         "foo\bar" in that case.
     * \param directoryName  name of the directory (should be all lower case)
     * \return Returns a container containing all virtual sub-directories of a
     *         directory.
     */
    std::unordered_set<std::string> getVirtualSubDirectories(const std::string& directoryName);


    /** \brief Gets the index of the given directory in the archive.
     *
     * \param directoryName  name of the directory (should be all lower case)
     * \return Returns the index of the directory, if the given directory is in
     *         the archive. Returns an empty optional otherwise.
     */
    std::optional<uint32_t> getIndexOfDirectory(std::string directoryName) const;

    /** \brief Gets the index of the file within the given directory block.
     *
     * \param directoryIndex  the index of the directory block
     * \param fileName     name of the file without directory path (should be
     *                     all lower case)
     * \return Returns the index of the file within the given directory block,
     *         if the given directory and file are in the archive.
     *         Returns an empty optional otherwise.
     */
    std::optional<uint32_t> getIndexOfFile(const uint32_t directoryIndex, std::string fileName) const;

    /** \brief Set @directoryIndex and @fileIndex to the directory and file indexes
     *         for the specified file.
     *
     * \param fileName     name of the file in the archive, including full path
     * \param directoryIndex  optional of uint32_t that will hold the directory's index
     * \param fileIndex    optional of uint32_t that will hold the file's index within that directory
     * \return Returns true in case of success. Returns false in case of error,
     *         e.g. if the archive has not been opened yet.
     * \remark The boolean return value does NOT indicate, whether the specified
     *         file is in the archive or not; it just indicates, whether or not
     *         the function could be executed successfully.
     *         If the given file exists in the archive, both @directoryIndex and
     *         @fileIndex will not be empty optionals.
     */
    bool getIndexPairForFile(const std::string& fileName, std::optional<uint32_t>& directoryIndex, std::optional<uint32_t>& fileIndex) const;

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
     * \param directoryIndex  index of the directory
     * \param fileIndex    index of the file within the directory
     * \return Returns true, if the file at the given indexes is compressed.
     *         Returns false otherwise.
     * \throws std::runtime_error if indices are invalid or the structural data
     *         required to determine the compression status is missing
     */
    bool isFileCompressed(const uint32_t directoryIndex, const uint32_t fileIndex) const;

    /** \brief Gets the actual size of a file when it will be extracted.
     *
     * \param directoryIndex  index of the directory
     * \param fileIndex    index of the file within the directory
     * \return Returns the extracted file's size in bytes in case of success.
     *         Returns an empty optional, if the size could not be determined.
     */
    std::optional<uint32_t> getExtractedFileSize(const uint32_t directoryIndex, const uint32_t fileIndex);

    /** \brief Reads the directory records.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabDirectoryData();

    /** \brief Reads the directory blocks.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabDirectoryBlocks();

    /** \brief Reads the file names.
     *
     * \return Returns true in case of success. Returns false otherwise.
     */
    bool grabFileNames();

    /** \brief Lists all files within the archive.
     *
     * \param stream  the output stream where the listing shall appear
     * \param withCompressionStatus  if set to true, it will also be listed,
     *                               whether or not a file is compressed
     */
    void listFileNames(std::ostream& stream, bool withCompressionStatus);

    /** Gets all directory names within the archive.
     *
     * \returns Returns a vector containing all directory names within the archive.
     * \remarks Only works properly, if either grabAllStructureData() or both of
     *          grabDirectoryData() and grabDirectoryBlocks() have been called with
     *          success.
     */
    std::vector<std::string> getDirectoryNames() const;

    /** \brief Extracts the file with the given indexes and writes it to the
     *         specified destination.
     *
     * \param directoryIndex  directory index of the wanted file
     * \param fileIndex       file index of the wanted file
     * \param outputFileName  name of the destination file on HDD
     * \return Returns true in case of success, false on failure.
     */
    bool extractFile(const uint32_t directoryIndex, const uint32_t fileIndex, const std::string& outputFileName);

    /** \brief Extracts the file with the given file name and writes it to the
     * specified destination.
     *
     * \param inArchiveFileName  name of the file in the archive, including full path
     * \param outputFileName     name of the destination file on HDD
     * \return Returns true in case of success. Returns false on failure.
     */
    bool extractFile(const std::string& inArchiveFileName, const std::string& outputFileName);

    /** \brief Extracts all files from the given archive directory and places
     * them in the given output directory.
     *
     * \param directoryIndex      index of the directory
     * \param outputDirName       path of the output directory, without
     *                            (back)slash at the end
     * \param extractedFileCount  variable that will contain the number of
     *                            extracted files
     * \return Returns true in case of success.
     *         Returns false if an error occurred.
     */
    bool extractDirectory(const uint32_t directoryIndex, const std::string& outputDirName, uint32_t& extractedFileCount);

    /** \brief Extracts all files from the given archive directory and places
     * them in the given output directory.
     *
     * \param directoryName       name of the directory in the archive
     * \param outputDirName       path of the output directory, without
     *                            (back)slash at the end
     * \param extractedFileCount  variable that will contain the number of
     *                            extracted files
     * \return Returns true in case of success.
     *         Returns false if an error occurred.
     */
    bool extractDirectory(const std::string& directoryName, const std::string& outputDirName, uint32_t& extractedFileCount);

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
  private:
    /** \brief Check whether the given indexes identify a valid index pair.
     *
     * \param directoryIndex  index of the directory
     * \param fileIndex       index of the file within the directory
     * \return Returns true, if the given indexes identify a valid index pair.
     *         Returns false otherwise.
     */
    bool isValidIndexPair(const uint32_t directoryIndex, const uint32_t fileIndex) const;

    /** \brief Handles the embedded file name (if present) during file extraction.
     *
     * \param file_block_size  the size of the file block in bytes
     * \remarks When called, the internal stream offset must be directly at the
     *          size byte of the embedded file name, i. e. the beginning of the
     *          file block.
     * \return Returns the number of bytes that were skipped due to the embedded
     *         file name in case of success. Returns an empty optional, if an
     *         error occurred.
     */
    std::optional<uint32_t> handleEmbeddedFileName(const uint32_t file_block_size);

    /// enumeration type for internal status
    enum class Status { Fresh, Open, OpenDirectoryData, OpenDirectoryBlocks,
                        OpenFileNames, Closed, Failed };

    Status m_Status; /**< internal status */

    std::ifstream m_Stream; /**< file stream associated with the archive after open() was called */

    // data read from the stream...
    BSAHeader m_Header;
    std::vector<BSADirectoryRecord> m_Directories;
    std::vector<BSADirectoryBlock> m_DirectoryBlocks;
}; // struct

} // namespace

#endif // SR_BSA_HPP
