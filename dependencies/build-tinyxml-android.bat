@echo OFF

REM
REM i6engine
REM Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
REM 
REM This file is part of i6engine; i6engine is free software; you can redistribute it and/or
REM modify it under the terms of the GNU Lesser General Public
REM License as published by the Free Software Foundation; either
REM version 2.1 of the License, or (at your option) any later version.
REM 
REM This library is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
REM Lesser General Public License for more details.
REM 
REM You should have received a copy of the GNU Lesser General Public
REM License along with this library; if not, write to the Free Software
REM Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
REM

call build-common.bat android

Set ARCHIVE=tinyxml2.zip
Set BUILD_DIR=%TMP_DIR%/tinyxml2
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/tinyxml2

IF EXIST %PREFIX% EXIT /B

echo "Compile tinyxml2"

echo "Extracting tinyxml2"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building tinyxml2"

cd %BUILD_DIR%\jni
xcopy /S /Y %DEP_DIR%\..\ext\patches\tinyxml2 .

call ndk-build
if %errorlevel% gtr 0 exit /b

echo "Installing tinyxml2"

mkdir "%PREFIX%"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include"

move tinyxml2.h %PREFIX%/include/tinyxml2.h
move ..\libs\armeabi\libtinyxml2.so %PREFIX%/lib/tinyxml2.so

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"

