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

Set ARCHIVE=cegui-0.8.7.tar.bz2
Set DEP_ARCHIVE=cegui-deps-0.8.x-src.zip
Set BUILD_DIR=%TMP_DIR%/cegui-0.8.7
Set BUILD_DIR_DEPS=%TMP_DIR%/cegui-deps-0.8.x-src
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/cegui/
Set PREFIX_DEPS=%DEP_DIR%/%ARCH_DIR%/ceguideps

IF EXIST %PREFIX% EXIT /B

echo "Compile CEGUI"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack %DEP_ARCHIVE% %BUILD_DIR_DEPS%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack cegui_0_8_4_patch.zip %TMP_DIR%

cd %BUILD_DIR_DEPS%
xcopy /S /Y "%TMP_DIR%/cegui-deps/src" "%BUILD_DIR_DEPS%/src"

echo "Configuring CEGUIDeps"
cd %BUILD_DIR_DEPS%
cmake -DCMAKE_INSTALL_PREFIX:PATH=%PREFIX_DEPS% -DCEGUI_BUILD_CORONA=OFF -DCEGUI_BUILD_DEVIL=OFF -DCEGUI_BUILD_EFFECTS11=OFF -DCEGUI_BUILD_EXPAT=ON -DCEGUI_BUILD_FREEIMAGE=ON -DCEGUI_BUILD_FREETYPE2=ON -DCEGUI_BUILD_GLEW=OFF -DCEGUI_BUILD_GLFW=OFF -DCEGUI_BUILD_GLM=OFF -DCEGUI_BUILD_LUA=OFF -DCEGUI_BUILD_MINIZIP=OFF -DCEGUI_BUILD_PCRE=ON -DCEGUI_BUILD_SILLY=OFF -DCEGUI_BUILD_TINYXML=OFF -DCEGUI_BUILD_TOLUAPP=OFF -DCEGUI_BUILD_XERCES=OFF -DCEGUI_BUILD_ZLIB=OFF -G "%VSCOMPILER%%VSARCH%" .

echo "Building CEGUIDeps"
MSBuild.exe CEGUI-DEPS.sln /p:Configuration=Release > NUL
MSBuild.exe CEGUI-DEPS.sln /p:Configuration=Debug > NUL

echo "Installing CEGUIDeps"
mkdir "%PREFIX_DEPS%"

if not exist %PREFIX_DEPS% exit /b
xcopy /S /Y "%BUILD_DIR_DEPS%/dependencies" "%PREFIX_DEPS%" > NUL

echo "Cleaning up CEGUIDeps"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR_DEPS%"

mkdir "%BUILD_DIR%/dependencies"
xcopy /S /Y "%PREFIX_DEPS%" "%BUILD_DIR%/dependencies" > NUL

echo "Patching CEGUI"
cd %BUILD_DIR%

echo "Configuring CEGUI"
cd %BUILD_DIR%
cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DCEGUI_BUILD_XMLPARSER_XERCES=OFF -DCEGUI_BUILD_XMLPARSER_LIBXML2=OFF -DCEGUI_BUILD_XMLPARSER_RAPIDXML=OFF -DCEGUI_BUILD_XMLPARSER_TINYXML=OFF -DCEGUI_BUILD_XMLPARSER_EXPAT=ON -DCEGUI_BUILD_IMAGECODEC_SILLY=OFF -DCEGUI_BUILD_IMAGECODEC_DEVIL=OFF -DCEGUI_BUILD_IMAGECODEC_FREEIMAGE=ON -DCEGUI_BUILD_IMAGECODEC_CORONA=OFF -DCEGUI_BUILD_IMAGECODEC_STB=OFF -DCEGUI_BUILD_IMAGECODEC_TGA=ON -DCEGUI_BUILD_IMAGECODEC_PVR=OFF -DCEGUI_BUILD_RENDERER_OGRE=ON -DCEGUI_BUILD_RENDERER_DIRECT3D9=OFF -DCEGUI_BUILD_RENDERER_DIRECT3D10=OFF -DCEGUI_BUILD_RENDERER_DIRECT3D11=OFF -DCEGUI_BUILD_RENDERER_OPENGL=OFF -DCEGUI_BUILD_RENDERER_OPENGL3=OFF -DCEGUI_BUILD_RENDERER_IRRLICHT=OFF -DCEGUI_BUILD_RENDERER_NULL=OFF -DCEGUI_BUILD_RENDERER_OPENGLES=OFF -DCEGUI_BUILD_LUA_MODULE=OFF -DCEGUI_BUILD_LUA_GENERATOR=OFF -DCEGUI_BUILD_PYTHON_MODULES=OFF -DCEGUI_SAMPLES_ENABLED=OFF -DCEGUI_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=%DEP_DIR%/%ARCH_DIR%/boost/ -DOGRE_H_BUILD_SETTINGS_PATH=%DEP_DIR%/%ARCH_DIR%/ogre/include/OGRE/ -DOGRE_H_PATH=%DEP_DIR%/%ARCH_DIR%/ogre/include/OGRE/ -DOGRE_LIB=%DEP_DIR%/%ARCH_DIR%/ogre/lib/OgreMain.lib -DOIS_H_PATH=%DEP_DIR%/%ARCH_DIR%/misc/include -DOIS_LIB=%DEP_DIR%/misc/lib/OIS.lib -DBoost_THREAD_LIBRARY_RELEASE=%DEP_DIR%/%ARCH_DIR%/boost/lib/boost_thread.lib DBoost_THREAD_LIBRARY_DEBUG=%DEP_DIR%/%ARCH_DIR%/boost/lib/boost_thread.lib -G "%VSCOMPILER%%VSARCH%" .

echo "Building CEGUI"
MSBuild.exe cegui.sln /p:Configuration=Release > NUL

echo "Installing CEGUI"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"

xcopy /S /Y "%PREFIX%/include/cegui-0" "%PREFIX%/include" > NUL
rd /S /Q "%PREFIX%/share"
rd /S /Q "%PREFIX%/include/cegui-0"

RD /S /Q "%TMP_DIR%"
