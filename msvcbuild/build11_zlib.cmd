@echo off

::------------------------------------------------------------------------------
::  This file is part of the Morrowind Tools Project.
::  Copyright (C) 2021, 2022  Dirk Stolle

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

cd ..\third-party\zlib
IF %ERRORLEVEL% == 1 (
  echo Could not change to zlib directory!
  exit /B 1
)

IF EXIST source\zlib.h (
  echo zlib source already exists.
) ELSE (
  git clone https://github.com/madler/zlib.git --depth=1 --branch=v1.2.12 .\source
  IF %ERRORLEVEL% == 1 (
    echo Could not get zlib sources via Git!
    exit /B 1
  )
)

cd source
IF %ERRORLEVEL% == 1 (
  echo Could not change to zlib source directory!
  exit /B 1
)

nmake -f win32\Makefile.msc
IF %ERRORLEVEL% == 1 (
  echo NMake build of zlib failed!
  exit /B 1
)

:: copy include files
copy zconf.h ..\include
IF %ERRORLEVEL% == 1 (
  echo Could not copy zconf.h of zlib!
  exit /B 1
)
copy zlib.h ..\include
IF %ERRORLEVEL% == 1 (
  echo Could not copy zlib.h of zlib!
  exit /B 1
)

:: copy library files
copy zdll.lib ..\lib
IF %ERRORLEVEL% == 1 (
  echo Could not copy zdll.lib of zlib!
  exit /B 1
)
copy zlib.lib ..\lib
IF %ERRORLEVEL% == 1 (
  echo Could not copy zlib.lib of zlib!
  exit /B 1
)
copy zlib.pdb ..\lib
IF %ERRORLEVEL% == 1 (
  echo Could not copy zlib.pdb of zlib!
  exit /B 1
)
copy zlib1.dll ..\lib
IF %ERRORLEVEL% == 1 (
  echo Could not copy zlib1.dll of zlib!
  exit /B 1
)
copy zlib1.pdb ..\lib
IF %ERRORLEVEL% == 1 (
  echo Could not copy zlib1.pdb of zlib!
  exit /B 1
)

cd ..
SET ZLIB_INCLUDE_DIR=%THIS_DIR%\..\third-party\zlib\include
SET ZLIB_LIBRARY=%THIS_DIR%\..\third-party\zlib\lib

echo Info: zlib include directory was set to %ZLIB_INCLUDE_DIR%.
echo Info: zlib library path was set to %ZLIB_LIBRARY%.
