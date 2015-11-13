call build-common.bat %1 %2

call build-boost.bat %1 %2
call build-bullet.bat %1 %2
REM depends on boost
call build-ogre.bat %1 %2
REM depends on ogre and boost
call build-cegui.bat %1 %2
call build-gperftools.bat %1 %2
call build-gmock.bat %1 %2
call build-tinyxml.bat %1 %2
call build-clockUtils.bat %1 %2
call build-openalsoft.bat %1 %2
call build-wxWidgets.bat %1 %2
REM depends on boost
REM call build-m2etis.bat

