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

REM Generator for use with CMake. Change, if you have a different VS version.
SET CMAKE_GENERATOR=Visual Studio 16 2019
SET CMAKE_ARCH=Win32

REM find CMake binary
SET CMAKER=NotFound

REM Test some default paths with different drive letters.
IF EXIST "C:\Program Files\CMake\bin\cmake.exe" SET CMAKER=C:\Program Files\CMake\bin\cmake.exe
IF EXIST "D:\Program Files\CMake\bin\cmake.exe" SET CMAKER=D:\Program Files\CMake\bin\cmake.exe
IF EXIST "E:\Program Files\CMake\bin\cmake.exe" SET CMAKER=E:\Program Files\CMake\bin\cmake.exe

IF "%CMAKER%"=="NotFound" (
  echo CMake binary was not found!
  echo Please install CMake 3.0 or later.
  exit /B 1
)

echo Info: CMake path is %CMAKER%.
