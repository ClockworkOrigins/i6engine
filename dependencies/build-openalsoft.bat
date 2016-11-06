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

call build-common.bat %1 %2

Set ARCHIVE=openal-soft-1.17.2.tar.bz2
Set BUILD_DIR=%TMP_DIR%/openal-soft-1.17.2
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/OpenALSoft

IF EXIST %PREFIX% EXIT /B

echo "Compile OpenAL Soft"

echo "Extracting OpenAL Soft"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Configuring OpenAL Soft"

cd %BUILD_DIR%\build
cmake -DALSOFT_NO_CONFIG_UTIL=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" ..

echo "Building OpenAL Soft"

MSBuild.exe OpenAL.sln /p:Configuration=Release

echo "Installing OpenAL Soft"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"

