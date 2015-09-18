call build-common.bat

Set ARCHIVE=wxWidgets-3.0.2.tar.bz2
Set BUILD_DIR=%TMP_DIR%\wxWidgets-3.0.2
Set PREFIX=%DEP_DIR%\wxWidgets

echo "Compile wxWidgets"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building wxWidgets"
cd %BUILD_DIR%\build\msw

MSBuild.exe wx_vc12.sln /p:Configuration="DLL Release" > NUL

echo "Installing wxWidgets"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/bin"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include/wx"
mkdir "%PREFIX%/lib/vc_dll"
mkdir "%PREFIX%/lib/vc_dll/mswu"
xcopy /S /Y "%BUILD_DIR%\include" "%PREFIX%\include\wx" > NUL
xcopy /S /Y "%BUILD_DIR%\include\msvc" "%PREFIX%/include\wx" > NUL

xcopy /S /Y "%BUILD_DIR%\lib\vc_dll\*.dll" "%PREFIX%\bin\" > NUL
xcopy /S /Y "%BUILD_DIR%\lib\vc_dll\*.lib" "%PREFIX%\lib\" > NUL
xcopy /S /Y "%BUILD_DIR%\lib\vc_dll\mswu" "%PREFIX%\lib\vc_dll\mswu" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
