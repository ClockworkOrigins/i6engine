@echo OFF
Set ROOT_DIR=%3
Set COPY_DIR=%4

call build-common.bat %1 %2

SET SOURCE_DIR=%cd%
SET DEP_DIR=%ROOT_DIR%
Set ARCHIVE=m2etis-0.3-rev26.zip
Set BUILD_DIR=%cd%\tmp\m2etis-0.3-rev26\library
Set PREFIX=%ROOT_DIR%/%ARCH_DIR%/m2etis

call %CONFIG_BAT%

echo "Compile m2etis"

if exist %PREFIX% exit /b

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

SET DEP_DIR=%ROOT_DIR%

echo "Configuring m2etis"

xcopy /I /S /Y "%DEP_DIR%\..\config\m2etis" "%BUILD_DIR%\include\m2etis\config\examples"

mkdir "%BUILD_DIR%\extern\i6engine\utils"
mkdir "%BUILD_DIR%\extern\i6engine\core\messaging"
mkdir "%BUILD_DIR%\extern\i6engine\api"

xcopy /F "%SOURCE_DIR%\..\libs\i6engine-utils\include\i6engine\utils\i6eSystemParameters.h" "%BUILD_DIR%\extern\i6engine\utils\i6eSystemParameters.h*"
xcopy /F "%SOURCE_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\IPKey.h" "%BUILD_DIR%\extern\i6engine\core\messaging\IPKey.h*"
xcopy /F "%SOURCE_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\Message.h" "%BUILD_DIR%\extern\i6engine\core\messaging\Message.h*"
xcopy /F "%SOURCE_DIR%\..\libs\i6engine-core\include\i6engine\core\messaging\MessageStruct.h" "%BUILD_DIR%\extern\i6engine\core\messaging"
xcopy /F "%SOURCE_DIR%\..\libs\i6engine-modules\include\i6engine\api\GameMessage.h" "%BUILD_DIR%\extern\i6engine\api\GameMessage.h*"
xcopy /F "%SOURCE_DIR%\..\libs\i6engine-modules\include\i6engine\api\GameMessageStruct.h" "%BUILD_DIR%\extern\i6engine\api\GameMessageStruct.h*"
xcopy /F "%COPY_DIR%\i6engine\i6engineBuildSettings.h" "%BUILD_DIR%\extern\i6engine\i6engineBuildSettings.h*"

cd %BUILD_DIR%

cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DADDITIONAL_INCLUDE_DIRECTORIES=%BUILD_DIR%\extern -DCMAKE_BUILD_TYPE=Release -DDEP_DIR=%DEP_DIR%/%ARCH_DIR%/ -DWITH_TESTING=OFF -DWITH_SIM=OFF -DWITH_LOGGING=OFF -DWITH_MESSAGECOMPRESSION=OFF -G "%VSCOMPILER%%VSARCH%" .

echo "Building m2etis"
MSBuild.exe m2etis.sln /p:Configuration=Release

echo "Installing m2etis"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%/.."
