call build-common.bat

Set ARCHIVE=gperftools-2.0.tar.gz
Set BUILD_DIR=%BUILD_ROOT%/gperftools-2.0
Set PREFIX=%cd%/gperftools

echo "Compile GPerfTools"

echo "Extracting GPerfTools"
if not exist %BUILD_ROOT% exit
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit

echo "Patching GPerfTools"
xcopy /S /Y "%PATCH_DIR%/Windows/gperftools/src" "%BUILD_DIR%/src"

echo "Building GPerfTools"
cd %BUILD_DIR%

for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%PATCH_DIR%/Windows/gperftools_sln" "%BUILD_DIR%" ) else ( devenv gperftools.sln /upgrade )

MSBuild.exe gperftools.sln /p:Configuration=Release > NUL

echo "Installing GPerfTools"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include/gperftools"
xcopy /S /Y "%BUILD_DIR%/src" "%PREFIX%/include/gperftools" > NUL

xcopy /S /Y "%BUILD_DIR%/Release" "%PREFIX%/lib" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"