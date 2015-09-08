call build-common.bat

Set ARCHIVE=wxWidgets-3.0.2.tar.bz2
Set BUILD_DIR=%TMP_DIR%\wxWidgets-3.0.2
Set PREFIX=%DEP_DIR%\wxWidgets

echo "Compile wxWidgets"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building wxWidgets"
cd %BUILD_DIR%\build\msw

nmake /f makefile.vc BUILD=release CXXFLAGS="/DNEED_PBT_H" > NUL

echo "Installing wxWidgets"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
xcopy /S /Y "%BUILD_DIR%\include" "%PREFIX%\include" > NUL
xcopy /S /Y "%BUILD_DIR%\lib\vc_lib\msw" "%PREFIX%/include" > NUL

xcopy /S /Y "%BUILD_DIR%\lib\vc_lib\*.lib" "%PREFIX%\lib\" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
