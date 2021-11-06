@echo off

::------------------------------------------------------------------------------
::  This file is part of the Morrowind Tools Project.
::  Copyright (C) 2021  Dirk Stolle

::  This program is free software: you can redistribute it and/or modify
::  it under the terms of the GNU General Public License as published by
::  the Free Software Foundation, either version 3 of the License, or
::  (at your option) any later version.

::  This program is distributed in the hope that it will be useful,
::  but WITHOUT ANY WARRANTY; without even the implied warranty of
::  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
::  GNU General Public License for more details.

::  You should have received a copy of the GNU General Public License
::  along with this program.  If not, see <http://www.gnu.org/licenses/>.
::------------------------------------------------------------------------------


REM Build script for MWTP on Windows systems using Microsoft Visual C++.
REM This script is still experimental.
REM

REM find vsdevcmd.bat, part of Visual Studio
SET DEVCMD=NotFound
IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdCPF14
IF EXIST "C:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdCP14
IF EXIST "D:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdDPF14
IF EXIST "D:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdDP14
IF EXIST "E:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdEPF14
IF EXIST "E:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" GOTO DevCmdEP14

IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" GOTO DevCmdCPF2019
IF EXIST "D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" GOTO DevCmdDPF2019
IF EXIST "E:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" GOTO DevCmdEPF2019

REM Not found
GOTO DevCmdFoundCheck

:DevCmdCPF14
SET DEVCMD=C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdCP14
SET DEVCMD=C:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdDPF14
SET DEVCMD=D:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdDP14
SET DEVCMD=D:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdEPF14
SET DEVCMD=E:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdEP14
SET DEVCMD=E:\Programme (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdCPF2019
SET DEVCMD=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdDPF2019
SET DEVCMD=D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdEPF2019
SET DEVCMD=E:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat
GOTO DevCmdFoundCheck

:DevCmdFoundCheck

IF "%DEVCMD%"=="NotFound" (
  echo vsdevcmd.bat was not found!
  echo Please install VisualStudio 2019 Community Edition or later.
  exit /B 1
)

echo Info: VsDevCmd.bat is at %DEVCMD%.
