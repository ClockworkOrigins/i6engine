call build-common.bat

Set ARCHIVE=ois_v1-3.tar.gz
Set BUILD_DIR=%BUILD_ROOT%/ois-v1-3
Set PREFIX=%cd%/ois/

echo "Compile OIS"

echo "Extracting OIS"
if not exist %BUILD_ROOT% exit
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit

echo "Configuring OIS"
cd %BUILD_DIR%/Win32

echo "Building OIS"
for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%PATCH_DIR%/Windows/ois" "%BUILD_DIR%/Win32" ) else ( devenv ois.sln /upgrade )

MSBuild.exe ois.sln /p:Configuration=OIS_ReleaseDll > NUL

echo "Installing OIS"
mkdir "%PREFIX%"
mkdir "%PREFIX%/bin"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include"
move ../includes %PREFIX%/include/OIS > NUL
move ../dll/OIS.dll %PREFIX%/bin/OIS.dll
move ../dll/OIS.lib %PREFIX%/lib/OIS.lib

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"