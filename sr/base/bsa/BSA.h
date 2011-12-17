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

    /* tries to read the folder records and returns true in case of success */
    bool grabFolderData();

    /* tries to read the folder blocks and returns true in case of success */
    bool grabFolderBlocks();

    /* tries to read the file names and returns true in case of success */
    bool grabFileNames();

    void listFileNames();
  protected:
    enum Status {bsFresh, bsOpen, bsClosed, bsFailed};
    Status m_Status;
    std::ifstream m_Stream;
    BSAHeader m_Header;
    std::vector<BSAFolderRecord> m_Folders;
    std::vector<BSAFolderBlock> m_FolderBlocks;
}; //struct

} //namespace

#endif // SR_BSA_H
