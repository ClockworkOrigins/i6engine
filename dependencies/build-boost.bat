call build-common.bat %1 %2

Set ARCHIVE=boost_1_58_0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/boost_1_58_0

Set PREFIX=%DEP_DIR%/%ARCH_DIR%/boost/
Set DEBUG_FLAG="variant=debug"
Set RELEASE_FLAG="variant=release"

Set BUILD_TYPE=%RELEASE_FLAG%

echo "Compile Boost"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack boost_1_58_patch_post.zip %TMP_DIR%

if not exist %BUILD_DIR% exit /b
cd %BUILD_DIR%

echo "Bootstrapping Boost"

if not exist bootstrap.bat exit /b

call bootstrap.bat

if not exist b2.exe exit /b

IF [%BOOSTARCH%] == [64] (
	echo using python : 2.7 : %PYTHON_PATH_x64% ; >> user-config.jam
)
IF [%BOOSTARCH%] == [32] (
	echo using python : 2.7 : %PYTHON_PATH_x86% ; >> user-config.jam
)

b2 --user-config=user-config.jam toolset=%BOOSTCOMPILER% address-model=%BOOSTARCH% --with-atomic --with-date_time --with-filesystem --with-log --with-python --with-regex --with-serialization --with-system --with-thread link=shared threading=multi --layout=system variant=release install --prefix=%PREFIX% stage > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
xcopy /I /S /Y "%TMP_DIR%\boost_post" "%PREFIX%\include\boost\config"

RD /S /Q "%TMP_DIR%"
