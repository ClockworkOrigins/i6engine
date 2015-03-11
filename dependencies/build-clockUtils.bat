call build-common.bat

Set ARCHIVE=clockUtils-0.3-rev105.zip
Set BUILD_DIR=%BUILD_ROOT%/clockUtils-0.3-rev105
Set PREFIX=%cd%/clockUtils/

echo "Compile clockUtils"

call download-dependency.bat %ARCHIVE%

echo "Extracting clockUtils"
if not exist %BUILD_ROOT% exit
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit
cd %BUILD_DIR%

echo "Configuring clockUtils"
cmake -DWITH_TESTING=OFF -DWITH_LIBRARY_SOCKETS=ON -DWITH_LIBRARY_INIPARSER=ON -DWITH_LIBRARY_ARGPARSER=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% .

echo "Building clockUtils"
MSBuild.exe clockUtils.sln /p:Configuration=Release > NUL

echo "Installing clockUtils"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL


echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%EX_DIR%\.."
