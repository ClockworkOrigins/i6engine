@echo OFF

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

call build-common.bat %1 %2

Set ARCHIVE=lua-5.3.3.tar.gz
Set BUILD_DIR=%TMP_DIR%/lua-5.3.3
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/lua/

IF EXIST %PREFIX% EXIT /B

echo "Compile Lua"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %BUILD_DIR%

echo "Configuring Lua"

xcopy /F "%DEP_DIR%\..\ext\lua\CMakeLists.txt" "%BUILD_DIR%\CMakeLists.txt*" > NUL

cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" .

echo "Building Lua"
MSBuild.exe lua.sln /p:Configuration=Release

echo "Installing Lua"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
