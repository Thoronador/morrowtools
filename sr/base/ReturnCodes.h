/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_RETURNCODES_H
#define SR_RETURNCODES_H

#include "../../mw/base/ReturnCodes.h"

namespace SRTP
{

/*general return codes */
const int rcInvalidParameter = MWTP::rcInvalidParameter; //1
const int rcFileError = MWTP::rcFileError; //2
const int rcDataError = MWTP::rcDataError; //3
const int rcOutputFailed = MWTP::rcOutputFailed; //4

/* return code currently only used by conv_cams */
const int rcNoChange = MWTP::rcNoChange; //9

/* special return codes for formID_finder */
const int rcWindowNotFound = 10;
const int rcSendDataNotSupported = 11;
const int rcSocketError = 12;

} //namespace

#endif // SR_RETURNCODES_H
