call build-common.bat

Set ARCHIVE=tinyxml2.zip
Set BUILD_DIR=%BUILD_ROOT%/tinyxml2
Set PREFIX=%cd%/tinyxml2/

echo "Compile TinyXML"

echo "Extracting TinyXML"
if not exist %BUILD_ROOT% exit
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE%

if not exist %BUILD_DIR% exit
cd %BUILD_DIR%

echo "Building TinyXML"
cl /EHsc /MP /GS /TP /analyze- /W3 /Zc:wchar_t /Z7 /Gm- /Ox /fp:precise /FD /D "WIN32" /D "_WINDOWS" /D "TINYXML2_EXPORT" /D "_WINDLL" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /GR /Gd /Oy- /MD /nologo /D_USRDLL tinyxml2.cpp /link /DLL /OUT:tinyxml2.dll

if %errorlevel% gtr 0 exit

echo "Installing TinyXML"
mkdir "%PREFIX%"
mkdir "%PREFIX%/bin"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include"

if not exist %PREFIX% exit
if not exist %PREFIX%/bin exit
if not exist %PREFIX%/lib exit
if not exist %PREFIX%/include exit

move tinyxml2.h %PREFIX%/include/tinyxml2.h
move tinyxml2.dll %PREFIX%/bin/tinyxml2.dll
move tinyxml2.lib %PREFIX%/lib/tinyxml2.lib

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
