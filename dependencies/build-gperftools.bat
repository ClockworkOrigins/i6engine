call build-common.bat

Set ARCHIVE=gperftools-2.4.tar.gz
Set BUILD_DIR=%TMP_DIR%/gperftools-2.4
Set PREFIX=%DEP_DIR%/gperftools

echo "Compile GPerfTools"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call download-dependency.bat gperftools_2_4_patch_sln.zip

cd %TMP_DIR%

winrar.exe x gperftools_2_4_patch_sln.zip

echo "Building GPerfTools"
cd %BUILD_DIR%

for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%TMP_DIR%/gperftools_sln" "%BUILD_DIR%" ) else ( devenv gperftools.sln /upgrade )

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
RD /S /Q "%TMP_DIR%"