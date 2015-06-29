@echo off
Set DEP_DIR=%cd%
Set TMP_DIR=%cd%\tmp
Set CONFIG_BAT="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

echo %1

IF "%1" == "downloadAndUnpack" (goto downloadAndUnpack)
EXIT /B

:downloadAndUnpack
Set DOWNLOAD_URL=%4
IF [%4] == [] Set DOWNLOAD_URL=http://www.clockwork-origins.de/dependencies/
IF not exist %TMP_DIR% (mkdir %TMP_DIR%)
IF not exist %TMP_DIR%\%2 (bitsadmin /transfer "myDownloadJob%2" /download /priority normal %DOWNLOAD_URL%%2 %TMP_DIR%\%2)
cd %TMP_DIR%
if exist %3 RD /S /Q "%2"
winrar.exe x %2
if not exist %3 exit /b
EXIT /B 0
