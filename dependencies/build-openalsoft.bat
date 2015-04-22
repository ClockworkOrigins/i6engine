call build-common.bat

Set ARCHIVE=openal-soft-1.16.0.tar.bz2
Set BUILD_DIR=%BUILD_ROOT%/openal-soft-1.16.0
Set PREFIX=%cd%/OpenAL Soft/

echo "Compile OpenAL Soft"

call download-dependency.bat %ARCHIVE%

echo "Extracting OpenAL Soft"
if not exist %BUILD_ROOT% exit /b
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit /b
cd %BUILD_DIR%

echo "Configuring OpenAL Soft"
cd build
cmake -DALSOFT_NO_CONFIG_UTIL=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% ..

echo "Building OpenAL Soft"
MSBuild.exe OpenAL.sln /p:Configuration=Release > NUL

echo "Installing OpenAL Soft"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL


echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%EX_DIR%\.."
