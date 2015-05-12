call build-common.bat

Set ARCHIVE=sinbad-ogre-dd30349ea667.tar.bz2
Set DEP_ARCHIVE=cabalistic-ogredeps-0e96ef9d3475.zip
Set BUILD_DIR=%TMP_DIR%/sinbad-ogre-dd30349ea667
Set BUILD_DIR_DEPS=%TMP_DIR%/cabalistic-ogredeps-0e96ef9d3475
Set PREFIX=%DEP_DIR%/ogre/
Set PREFIX_DEPS=%DEP_DIR%/misc/

echo "Compile Ogre"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack %DEP_ARCHIVE% %BUILD_DIR_DEPS%

echo "Configuring OgreDeps"
cd %BUILD_DIR_DEPS%
cmake -DCMAKE_INSTALL_PREFIX:PATH=%PREFIX_DEPS% -DOGREDEPS_BUILD_OIS=OFF .

echo "Building OgreDeps"
MSBuild.exe OGREDEPS.sln /p:Configuration=Release > NUL

echo "Installing OgreDeps"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up OgreDeps"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR_DEPS%"

xcopy /S /Y "%PREFIX_DEPS%/bin/Release" "%PREFIX_DEPS%/bin"
xcopy /S /Y "%PREFIX_DEPS%/lib/Release" "%PREFIX_DEPS%/lib"
rd /S /Q "%PREFIX_DEPS%/bin/Release"
rd /S /Q "%PREFIX_DEPS%/bin/Debug"
rd /S /Q "%PREFIX_DEPS%/lib/Release"
rd /S /Q "%PREFIX_DEPS%/lib/Debug"

echo "Configuring Ogre"

cd %BUILD_DIR%

cmake -DOGRE_BUILD_COMPONENT_PAGING=ON -DOGRE_BUILD_COMPONENT_PROPERTY=ON -DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM=ON -DOGRE_BUILD_COMPONENT_TERRAIN=ON -DOGRE_BUILD_PLUGIN_BSP=ON -DOGRE_BUILD_PLUGIN_CG=ON -DOGRE_BUILD_PLUGIN_OCTREE=ON -DOGRE_BUILD_PLUGIN_PCZ=ON -DOGRE_BUILD_PLUGIN_PFX=ON -DOGRE_BUILD_RENDERSYSTEM_GL=ON -DOGRE_BUILD_RENDERSYSTEM_D3D9=OFF -DOGRE_BUILD_RENDERSYSTEM_D3D11=OFF -DOGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS=ON -DOGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS=ON -DOGRE_BUILD_TESTS=OFF -DOGRE_BUILD_SAMPLES=OFF -DOGRE_BUILD_TOOLS=ON -DOGRE_CONFIG_ALLOCATOR:STRING=1 -DOGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR=ON -DOGRE_CONFIG_ENABLE_DDS=ON -DOGRE_CONFIG_ENABLE_FREEIMAGE=ON -DOGRE_CONFIG_ENABLE_PVRTC=OFF -DOGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE=OFF -DOGRE_CONFIG_ENABLE_ZIP=ON -DOGRE_CONFIG_MEMTRACK_RELEASE=OFF -DOGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR=OFF -DOGRE_CONFIG_THREADS:STRING=2 -DOGRE_CONFIG_DOUBLE=ON -DOGRE_CONFIG_THREAD_PROVIDER:STRING=boost -DOGRE_FULL_RPATH=OFF -DOGRE_INSTALL_DEPENDENCIES=OFF -DOGRE_INSTALL_DOCS=OFF -DOGRE_INSTALL_SAMPLES=OFF -DOGRE_INSTALL_SAMPLES_SOURCE=OFF -DOGRE_INSTALL_TOOLS=ON -DOGRE_LIB_DIRECTORY:STRING=lib -DOGRE_PROFILING=OFF -DOGRE_STATIC=OFF -DCMAKE_INSTALL_PREFIX:PATH=%PREFIX% -DOGRE_USE_BOOST=ON -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=%DEP_DIR%/boost -DOIS_HOME=%DEP_DIR%/ois -DOIS_BINARY_REL=%DEP_DIR%/ois/bin/OIS.dll -DOIS_LIBRARY_REL=%DEP_DIR%/ois/lib/OIS.lib -DOGRE_DEPENDENCIES_DIR=%PREFIX_DEPS% .

echo "Building Ogre"
MSBuild.exe OGRE.sln /p:Configuration=Release > NUL

echo "Installing Ogre"
MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"

xcopy /S /Y "%PREFIX%/bin/Release" "%PREFIX%/bin" > NUL
xcopy /S /Y "%PREFIX%/lib/Release" "%PREFIX%/lib" > NUL
rd /S /Q "%PREFIX%/bin/Release"
rd /S /Q "%PREFIX%/lib/Release"
rd /S /Q "%PREFIX%/include/OIS"
rd /S /Q "%PREFIX%/CMake"
RD /S /Q "%TMP_DIR%"
