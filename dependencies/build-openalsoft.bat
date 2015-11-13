call build-common.bat %1 %2

Set ARCHIVE=openal-soft-1.16.0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/openal-soft-1.16.0
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/OpenAL Soft/

echo "Compile OpenAL Soft"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %BUILD_DIR%

echo "Configuring OpenAL Soft"
cd build
cmake -DALSOFT_NO_CONFIG_UTIL=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" ..

echo "Building OpenAL Soft"
MSBuild.exe OpenAL.sln /p:Configuration=Release > NUL

echo "Installing OpenAL Soft"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
