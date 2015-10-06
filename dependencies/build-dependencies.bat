SET ARCH=32

IF [%1] == [64] (
	SET ARCH=64
)
IF [%1] == [] (
	SET ARCH=32
)

call build-common.bat

call build-boost.bat %ARCH%
call build-bullet.bat %ARCH%
REM depends on boost
call build-ogre.bat %ARCH%
REM depends on ogre and boost
call build-cegui.bat %ARCH%
call build-gperftools.bat %ARCH%
call build-gmock.bat %ARCH%
call build-tinyxml.bat %ARCH%
call build-clockUtils.bat %ARCH%
call build-openalsoft.bat %ARCH%
REM depends on boost
REM call build-m2etis.bat

