call build-common.bat

Set ARCHIVE=wxWidgets-3.0.2.tar.bz2
Set BUILD_DIR=%TMP_DIR%\wxWidgets-3.0.2
Set PREFIX=%DEP_DIR%\wxWidgets

echo "Compile wxWidgets"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building wxWidgets"
cd %BUILD_DIR%\build\msw

MSBuild.exe wx_vc12.sln /p:Configuration=Release > NUL

echo "Installing wxWidgets"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include/wx"
mkdir "%PREFIX%/lib/vc_lib"
mkdir "%PREFIX%/lib/vc_lib/mswu"
xcopy /S /Y "%BUILD_DIR%\include" "%PREFIX%\include\wx" > NUL
xcopy /S /Y "%BUILD_DIR%\include\msvc" "%PREFIX%/include\wx" > NUL

xcopy /S /Y "%BUILD_DIR%\lib\vc_lib\*.lib" "%PREFIX%\lib\" > NUL
xcopy /S /Y "%BUILD_DIR%\lib\vc_lib\mswu" "%PREFIX%\lib\vc_lib\mswu" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
