SET ARCH=Visual Studio 12
IF [%1] == [64] (
	SET ARCH=Visual Studio 12 Win64
)
IF [%1] == [32] (
	SET ARCH=Visual Studio 12
)

call build-common.bat

Set ARCHIVE=clockUtils-0.3-rev128.zip
Set BUILD_DIR=%TMP_DIR%/clockUtils-0.3-rev128
Set PREFIX=%DEP_DIR%/clockUtils/

echo "Compile clockUtils"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %BUILD_DIR%

echo "Configuring clockUtils"
cmake -DWITH_TESTING=OFF -DWITH_LIBRARY_SOCKETS=ON -DWITH_LIBRARY_INIPARSER=ON -DWITH_LIBRARY_ARGPARSER=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%ARCH%" .

echo "Building clockUtils"
MSBuild.exe clockUtils.sln /p:Configuration=Release > NUL

echo "Installing clockUtils"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL


echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
