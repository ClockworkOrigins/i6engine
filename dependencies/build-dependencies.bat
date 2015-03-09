call build-common.bat

call build-boost.bat
call build-bullet.bat
call build-ois.bat
REM depends on boost and ois
call build-ogre.bat
REM depends on ois, ogre and boost
call build-cegui.bat
call build-gperftools.bat
call build-gmock.bat
call build-tinyxml.bat
call build-clockUtils.bat
call build-openalsoft.bat
REM depends on boost
REM call build-m2etis.bat

