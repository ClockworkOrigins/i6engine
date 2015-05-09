call build-common.bat

Set ARCHIVE=ois_v1-3.tar.gz
Set BUILD_DIR=%TMP_DIR%/ois-v1-3
Set PREFIX=%DEP_DIR%/ois/

echo "Compile OIS"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%
call download-dependency.bat ois_1_3_patch.zip

winrar.exe x ois_1_3_patch.zip

echo "Configuring OIS"
cd %BUILD_DIR%/Win32

echo "Building OIS"
for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%TMP_DIR%/ois" "%BUILD_DIR%/Win32" ) else ( devenv ois.sln /upgrade )

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
RD /S /Q "%TMP_DIR%"