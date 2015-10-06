SET ARCH=Visual Studio 12
IF [%1] == [64] (
	SET ARCH=Visual Studio 12 Win64
)
IF [%1] == [32] (
	SET ARCH=Visual Studio 12
)

call build-common.bat

Set ARCHIVE=gmock-1.7.0.zip
Set BUILD_DIR=%TMP_DIR%/gmock-1.7.0
Set PREFIX=%DEP_DIR%/gmock

echo "Compile GoogleMock with GoogleTest"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Configuring GoogleMock with GoogleTest"
cd %BUILD_DIR%
cmake . -DCMAKE_INSTALL_PREFIX=%PREFIX% -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DGTEST_CREATE_SHARED_LIBRARY=1 -G "%ARCH%" .

echo "Building GoogleMock with GoogleTest"
MSBuild.exe gmock.sln /p:Configuration=Release > NUL

echo "Installing GoogleMock with GoogleTest"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
xcopy /S /Y "%BUILD_DIR%/gtest/include" "%PREFIX%/include" > NUL
xcopy /S /Y "%BUILD_DIR%/include" "%PREFIX%/include" > NUL

xcopy /S /Y "%BUILD_DIR%/gtest/Release" "%PREFIX%/lib" > NUL
xcopy /S /Y "%BUILD_DIR%/Release" "%PREFIX%/lib" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"