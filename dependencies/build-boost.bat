SET ARCH=32
IF [%1] == [64] (
	SET ARCH=64
)
IF [%1] == [32] (
	SET ARCH=32
)

call build-common.bat

Set ARCHIVE=boost_1_58_0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/boost_1_58_0

Set PREFIX=%DEP_DIR%/boost/
Set DEBUG_FLAG="variant=debug"
Set RELEASE_FLAG="variant=release"

Set BUILD_TYPE=%RELEASE_FLAG%

echo "Compile Boost"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call download-dependency.bat boost_1_58_patch_post.zip

cd %TMP_DIR%

winrar.exe x boost_1_58_patch_post.zip

if not exist %BUILD_DIR% exit /b
cd %BUILD_DIR%

echo "Bootstrapping Boost"

if not exist bootstrap.bat exit /b

call bootstrap.bat

if not exist b2.exe exit /b

IF [%ARCH%] == [64] (
	echo using python : 2.7 : %PYTHON_PATH_x64% ; >> user-config.jam
)
IF [%ARCH%] == [32] (
	echo using python : 2.7 : %PYTHON_PATH_x86% ; >> user-config.jam
)

b2 --user-config=user-config.jam toolset=msvc address-model=%ARCH% --with-atomic --with-date_time --with-filesystem --with-log --with-python --with-regex --with-serialization --with-system --with-thread link=shared threading=multi --layout=system variant=release install --prefix=%PREFIX% stage > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
xcopy /S /Y "%TMP_DIR%\boost_post" "%PREFIX%\include\boost\config"

RD /S /Q "%TMP_DIR%"
