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

Set ARCHIVE=bullet-2.82-r2704.tgz
Set BUILD_DIR=%TMP_DIR%/bullet-2.82-r2704
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/bullet/

echo "Compile Bullet"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack bullet_2_82_patch.zip %TMP_DIR%

echo "Patching Bullet"
xcopy /S /Y "%TMP_DIR%\bullet\Extras" "%BUILD_DIR%\Extras"

echo "Configuring Bullet"
cd %BUILD_DIR%
cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DBUILD_DEMOS=OFF -DINSTALL_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DUSE_DOUBLE_PRECISION=ON -DUSE_GRAPHICAL_BENCHMARK=OFF -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DBUILD_CPU_DEMOS=OFF -DBUILD_INTEL_OPENCL_DEMOS=OFF -DBUILD_EXTRAS=ON -DBUILD_MINICL_OPENCL_DEMOS=OFF -DINSTALL_EXTRA_LIBS=ON -DUSE_DX11=OFF -DUSE_GLUT=ON -G "%VSCOMPILER%%VSARCH%" .

echo "Building Bullet"
MSBuild.exe BULLET_PHYSICS.sln /p:Configuration=Release > NUL

echo "Installing Bullet"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%

RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"