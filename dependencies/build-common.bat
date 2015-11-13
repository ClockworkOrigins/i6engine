@echo OFF
Set DEP_DIR=%cd%
Set TMP_DIR=%cd%\tmp
Set CONFIG_BAT="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

IF "%1" == "downloadAndUnpack" (goto downloadAndUnpack)

SET VSCOMPILER=Visual Studio 12
SET VSARCH=
SET BOOSTCOMPILER=msvc-12.0
SET BOOSTARCH=32
SET ARCH_DIR=msvc12_x86

IF [%1] == [msvc12] (
	SET VSCOMPILER=Visual Studio 12
	SET BOOSTCOMPILER=msvc-12.0
	SET ARCH_DIR=msvc12_
)
IF [%1] == [msvc14] (
	SET VSCOMPILER=Visual Studio 14
	SET BOOSTCOMPILER=msvc-14.0
	SET ARCH_DIR=msvc14_
)
IF [%1] == [android] (
	SET VSCOMPILER=
	SET BOOSTCOMPILER=
	SET ARCH_DIR=android
)
IF [%1] == [] (
	SET VSCOMPILER=Visual Studio 12
	SET BOOSTCOMPILER=msvc-12.0
	SET ARCH_DIR=msvc12_
)
IF [%2] == [32] (
	SET VSARCH=
	SET BOOSTARCH=32
	SET ARCH_DIR=%ARCH_DIR%x86
)
IF [%2] == [64] (
	SET VSARCH= Win64
	SET BOOSTARCH=64
	SET ARCH_DIR=%ARCH_DIR%x64
)
IF [%2] == [] (
	SET VSARCH=
	SET BOOSTARCH=32
	IF NOT [%ARCH_DIR%] == [android] (
		SET ARCH_DIR=%ARCH_DIR%x86
	)
)

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
