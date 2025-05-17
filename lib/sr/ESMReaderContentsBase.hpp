/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022, 2025  Dirk Stolle

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

#ifndef SR_ESMREADERCONTENTSBASE_HPP
#define SR_ESMREADERCONTENTSBASE_HPP

#include "ESMReader.hpp"
#include "ESMFileContents.hpp"

namespace SRTP
{

class ESMReaderContentsBase: public ESMReader
{
  public:
    /** \brief Constructs a new reader based on a file contents structure.
     *
     * \param storage  reference to the structure that will hold the records and
     *                 groups that will be read. This reference has to live as
     *                 long as the ESMReaderContentsBase instance, at least.
     */
    ESMReaderContentsBase(ESMFileContents& storage);

    virtual ~ESMReaderContentsBase();
  protected:
    /** \brief This function's sole purpose is to "notify" the reader that a
     *         new group was encountered and give the classes derived from
     *         ESMReader the possibility to update their internal state
     *         accordingly. Note that this function will only be called for
     *         groups where needGroup() returns true.
     *         Skipped groups will not trigger that function.
     *         Shall return false only if the read process should be aborted.
     * \param g_data  the group header data
     * \param sub     if set to true, the new group is a sub-group of another
     *                group
     * \return Shall returns true, if reading from the stream shall continue.
     *         Shall return false, only if the read process shall be aborted.
     */
    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub) override;

    /** \brief This function's sole purpose is to "notify" the reader that a
     *         started group has been read (or skipped) completely and give the
     *         classes derived from ESMReader the possibility to update their
     *         internal state accordingly.
     * \remarks
     * Note that this function will only be called for groups where needGroup()
     * returns true. Skipped groups will not trigger that function, but it might
     * be possible that all records and subgroups in such a group have been
     * skipped, depending on the implementation of readNextRecord() and
     * readGroup().
     * Shall return false only if the read process should be aborted.
     * \param g_data  the group header data
     * \return Shall returns true, if reading from the stream shall continue.
     *         Shall return false, only if the read process shall be aborted.
     */
    virtual bool groupFinished(const GroupData& g_data) override;

    /** reference to the structure where the read data will be stored */
    ESMFileContents& contents;

    /* internal group nesting level counter and pointer to latest group - should
       be made private later on */
    unsigned int m_InternalGroupLevel;
    Group * m_InternalGroup;
}; // class

} // namespace

#endif // SR_ESMREADERCONTENTSBASE_HPP
