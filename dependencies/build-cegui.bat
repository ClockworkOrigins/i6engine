call build-common.bat

Set ARCHIVE=cegui-0.8.4.tar.bz2
Set DEP_ARCHIVE=cegui-deps-0.8.x-src.zip
Set BUILD_DIR=%BUILD_ROOT%/cegui-0.8.4
Set BUILD_DIR_DEPS=%BUILD_ROOT%/cegui-deps-0.8.x-src
Set PREFIX=%cd%/cegui/
Set PREFIX_DEPS=%cd%/ceguideps

echo "Compile CEGUI"

call download-dependency.bat %ARCHIVE%
call download-dependency.bat %DEP_ARCHIVE%
call download-dependency.bat cegui_0_8_4_patch.zip

mkdir %PATCH_DIR%\Windows

move %EX_DIR%\cegui_0_8_4_patch.zip %PATCH_DIR%\Windows

cd %PATCH_DIR%\Windows

winrar.exe x cegui_0_8_4_patch.zip

echo "Extracting CEGUIDeps"
if not exist %BUILD_ROOT% exit /b
cd %BUILD_ROOT%

if exist %BUILD_DIR_DEPS% RD /S /Q "%BUILD_DIR_DEPS%"

winrar.exe x %EX_DIR%/%DEP_ARCHIVE%

if not exist %BUILD_DIR_DEPS% exit /b

echo "Patching CEGUIDeps"
cd %BUILD_DIR_DEPS%
xcopy /S /Y "%PATCH_DIR%/Windows/cegui-deps/src" "%BUILD_DIR_DEPS%/src"

echo "Configuring CEGUIDeps"
cd %BUILD_DIR_DEPS%
cmake -DCMAKE_INSTALL_PREFIX:PATH=%PREFIX_DEPS% -DCEGUI_BUILD_CORONA=OFF -DCEGUI_BUILD_DEVIL=OFF -DCEGUI_BUILD_EFFECTS11=OFF -DCEGUI_BUILD_EXPAT=ON -DCEGUI_BUILD_FREEIMAGE=ON -DCEGUI_BUILD_FREETYPE2=ON -DCEGUI_BUILD_GLEW=OFF -DCEGUI_BUILD_GLFW=OFF -DCEGUI_BUILD_GLM=OFF -DCEGUI_BUILD_LUA=OFF -DCEGUI_BUILD_MINIZIP=OFF -DCEGUI_BUILD_PCRE=ON -DCEGUI_BUILD_SILLY=OFF -DCEGUI_BUILD_TINYXML=OFF -DCEGUI_BUILD_TOLUAPP=OFF -DCEGUI_BUILD_XERCES=OFF -DCEGUI_BUILD_ZLIB=OFF .

echo "Building CEGUIDeps"
MSBuild.exe CEGUI-DEPS.sln /p:Configuration=Release > NUL
MSBuild.exe CEGUI-DEPS.sln /p:Configuration=Debug > NUL

echo "Installing CEGUIDeps"
mkdir "%PREFIX_DEPS%"

if not exist %PREFIX_DEPS% exit /b
xcopy /S /Y "%BUILD_DIR_DEPS%/dependencies" "%PREFIX_DEPS%" > NUL

echo "Cleaning up CEGUIDeps"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR_DEPS%"

echo "Extracting CEGUI"
if not exist %BUILD_ROOT% exit /b
cd %BUILD_ROOT%

if exist %BUILD_DIR% RD /S /Q "%BUILD_DIR%"

winrar.exe x %EX_DIR%/%ARCHIVE% > NUL

if not exist %BUILD_DIR% exit /b

mkdir "%BUILD_DIR%/dependencies"
xcopy /S /Y "%PREFIX_DEPS%" "%BUILD_DIR%/dependencies" > NUL

echo "Patching CEGUI"
cd %BUILD_DIR%

echo "Configuring CEGUI"
cd %BUILD_DIR%
cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DCEGUI_BUILD_XMLPARSER_XERCES=OFF -DCEGUI_BUILD_XMLPARSER_LIBXML2=OFF -DCEGUI_BUILD_XMLPARSER_RAPIDXML=OFF -DCEGUI_BUILD_XMLPARSER_TINYXML=OFF -DCEGUI_BUILD_XMLPARSER_EXPAT=ON -DCEGUI_BUILD_IMAGECODEC_SILLY=OFF -DCEGUI_BUILD_IMAGECODEC_DEVIL=OFF -DCEGUI_BUILD_IMAGECODEC_FREEIMAGE=ON -DCEGUI_BUILD_IMAGECODEC_CORONA=OFF -DCEGUI_BUILD_IMAGECODEC_STB=OFF -DCEGUI_BUILD_IMAGECODEC_TGA=ON -DCEGUI_BUILD_IMAGECODEC_PVR=OFF -DCEGUI_BUILD_RENDERER_OGRE=ON -DCEGUI_BUILD_RENDERER_DIRECT3D9=OFF -DCEGUI_BUILD_RENDERER_DIRECT3D10=OFF -DCEGUI_BUILD_RENDERER_DIRECT3D11=OFF -DCEGUI_BUILD_RENDERER_OPENGL=OFF -DCEGUI_BUILD_RENDERER_OPENGL3=OFF -DCEGUI_BUILD_RENDERER_IRRLICHT=OFF -DCEGUI_BUILD_RENDERER_NULL=OFF -DCEGUI_BUILD_RENDERER_OPENGLES=OFF -DCEGUI_BUILD_LUA_MODULE=OFF -DCEGUI_BUILD_LUA_GENERATOR=OFF -DCEGUI_BUILD_PYTHON_MODULES=OFF -DCEGUI_SAMPLES_ENABLED=OFF -DCEGUI_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=%DEP_DIR%/boost/ -DOGRE_H_BUILD_SETTINGS_PATH=%DEP_DIR%/ogre/include/OGRE/ -DOGRE_H_PATH=%DEP_DIR%/ogre/include/OGRE/ -DOGRE_LIB=%DEP_DIR%/ogre/lib/OgreMain.lib -DOIS_H_PATH=%DEP_DIR%/ois/include -DOIS_LIB=%DEP_DIR%/ois/lib/OIS.lib -DBoost_THREAD_LIBRARY_RELEASE=%DEP_DIR%/boost/lib/boost_thread.lib DBoost_THREAD_LIBRARY_DEBUG=%DEP_DIR%/boost/lib/boost_thread.lib .

echo "Building CEGUI"
MSBuild.exe cegui.sln /p:Configuration=Release > NUL

echo "Installing CEGUI"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"

xcopy /S /Y "%PREFIX%/include/cegui-0" "%PREFIX%/include" > NUL
rd /S /Q "%PREFIX%/share"
rd /S /Q "%PREFIX%/include/cegui-0"

RD /S /Q "%EX_DIR%\.."
