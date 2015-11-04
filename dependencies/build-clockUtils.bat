call build-common.bat %1 %2

Set ARCHIVE=clockutils-0.4.0-src.zip
Set BUILD_DIR=%TMP_DIR%/clockutils-0.4.0-src
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/clockUtils/

echo "Compile clockUtils"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR% http://clockwork-origins.de/clockUtils/downloads/

cd %BUILD_DIR%

echo "Configuring clockUtils"
cmake -DWITH_TESTING=OFF -DWITH_LIBRARY_COMPRESSION=ON -DWITH_LIBRARY_SOCKETS=ON -DWITH_LIBRARY_INIPARSER=ON -DWITH_LIBRARY_ARGPARSER=OFF -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" .

echo "Building clockUtils"
MSBuild.exe clockUtils.sln /p:Configuration=Release > NUL

echo "Installing clockUtils"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL


echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
