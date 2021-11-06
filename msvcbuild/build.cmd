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

SET THIS_DRIVE=%~d0
SET THIS_DIR=%~dp0
SET THIS_PATH=%~p0

%THIS_DRIVE%
cd %THIS_PATH%

CALL build01_cmake.cmd
IF %ERRORLEVEL% == 1 (
  echo Build script cannot find CMake!
  exit /B 1
)

CALL build02_devcmd.cmd
IF %ERRORLEVEL% == 1 (
  echo Build script cannot find Visual Studio 2019!
  exit /B 1
)
CALL "%DEVCMD%"
IF %ERRORLEVEL% == 1 (
  echo vsdevcmd.bat error!
  exit /B 1
)

:: build zlib
CALL build11_zlib.cmd
IF %ERRORLEVEL% == 1 (
  echo Build script cannot compile zlib library!
  exit /B 1
)

%THIS_DRIVE%
cd %THIS_PATH%

:: Now start the build here.
"%CMAKER%" -DZLIB_INCLUDE_DIR="%ZLIB_INCLUDE_DIR%" -DZLIB_LIBRARY="%ZLIB_LIBRARY%"\zlib.lib ..
nmake
