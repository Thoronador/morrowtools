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

#ifndef SR_ESMREADERKEYWORDS_H
#define SR_ESMREADERKEYWORDS_H

#include "ESMReaderSingleType.h"
#include "records/KeywordRecord.h"
#include "Keywords.h"
#include "SR_Constants.h"

namespace SRTP
{

/* This descendant of the ESMReader class tries to read all keyword records from
   the given .esm/.esp file.
*/
typedef ESMReaderSingleType<KeywordRecord, Keywords, cKYWD> ESMReaderKeywords;

} //namespace

#endif // SR_ESMREADERKEYWORDS_H
