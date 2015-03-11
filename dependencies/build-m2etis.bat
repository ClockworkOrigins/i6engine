Set ROOT_DIR=%1
Set COPY_DIR=%2

call "%ROOT_DIR%\build-common.bat"

Set DEP_DIR=%ROOT_DIR%
Set EX_DIR=%DEP_DIR%\..\externals\libs
Set PATCH_DIR=%DEP_DIR%\..\externals\patches
Set BUILD_ROOT=%EX_DIR%

Set ARCHIVE=m2etis-0.3-rev20.zip
Set BUILD_DIR=%BUILD_ROOT%/m2etis-0.3-rev20/library
Set PREFIX=%ROOT_DIR%/m2etis

call %CONFIG_BAT%

echo "Compile m2etis"

call download-dependency.bat %ARCHIVE%

if exist %PREFIX% exit

echo "Extracting m2etis"

if not exist %BUILD_ROOT% exit
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

echo %EX_DIR%/%ARCHIVE%

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit

echo "Configuring m2etis"

xcopy /S /Y "%DEP_DIR%\config\m2etis" "%BUILD_DIR%\include\m2etis\config\examples"

mkdir "%BUILD_DIR%\extern\i6engine\utils"
mkdir "%BUILD_DIR%\extern\i6engine\core\messaging"
mkdir "%BUILD_DIR%\extern\i6engine\api"

xcopy "%DEP_DIR%\..\libs\i6engine-utils\include\i6engine\utils\i6eSystemParameters.h" "%BUILD_DIR%\extern\i6engine\utils"
xcopy "%DEP_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\IPKey.h" "%BUILD_DIR%\extern\i6engine\core\messaging"
xcopy "%DEP_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\Message.h" "%BUILD_DIR%\extern\i6engine\core\messaging"
xcopy "%DEP_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\MessageStruct.h" "%BUILD_DIR%\extern\i6engine\core\messaging"
xcopy "%DEP_DIR%\..\libs\i6engine-modules\include\i6engine\api\GameMessage.h" "%BUILD_DIR%\extern\i6engine\api"
xcopy "%DEP_DIR%\..\libs\i6engine-modules\include\i6engine\api\GameMessageStruct.h" "%BUILD_DIR%\extern\i6engine\api"
xcopy "%COPY_DIR%\i6engine\i6engineBuildSettings.h" "%BUILD_DIR%\extern\i6engine"

cd %BUILD_DIR%
cmake . -DCMAKE_INSTALL_PREFIX=%PREFIX% -DADDITIONAL_INCLUDE_DIRECTORIES=%BUILD_DIR%\extern -DCMAKE_BUILD_TYPE=Release -DDEP_DIR=%DEP_DIR%/ -DWITH_TESTING=OFF -DWITH_SIM=OFF -DWITH_LOGGING=OFF -DWITH_MESSAGECOMPRESSION=OFF .

echo "Building m2etis"
MSBuild.exe m2etis.sln /p:Configuration=Release

echo "Installing m2etis"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%/.."
RD /S /Q "%EX_DIR%\.."
